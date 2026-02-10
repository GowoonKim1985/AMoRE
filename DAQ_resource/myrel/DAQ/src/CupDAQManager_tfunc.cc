#include "TMonitor.h"
#include "TRandom3.h"
#include "TServerSocket.h"

#include "OnlHistogramer/AbsHistogramer.hh"
#include "OnlHistogramer/FADCHistogramer.hh"
#include "OnlHistogramer/SADCHistogramer.hh"

#include "DAQ/CupDAQManager.hh"

using namespace std;

void CupDAQManager::TF_RunManager()
{
  fLog->Info("CupDAQManager::TF_RunManager", "run manager started");

  if (!WaitState(fStatus, RUNSTATE::kCONFIGURED)) { return; }

  fTCB->StartTrigger();
  time(&fStartDatime);
  RUNSTATE::SetState(fStatus, RUNSTATE::kRUNNING);

  bool iendrun = false;
  std::unique_lock<std::mutex> mlock(fMonitorMutex, std::defer_lock);

  while (true) {
    if (IsForcedEndRunFile()) {
      fLog->Info("CupDAQManager::TF_RunManager",
                 "daq will be ended by ENDRUN command");
      fDoEndRun = true;
      break;
    }

    if (fSetNEvent > 0 || fSetDAQTime > 0) {
      mlock.lock();
      int triggernumber = fTriggerNumber;
      double triggertime = fTriggerTime / kDONESECOND;
      mlock.unlock();

      if (triggernumber >= fSetNEvent) { iendrun = true; }
      if (triggertime >= fSetDAQTime) { iendrun = true; }
      if (iendrun) {
        fLog->Info("CupDAQManager::TF_RunManager",
                   "daq will be ended by preset condition");
        fDoEndRun = true;
        break;
      }
    }

    // if (fStatus == kERROR) {
    if (RUNSTATE::CheckError(fStatus)) {
      fLog->Info("CupDAQManager::TF_RunManager", "daq will be ended by error");
      fDoEndRun = true;
      break;
    }

    if (fDoEndRun) {
      fLog->Info("CupDAQManager::TF_RunManager",
                 "daq will be ended by ENDRUN command");
      break;
    }

    if (fDoSplitOutputFile) {
      if (!OpenNewOutputFile()) { RUNSTATE::SetError(fStatus); }
      fDoSplitOutputFile = false;
    }

    gSystem->Sleep(1);
  }

  fTCB->StopTrigger();
  time(&fEndDatime);
  RUNSTATE::SetState(fStatus, RUNSTATE::kRUNENDED);

  WaitState(fStatus, RUNSTATE::kPROCENDED, false);
  fLog->Info("CupDAQManager::TF_RunManager", "all processes ended");

  PrintDAQSummary();
  gSystem->Sleep(1000);

  fLog->Info("CupDAQManager::TF_RunManager", "run manger ended");
}

void CupDAQManager::TF_TriggerMon()
{
  if (!ThreadWait(fStatus, fDoExit)) { return; }

  fLog->Info("CupDAQManager::TF_TriggerMon", "trigger monitoring started");

  unsigned int dummynevent = 0;
  double dummytime = 0.0;

  std::unique_lock<std::mutex> mlock(fMonitorMutex, std::defer_lock);

  TStopwatch sw;
  sw.Start();

  while (true) {
    double elapsetime = sw.RealTime();
    sw.Continue();

    if (elapsetime > fTriggerMonTime) {
      sw.Start(true);

      mlock.lock();
      int triggernumber = fTriggerNumber;
      double triggertime = fTriggerTime / kDONESECOND;
      mlock.unlock();

      double dtime = triggertime - dummytime;
      double dnevent = triggernumber - dummynevent;

      double insrate = dtime > 0 ? dnevent / dtime : 0.;
      double accrate = triggertime > 0 ? triggernumber / triggertime : 0.;

      fLog->Info("TriggerMon",
                 "%4d events triggered [%8d | %8d / %3.1f(%3.1f) Hz / %.1f s]",
                 int(dnevent), triggernumber, fNBuiltEvent, insrate, accrate,
                 triggertime);

      dummynevent = triggernumber;
      dummytime = triggertime;
    }

    bool runstate = RUNSTATE::CheckState(fStatus, RUNSTATE::kRUNENDED) ||
                    RUNSTATE::CheckError(fStatus) || fDoExit;
    if (runstate) break;

    gSystem->Sleep(10);
  }

  fLog->Info("CupDAQManager::TF_TriggerMon", "trigger monitoring ended");
}

