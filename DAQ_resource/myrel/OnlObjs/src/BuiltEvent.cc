#include "OnlObjs/BuiltEvent.hh"
#include "OnlObjs/FADCRawEvent.hh"
#include "OnlObjs/SADCRawEvent.hh"

ClassImp(BuiltEvent)

BuiltEvent::BuiltEvent()
    : TObject()
{
  fEventNumber = 0;
  fTimeConsts = nullptr;
  fADCEvents = new TObjArray();
}

BuiltEvent::BuiltEvent(const BuiltEvent & builtevent)
    : TObject(builtevent)
{
  fEventNumber = builtevent.GetEventNumber();
  fADCEvents = new TObjArray();

  auto * adcevents = builtevent.GetADCEvents();
  for (int i = 0; i < adcevents->GetEntries(); i++) {
    auto * event = (AbsADCRaw *)adcevents->At(i);

    switch (event->GetADCMode()) {
      case ADC::FMODE: {
        auto * adc = (FADCRawEvent *)event;
        auto * newadc = new FADCRawEvent(*adc);
        fADCEvents->Add(newadc);
        break;
      }
      case ADC::SMODE: {
        auto * adc = (SADCRawEvent *)event;
        auto * newadc = new SADCRawEvent(*adc);
        fADCEvents->Add(newadc);
        break;
      }
      default: break;
    }
  }
}

BuiltEvent::~BuiltEvent()
{
  fADCEvents->Delete();
  delete fADCEvents;
}

unsigned long BuiltEvent::GetTriggerTime() const
{
  unsigned long fastttime = UINT64_MAX;
  for (int i = 0; i < fADCEvents->GetEntries(); i++) {
    auto * adcevent = (AbsADCRaw *)fADCEvents->At(i);
    unsigned long ttime = adcevent->GetTriggerTime();
    if (ttime < fastttime) { fastttime = ttime; }
  }
  return fastttime;
}

int BuiltEvent::Compare(const TObject * object) const
{
  auto * comp = (BuiltEvent *)object;
  if (this->GetTriggerTime() > comp->GetTriggerTime()) { return 1; }
  else if (this->GetTriggerTime() < comp->GetTriggerTime()) {
    return -1;
  }
  return 0;
}