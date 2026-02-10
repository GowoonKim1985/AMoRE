#ifndef WCEvent_hh
#define WCEvent_hh

#include "TClonesArray.h"
#include "TObject.h"

typedef TClonesArray WCPmtColl;

class WCPmt;
class WCEvent : public TObject {
public:
  WCEvent();
  WCEvent(const WCEvent & data);
  virtual ~WCEvent();

  void SetEventNumber(unsigned int n);
  void SetTriggerTime(unsigned long t);

  virtual void Clear(const Option_t * opt = "");

  WCPmt * Add();
  WCPmt * Add(unsigned short id);
  void Remove(WCPmt * hit);

  unsigned int GetEventNumber() const;
  unsigned long GetTriggerTime() const;

  int GetN() const;
  WCPmt * Get(int n) const;
  WCPmt * GetByID(int id) const;
  WCPmtColl * GetColl() const;

private:
  unsigned int fEventNumber;
  unsigned long fTriggerTime;
  int fN; //! just for counter
  WCPmtColl * fColl;

  ClassDef(WCEvent, 1)
};

//
// Inline functions
//

inline void WCEvent::SetEventNumber(unsigned int n) { fEventNumber = n; }
inline void WCEvent::SetTriggerTime(unsigned long t) { fTriggerTime = t; }

inline unsigned int WCEvent::GetEventNumber() const { return fEventNumber; }
inline unsigned long WCEvent::GetTriggerTime() const { return fTriggerTime; }

inline WCPmtColl * WCEvent::GetColl() const { return fColl; }
inline int WCEvent::GetN() const { return fColl->GetEntries(); }

inline WCPmt * WCEvent::Get(int n) const { return (WCPmt *)fColl->At(n); }

#endif