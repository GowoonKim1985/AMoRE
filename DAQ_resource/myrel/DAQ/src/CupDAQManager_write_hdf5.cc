#include "HDF5Utils/EDM.hh"
#include "HDF5Utils/H5Event.hh"
#include "OnlObjs/ADCHeader.hh"
#include "OnlObjs/FADCRawEvent.hh"
#include "OnlObjs/SADCRawEvent.hh"
#include "TObjString.h"

#include "DAQ/CupDAQManager.hh"

void CupDAQManager::WriteFADC_MOD_HDF5()
{
  fH5Event = new H5Event<FChannel_t>;
  fHDF5File->SetEvent(fH5Event);
  if (!fHDF5File->Open()) {
    fLog->Error("CupDAQManager::WriteFADC_MOD_HDF5",
                "can\'t open hdf5 output file");
    fStatus = kERROR;
    return;
  }

  auto * h5event = (H5Event<FChannel_t> *)fH5Event;

  EventInfo_t eventinfo;
  std::vector<FChannel_t> chdata;

  double perror = 0;
  double integral = 0;

  std::unique_lock<std::mutex> wlock(fWriteFileMutex, std::defer_lock);

  while (true) {
    fWriteState = RUNNING;

    // for emergent exit
    // if (fDoExit || fStatus == kERROR)
    if (fDoExit || RUNSTATE::CheckError(fStatus)) break;

    if (fBuiltEventBuffer1.empty()) {
      if (fBuildSleep == ENDED) break;
    }
    else {
      chdata.clear();

      BuiltEvent * bevent = fBuiltEventBuffer1.popfront();
      TObjArray * adcevents = bevent->GetADCEvents();

      unsigned long fastttime = UINT64_MAX;
      unsigned int tnum = 0;
      unsigned int ttype = 0;
      unsigned short nhit = 0;

      for (int j = 0; j < adcevents->GetEntries(); j++) {
        auto * adcevent = (FADCRawEvent *)adcevents->At(j);
        auto * header = adcevent->GetADCHeader();

        if (header->GetLocalTrgTime() < fastttime) {
          fastttime = header->GetLocalTrgTime();
          tnum = header->GetTrgNumber();
          ttype = header->GetTrgType();
        }

        AbsADC * adc = FindADC(header->GetMID());
        AbsConf * conf = adc->GetConfig();

        for (int i = 0; i < kNCHFADC; i++) {
          if (header->GetZero(i)) continue;

          FChannel_t channel;
          channel.id = conf->PID(i);
          channel.tbit = header->GetTrgBit(i);
          channel.ped = header->GetPedestal(i);
          channel.SetWaveform(adcevent->GetChannelData(i));
          // memcpy(channel.waveform, adcevent->GetChannelData(i),
          // kH5FADCNDP*sizeof(unsigned short));

          chdata.push_back(channel);
          nhit += 1;
        }
      }
      eventinfo.tnum = tnum;
      eventinfo.ttime = fastttime;
      eventinfo.ttype = ttype;
      eventinfo.nhit = nhit;

      wlock.lock();
      h5event->WriteEvent(eventinfo, chdata);
      wlock.unlock();

      delete bevent;
    }

    int size = fBuiltEventBuffer1.size();
    ThreadSleep(fWriteSleep, perror, integral, size);
  }
}

void CupDAQManager::WriteSADC_MOD_HDF5()
{
  fH5Event = new H5Event<AChannel_t>;
  fHDF5File->SetEvent(fH5Event);
  if (!fHDF5File->Open()) {
    fLog->Error("CupDAQManager::WriteFADC_MOD_HDF5",
                "can\'t open hdf5 output file");
    fStatus = kERROR;
    return;
  }

  auto * h5event = (H5Event<AChannel_t> *)fH5Event;

  EventInfo_t eventinfo;
  std::vector<AChannel_t> chdata;

  double perror = 0;
  double integral = 0;

  std::unique_lock<std::mutex> wlock(fWriteFileMutex, std::defer_lock);

  while (true) {
    fWriteState = RUNNING;

    // for emergent exit
    // if (fDoExit || fStatus == kERROR)
    if (fDoExit || RUNSTATE::CheckError(fStatus)) break;

    if (fBuiltEventBuffer1.empty()) {
      if (fBuildSleep == ENDED) break;
    }
    else {
      chdata.clear();

      BuiltEvent * bevent = fBuiltEventBuffer1.popfront();
      TObjArray * adcevents = bevent->GetADCEvents();

      unsigned long fastttime = UINT64_MAX;
      unsigned int tnum = 0;
      unsigned int ttype = 0;
      unsigned short nhit = 0;

      for (int j = 0; j < adcevents->GetEntries(); j++) {
        auto * adcevent = (SADCRawEvent *)adcevents->At(j);
        auto * header = adcevent->GetADCHeader();

        if (header->GetLocalTrgTime() < fastttime) {
          fastttime = header->GetLocalTrgTime();
          tnum = header->GetTrgNumber();
          ttype = header->GetTrgType();
        }

        AbsADC * adc = FindADC(header->GetMID());
        AbsConf * conf = adc->GetConfig();

        for (int i = 0; i < kNCHSADC; i++) {
          if (header->GetZero(i)) continue;

          AChannel_t channel;
          channel.id = conf->PID(i);
          channel.tbit = header->GetTrgBit(i);
          channel.adc = adcevent->GetADC(i);
          channel.time = adcevent->GetTime(i);

          chdata.push_back(channel);
          nhit += 1;
        }
      }
      eventinfo.tnum = tnum;
      eventinfo.ttime = fastttime;
      eventinfo.ttype = ttype;
      eventinfo.nhit = nhit;

      wlock.lock();
      h5event->WriteEvent(eventinfo, chdata);
      wlock.unlock();

      delete bevent;
    }

    int size = fBuiltEventBuffer1.size();
    ThreadSleep(fWriteSleep, perror, integral, size);
  }
}

long CupDAQManager::OpenNewHDF5File(const char * filename)
{
  long retval = 0;

  TString bname = gSystem->BaseName(filename);
  TObjArray * objs = bname.Tokenize(".");
  int subnum =
      TString(((TObjString *)objs->At(objs->GetEntries() - 1))->GetName())
          .Atoi();

  if (subnum == 0) {
    fHDF5File = new H5DataWriter(filename, fCompressionLevel);
    fHDF5File->SetSubrun(0);
  }
  else {
    retval = fHDF5File->GetFileSize();
    fHDF5File->Close();
    delete fHDF5File;
    fHDF5File = new H5DataWriter(filename, fCompressionLevel);
    fHDF5File->SetSubrun(subnum);
    fHDF5File->SetEvent(fH5Event);
    if (!fHDF5File->Open()) {
      fLog->Error("CupDAQManager::OpenNewHDF5File",
                  "can\'t open output file %s", filename);
      return -1;
    }
  }

  fLog->Info("CupDAQManager::OpenNewHDF5File", "%s opened", filename);
  return retval;
}
