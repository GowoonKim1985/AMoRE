#include "DAQ/CupDAQManager.hh"

using namespace std;

void CupDAQManager::PrintDAQSummary()
{
  const char * pname[4] = {"ReadData", "SortData", "BuildEvent", "WriteData"};

  cout << endl;
  cout << "====================================================" << endl;
  cout << Form("%12s %16s %16s", "process", "cpu time[s]", "real time[s]")
       << endl;
  cout << "----------------------------------------------------" << endl;
  double rt = 0, ct = 0;
  for (int i = 0; i < 4; i++) {
    double cputime = fBenchmark->GetCpuTime(pname[i]);
    double realtime = fBenchmark->GetRealTime(pname[i]);
    cout << Form("%12s %16.1f %16.1f", pname[i], cputime, realtime) << endl;
    ct += cputime;
    rt += realtime;
  }
  cout << "----------------------------------------------------" << endl;
  cout << Form("%12s %16.1f %16.1f", "total", ct, rt) << endl;
  cout << "====================================================" << endl;
  cout << endl;

  int nadc = GetEntries();
  auto * theADC = (AbsADC *)fCont[0];

  unsigned long totalReadDataSize =
      nadc * theADC->GetTotalBCount() * kKILOBYTES;

  double liveTime = theADC->GetCurrentTime() / kDONESECOND;
  double recvDataSize = totalReadDataSize / kDGIGABYTES;
  double outputDataSize = fTotalWrittenDataSize / kDGIGABYTES;

  double trate = liveTime > 0 ? fTriggerNumber / liveTime : 0;
  double drate = liveTime > 0 ? recvDataSize * 1024 / liveTime : 0;
  double orate = liveTime > 0 ? outputDataSize * 1024 / liveTime : 0;

  cout << endl;
  cout << "************************* DAQ Summary *************************"
       << endl;
  cout << Form("%32s", "Run number : ") << fRunNumber << endl;
  cout << Form("%32s", "Start Time : ") << TDatime(fStartDatime).AsSQLString()
       << endl;
  cout << Form("%32s", "End Time : ") << TDatime(fEndDatime).AsSQLString()
       << endl;
  cout << endl;
  cout << Form("%32s", "Live time : ") << Form("%.1f", liveTime) << " [s]"
       << endl;
  cout << Form("%32s", "Total number of trigger : ")
       << Form("%d", fTriggerNumber) << endl;
  cout << Form("%32s", "Trigger rate : ") << Form("%.2f", trate) << " [Hz]"
       << endl;
  cout << Form("%32s", "Total number of event : ") << Form("%d", fNBuiltEvent)
       << endl;
  cout << Form("%32s", "Software Trigger efficiency : ")
       << Form("%5.2f [%%]", fSoftTrigger->GetEfficiency()) << endl;
  cout << endl;
  cout << Form("%32s", "Received data size : ")
       << Form("%.3f GBytes (%.3f MB/sec)", recvDataSize, drate) << endl;
  cout << Form("%32s", "Written data size : ")
       << Form("%.3f GBytes (%.3f MB/sec)", outputDataSize, orate) << endl;
  cout << "***************************************************************"
       << endl;
  cout << endl;
}

bool CupDAQManager::ThreadWait(bool ontcb) const
{
  if (!ontcb) {
    while (true) {
      if (RUNSTATE::CheckState(fStatus, RUNSTATE::kRUNNING)) { break; }
      else if (RUNSTATE::CheckError(fStatus)) {
        return false;
      }
      if (fDoExit) { return false; }
      gSystem->Sleep(10);
    }
  }
  else {
    while (true) {
      if (fStatusTCB == kRUNNING) { break; }
      else if (fStatusTCB == kERROR) {
        return false;
      }
      if (fDoExitTCB) { return false; }
      gSystem->Sleep(10);
    }
  }

  return true;
}

bool CupDAQManager::ThreadWait(unsigned long & state, bool & exit) const
{
  while (true) {
    if (RUNSTATE::CheckState(state, RUNSTATE::kRUNNING)) { break; }
    else if (RUNSTATE::CheckError(state)) {
      return false;
    }
    if (exit) { return false; }
    gSystem->Sleep(10);
  }

  return true;
}

bool CupDAQManager::ThreadWait(unsigned long * state, bool * exit) const
{
  while (true) {
    if (RUNSTATE::CheckState(*state, RUNSTATE::kRUNNING)) { break; }
    else if (RUNSTATE::CheckError(*state)) {
      return false;
    }
    if (*exit) { return false; }
    gSystem->Sleep(10);
  }

  return true;
}

