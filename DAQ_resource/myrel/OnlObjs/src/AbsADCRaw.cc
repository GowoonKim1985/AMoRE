#include "OnlObjs/AbsADCRaw.hh"

ClassImp(AbsADCRaw)

AbsADCRaw::AbsADCRaw(int s, ADC::TYPE t)
    : TObject()
{
  fType = t;
  fSize = s;
  fData = nullptr;
  if (fSize > 0) { fData = new unsigned char[fSize]; }
  fHeader = new ADCHeader();
}

AbsADCRaw::AbsADCRaw(const AbsADCRaw & raw)
    : TObject(raw)
{
  fType = raw.GetADCType();
  fMode = raw.GetADCMode();
  fSize = raw.GetRawDataSize();
  fData = nullptr;
  if (fSize > 0) {
    fData = new unsigned char[fSize];
    memcpy(fData, raw.GetRawData(), fSize);
  }
  fHeader = new ADCHeader(*(raw.GetADCHeader()));
}

AbsADCRaw::~AbsADCRaw()
{
  if (fData) { delete[] fData; }
  delete fHeader;
}

void AbsADCRaw::CopyDataFrom(unsigned char * raw)
{
  memcpy(fData, raw, fSize * sizeof(unsigned char));
}

void AbsADCRaw::PrintHeader() const { fHeader->Print(); }