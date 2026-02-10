/*
 *
 *  Module:  CupDAQManager
 *
 *  Author:  Jaison Lee
 *
 *  Purpose: DAQ helper class to simplify read data from ADCs
 *
 *  Last Update:      $Author: cupsoft $
 *  Update Date:      $Date: 2022/12/21 00:02:44 $
 *  CVS/RCS Revision: $Revision: 1.10 $
 *  Status:           $State: Exp $
 *
 */

#ifndef CupDAQManager_hh
#define CupDAQManager_hh

#include <iostream>
#include <mutex>
#include <time.h>
#include <vector>

#include "TBenchmark.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TSocket.h"
#include "TString.h"
#include "TTree.h"

#include "DAQ/CupGeneralTCB.hh"
#include "DAQConfig/AbsConf.hh"
#include "DAQConfig/AbsConfList.hh"
#include "DAQSystem/AbsADC.hh"
#include "DAQTrigger/AbsSoftTrigger.hh"
#include "DAQUtils/ConcurrentDeque.hh"
#include "DAQUtils/ELogger.hh"
#include "HDF5Utils/AbsH5Event.hh"
#include "HDF5Utils/H5DataWriter.hh"
#include "Notice/usb3comroot.hh"
#include "OnlConsts/adcconsts.hh"
#include "OnlConsts/onlconsts.hh"
#include "OnlObjs/AbsADCRaw.hh"
#include "OnlObjs/BuiltEvent.hh"

class CupDAQManager : public TObjArray {
public:
  enum PROCSTATE { NONE, READY, RUNNING, ENDED, ERROR };

  CupDAQManager();
  virtual ~CupDAQManager();

  void SetRunNumber(int run);
  void SetDAQId(int id);
  void SetDAQType(DAQ::TYPE type);
  void SetADCType(ADC::TYPE type);
  void SetTriggerMode(TRIGGER::MODE mode);
  void SetSoftTrigger(AbsSoftTrigger * trigger);
  void SetConfigFilename(const char * name);
  void SetMinimumBCount(int val);

  void AddADC(AbsADC * adc);
  bool AddADC(AbsConf * conf);
  bool AddADC(AbsConfList * conflist);
  void AddDAQ(ADC::TYPE daq);

  AbsADC * FindADC(int sid);
  int FindADCAt(int sid);

  //
  // For DAQ operation
  //
  bool OpenDAQ();
  void CloseDAQ();
  bool PrepareDAQ();
  // For standalone DAQ
  bool ConfigureDAQ();
  bool InitializeDAQ();
  void StartTrigger();
  void StopTrigger();

  //
  // For reading data from adc
  //
  int ReadBCount(int n);
  int ReadBCountMin(int * bcounts = nullptr);
  int ReadBCountMax(int * bcounts = nullptr);
  int ReadADCData(int n, int bcount, unsigned char * databuffer = nullptr);
  int ReadData(int bcount, unsigned char ** databuffer);

  //
  // Writing output file
  //
  void SetOutputFilename(const char * fname);
  void SetCompressionLevel(int level);
  void SetOutputSplitTime(int time);

  //
  // for monitoring
  //
  void SetVerboseLevel(int level);
  void SetTriggerMonTime(int time);
  void SetNEvent(int n);
  void SetDAQTime(int t);
  void EnableHistograming();

  virtual void Run();

  // thread functions
  void TF_TriggerMon();
  void TF_DebugMon();
  void TF_RunManager();
  void TF_MsgServer();
  void TF_ReadData();
  void TF_SortData();
  void TF_BuildEvent();
  void TF_WriteData();
  void TF_SplitOutput(bool ontcb);
  void TF_Histogramer();
  void TF_ShrinkToFit();

protected:
  bool IsStandaloneDAQ() const;

  int GetNADC() const;
  int GetNDP() const;
  int GetADCEventDataSize() const;
  int GetADCChannelDataSize() const;

  // run control
  void RC_TCB();
  void RC_STDDAQ();
  void RC_TCBDAQ();
  void RC_TCBCTRLDAQ();
  void RC_NullDAQ();

