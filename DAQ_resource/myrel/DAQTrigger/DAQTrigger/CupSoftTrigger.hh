#ifndef CupSoftTrigger_hh
#define CupsoftTrigger_hh

#include "DAQTrigger/AbsSoftTrigger.hh"

class CupSoftTrigger : public AbsSoftTrigger {
public:
  CupSoftTrigger();
  CupSoftTrigger(AbsConf * config);
  virtual ~CupSoftTrigger();

  virtual void InitTrigger();
  virtual bool DoTrigger(BuiltEvent * event);

private:
  bool DoTriggerFADC(BuiltEvent * event);
  bool DoTriggerSADC(BuiltEvent * event);

private:
  bool fDoZSUTBit;
  bool fDoZSUQSum;
  bool fDoPrescale;
  int fQSumThreshold;
  int fPrescale;
};

#endif