void CupDAQManager::TF_DebugMon()
{
  if (!ThreadWait(&fStatus, &fDoExit)) { return; }

  int nadc = GetEntries();
  double debugmontime = 1;

  fLog->Info("CupDAQManager::TF_DebugMon", "debug monitoring started");

  TStopwatch sw;
  sw.Start();

  while (true) {
    double elapsetime = sw.RealTime();
    sw.Continue();

    if (elapsetime > debugmontime) {
      sw.Start(true);

      TString adcbcountsize;
      TString adcbufsize;
      TString sortbufsize;

      for (int i = 0; i < nadc; i++) {
        adcbcountsize += Form("%5d ", fRemainingBCount[i]);
        auto * adc = (AbsADC *)fCont[i];
        adcbufsize += Form("%5d ", adc->Bsize());
        ConcurrentDeque<AbsADCRaw *> * modraw = fADCRawBuffers.at(i);
        sortbufsize += Form("%5d ", modraw->size());
      }

      fLog->Debug("DebugMon", "ADC bcount size: %s", adcbcountsize.Data());
      fLog->Debug("DebugMon", "ADC buffer size: %s", adcbufsize.Data());
      fLog->Debug("DebugMon", "sorting buffer size: %s", sortbufsize.Data());
      fLog->Debug("DebugMon", "building buffer size: %5d %5d",
                  fBuiltEventBuffer1.size(), fBuiltEventBuffer2.size());
      fLog->Debug("DebugMon", "%.3f(r) %.3f(s) %.3f(b) %.3f(w)",
                  fReadSleep / 1000., fSortSleep / 1000., fBuildSleep / 1000.,
                  fWriteSleep / 1000.);
    }

    bool runstate = RUNSTATE::CheckState(fStatus, RUNSTATE::kRUNENDED) ||
                    RUNSTATE::CheckError(fStatus) || fDoExit;
    if (runstate) break;

    gSystem->Sleep(10);
  }

  fLog->Info("CupDAQManager::TF_DebugMon", "debug monitoring ended");
}

