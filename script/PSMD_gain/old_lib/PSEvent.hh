#ifndef PSEvent_hh
#define PSEvent_hh

#include "TClonesArray.h"
#include "TObject.h"

typedef TClonesArray PSHitColl;

class PSHit;
class PSEvent : public TObject {
public:
  PSEvent();
  PSEvent(const PSEvent & data);
  virtual ~PSEvent();

  void SetEventNumber(unsigned int n);
  void SetTriggerTime(unsigned long t);

  virtual void Clear(const Option_t * opt = "");

  PSHit * Add();
  PSHit * Add(unsigned short id);
  void Remove(PSHit * hit);

  unsigned int GetEventNumber() const;
  unsigned long GetTriggerTime() const;

  int GetN() const;
  PSHit * Get(int n) const;
  PSHit * GetByID(int id) const;
  PSHitColl * GetColl() const;

private:
  unsigned int fEventNumber;
  unsigned long fTriggerTime;
  int fN; //! just for counter
  PSHitColl * fColl;

  ClassDef(PSEvent, 1)
};

//
// Inline functions
//

inline void PSEvent::SetEventNumber(unsigned int n) { fEventNumber = n; }
inline void PSEvent::SetTriggerTime(unsigned long t) { fTriggerTime = t; }

inline unsigned int PSEvent::GetEventNumber() const { return fEventNumber; }
inline unsigned long PSEvent::GetTriggerTime() const { return fTriggerTime; }

inline PSHitColl * PSEvent::GetColl() const { return fColl; }
inline int PSEvent::GetN() const { return fColl->GetEntries(); }

inline PSHit * PSEvent::Get(int n) const { return (PSHit *)fColl->At(n); }

#endif