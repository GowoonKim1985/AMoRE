//
// Created by cupsoft on 7/24/19.
//
#include "DAQConfig/AmoreADCConf.hh"
#include "DAQConfig/FADCSConf.hh"
#include "DAQConfig/FADCTConf.hh"
#include "DAQConfig/GADCSConf.hh"
#include "DAQConfig/MADCSConf.hh"

#include "DAQ/CupDAQManager.hh"

using namespace std;

void CupDAQManager::TF_BuildEvent()
{
  fBuildState = READY;

  if (!ThreadWait(&fStatus, &fDoExit)) { return; }
  fLog->Info("CupDAQManager::TF_BuildEvent", "event building started");

  auto adctype = (ADC::TYPE)(fADCType % 10);
  auto * conf = fConfigList->GetSTRGConfig(adctype);
  if (conf) fSoftTrigger->SetConfig(conf);

  if (fSoftTrigger->IsEnabled()) { fSoftTrigger->InitTrigger(); }

  StartBenchmark("BuildEvent");
  if (fTriggerMode == TRIGGER::GLOBAL) { BuildEvent_GLT(); }
  else if (fTriggerMode == TRIGGER::MODULE) {
    BuildEvent_MOD();
  }
  else {
    BuildEvent_SLF();
  }
  StopBenchmark("BuildEvent");

  if (fBuildState != ERROR) fBuildState = ENDED;
  fLog->Info("CupDAQManager::TF_BuildEvent", "event building ended");
}

void CupDAQManager::BuildEvent_GLT()
{
  int nadc = GetEntries();

  auto ** header = new ADCHeader *[nadc];
  auto * sanity = new int[nadc];

  double perror = 0;
  double integral = 0;

  std::unique_lock<std::mutex> mlock(fMonitorMutex, std::defer_lock);

  while (true) {
    fBuildState = RUNNING;

    // for emergent exit
    // if (fDoExit || fStatus == kERROR)
    if (fDoExit || RUNSTATE::CheckError(fStatus)) break;

    int nmod = 0;
    if (fSortState == ENDED) {
      for (int i = 0; i < nadc; i++) {
        ConcurrentDeque<AbsADCRaw *> * adceventbuffer = fADCRawBuffers.at(i);
        if (adceventbuffer->empty()) continue;
        nmod += 1;
      }
      if (nmod < nadc) break;
    }

    int totalsize = 0;

    nmod = 0;
    for (int i = 0; i < nadc; i++) {
      ConcurrentDeque<AbsADCRaw *> * adceventbuffer = fADCRawBuffers.at(i);
      totalsize += adceventbuffer->size();

      AbsADCRaw * adcevent = adceventbuffer->front(false);
      if (!adcevent) continue;

      header[i] = adcevent->GetADCHeader();
      nmod += 1;
    }

    totalsize = totalsize / nadc;

    if (nmod == nadc) {
      CheckEventSanity(header, sanity);

      if (fADCType == ADC::SADCS || fADCType == ADC::SADCT) {
        int temp = 0;
        for (int i = 0; i < nadc; i++) {
          temp += sanity[i];
        }
        if (temp < 0) {
          fLog->Info("CupDAQManager::BuildEvent_GLT",
                     "SADC null event, will be skipped");
          for (int i = 0; i < nadc; i++) {
            while (true) {
              ConcurrentDeque<AbsADCRaw *> * adceventbuffer =
                  fADCRawBuffers.at(i);
              if (!adceventbuffer->empty()) {
                AbsADCRaw * adcevent = adceventbuffer->popfront();
                delete adcevent;
              }
              else {
                break;
              }
            }
          }
          continue;
        }
      }

      auto * builtevent = new BuiltEvent();
      TObjArray * adcevents = builtevent->GetADCEvents();

      int nerror = 0;
      for (int i = 0; i < nadc; i++) {
        auto * adc = (AbsADC *)fCont[i];

        ConcurrentDeque<AbsADCRaw *> * adceventbuffer = fADCRawBuffers.at(i);

        if (sanity[i] == 1) {
          fLog->Error("CupDAQManager::BuildEvent_GLT",
                      "ADC header is corrupted [mid=%d]", adc->GetSID());
          nerror += 1;
          continue;
        }
        else if (sanity[i] == 2) {
          fLog->Warning("CupDAQManager::BuildEvent_GLT",
                        "event missed in ADC [mid=%d]", adc->GetSID());
          continue;
        }
        else if (sanity[i] == 3) {
          fLog->Error("CupDAQManager::BuildEvent_GLT",
                      "local trigger time error in ADC [mid=%d]",
                      adc->GetSID());
          nerror += 1;
          continue;
        }

        AbsADCRaw * adcevent = adceventbuffer->popfront();
        adcevents->Add(adcevent);
      }

      if (nerror > 0) {
        // fStatus = kERROR;
        RUNSTATE::SetError(fStatus);
        fBuildState = ERROR;
        break;
      }

      bool istriggered = true;

      // software trigger
      if (fSoftTrigger->IsEnabled() && !fSoftTrigger->DoTrigger(builtevent)) {
        istriggered = false;
      }

      if (istriggered) {
        mlock.lock();
        fNBuiltEvent += 1;
        mlock.unlock();

        builtevent->SetEventNumber(fNBuiltEvent);
        fBuiltEventBuffer1.push_back(builtevent);
        if (fDoHistograming) {
          auto * builtevent2 = new BuiltEvent(*builtevent);
          fBuiltEventBuffer2.push_back(builtevent2);
        }
      }
      else {
        delete builtevent;
      }
    }

    ThreadSleep(fBuildSleep, perror, integral, totalsize);
  }

  delete[] sanity;
  delete[] header;
}