void CupDAQManager::TF_MsgServer()
{
  unsigned short daqtag = 0;
  int port = 0;
  TString name;
  bool istcb = false;

  if (fDAQType == DAQ::TCB || fDAQType == DAQ::TEST) {
    daqtag = kTCBTAG;
    port = kTCBPORT;
    name = "TCB";
    istcb = true;
  }
  else {
    switch (fADCType) {
      case ADC::FADCT: {
        daqtag = kFADCDAQTAG;
        port = kFADCDAQPORT;
        name = "FADC";
        break;
      }
      case ADC::SADCT: {
        daqtag = kSADCDAQTAG;
        port = kSADCDAQPORT;
        name = "SADC";
        break;
      }
      case ADC::GADCT: {
        daqtag = kGADCDAQTAG;
        port = kGADCDAQPORT;
        name = "GADC";
        break;
      }
      case ADC::IADCT: {
        daqtag = kIADCDAQTAG;
        port = kIADCDAQPORT;
        name = "IADC";
        break;
      }
      default: break;
    }
  }

  auto * server = new TServerSocket(port, true);
  auto * monitor = new TMonitor();
  monitor->Add(server);

  fLog->Info("CupDAQManager::TF_MsgServer", "[%s] message server start",
             name.Data());

  char buffer[kMESSLEN];
  std::unique_lock<std::mutex> mlock(fMonitorMutex, std::defer_lock);

  while (true) {
    if (!istcb) {
      if (fStatus == kDOWN || fDoExit) { break; }
    }
    else {
      if (fStatusTCB == kDOWN || fDoExitTCB) { break; }
    }

    TSocket * socket = monitor->Select(100);
    if (socket == (TSocket *)-1) continue;

    TInetAddress addr = socket->GetInetAddress();

    if (socket->IsA() == TServerSocket::Class()) {
      TSocket * s = ((TServerSocket *)socket)->Accept();
      monitor->Add(s);

      addr = s->GetInetAddress();
      fLog->Info("CupDAQManager::TF_MsgServer",
                 "[%s] new client connected [ip=%s, port=%d]", name.Data(),
                 addr.GetHostAddress(), addr.GetPort());
      continue;
    }

    int stat = socket->RecvRaw(buffer, kMESSLEN);

    if (stat <= 0) {
      if (stat == 0 || stat == -5) {
        fLog->Info("CupDAQManager::TF_MsgServer",
                   "[%s] client disconnected [ip=%s, port=%d]", name.Data(),
                   addr.GetHostAddress(), addr.GetPort());
      }
      else {
        fLog->Warning("CupDAQManager::TF_MsgServer",
                      "[%s] received error (%d) [ip=%s, port=%d]", name.Data(),
                      stat, addr.GetHostAddress(), addr.GetPort());
      }
      socket->Close();
      monitor->Remove(socket);
      delete socket;
    }
    else {
      unsigned short from, to;
      unsigned long command, dummy;
      DecodeMsg(buffer, from, to, command, dummy);

      if (command == kQUERYDAQSTATUS) {
        EncodeMsg(buffer, daqtag, from, istcb ? fStatusTCB : fStatus);
        socket->SendRaw(buffer, kMESSLEN);
      }
      else if (command == kQUERYRUNINFO) {
        EncodeMsg(buffer, daqtag, from, fRunNumber, fSubRunNumber);
        socket->SendRaw(buffer, kMESSLEN);

        EncodeMsg(buffer, daqtag, from, fStartDatime, fEndDatime);
        socket->SendRaw(buffer, kMESSLEN);
      }
      else if (command == kQUERYTRGINFO) {
        unsigned long nevent, daqtime;

        mlock.lock();
        nevent = fTriggerNumber;
        daqtime = fCurrentTime;
        mlock.unlock();

        EncodeMsg(buffer, daqtag, from, nevent, daqtime);
        socket->SendRaw(buffer, kMESSLEN);
      }
      else if (command == kREQUESTCONFIG) {
        socket->SendObject(fConfigList);
        fLog->Info("CupDAQManager::TF_MsgServer",
                   "[%s] sent config list to DAQ", name.Data());
      }
      else if (command == kSPLITOUTPUTFILE) {
        if (istcb) { fDoSplitOutputFileTCB = true; }
        else {
          fDoSplitOutputFile = true;
        }
        fLog->Info("CupDAQManager::TF_MsgServer",
                   "[%s] output file split command received", name.Data());
      }
      else if (command == kCONFIGRUN) {
        if (istcb) { fDoConfigRunTCB = true; }
        else {
          fDoConfigRun = true;
        }
        fLog->Info("CupDAQManager::TF_MsgServer",
                   "[%s] CONFIGRUN command received", name.Data());
      }
      else if (command == kSTARTRUN) {
        if (istcb) { fDoStartRunTCB = true; }
        else {
          fDoStartRun = true;
        }
        fLog->Info("CupDAQManager::TF_MsgServer",
                   "[%s] STARTRUN command received", name.Data());
      }
      else if (command == kENDRUN) {
        if (istcb) { fDoEndRunTCB = true; }
        else {
          fDoEndRun = true;
        }
        fLog->Info("CupDAQManager::TF_MsgServer",
                   "[%s] ENDRUN command received", name.Data());
      }
      else if (command == kEXIT) {
        if (istcb) { fDoExitTCB = true; }
        else {
          fDoExit = true;
        }
        fLog->Info("CupDAQManager::TF_MsgServer", "[%s] EXIT command received",
                   name.Data());
      }
      else {
        fLog->Warning("CupDAQManager::TF_MsgServer",
                      "[%s] Unknown command [%ld] received", name.Data(),
                      command);
      }
    }
  }

  monitor->DeActivateAll();
  TList * sockets = monitor->GetListOfDeActives();
  for (int i = 0; i < sockets->GetSize(); i++) {
    ((TSocket *)(sockets->At(i)))->Close();
    delete ((TSocket *)(sockets->At(i)));
  }

  monitor->RemoveAll();
  delete monitor;

  fLog->Info("CupDAQManager::TF_MsgServer", "[%s] message server ended",
             name.Data());
}