void CupDAQManager::ThreadSleep(int & sleep, double & perror, double & integral,
                                int size, int tsize)
{
  if (!RUNSTATE::CheckState(fStatus, RUNSTATE::kRUNNING)) return;

  const double Kp = sleep / 20.;
  const double Kd = sleep / 10.;
  const double Ki = 0.01;
  const double dt = 1;

  const double max = 1000000;
  const double min = -1000000;

  double error = tsize - size;
  double Pout = Kp * error;

  integral += error * dt;
  double Iout = Ki * integral;

  double derivative = (error - perror) / dt;
  double Dout = Kd * derivative;

  double output = Pout + Iout + Dout;
  output = std::max(min, std::min(max, output));

  sleep += int(output);
  if (sleep < 0) sleep = 0;
  perror = error;

  std::this_thread::sleep_for(std::chrono::microseconds(sleep));
}

unsigned long CupDAQManager::QueryDAQStatus(unsigned long daqtag,
                                            TSocket * socket) const
{
  char data[kMESSLEN];
  unsigned short from, to;
  unsigned long mess1, mess2;

  EncodeMsg(data, kTCBTAG, daqtag, kQUERYDAQSTATUS);
  if (socket->SendRaw(data, kMESSLEN) < 0) { return 0; }
  memset(data, 0, kMESSLEN);
  if (socket->RecvRaw(data, kMESSLEN) < 0) { return 0; }
  DecodeMsg(data, from, to, mess1, mess2);

  return mess1;
}

void CupDAQManager::SendCommandToDAQ(unsigned long cmd) const
{
  char data[kMESSLEN];
  EncodeMsg(data, kTCBTAG, 0, cmd);

  for (auto * socket : fDAQSocket) {
    socket->SendRaw(data, kMESSLEN);
  }
}

bool CupDAQManager::WaitDAQStatus(unsigned long status) const
{
  while (true) {
    bool totalstate = true;
    for (auto * socket : fDAQSocket) {
      unsigned long daqstate = QueryDAQStatus(0, socket);
      if (daqstate == 0) {
        fLog->Error("CupDAQManager::WaitDAQStatus", "%s connection down",
                    socket->GetName());
        return false;
      }
      if (daqstate == kERROR) {
        fLog->Error("CupDAQManager::WaitDAQStatus", "%s got error",
                    socket->GetName());
        return false;
      }
      if (daqstate != status) { totalstate &= false; }
    }
    if (totalstate) break;

    gSystem->Sleep(10);
  }
  return true;
}

bool CupDAQManager::IsDAQRunning() const
{
  bool retval = true;
  for (auto * socket : fDAQSocket) {
    unsigned long daqstate = QueryDAQStatus(0, socket);
    if (daqstate == 0) {
      fLog->Error("%s connection down", socket->GetName());
      retval = false;
    }
    else if (daqstate != kRUNNING) {
      fLog->Error("%s is not running", socket->GetName());
      retval = false;
    }
  }
  return retval;
}

bool CupDAQManager::IsDAQFail(TSocket * socket) const
{
  unsigned long daqstate = QueryDAQStatus(0, socket);
  if (daqstate == 0) {
    fLog->Error("%s connection down", socket->GetName());
    return true;
  }
  else if (daqstate == kERROR) {
    fLog->Error("%s got error", socket->GetName());
    return true;
  }

  return false;
}

int CupDAQManager::Wait(bool * isgo, unsigned long * status) const
{
  while (true) {
    if (*isgo) break;
    if (*status == kERROR) return 0;
    gSystem->Sleep(10);
  }

  return 1;
}

int CupDAQManager::Wait(unsigned long * status, unsigned long value)
{
  while (true) {
    if (*status == value) return 1;
    if (*status == kERROR) return 0;
    gSystem->Sleep(10);
  }
}

bool CupDAQManager::WaitState(unsigned long & status, RUNSTATE::STATE pstate,
                              bool errorexit) const
{
  while (true) {
    if (RUNSTATE::CheckState(status, pstate)) break;
    if (errorexit && RUNSTATE::CheckError(status)) return false;
    gSystem->Sleep(10);
  }
  return true;
}

