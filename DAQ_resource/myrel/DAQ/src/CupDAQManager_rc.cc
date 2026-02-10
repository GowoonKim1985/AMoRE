//
// Created by cupsoft on 7/24/19.
//

#include <thread>

#include "TMessage.h"
#include "TRandom3.h"
#include "TStopwatch.h"

#include "DAQ/CupDAQManager.hh"
#include "DAQConfig/RunConfig.hh"

void CupDAQManager::Run()
{
  if (IsForcedEndRunFile()) {
    gSystem->Exec(Form("rm -f %s", kFORCEDENDRUNFILE));
  }

  auto * runConfig = new RunConfig();
  if (fDAQType != DAQ::TCBCTRL) {
    if (fConfigFilename.IsNull()) {
      fLog->Error("CupDAQManager::Run", "no config filename");
      return;
    }
    if (!runConfig->ReadConfig(fConfigFilename.Data())) {
      fLog->Error("CupDAQManager::Run", "error in reading config file \"%s\"",
                  fConfigFilename.Data());
      return;
    }
    fConfigList = runConfig->GetConfigs();
  }

  switch (fDAQType) {
    case DAQ::STDDAQ: RC_STDDAQ(); break;
    case DAQ::TCBDAQ: RC_TCBDAQ(); break;
    case DAQ::TCBCTRL: RC_TCBCTRLDAQ(); break;
    case DAQ::TCB: RC_TCB(); break;
    case DAQ::TEST: RC_NullDAQ(); break;
    default: break;
  }

  delete runConfig;
}

void CupDAQManager::RC_TCB()
{
  int state = 0;

  fLog->Info("CupDAQManager::RC_TCB", "TCB controller now starting [run=%d]",
             fRunNumber);

  sort(fDAQ.begin(), fDAQ.end());
  if (fDAQ.empty()) {
    fLog->Error("CupDAQManager::RC_TCB", "no DAQ selected");
    return;
  }
  else {
    for (auto daq : fDAQ) {
      fLog->Info("CupDAQManager::RC_TCB", "DAQ selected", GetADCName(daq));
    }
  }

  std::thread th0(&CupDAQManager::TF_MsgServer, this);
  std::thread th1;

  //
  // prepare client sockets correspond to DAQs
  //
  bool socketerror = false;
  TSocket * socket = nullptr;
  for (auto daq : fDAQ) {
    switch (daq) {
      case ADC::FADCT:
        socket = new TSocket(kFADCDAQIPADDR, kFADCDAQPORT);
        socket->SetName("FADCDAQ");
        break;
      case ADC::GADCT:
        socket = new TSocket(kGADCDAQIPADDR, kGADCDAQPORT);
        socket->SetName("GADCDAQ");
        break;
      case ADC::SADCT:
        socket = new TSocket(kSADCDAQIPADDR, kSADCDAQPORT);
        socket->SetName("SADCDAQ");
        break;
      default: break;
    }

    if (socket->GetErrorCode() < 0) {
      socketerror = true;
      fLog->Error("CupDAQManager::RC_TCB", "%s connection failed",
                  socket->GetName());
      break;
    }

    fDAQSocket.push_back(socket);
    fLog->Info("CupDAQManager::RC_TCB", "%s connected", socket->GetName());
  }

  if (socketerror) {
    fStatusTCB = kERROR;
    goto TERMINATE;
  }

  // checking DAQs' status == kBOOTED
  if (!WaitDAQStatus(kBOOTED)) {
    fStatusTCB = kERROR;
    goto TERMINATE;
  }
  fStatusTCB = kBOOTED;
  fLog->Info("CupDAQManager::RC_TCB", "all DAQs were booted");

  //
  // wait for config run command
  //
  state = WaitUntil(&fDoConfigRunTCB, &fDoExitTCB);
  if (state != 0) {
    if (state == 1) {
      fLog->Info("CupDAQManager::RC_TCB", "run=%d exited by Run Control",
                 fRunNumber);
    }
    else if (state < 0) {
      fStatusTCB = kERROR;
    }
    goto TERMINATE;
  }

  fTCB = new CupGeneralTCB();
  fTCB->SetConfig(fConfigList);
  if (fTCB->Open() != 0) {
    fStatusTCB = kERROR;
    goto TERMINATE;
  }

  if (!fTCB->Config()) {
    fStatusTCB = kERROR;
    goto TERMINATE;
  }

  SendCommandToDAQ(kCONFIGRUN);

  // checking DAQs' status == kCONFIGURED
  if (!WaitDAQStatus(kCONFIGURED)) {
    fStatusTCB = kERROR;
    goto TERMINATE;
  }
  fStatusTCB = kCONFIGURED;
  fLog->Info("CupDAQManager::RC_TCB", "all DAQs were configured");

  th1 = std::thread(&CupDAQManager::TF_SplitOutput, this, true);

  //
  // wait for start run command
  //
  state = WaitUntil(&fDoStartRunTCB, &fDoExitTCB);
  if (state != 0) {
    if (state == 1) {
      fLog->Info("CupDAQManager::RC_TCB", "run=%d exited by Run Control",
                 fRunNumber);
    }
    else if (state < 0) {
      fStatusTCB = kERROR;
    }
    goto TERMINATE;
  }

  fTCB->StartTrigger();
  time(&fStartDatime);

  SendCommandToDAQ(kSTARTRUN);

  // checking DAQs' status == kRUNNING
  if (!WaitDAQStatus(kRUNNING)) {
    fStatusTCB = kERROR;
    goto TERMINATE;
  }
  fStatusTCB = kRUNNING;
  fLog->Info("CupDAQManager::RC_TCB", "all DAQs are running");

  while (true) {
    if (fDoEndRunTCB || IsForcedEndRunFile()) { break; }
    if (!IsDAQRunning()) { break; }
    if (fDoSplitOutputFile) {
      SendCommandToDAQ(kSPLITOUTPUTFILE);
      fDoSplitOutputFile = false;
    }
    gSystem->Sleep(100);
  }

  fTCB->StopTrigger();
  time(&fEndDatime);

  SendCommandToDAQ(kENDRUN);

  // checking DAQs' status == kRUNENDED
  if (!WaitDAQStatus(kRUNENDED)) {
    fStatusTCB = kERROR;
    goto TERMINATE;
  }

  gSystem->Sleep(2000);

  fStatusTCB = kRUNENDED;
  fLog->Info("CupDAQManager::RC_TCB", "all DAQs were run ended");

TERMINATE:
  WaitUntil(&fDoExitTCB);

  if (th1.joinable()) { th1.join(); }

  SendCommandToDAQ(kEXIT);

  for (auto * sock : fDAQSocket) {
    sock->Close();
    delete sock;
  }

  th0.join();

  if (fTCB) {
    fTCB->Close();
    delete fTCB;
  }

  fLog->Info("CupDAQManager::RC_TCB", "TCB controller ended");
}