void CupDAQManager::TF_ShrinkToFit()
{
  if (!ThreadWait(&fStatus, &fDoExit)) { return; }

  fLog->Info("CupDAQManager::TF_ShrinkToFit",
             "shrink buffer memory to fit started");

  TStopwatch sw;
  sw.Start();

  int nadc = GetEntries();
  while (true) {
    double elapsetime = sw.RealTime();
    sw.Continue();
    if (elapsetime >= 10) {
      sw.Start(true);

      for (int i = 0; i < nadc; i++) {
        auto * adc = (AbsADC *)fCont[i];
        adc->Bshrink_to_fit();

        ConcurrentDeque<AbsADCRaw *> * modbuffer = fADCRawBuffers.at(i);
        modbuffer->shrink_to_fit();
      }

      fBuiltEventBuffer1.shrink_to_fit();
      fBuiltEventBuffer2.shrink_to_fit();

      if (fVerboseLevel > 0) {
        fLog->Info("CupDAQManager::TF_ShrinkToFit",
                   "shrink buffer memory to fit");
      }
    }

    // if (fDoEndRun || fStatus == kERROR || fDoExit) {
    //   break;
    // }
    bool runstate = RUNSTATE::CheckState(fStatus, RUNSTATE::kRUNENDED) ||
                    RUNSTATE::CheckError(fStatus) || fDoExit;
    if (runstate) break;

    gSystem->Sleep(1000);
  }

  fLog->Info("CupDAQManager::TF_ShrinkToFit",
             "shrink buffer memory to fit ended");
}