  void ReadData_GLT();
  void ReadData_MOD();
  void SortData_MOD();
  void SortData_CHA();
  void BuildEvent_GLT();
  void BuildEvent_MOD();
  void BuildEvent_SLF();
  void WriteSADC_MOD_ROOT();
  void WriteFADC_MOD_ROOT();
  void WriteSADC_MOD_HDF5();
  void WriteFADC_MOD_HDF5();
  void WriteSADC_MOD_GZIP();
  void WriteFADC_MOD_GZIP();

  bool OpenNewOutputFile();
  long OpenNewROOTFile(const char * fname);
  long OpenNewHDF5File(const char * fname);
  long OpenNewGZIPFile(const char * fname);
  long SwitchRootFile(TFile *& oldfile, TFile * newfile);

  void CheckEventSanity(ADCHeader ** header, int * status);
  virtual void PrintDAQSummary();

  // run control utilities
  bool ThreadWait(bool ontcb = false) const;
  bool ThreadWait(unsigned long & state, bool & exit) const;
  bool ThreadWait(unsigned long * state, bool * exit) const;
  void ThreadSleep(int & sleep, double & perror, double & integral, int size,
                   int tsize = 1);
  bool WaitDAQStatus(unsigned long status) const;
  bool IsDAQRunning() const;
  bool IsDAQFail(TSocket * socket) const;
  void SendCommandToDAQ(unsigned long cmd) const;
  bool IsForcedEndRunFile(bool useRC = false) const;
  int Wait(bool * isgo, unsigned long * status) const;
  int Wait(unsigned long * status, unsigned long value);
  bool WaitState(unsigned long & status, RUNSTATE::STATE pstate,
                 bool errorexit = true) const;
  int WaitUntil(bool * isgo) const;
  int WaitUntil(bool * isgo, bool * exit) const;
  int WaitUntil(bool * isgo, unsigned long * status) const;

  void EncodeMsg(char * buffer, unsigned short from, unsigned short to,
                 unsigned long message1, unsigned long message2 = 0) const;
  void DecodeMsg(char * buffer, unsigned short & from, unsigned short & to,
                 unsigned long & message1, unsigned long & message2) const;
  unsigned long QueryDAQStatus(unsigned long daqtag, TSocket * socket) const;

  const char * GetADCName() const;
  const char * GetADCName(ADC::TYPE type) const;

  void StartBenchmark(const char * name);
  void StopBenchmark(const char * name);

protected:
  ELogger * fLog;

  int fRunNumber;
  int fSubRunNumber;

  int fDAQId;
  DAQ::TYPE fDAQType;
  ADC::TYPE fADCType;
  TRIGGER::MODE fTriggerMode;

  TString fConfigFilename;
  AbsConfList * fConfigList;

  usb3comroot * fUSB;
  CupGeneralTCB * fTCB;
  std::vector<ADC::TYPE> fDAQ;
  std::vector<TSocket *> fDAQSocket;

  int fRecordLength;
  int fMinimumBCount;
  int fADCEventDataSize;
  int fADCChannelDataSize;
  int fNDP;

  bool fIsOwnADC;

  //
  // run control
  //
  unsigned long fStatus;
  unsigned long fErrorCode;
  bool fDoConfigRun;
  bool fDoStartRun;
  bool fDoEndRun;
  bool fDoExit;
  bool fDoSplitOutputFile;

  unsigned long fStatusTCB;
  unsigned long fErrorCodeTCB;
  bool fDoConfigRunTCB;
  bool fDoStartRunTCB;
  bool fDoEndRunTCB;
  bool fDoExitTCB;
  bool fDoSplitOutputFileTCB;

  PROCSTATE fReadState;
  PROCSTATE fSortState;
  PROCSTATE fBuildState;
  PROCSTATE fWriteState;

  int fReadSleep;
  int fSortSleep;
  int fBuildSleep;
  int fWriteSleep;

  //
  // buffers
  //
  std::vector<ConcurrentDeque<AbsADCRaw *> *> fADCRawBuffers;
  ConcurrentDeque<BuiltEvent *> fBuiltEventBuffer1;
  ConcurrentDeque<BuiltEvent *> fBuiltEventBuffer2; // for histogramer

  //
  // for output
  //
  OUTPUT::FORMAT fOutputFileFormat;
  TString fOutputFilename;
  int fCompressionLevel;
  int fOutputSplitTime;
  std::vector<const char *> fOutputFileList;

