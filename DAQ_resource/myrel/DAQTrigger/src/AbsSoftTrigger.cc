//
// Created by cupsoft on 6/29/20.
//
#include "DAQTrigger/AbsSoftTrigger.hh"

AbsSoftTrigger::AbsSoftTrigger()
{
  fIsEnabled = false;
  fADCType = ADC::NONE;

  fVerboseLevel = 0;

  fTotalInputEvent = 0;
  fNTriggeredEvent = 0;
}

AbsSoftTrigger::AbsSoftTrigger(AbsConf * config)
{
  fIsEnabled = config->IsEnabled();
  fADCType = config->GetADCType();

  fVerboseLevel = 0;

  fTotalInputEvent = 0;
  fNTriggeredEvent = 0;
}

AbsSoftTrigger::~AbsSoftTrigger() {}