/*
void CupDAQManager::TF_SplitOutput(bool ontcb)
{
  if (!ThreadWait(ontcb)) {
    return;
  }

  fLog->Info("CupDAQManager::TF_SplitOutput", "output splitter started");

  TStopwatch sw;
  sw.Start();

  while (true) {
    double elapsetime = sw.RealTime();
    sw.Continue();
    if (elapsetime >= fOutputSplitTime) {
      sw.Start(true);

      if (ontcb) {
        SendCommandToDAQ(kSPLITOUTPUTFILE);
      } else {
        fDoSplitOutputFile = true;
      }

      fSubRunNumber += 1;

      if (fVerboseLevel >= 1) {
        fLog->Info("CupDAQManager::TF_SplitOutput", "output file will be
split");
      }
    }

    if (ontcb) {
      if (fDoEndRunTCB || fStatusTCB == kERROR) {
        break;
      }
    } else {
      if (fDoEndRun || fStatus == kERROR || fDoExit) {
        break;
      }
    }

    gSystem->Sleep(10);
  }

  fLog->Info("CupDAQManager::TF_SplitOutput", "output splitter ended");
}
*/
void CupDAQManager::TF_SplitOutput(bool ontcb)
{
  TStopwatch sw;

  if (ontcb) {
    if (!ThreadWait(&fStatusTCB, &fDoExitTCB)) { return; }
    fLog->Info("CupDAQManager::TF_SplitOutput", "output splitter started");

    sw.Start();
    while (true) {
      double elapsetime = sw.RealTime();
      sw.Continue();
      if (elapsetime >= fOutputSplitTime) {
        sw.Start(true);
        SendCommandToDAQ(kSPLITOUTPUTFILE);
        fSubRunNumber += 1;

        if (fVerboseLevel >= 1) {
          fLog->Info("CupDAQManager::TF_SplitOutput",
                     "output file will be split");
        }
      }
      if (fDoEndRunTCB || fStatusTCB == kERROR) { break; }
      gSystem->Sleep(10);
    }
  }
  else {
    if (!ThreadWait(&fStatus, &fDoExit)) { return; }
    fLog->Info("CupDAQManager::TF_SplitOutput", "output splitter started");

    sw.Start();
    while (true) {
      double elapsetime = sw.RealTime();
      sw.Continue();
      if (elapsetime >= fOutputSplitTime) {
        sw.Start(true);
        fDoSplitOutputFile = true;
        fSubRunNumber += 1;

        if (fVerboseLevel >= 1) {
          fLog->Info("CupDAQManager::TF_SplitOutput",
                     "output file will be split");
        }
      }
      bool runstate = RUNSTATE::CheckState(fStatus, RUNSTATE::kRUNENDED) ||
                      RUNSTATE::CheckError(fStatus) || fDoExit;
      if (runstate) break;

      gSystem->Sleep(10);
    }
  }

  fLog->Info("CupDAQManager::TF_SplitOutput", "output splitter ended");
}

void CupDAQManager::TF_Histogramer()
{
  if (!ThreadWait()) { return; }

  fLog->Info("CupDAQManager::TF_Histogramer", "histogramer started");

  //
  // Create histogramer
  //
  AbsHistogramer * histogramer = nullptr;
  switch (fADCType) {
    case ADC::FADCT: histogramer = new FADCHistogramer(); break;
    case ADC::FADCS: histogramer = new FADCHistogramer(); break;
    case ADC::GADCT: histogramer = new FADCHistogramer(); break;
    case ADC::GADCS: histogramer = new FADCHistogramer(); break;
    case ADC::MADCS: histogramer = new FADCHistogramer(); break;
    case ADC::SADCT: histogramer = new SADCHistogramer(); break;
    case ADC::SADCS: histogramer = new SADCHistogramer(); break;
    default: break;
  }
  histogramer->SetRunNumber(fRunNumber);
  histogramer->SetADCType(fADCType);
  histogramer->SetConfigList(fConfigList);
  histogramer->SetStartDatime(fStartDatime);

  // open histogramer root file
  if (fHistFilename.IsNull()) {
    TString filename;
    TString dirname = gSystem->Getenv("RAWDATA_DIR");
    if (dirname.IsNull()) {
      fLog->Warning("CupDAQManager::TF_Histogramer",
                    "variable RAWDATA_DIR is not set");
      filename = Form("hist_%s_%06d.root", GetADCName(fADCType), fRunNumber);
    }
    else {
      dirname += Form("/HIST/%06d", fRunNumber);
      int isdir = gSystem->Exec(Form("test -d %s", dirname.Data()));
      if (!isdir) {
        gSystem->Exec(Form("mkdir %s", dirname.Data()));
        fLog->Info("CupDAQManager::TF_Histogramer", "%s created",
                   dirname.Data());
      }
      fLog->Info("CupDAQManager::TF_Histogramer", "%s already exist",
                 dirname.Data());

      filename = Form("%s/hist_%s_%06d.root", dirname.Data(),
                      GetADCName(fADCType), fRunNumber);
    }
    histogramer->SetFilename(filename);
    fHistFilename = filename;
  }

  if (!histogramer->Open()) {
    fLog->Warning(
        "CupDAQManager::TF_Histogramer",
        "Can\'t open histogramer root file %s, histogramer will be ended",
        fHistFilename.Data());
    return;
  }

  // booking histograms
  histogramer->Book();

  int eventnumber = 0;
  int ntotalmonitoredevent = 0;

  double trate = 0.3;

  double perror = 0;
  double integral = 0;

  fBenchmark->Start("Histogramer");
  while (true) {
    if (fBuildState == ENDED) {
      if (fBuiltEventBuffer2.empty()) break;
    }

    if (fStatus == kERROR || fDoExit) { break; }

    BuiltEvent * builtevent = nullptr;

    int size = fBuiltEventBuffer2.size();

    if (size > 0) { builtevent = fBuiltEventBuffer2.popfront(); }

    if (builtevent) {
      eventnumber = builtevent->GetEventNumber();
      if (gRandom->Rndm() < trate) {
        histogramer->Fill(builtevent);
        histogramer->Update();
        ntotalmonitoredevent += 1;
      }
      delete builtevent;
    }

    ThreadSleep(fHistSleep, perror, integral, size);
  }
  fBenchmark->Stop("Histogramer");

  histogramer->Close();
  delete histogramer;

  fHistogramerEnded = true;

  fLog->Info("CupDAQManager::TF_Histogramer",
             "total monitored event = %d (%.2f%%)", ntotalmonitoredevent,
             100. * ntotalmonitoredevent / double(eventnumber));
  fLog->Info("CupDAQManager::TF_Histogramer", "online histogramer ended");
}

