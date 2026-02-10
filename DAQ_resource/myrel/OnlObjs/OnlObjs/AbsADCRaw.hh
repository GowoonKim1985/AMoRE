#ifndef AbsADCRaw_hh
#define AbsADCRaw_hh

#include "TObject.h"

#include "DAQConfig/AbsConf.hh"
#include "OnlConsts/adcconsts.hh"
#include "OnlObjs/ADCHeader.hh"
#include "OnlObjs/TimeCalConsts.hh"

class AbsADCRaw : public TObject {
protected:
  ADC::TYPE fType;
  ADC::MODE fMode;
  ADCHeader * fHeader; //
  int fSize;
  unsigned char * fData; //!

public:
  AbsADCRaw(int s = 0, ADC::TYPE = ADC::NONE);
  AbsADCRaw(const AbsADCRaw & raw);
  virtual ~AbsADCRaw();

  virtual void CopyDataFrom(unsigned char * raw);
  virtual void SetTimeCalConsts(TimeCalConsts * c);
  virtual void Unpack(AbsConf * conf, int verbose) = 0;

  virtual ADC::TYPE GetADCType() const;
  virtual ADC::MODE GetADCMode() const;
  virtual ADCHeader * GetADCHeader() const;
  virtual int GetRawDataSize() const;
  virtual unsigned char * GetRawData() const;
  virtual unsigned long GetTriggerTime() const;

  virtual void PrintHeader() const;

  ClassDef(AbsADCRaw, 1)
};

inline void AbsADCRaw::SetTimeCalConsts(TimeCalConsts * c)
{
  fHeader->SetTimeCalConsts(c);
}

inline ADC::TYPE AbsADCRaw::GetADCType() const { return fType; }

inline ADC::MODE AbsADCRaw::GetADCMode() const { return fMode; }

inline int AbsADCRaw::GetRawDataSize() const { return fSize; }

inline unsigned char * AbsADCRaw::GetRawData() const { return fData; }

inline ADCHeader * AbsADCRaw::GetADCHeader() const { return fHeader; }

inline unsigned long AbsADCRaw::GetTriggerTime() const
{
  return fHeader->GetLocalTrgTime();
}

#endif