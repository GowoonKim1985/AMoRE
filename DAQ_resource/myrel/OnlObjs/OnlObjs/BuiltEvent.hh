#ifndef BuiltEvent_hh
#define BuiltEvent_hh

#include "TObjArray.h"
#include "TObject.h"

#include "OnlObjs/AbsADCRaw.hh"
#include "OnlObjs/TimeCalConsts.hh"

class BuiltEvent : public TObject {
private:
  unsigned int fEventNumber;
  TimeCalConsts * fTimeConsts;
  TObjArray * fADCEvents;

public:
  BuiltEvent();
  BuiltEvent(const BuiltEvent & event);
  virtual ~BuiltEvent();

  void SetEventNumber(int n);
  void AddADCEvent(AbsADCRaw * event);

  unsigned int GetEventNumber() const;
  unsigned long GetTriggerTime() const;
  TObjArray * GetADCEvents() const;

  virtual bool IsSortable() const { return true; }
  virtual int Compare(const TObject * object) const;

  ClassDef(BuiltEvent, 1)
};

inline void BuiltEvent::SetEventNumber(int n) { fEventNumber = n; }

inline void BuiltEvent::AddADCEvent(AbsADCRaw * event)
{
  fADCEvents->Add(event);
}

inline unsigned int BuiltEvent::GetEventNumber() const { return fEventNumber; }

inline TObjArray * BuiltEvent::GetADCEvents() const { return fADCEvents; }

#endif