  TFile * fROOTFile;
  TTree * fROOTTree;

  H5DataWriter * fHDF5File;
  AbsH5Event * fH5Event;

  //
  // for histogramer
  //
  TString fHistFilename;
  bool fDoHistograming;
  bool fHistogramerEnded;
  int fHistSleep;

  //
  // for monitoring
  //
  int fVerboseLevel;
  int fTriggerMonTime;
  int * fRemainingBCount;
  unsigned long fCurrentTime;
  unsigned long fTriggerTime;
  unsigned int fTriggerNumber;
  unsigned int * fTriggerNumberChannel;
  unsigned int fNBuiltEvent;
  double fTotalReadDataSize;
  double fTotalWrittenDataSize;

  time_t fStartDatime;
  time_t fEndDatime;

  int fSetNEvent;
  int fSetDAQTime;

  TBenchmark * fBenchmark;

  // software trigger
  AbsSoftTrigger * fSoftTrigger;

  //
  // mutexes
  //
  std::mutex fMonitorMutex;
  std::mutex fHistogramMutex;
  std::mutex fSTDADCMutex;
  std::mutex fWriteFileMutex;
  std::mutex fBenchmarkMutex;

  TString fADCName;

  ClassDef(CupDAQManager, 0)
};

inline void CupDAQManager::SetRunNumber(int run) { fRunNumber = run; }

inline void CupDAQManager::SetDAQId(int id) { fDAQId = id; }

inline void CupDAQManager::SetDAQType(DAQ::TYPE type) { fDAQType = type; }

inline void CupDAQManager::SetADCType(ADC::TYPE type)
{
  fADCType = type;

  switch (type) {
    case ADC::SADCS: fADCName = "SADCS"; break;
    case ADC::SADCT: fADCName = "SADCT"; break;
    case ADC::FADCS: fADCName = "FADCS"; break;
    case ADC::FADCT: fADCName = "FADCT"; break;
    case ADC::GADCS: fADCName = "GADCS"; break;
    case ADC::GADCT: fADCName = "GADCT"; break;
    case ADC::MADCS: fADCName = "MADCS"; break;
    default: break;
  }
}

inline void CupDAQManager::SetTriggerMode(TRIGGER::MODE mode)
{
  fTriggerMode = mode;
}

inline void CupDAQManager::SetSoftTrigger(AbsSoftTrigger * trigger)
{
  fSoftTrigger = trigger;
}

inline void CupDAQManager::SetConfigFilename(const char * name)
{
  fConfigFilename = name;
}

inline void CupDAQManager::AddDAQ(ADC::TYPE daq) { fDAQ.push_back(daq); }

inline void CupDAQManager::SetMinimumBCount(int val) { fMinimumBCount = val; }

inline void CupDAQManager::EnableHistograming() { fDoHistograming = true; }

inline void CupDAQManager::SetVerboseLevel(int level) { fVerboseLevel = level; }

inline void CupDAQManager::SetTriggerMonTime(int time)
{
  fTriggerMonTime = time;
}

inline void CupDAQManager::SetOutputFilename(const char * fname)
{
  fOutputFilename = fname;
}

inline void CupDAQManager::SetCompressionLevel(int level)
{
  fCompressionLevel = level;
}

inline void CupDAQManager::SetOutputSplitTime(int time)
{
  fOutputSplitTime = time;
}

inline void CupDAQManager::SetNEvent(int n) { fSetNEvent = n; }

inline void CupDAQManager::SetDAQTime(int t) { fSetDAQTime = t; }

inline bool CupDAQManager::IsStandaloneDAQ() const
{
  return fADCType == ADC::FADCS || fADCType == ADC::SADCS ||
                 fADCType == ADC::GADCS || fADCType == ADC::MADCS
             ? true
             : false;
}

inline int CupDAQManager::GetNADC() const { return GetEntries(); }

