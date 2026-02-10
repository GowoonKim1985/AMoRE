#include "TBranchRef.h"
#include "TObjString.h"

#include "DAQ/CupDAQManager.hh"
#include "OnlObjs/ADCHeader.hh"
#include "OnlObjs/FADCRawEvent.hh"
#include "OnlObjs/SADCRawEvent.hh"
#include "RawObjs/AChannel.hh"
#include "RawObjs/AChannelData.hh"
#include "RawObjs/EventInfo.hh"
#include "RawObjs/FChannel.hh"
#include "RawObjs/FChannelData.hh"

void CupDAQManager::WriteFADC_MOD_ROOT()
{
  fROOTFile->cd();
  auto * eventinfo = new EventInfo();
  fROOTTree->Branch("EventInfo", &eventinfo);
  auto * chdata = new FChannelData();
  fROOTTree->Branch("FChannelData", &chdata);

  double perror = 0;
  double integral = 0;

  std::unique_lock<std::mutex> wlock(fWriteFileMutex, std::defer_lock);

  while (true) {
    fWriteState = RUNNING;

    // for emergent exit
    // if (fDoExit || fStatus == kERROR)
    if (fDoExit || RUNSTATE::CheckError(fStatus)) break;

    if (fBuiltEventBuffer1.empty()) {
      if (fBuildState == ENDED) break;
    }
    else {
      chdata->Clear();

      BuiltEvent * bevent = fBuiltEventBuffer1.popfront();
      TObjArray * adcevents = bevent->GetADCEvents();

      unsigned long fastttime = UINT64_MAX;
      unsigned int tnum = 0;
      unsigned int ttype = 0;

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

          FChannel * channel = chdata->Add(conf->PID(i), adcevent->GetNDP());
          channel->SetPedestal(header->GetPedestal(i));
          channel->SetWaveform(adcevent->GetChannelData(i));
          channel->SetBit(header->GetTrgBit(i));
        }
      }
      eventinfo->SetTrgNumber(tnum);
      eventinfo->SetTrgTime(fastttime);
      eventinfo->SetTrgType(ttype);

      delete bevent;

      wlock.lock();
      fROOTTree->Fill();
      wlock.unlock();
    }

    int size = fBuiltEventBuffer1.size();
    ThreadSleep(fWriteSleep, perror, integral, size);
  }
}

void CupDAQManager::WriteSADC_MOD_ROOT()
{
  fROOTFile->cd();
  auto * eventinfo = new EventInfo();
  fROOTTree->Branch("EventInfo", &eventinfo);
  auto * chdata = new AChannelData();
  fROOTTree->Branch("AChannelData", &chdata);

  double perror = 0;
  double integral = 0;

  std::unique_lock<std::mutex> wlock(fWriteFileMutex, std::defer_lock);

  while (true) {
    fWriteState = RUNNING;

    // for emergent exit
    // if (fDoExit || fStatus == kERROR)
    if (fDoExit || RUNSTATE::CheckError(fStatus)) break;

    if (fBuiltEventBuffer1.empty()) {
      if (fBuildState == ENDED) break;
    }
    else {
      chdata->Clear();

      BuiltEvent * bevent = fBuiltEventBuffer1.popfront();
      TObjArray * adcevents = bevent->GetADCEvents();

      unsigned long fastttime = UINT64_MAX;
      unsigned int tnum = 0;
      unsigned int ttype = 0;

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

          AChannel * channel = chdata->Add(conf->PID(i));
          channel->SetBit(header->GetTrgBit(i));
          channel->SetADC(adcevent->GetADC(i));
          channel->SetTime(adcevent->GetTime(i));
        }
      }
      eventinfo->SetTrgNumber(tnum);
      eventinfo->SetTrgTime(fastttime);
      eventinfo->SetTrgType(ttype);

      delete bevent;

      wlock.lock();
      fROOTTree->Fill();
      wlock.unlock();
    }

    int size = fBuiltEventBuffer1.size();
    ThreadSleep(fWriteSleep, perror, integral, size);
  }
}

long CupDAQManager::OpenNewROOTFile(const char * filename)
{
  long retval = 0;

  TString bname = gSystem->BaseName(filename);
  TObjArray * objs = bname.Tokenize(".");
  int subnum =
      TString(((TObjString *)objs->At(objs->GetEntries() - 1))->GetName())
          .Atoi();

  if (subnum == 0) {
    fROOTFile = new TFile(filename, "recreate", "", fCompressionLevel);
    if (!fROOTFile->IsOpen()) {
      fLog->Error("CupDAQManager::OpenNewROOTFile",
                  "can\'t open output file %s", filename);
      return -1;
    }
    fROOTTree = new TTree("AbsEvent", "AbsEvent");
  }
  else {
    fROOTFile->cd();
    fROOTTree->Write();
    fROOTTree->Reset();

    auto * newfile = new TFile(filename, "recreate", "", fCompressionLevel);
    if (!fROOTFile->IsOpen()) {
      fLog->Error("CupDAQManager::OpenNewROOTFile",
                  "can\'t open output file %s", filename);
      return -1;
    }

    retval = SwitchRootFile(fROOTFile, newfile);
  }

  fLog->Info("CupDAQManager::OpenNewROOTFile", "%s opened", filename);
  return retval;
}

long CupDAQManager::SwitchRootFile(TFile *& oldfile, TFile * newfile)
{
  long fsize = oldfile->GetEND();

  TBranch * branch = nullptr;
  TObject * obj = nullptr;

  while ((obj = oldfile->GetList()->First())) {
    oldfile->Remove(obj);

    if (obj->InheritsFrom(TTree::Class())) {
      auto * t = (TTree *)obj;
      t->SetDirectory(newfile);

      TIter nextb(t->GetListOfBranches());
      while ((branch = (TBranch *)nextb())) {
        branch->SetFile(newfile);
      }

      if (t->GetBranchRef()) { t->GetBranchRef()->SetFile(newfile); }
      continue;
    }

    if (newfile) newfile->Append(obj);
    oldfile->Remove(obj);
  }

  delete oldfile;
  oldfile = nullptr;
  oldfile = newfile;

  return fsize;
}
