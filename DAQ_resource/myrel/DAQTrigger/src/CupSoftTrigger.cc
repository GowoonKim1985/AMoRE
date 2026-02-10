//
// Created by cupsoft on 6/30/20.
//
#include "TRandom3.h"

#include "DAQConfig/STRGConf.hh"
#include "DAQTrigger/CupSoftTrigger.hh"
#include "OnlObjs/FADCRawEvent.hh"
#include "OnlObjs/SADCRawEvent.hh"

CupSoftTrigger::CupSoftTrigger()
    : AbsSoftTrigger()
{
  fDoZSUTBit = false;
  fDoZSUQSum = false;
  fDoPrescale = false;

  fQSumThreshold = 0;
  fPrescale = 1;


  gRandom->SetSeed(0);
}

CupSoftTrigger::CupSoftTrigger(AbsConf * config)
    : AbsSoftTrigger(config)
{
  fDoZSUTBit = false;
  fDoZSUQSum = false;
  fDoPrescale = false;

  fQSumThreshold = 0;
  fPrescale = 1;

  gRandom->SetSeed(0);
}

CupSoftTrigger::~CupSoftTrigger() {}

void CupSoftTrigger::InitTrigger()
{
  auto * config = (STRGConf *)fConfig;
  int mode = config->GetIPar(0);

  fDoZSUTBit = mode & (1 << 0); // 1: tbit zero suppression
  fDoZSUQSum = mode & (1 << 1); // 2: qsum zero suppression
  // fDoPrescale = mode & (1 << 2); // 4: prescale

  fQSumThreshold = config->GetIPar(1);
  fPrescale = config->GetIPar(2);
}

bool CupSoftTrigger::DoTrigger(BuiltEvent * event)
{
  fTotalInputEvent += 1;

  bool state = false;

  switch (fADCType) {
    case ADC::FADC: state = DoTriggerFADC(event); break;
    case ADC::SADC: state = DoTriggerSADC(event); break;
    default: break;
  }

  if (state) { fNTriggeredEvent += 1; }

  return state;
}

bool CupSoftTrigger::DoTriggerFADC(BuiltEvent * event)
{
  if (fPrescale > 1) {
    if (gRandom->Rndm() > 1 / (double)fPrescale) { return false; }
  }

  int neventqsum = 0;

  ADCHeader * header = nullptr;
  TObjArray * adcevents = event->GetADCEvents();
  for (int k = 0; k < adcevents->GetEntries(); k++) {
    auto * adcevent = (FADCRawEvent *)adcevents->At(k);

    header = adcevent->GetADCHeader();
    if (header->GetTrgType() == 1) { 
      neventqsum = 100;
      break; // pedestal trigger
    }

    int qsum = 0;
    for (int i = 0; i < kNCHFADC; i++) {
      if (header->GetZero()) continue; // unused channel

      if (fDoZSUTBit) {
        if (header->GetTrgBit(i) == 0) header->SetZero(i);
      }

      if (fDoZSUQSum) {
        unsigned int ped = header->GetPedestal(i);
        int ndp = (header->GetDataLength() - 32) / 2;

        unsigned short * adc = adcevent->GetChannelData(i);

        for (int j = 0; j < ndp; j++) {
          qsum += adc[j] - ped;
        }

        if (qsum < fQSumThreshold) header->SetZero(i);
        else neventqsum += 1;
      }
    }
  }

  if (fDoZSUQSum && neventqsum == 0) return false;
  return true;
}

bool CupSoftTrigger::DoTriggerSADC(BuiltEvent * event)
{
  if (fDoPrescale) {
    if (gRandom->Rndm() > 1 / (double)fPrescale) { return false; }
  }

  ADCHeader * header = nullptr;

  TObjArray * adcevents = event->GetADCEvents();
  for (int k = 0; k < adcevents->GetEntries(); k++) {
    auto * adcevent = (SADCRawEvent *)adcevents->At(k);

    header = adcevent->GetADCHeader();
    if (header->GetTrgType() == 1) continue; // pedestal trigger

    for (int i = 0; i < kNCHSADC; i++) {
      if (header->GetZero()) continue; // unused channel

      if (fDoZSUTBit) {
        if (header->GetTrgBit(i) == 0) { header->SetZero(i); }
      }
    }
  }

  return true;
}
