//
// Created by cupsoft on 7/24/19.
//
#include "OnlObjs/FADCRawEvent.hh"
#include "OnlObjs/SADCRawEvent.hh"
#include "DAQConfig/IADCTConf.hh"
#include "DAQ/CupDAQManager.hh"

void CupDAQManager::TF_SortData()
{
  fSortState = READY;

  if (!ThreadWait(&fStatus, &fDoExit)) {
    return;
  }
  fLog->Info("CupDAQManager::TF_SortData", "sorting data started");

  StartBenchmark("SortData");
  if (fTriggerMode == TRIGGER::SELF) {
    SortData_CHA();
  }
  else {
    SortData_MOD();
  }
  StopBenchmark("SortData");

  fSortState = ENDED;
  fLog->Info("CupDAQManager::TF_SortData", "sorting data ended");
}

void CupDAQManager::SortData_MOD()
{
  int nadc = GetEntries();

  AbsADCRaw * adcevent = nullptr;

  double perror = 0;
  double integral = 0;  

  while (true) {
    fSortState = RUNNING;

    // for emergent exit
    //if (fDoExit || fStatus == kERROR)    
    if (fDoExit || RUNSTATE::CheckError(fStatus))
      break;

    if (fReadState == ENDED) {
      int remain = 0;
      for (int i = 0; i < nadc; i++) {
        auto * adc = (AbsADC *)fCont[i];
        remain += adc->Bsize();
      }
      if (remain == 0) {
        break;
      }
    }

    int totalsize = 0;
    for (int i = 0; i < nadc; i++) {
      auto * adc = (AbsADC *)fCont[i];
      totalsize += adc->Bsize();

      if (adc->Bempty()) continue;

      ChunkData * chunkdata = adc->Bpopfront(false);
      unsigned char * data = chunkdata->data;

      int nevent = kKILOBYTES*chunkdata->size/fADCEventDataSize;
      for (int j = 0; j < nevent; j++) {
        switch (fADCType) {
          case ADC::FADCS:adcevent = new FADCRawEvent(fNDP, fADCEventDataSize, ADC::FADC);
            break;
          case ADC::FADCT:adcevent = new FADCRawEvent(fNDP, fADCEventDataSize, ADC::FADC);
            break;
          case ADC::GADCS:adcevent = new FADCRawEvent(fNDP, fADCEventDataSize, ADC::GADC);
            break;
          case ADC::GADCT:adcevent = new FADCRawEvent(fNDP, fADCEventDataSize, ADC::GADC);
            break;
          case ADC::MADCS:adcevent = new FADCRawEvent(fNDP, fADCEventDataSize, ADC::MADC);
            break;
          case ADC::SADCS:adcevent = new SADCRawEvent(fADCEventDataSize, ADC::SADC);
            break;
          case ADC::SADCT:adcevent = new SADCRawEvent(fADCEventDataSize, ADC::SADC);
            break;
          case ADC::IADCT: {
            auto * conf = (IADCTConf*)adc->GetConfig();
            if (conf->MODE() > 0)
              adcevent = new FADCRawEvent(fNDP, fADCEventDataSize, ADC::IADC);
            else
              adcevent = new SADCRawEvent(fADCEventDataSize, ADC::IADC);
            break;
          }
          default: break;
        }
        adcevent->CopyDataFrom(data + j*fADCEventDataSize);
        adcevent->Unpack(adc->GetConfig(), fVerboseLevel);

        ConcurrentDeque<AbsADCRaw *> * buffer = fADCRawBuffers.at(i);
        buffer->push_back(adcevent);
      }

      delete chunkdata;
    }

    totalsize = totalsize/nadc;
    ThreadSleep(fSortSleep, perror, integral, totalsize);
  }
}

void CupDAQManager::SortData_CHA()
{
}