/*
void CupDAQManager::TF_MsgServer(bool amoreadc)
{
  unsigned short daqtag = 0;
  int port = 0;
  TString name;
  bool istcb = false;

  if (fDAQType == DAQ::TCB ||
      fDAQType == DAQ::AMORETCB ||
      fDAQType == DAQ::TEST) {
    daqtag = kTCBTAG;
    port = kTCBPORT;
    name = "TCB";
    istcb = true;
  }
  else {
    switch (fADCType) {
      case ADC::FADCT: {
        daqtag = kFADCDAQTAG;
        port = kFADCDAQPORT;
        name = "FADC";
        break;
      }
      case ADC::SADCT: {
        daqtag = kSADCDAQTAG;
        port = kSADCDAQPORT;
        name = "SADC";
        break;
      }
      case ADC::GADCT: {
        daqtag = kGADCDAQTAG;
        port = kGADCDAQPORT;
        name = "GADC";
        break;
      }
      default: break;
    }
  }

  if (amoreadc) {
    daqtag = kAMOREADCDAQTAG;
    port = kAMOREADCDAQPORT;
    name = "AMOREADC";
    istcb = false;
  }

  fLog->Info("CupDAQManager::TF_Monitor", "[%s] message server start",
name.Data());

  TStopwatch bench;
  bench.Start();

  const int max_clients = 10;

  int client_socket[max_clients];
  auto ** root_socket = new TSocket * [max_clients];
  for (int i = 0; i < max_clients; i++) {
    client_socket[i] = -1;
    root_socket[i] = nullptr;
  }

  int master_socket;
  if ((master_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    fLog->Error("CupDAQManager::TF_Monitor", "[%s] socket failed", name.Data());
    istcb ? fStatusTCB = kERROR : fStatus = kERROR;
    return;
  }

  int opt = true;
  if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR, (char *)&opt,
sizeof(opt)) < 0) { fLog->Error("CupDAQManager::TF_Monitor", "[%s] setsockopt
failed", name.Data()); istcb ? fStatusTCB = kERROR : fStatus = kERROR; return;
  }

  struct sockaddr_in address;
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(port);

  int addrlen = sizeof(address);

  if (bind(master_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
    fLog->Error("CupDAQManager::TF_Monitor", "[%s] socket bind failed on port
%d", name.Data(), port); istcb ? fStatusTCB = kERROR : fStatus = kERROR; return;
  }
  fLog->Info("CupDAQManager::TF_Monitor", "[%s] message server on port %d",
name.Data(), port);

  if (listen(master_socket, 3) < 0) {
    fLog->Error("CupDAQManager::TF_Monitor", "[%s] socket listen error",
name.Data()); istcb ? fStatusTCB = kERROR : fStatus = kERROR; return;
  }

  char buffer[kMESSLEN];

  fLog->Info("CupDAQManager::TF_Monitor", "[%s] waiting for client connections
...", name.Data());

  struct timeval tv;
  fd_set readfds;

  while (true) {
    if (!istcb) {
      if (fStatus == kDOWN || fDoExit) {
        break;
      }
    }
    else {
      if (fStatusTCB == kDOWN || fDoExitTCB) {
        break;
      }
    }

    FD_ZERO(&readfds);
    FD_SET(master_socket, &readfds);

    int max_sd = master_socket;

    for (int sd : client_socket) {
      if (sd > 0)
        FD_SET(sd, &readfds);

      if (sd > max_sd) {
        max_sd = sd;
      }
    }

    tv.tv_sec = 1;
    tv.tv_usec = 0;

    int activity = select(max_sd + 1, &readfds, nullptr, nullptr, &tv);
    if (activity == 0) {
      continue;
    }
    else if ((activity < 0) && (errno != EINTR)) {
      //fLog->Warning("[%s] select error occurred", name.Data());
      continue;
    }

    if (FD_ISSET(master_socket, &readfds)) {
      int new_socket = accept(master_socket, (struct sockaddr *)&address,
(socklen_t *)&addrlen);

      if (new_socket < 0) {
        fLog->Error("CupDAQManager::TF_Monitor", "[%s] accept error occurred",
name.Data()); istcb ? fStatusTCB = kERROR : fStatus = kERROR; return;
      }

      fLog->Info("CupDAQManager::TF_Monitor", "[%s] new client connection,
socket fd: %d, ip: %s, port: %d", name.Data(), new_socket,
                 inet_ntoa(address.sin_addr),
                 ntohs(address.sin_port));

      for (int i = 0; i < max_clients; i++) {
        if (client_socket[i] == -1) {
          client_socket[i] = new_socket;
          root_socket[i] = new TSocket(new_socket);
          fLog->Info("CupDAQManager::TF_Monitor", "[%s] adding to list of
sockets at %d", name.Data(), i); break;
        }
      }
    }

    for (int i = 0; i < max_clients; i++) {
      int sd = client_socket[i];

      if (FD_ISSET(sd, &readfds)) {
        memset(buffer, 0, kMESSLEN);
        int valread = read(sd, buffer, kMESSLEN);

        if (valread == 0) {
          getpeername(sd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
          fLog->Info("CupDAQManager::TF_Monitor", "[%s] host disconnected,
socket fd: %d, ip: %s, port: %d", name.Data(), sd, inet_ntoa(address.sin_addr),
                     ntohs(address.sin_port));

          close(sd);
          client_socket[i] = -1;
          delete root_socket[i];
          root_socket[i] = nullptr;
        }
        else {
          unsigned short from, to;
          unsigned long command, dummy;
          DecodeMsg(buffer, from, to, command, dummy);

          if (command == kQUERYDAQSTATUS) {
            EncodeMsg(buffer, daqtag, from, istcb ? fStatusTCB : fStatus);
            send(sd, buffer, kMESSLEN, 0);
          }
          else if (command == kQUERYRUNINFO) {
            EncodeMsg(buffer, daqtag, from, fRunNumber, fSubRunNumber);
            send(sd, buffer, kMESSLEN, 0);

            EncodeMsg(buffer, daqtag, from, fStartDatime, fEndDatime);
            send(sd, buffer, kMESSLEN, 0);
          }
          else if (command == kQUERYRUNSTATS) {
            for (auto adc : fDAQ) {
              std::unique_lock<std::mutex> lock(fMonitorMutex);
              unsigned long nadc = fTriggerNumberTCB[adc - 1];
              unsigned long tadc = fTriggerTimeTCB[adc - 1];
              lock.unlock();
              EncodeMsg(buffer, daqtag, from, nadc, tadc);
              send(sd, buffer, kMESSLEN, 0);
            }
          }
          else if (command == kQUERYTRGINFO) {
            unsigned long nevent, daqtime;

            std::unique_lock<std::mutex> lock1(fMonitorMutex);
            nevent = fTriggerNumber;
            daqtime = fCurrentTime;
            lock1.unlock();

            EncodeMsg(buffer, daqtag, from, nevent, daqtime);
            send(sd, buffer, kMESSLEN, 0);
          }
          else if (command == kQUERYCHTRGINFO) {
            //std::unique_lock<std::mutex> lock1(fMonitorMutex);
            //for (int j = 0; j < AMORE::kNCRYSTAL; j++) {
            //  EncodeMsg(buffer, daqtag, from, fTriggerNumberChannel[j],
fTriggerTime);
            //  send(sd, buffer, kMESSLEN, 0);
            //}
            //lock1.unlock();
          }
          else if (command == kQUERYANEVENT) {
            EncodeMsg(buffer, daqtag, from, fStatus);
            send(sd, buffer, kMESSLEN, 0);

            if (fStatus == kRUNNING) {
              BuiltEvent * event = fBuiltEventBuffer.front(true, 100);
              root_socket[i]->SendObject(event);
            }
          }
          else if (command == kREQUESTCONFIG) {
            root_socket[i]->SendObject(fConfigList);
            fLog->Info("CupDAQManager::TF_Monitor", "[%s] sent config list to
DAQ", name.Data());
          }
          else if (command == kSPLITOUTPUTFILE) {
            if (istcb) { fDoSplitOutputFileTCB = true; }
            else { fDoSplitOutputFile = true; }
            fLog->Info("CupDAQManager::TF_Monitor", "[%s] output file split
command received", name.Data());
          }
          else if (command == kCONFIGRUN) {
            if (istcb) { fDoConfigRunTCB = true; }
            else { fDoConfigRun = true; }
            fLog->Info("CupDAQManager::TF_Monitor", "[%s] CONFIGRUN command
received", name.Data());
          }
          else if (command == kSTARTRUN) {
            if (istcb) { fDoStartRunTCB = true; }
            else { fDoStartRun = true; }
            fLog->Info("CupDAQManager::TF_Monitor", "[%s] STARTRUN command
received", name.Data());
          }
          else if (command == kENDRUN) {
            if (istcb) { fDoEndRunTCB = true; }
            else { fDoEndRun = true; }
            fLog->Info("CupDAQManager::TF_Monitor", "[%s] ENDRUN command
received", name.Data());
          }
          else if (command == kEXIT) {
            if (istcb) { fDoExitTCB = true; }
            else { fDoExit = true; }
            fLog->Info("CupDAQManager::TF_Monitor", "[%s] EXIT command
received", name.Data());
          }
          else {
            fLog->Warning("CupDAQManager::TF_Monitor", "[%s] Unknown command
[%ld] received from [%d], %d will be close", name.Data(), command, sd, sd);
close(sd); client_socket[i] = -1; delete root_socket[i]; root_socket[i] =
nullptr;
          }
        }
      }
    }

    gSystem->Sleep(10);
  }
  close(master_socket);

  for (int i = 0; i < max_clients; i++) {
    if (root_socket[i]) {
      delete root_socket[i];
    }
  }
  delete[] root_socket;

  bench.Stop();
  fLog->Info("CupDAQManager::TF_Monitor", "real time=%.2f cpu time=%.2f" ,
bench.RealTime(), bench.CpuTime());

  fLog->Info("CupDAQManager::TF_Monitor", "[%s] message server ended",
name.Data());
}
*/