void CupDAQManager::BuildEvent_MOD() {}

void CupDAQManager::BuildEvent_SLF() {}

void CupDAQManager::CheckEventSanity(ADCHeader ** header, int * error)
{
  int nadc = GetEntries();

  memset(error, 0, nadc * sizeof(int));

  if (nadc == 1) {
    if (header[0]->GetError()) { error[0] = 1; }
    return;
  }

  //
  // SADC null events
  int temp = 0;
  for (int i = 0; i < nadc; i++) {
    if (header[i]->GetMID() == 0) {
      error[i] = -1;
      temp += error[i];
    }
  }

  if (temp < 0) { return; }

  auto * trgnum = new unsigned int[nadc];
  auto * trgtime = new unsigned long[nadc];
  for (int i = 0; i < nadc; i++) {
    if (header[i]->GetError()) {
      error[0] = 1;
      continue;
    }
    trgnum[i] = header[i]->GetLocalTrgNumber();
    trgtime[i] = header[i]->GetLocalTrgTime();
  }

  bool missed = false;
  unsigned int nmin = *std::min_element(trgnum, trgnum + nadc);
  for (int i = 0; i < nadc; i++) {
    if (header[i]->GetLocalTrgNumber() > nmin) {
      fLog->Debug("CupDAQManager::CheckEventSanity",
                  "min trgnum = %d [mid=%d, %d]", nmin, header[i]->GetMID(),
                  header[i]->GetLocalTrgNumber());
      error[i] = 2;
      missed = true;
    }
  }

  if (missed) {
    delete[] trgnum;
    delete[] trgtime;
    return;
  }

  unsigned long tmin = *std::min_element(trgtime, trgtime + nadc);
  for (int i = 0; i < nadc; i++) {
    if (error[i] == 1 || error[i] == 2) { continue; }
    if (header[i]->GetLocalTrgTime() > tmin) {
      fLog->Debug("CupDAQManager::CheckEventSanity",
                  "min trgtime = %ld [mid=%d, %ld]", tmin, header[i]->GetMID(),
                  header[i]->GetLocalTrgTime());
      error[i] = 3;
    }
  }

  delete[] trgnum;
  delete[] trgtime;
}
