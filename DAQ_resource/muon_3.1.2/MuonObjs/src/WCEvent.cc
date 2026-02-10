#include "/home/kmseo/Works/muon_3.1.2/MuonObjs/MuonObjs/WCEvent.hh"
#include "/home/kmseo/Works/muon_3.1.2/MuonObjs/MuonObjs/WCPmt.hh"

ClassImp(WCEvent)

WCEvent::WCEvent()
    : TObject()
{
  fEventNumber = 0;
  fTriggerTime = 0;

  fN = 0;
  fColl = new WCPmtColl("WCPmt");
}

WCEvent::WCEvent(const WCEvent & data)
    : TObject(data),
      fColl(data.GetColl())
{
  fEventNumber = data.GetEventNumber();
  fTriggerTime = data.GetTriggerTime();
}

WCEvent::~WCEvent() { delete fColl; }

WCPmt * WCEvent::Add() { return new ((*fColl)[fN++]) WCPmt(); }

WCPmt * WCEvent::Add(unsigned short id)
{
  return new ((*fColl)[fN++]) WCPmt(id);
}

void WCEvent::Remove(WCPmt * hit) { fColl->Remove(hit); }

WCPmt * WCEvent::GetByID(int id) const
{
  WCPmt * hit = nullptr;
  for (int i = 0; i < fColl->GetEntries(); i++) {
    hit = (WCPmt *)fColl->At(i);
    if (hit->GetID() == id) return hit;
  }
  return nullptr;
}

void WCEvent::Clear(const Option_t *)
{
  fColl->Delete();
  fN = 0;
}