void CupDAQManager::RC_TCBCTRLDAQ()
{
  fLog->Info("CupDAQManager::RC_TCBCTRLDAQ", "TCB based DAQ now starting");

  std::thread th0(&CupDAQManager::TF_MsgServer, this);

  if (!WaitUntil(&fDoConfigRun, &fDoExit)) {
    fLog->Warning("CupDAQManager::RC_TCBCTRLDAQ", "run=%d exited by TCB",
                  fRunNumber);
    th0.join();
    return;
  }

  TSocket * socket = new TSocket(kTCBIPADDR, kTCBPORT);
  if (socket->GetErrorCode() < 0) {
    delete socket;
    fLog->Error("CupDAQManager::RC_TCBCTRLDAQ", "TCB connection failed");
    fStatus = kERROR;
    WaitUntil(&fDoExit);
    th0.join();
    return;
  }

  char data[kMESSLEN];
  EncodeMsg(data, 0, 0, kREQUESTCONFIG);
  socket->SendRaw(data, kMESSLEN);

  TMessage * mess;
  socket->Recv(mess);
  fConfigList = (AbsConfList *)mess->ReadObject(mess->GetClass());

  socket->Close();
  delete socket;

  if (!fConfigList) {
    fLog->Error("CupDAQManager::RC_TCBCTRLDAQ", "null config list received");
    fStatus = kERROR;
    WaitUntil(&fDoExit);
    th0.join();
    return;
  }
  if (!AddADC(fConfigList)) {
    fStatus = kERROR;
    WaitUntil(&fDoExit);
    th0.join();
    return;
  }
  if (!PrepareDAQ()) {
    fStatus = kERROR;
    WaitUntil(&fDoExit);
    th0.join();
    return;
  }
  if (!OpenDAQ()) {
    fStatus = kERROR;
    WaitUntil(&fDoExit);
    th0.join();
    return;
  }

  // thread
  std::thread th1 = std::thread(&CupDAQManager::TF_TriggerMon, this);
  std::thread th2 = std::thread(&CupDAQManager::TF_WriteData, this);
  std::thread th3 = std::thread(&CupDAQManager::TF_BuildEvent, this);
  std::thread th4 = std::thread(&CupDAQManager::TF_SortData, this);
  std::thread th5 = std::thread(&CupDAQManager::TF_ReadData, this);
  std::thread th6 = std::thread(&CupDAQManager::TF_ShrinkToFit, this);

  fStatus = kCONFIGURED;

  if (!WaitUntil(&fDoStartRun, &fDoExit)) {
    fLog->Warning("CupDAQManager::RC_TCBCTRLDAQ", "run=%d exited by TCB",
                  fRunNumber);
    goto TERMINATE;
  }

  fStatus = kRUNNING;
  time(&fStartDatime);

  if (!WaitUntil(&fDoEndRun, &fStatus)) {
    fLog->Warning("CupDAQManager::RC_TCBCTRLDAQ", "run=%d ended by error state",
                  fRunNumber);
  }

  fStatus = kRUNENDED;
  time(&fEndDatime);

TERMINATE:
  if (th6.joinable()) th6.join();
  if (th5.joinable()) th5.join();
  if (th4.joinable()) th4.join();
  if (th3.joinable()) th3.join();
  if (th2.joinable()) th2.join();
  if (th1.joinable()) th1.join();

  WaitUntil(&fDoExit);

  th0.join();
  CloseDAQ();

  fLog->Info("CupDAQManager::RC_TCBCTRLDAQ", "TCB based DAQ ended");
}