int CupDAQManager::WaitUntil(bool * isgo) const
{
  while (true) {
    for (auto * socket : fDAQSocket) {
      if (IsDAQFail(socket)) return -1;
    }
    if (*isgo) break;
    gSystem->Sleep(10);
  }

  return 0;
}

int CupDAQManager::WaitUntil(bool * isgo, bool * exit) const
{
  while (true) {
    for (auto * socket : fDAQSocket) {
      if (IsDAQFail(socket)) return -1;
    }
    if (*isgo) break;
    if (*exit) return 1;
    gSystem->Sleep(10);
  }

  return 0;
}

int CupDAQManager::WaitUntil(bool * isgo, unsigned long * status) const
{
  while (true) {
    for (auto * socket : fDAQSocket) {
      if (IsDAQFail(socket)) return -1;
    }
    if (*isgo) break;
    if (*status == kERROR) return 1;
    gSystem->Sleep(10);
  }

  return 0;
}

void CupDAQManager::EncodeMsg(char * buffer, unsigned short from,
                              unsigned short to, unsigned long message1,
                              unsigned long message2) const
{
  memset(buffer, 0, kMESSLEN);

  buffer[0] = from & 0xFF;
  buffer[1] = (from >> 8) & 0xFF;

  buffer[2] = to & 0xFF;
  buffer[3] = (to >> 8) & 0xFF;

  buffer[4] = message1 & 0xFF;
  buffer[5] = (message1 >> 8) & 0xFF;
  buffer[6] = (message1 >> 16) & 0xFF;
  buffer[7] = (message1 >> 24) & 0xFF;
  buffer[8] = (message1 >> 32) & 0xFF;
  buffer[9] = (message1 >> 40) & 0xFF;
  buffer[10] = (message1 >> 48) & 0xFF;
  buffer[11] = (message1 >> 56) & 0xFF;

  buffer[12] = message2 & 0xFF;
  buffer[13] = (message2 >> 8) & 0xFF;
  buffer[14] = (message2 >> 16) & 0xFF;
  buffer[15] = (message2 >> 24) & 0xFF;
  buffer[16] = (message2 >> 32) & 0xFF;
  buffer[17] = (message2 >> 40) & 0xFF;
  buffer[18] = (message2 >> 48) & 0xFF;
  buffer[19] = (message2 >> 56) & 0xFF;
}

void CupDAQManager::DecodeMsg(char * buffer, unsigned short & from,
                              unsigned short & to, unsigned long & message1,
                              unsigned long & message2) const
{
  from = buffer[0];
  from += (buffer[1] << 8);

  to = buffer[2];
  to += (buffer[3] << 8);

  message1 = buffer[4] & 0xFF;
  message1 += (unsigned long)(buffer[5] & 0xFF) << 8;
  message1 += (unsigned long)(buffer[6] & 0xFF) << 16;
  message1 += (unsigned long)(buffer[7] & 0xFF) << 24;
  message1 += (unsigned long)(buffer[8] & 0xFF) << 32;
  message1 += (unsigned long)(buffer[9] & 0xFF) << 40;
  message1 += (unsigned long)(buffer[10] & 0xFF) << 48;
  message1 += (unsigned long)(buffer[11] & 0xFF) << 56;

  message2 = buffer[12] & 0xFF;
  message2 += (unsigned long)(buffer[13] & 0xFF) << 8;
  message2 += (unsigned long)(buffer[14] & 0xFF) << 16;
  message2 += (unsigned long)(buffer[15] & 0xFF) << 24;
  message2 += (unsigned long)(buffer[16] & 0xFF) << 32;
  message2 += (unsigned long)(buffer[17] & 0xFF) << 40;
  message2 += (unsigned long)(buffer[18] & 0xFF) << 48;
  message2 += (unsigned long)(buffer[19] & 0xFF) << 56;
}

bool CupDAQManager::IsForcedEndRunFile(bool useRC) const
{
  if (!useRC) {
    std::ifstream status;
    status.open(kFORCEDENDRUNFILE);
    if (status.is_open()) {
      status.close();
      return true;
    }
  }
  return false;
}

void CupDAQManager::StartBenchmark(const char * name)
{
  std::lock_guard<std::mutex> lock(fBenchmarkMutex);
  fBenchmark->Start(name);
}

void CupDAQManager::StopBenchmark(const char * name)
{
  std::lock_guard<std::mutex> lock(fBenchmarkMutex);
  fBenchmark->Stop(name);
}