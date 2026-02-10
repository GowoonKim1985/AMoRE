#include "/home/kmseo/Works/muon_3.1.2/test/PSEvent.hh"
#include "/home/kmseo/Works/muon_3.1.2/test/PSHit.hh"

ClassImp(PSEvent)

PSEvent::PSEvent()
    : TObject()
{
  fEventNumber = 0;
  fTriggerTime = 0;

  fN = 0;
  fColl = new PSHitColl("PSHit");
}

PSEvent::PSEvent(const PSEvent & data)
    : TObject(data),
      fColl(data.GetColl())
{
  fEventNumber = data.GetEventNumber();
  fTriggerTime = data.GetTriggerTime();
}

PSEvent::~PSEvent() { delete fColl; }

PSHit * PSEvent::Add() { return new ((*fColl)[fN++]) PSHit(); }

PSHit * PSEvent::Add(unsigned short id)
{
  return new ((*fColl)[fN++]) PSHit(id);
}

void PSEvent::Remove(PSHit * hit) { fColl->Remove(hit); }

PSHit * PSEvent::GetByID(int id) const
{
  PSHit * hit = nullptr;
  for (int i = 0; i < fColl->GetEntries(); i++) {
    hit = (PSHit *)fColl->At(i);
    if (hit->GetID() == id) return hit;
  }
  return nullptr;
}

void PSEvent::Clear(const Option_t *)
{
  fColl->Delete();
  fN = 0;
}