inline int CupDAQManager::GetNDP() const
{
  int ndp = 0;
  switch (fADCType) {
    case ADC::SADCS: break;
    case ADC::SADCT: break;
    case ADC::FADCS: ndp = 64 * fRecordLength - 16; break;
    case ADC::FADCT: ndp = 64 * fRecordLength - 16; break;
    case ADC::GADCS: ndp = 16 * fRecordLength - 2; break;
    case ADC::GADCT: ndp = 16 * fRecordLength - 2; break;
    case ADC::MADCS: ndp = 16 * fRecordLength - 16; break;
    case ADC::IADCT: {
      if (fRecordLength > 0) ndp = 8 * fRecordLength - 2;
      else ndp = 0;
      break;
    }
    default: break;
  }

  return ndp;
}

inline int CupDAQManager::GetADCEventDataSize() const
{
  int dataSize = 0;

  switch (fADCType) {
    case ADC::SADCS: dataSize = kBYTESPEREVENTSADC; break;
    case ADC::SADCT: dataSize = kBYTESPEREVENTSADC; break;
    case ADC::FADCS: dataSize = kNCHFADC * 128 * fRecordLength; break;
    case ADC::FADCT: dataSize = kNCHFADC * 128 * fRecordLength; break;
    case ADC::GADCS: dataSize = kNCHGADC * 32 * fRecordLength; break;
    case ADC::GADCT: dataSize = kNCHGADC * 32 * fRecordLength; break;
    case ADC::MADCS: dataSize = kNCHMADC * 32 * fRecordLength; break;
    case ADC::IADCT: {
      if (fRecordLength > 0) dataSize = 512 * fRecordLength;
      else dataSize = kBYTESPEREVENTIADC;
      break;
    }
    default: break;
  }

  return dataSize;
}

inline int CupDAQManager::GetADCChannelDataSize() const
{
  int dataSize = 0;

  switch (fADCType) {
    case ADC::SADCS: dataSize = 0; break;
    case ADC::SADCT: dataSize = 0; break;
    case ADC::FADCS: dataSize = 128 * fRecordLength; break;
    case ADC::FADCT: dataSize = 128 * fRecordLength; break;
    case ADC::GADCS: dataSize = 32 * fRecordLength; break;
    case ADC::GADCT: dataSize = 32 * fRecordLength; break;
    case ADC::MADCS: dataSize = 32 * fRecordLength; break;
    case ADC::IADCT: {
      if (fRecordLength > 0) dataSize = 128 * fRecordLength;
      else dataSize = 0;
      break;
    }
    default: break;
  }

  return dataSize;
}

inline const char * CupDAQManager::GetADCName() const
{
  return fADCName.Data();
}

inline const char * CupDAQManager::GetADCName(ADC::TYPE type) const
{
  TString ADCName;

  switch (type) {
    case ADC::SADCS: ADCName = "SADCS"; break;
    case ADC::SADCT: ADCName = "SADCT"; break;
    case ADC::FADCS: ADCName = "FADCS"; break;
    case ADC::FADCT: ADCName = "FADCT"; break;
    case ADC::GADCS: ADCName = "GADCS"; break;
    case ADC::GADCT: ADCName = "GADCT"; break;
    case ADC::IADCT: ADCName = "IADCT"; break;
    case ADC::MADCS: ADCName = "MADCS"; break;
    default: break;
  }

  return ADCName.Data();
}

#endif

/**
$Log: CupDAQManager.hh,v $
Revision 1.10  2022/12/21 00:02:44  cupsoft
*** empty log message ***

Revision 1.12  2020/07/30 01:48:01  cupsoft
*** empty log message ***

Revision 1.11  2020/07/13 01:25:14  cupsoft
*** empty log message ***

Revision 1.10  2020/07/10 02:37:31  cupsoft
*** empty log message ***

Revision 1.9  2020/03/02 01:07:18  cupsoft
*** empty log message ***

Revision 1.8  2020/01/09 04:56:56  cupsoft
*** empty log message ***

Revision 1.7  2019/11/27 23:58:35  cupsoft
*** empty log message ***

Revision 1.6  2019/08/07 01:35:10  cupsoft
*** empty log message ***

Revision 1.5  2019/08/04 01:21:37  cupsoft
*** empty log message ***

Revision 1.4  2019/07/24 06:14:57  cupsoft
*** empty log message ***

Revision 1.3  2019/07/11 08:21:35  cupsoft
*** empty log message ***

Revision 1.2  2019/07/05 00:25:04  cupsoft
*** empty log message ***

**/