void CupDAQManager::RC_STDDAQ()
{
  fLog->Info("CupDAQManager::RC_STDDAQ", "STDDAQ now starting");

  std::thread th1;
  std::thread th2;
  std::thread th3;
  std::thread th4;
  std::thread th5;
  std::thread th6;
  std::thread th7;

  if (!AddADC(fConfigList)) return;

  if (!OpenDAQ()) return;
  if (!ConfigureDAQ()) goto TERMINATE;
  if (!PrepareDAQ()) goto TERMINATE;
  if (!InitializeDAQ()) goto TERMINATE;

  // thread
  th1 = std::thread(&CupDAQManager::TF_TriggerMon, this);
  th2 = std::thread(&CupDAQManager::TF_WriteData, this);
  th3 = std::thread(&CupDAQManager::TF_BuildEvent, this);
  th4 = std::thread(&CupDAQManager::TF_SortData, this);
  th5 = std::thread(&CupDAQManager::TF_ReadData, this);
  th6 = std::thread(&CupDAQManager::TF_SplitOutput, this, false);
  th7 = std::thread(&CupDAQManager::TF_ShrinkToFit, this);

  StartTrigger();
  time(&fStartDatime);

  fStatus = kRUNNING;

  while (true) {
    if (IsForcedEndRunFile() || fStatus == kERROR) {
      StopTrigger();
      time(&fEndDatime);

      fDoEndRun = true;

      fLog->Info("CupDAQManager::RC_STDDAQ", "trigger stopped");
      gSystem->Sleep(1000);
      break;
    }
    gSystem->Sleep(1);
  }

  if (th7.joinable()) th7.join();
  if (th6.joinable()) th6.join();
  if (th5.joinable()) th5.join();
  if (th4.joinable()) th4.join();
  if (th3.joinable()) th3.join();
  if (th2.joinable()) th2.join();
  if (th1.joinable()) th1.join();

TERMINATE:
  CloseDAQ();

  fLog->Info("CupDAQManager::RC_STDDAQ", "STDDAQ ended");
}

void CupDAQManager::RC_TCBDAQ()
{
  fLog->Info("CupDAQManager::RC_TCBDAQ", "TCBDAQ now starting");

  std::thread th0;
  std::thread th1;
  std::thread th2;
  std::thread th3;
  std::thread th4;
  std::thread th5;
  std::thread th6;
  std::thread th7;
  std::thread th8;

  fTCB = new CupGeneralTCB();
  fTCB->SetConfig(fConfigList);
  fTCB->SetADCType(fADCType);

  if (fTCB->Open() != 0) {
    delete fTCB;
    return;
  }
  if (!fTCB->Config()) goto TERMINATE;
  if (!AddADC(fConfigList)) goto TERMINATE;
  if (!PrepareDAQ()) goto TERMINATE;
  if (!OpenDAQ()) goto TERMINATE;

  // thread
  th0 = std::thread(&CupDAQManager::TF_RunManager, this);
  th1 = std::thread(&CupDAQManager::TF_TriggerMon, this);
  th2 = std::thread(&CupDAQManager::TF_WriteData, this);
  th3 = std::thread(&CupDAQManager::TF_BuildEvent, this);
  th4 = std::thread(&CupDAQManager::TF_SortData, this);
  th5 = std::thread(&CupDAQManager::TF_ReadData, this);
  th6 = std::thread(&CupDAQManager::TF_SplitOutput, this, false);
  th7 = std::thread(&CupDAQManager::TF_ShrinkToFit, this);
  if (fVerboseLevel > 0) {
    th8 = std::thread(&CupDAQManager::TF_DebugMon, this);
  }

  gSystem->Sleep(1000);
  RUNSTATE::SetState(fStatus, RUNSTATE::kCONFIGURED);

  if (th8.joinable()) th8.join();
  if (th7.joinable()) th7.join();
  if (th6.joinable()) th6.join();
  if (th5.joinable()) th5.join();
  if (th4.joinable()) th4.join();
  if (th3.joinable()) th3.join();
  if (th2.joinable()) th2.join();
  if (th1.joinable()) th1.join();

  gSystem->Sleep(1000);
  RUNSTATE::SetState(fStatus, RUNSTATE::kPROCENDED);

  if (th0.joinable()) th0.join();

TERMINATE:
  CloseDAQ();

  fTCB->Close();
  delete fTCB;

  fLog->Info("CupDAQManager::RC_TCBDAQ", "TCBDAQ ended");
}

void CupDAQManager::RC_NullDAQ() {}
