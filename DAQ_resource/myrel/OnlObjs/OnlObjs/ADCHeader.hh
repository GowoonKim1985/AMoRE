#ifndef ADCHeader_hh
#define ADCHeader_hh

#include "TObject.h"

#include "OnlObjs/TimeCalConsts.hh"

const int kNMAXADCCH = 40;

class ADCHeader : public TObject {
public:
  ADCHeader();
  ADCHeader(const ADCHeader & header);
  virtual ~ADCHeader();

  void SetMID(int n);
  void SetCID(int n);
  void SetError();
  void SetZero(int n);
  void SetTrgBit(int n);
  void SetPedestal(int n, int p);
  void SetDataLength(unsigned int n);
  void SetTrgType(unsigned int n);
  void SetTrgNumber(unsigned int n);
  void SetTrgTime(unsigned long n);
  void SetLocalTrgPattern(unsigned int n);
  void SetLocalTrgNumber(unsigned int n);
  void SetLocalTrgTime(unsigned long n);
  void SetTimeCalConsts(TimeCalConsts * consts);

  int GetMID() const;
  int GetCID() const;
  bool GetError() const;
  bool GetZero(int n) const;
  bool * GetZero() const;
  bool GetTrgBit(int n) const;
  bool * GetTrgBit() const;
  unsigned int GetPedestal(int n) const;
  unsigned int * GetPedestal() const;
  unsigned int GetDataLength() const;
  unsigned int GetTrgType() const;
  unsigned int GetTrgNumber() const;
  unsigned long GetTrgTime() const;
  unsigned int GetLocalTrgPattern() const;
  unsigned int GetLocalTrgNumber() const;
  unsigned long GetLocalTrgTime() const;

  virtual void Print(Option_t * opt = "") const;

  TimeCalConsts * calconsts; //!

private:
  bool error;
  bool * zero; //[kNMAXADCCH]
  bool * tbit; //[kNMAXADCCH]

  int mid;
  int cid;

  unsigned int ttype;
  unsigned int dlen;
  unsigned int * ped; //[kNMAXADCCH]
  unsigned int tnum;
  unsigned int loctnum;
  unsigned int loctptn;
  unsigned long ttime;
  unsigned long locttime;

  ClassDef(ADCHeader, 1)
};

inline void ADCHeader::SetMID(int n) { mid = n; }

inline void ADCHeader::SetCID(int n) { cid = n; }

inline void ADCHeader::SetError()
{
  if (!error) error = true;
}

inline void ADCHeader::SetZero(int n) { zero[n] = true; }

inline void ADCHeader::SetTrgBit(int n) { tbit[n] = true; }

inline void ADCHeader::SetPedestal(int n, int p) { ped[n] = p; }

inline void ADCHeader::SetDataLength(unsigned int n) { dlen = n; }

inline void ADCHeader::SetTrgType(unsigned int n) { ttype = n; }

inline void ADCHeader::SetTrgNumber(unsigned int n) { tnum = n; }

inline void ADCHeader::SetTrgTime(unsigned long n) { ttime = n; }

inline void ADCHeader::SetLocalTrgPattern(unsigned int n) { loctptn = n; }

inline void ADCHeader::SetLocalTrgNumber(unsigned int n) { loctnum = n; }

inline void ADCHeader::SetLocalTrgTime(unsigned long n)
{
  locttime = n;
  if (calconsts) { locttime += calconsts->GetConst(mid); }
}

inline void ADCHeader::SetTimeCalConsts(TimeCalConsts * consts)
{
  calconsts = consts;
}

inline int ADCHeader::GetMID() const { return mid; }

inline int ADCHeader::GetCID() const { return cid; }

inline bool ADCHeader::GetError() const { return error; }

inline bool ADCHeader::GetZero(int n) const { return zero[n]; }

inline bool * ADCHeader::GetZero() const { return zero; }

inline bool ADCHeader::GetTrgBit(int n) const { return tbit[n]; }

inline bool * ADCHeader::GetTrgBit() const { return tbit; }

inline unsigned int ADCHeader::GetPedestal(int n) const { return ped[n]; }

inline unsigned int * ADCHeader::GetPedestal() const { return ped; }

inline unsigned int ADCHeader::GetDataLength() const { return dlen; }

inline unsigned int ADCHeader::GetTrgType() const { return ttype; }

inline unsigned int ADCHeader::GetTrgNumber() const { return tnum; }

inline unsigned long ADCHeader::GetTrgTime() const { return ttime; }

inline unsigned int ADCHeader::GetLocalTrgPattern() const { return loctptn; }

inline unsigned int ADCHeader::GetLocalTrgNumber() const { return loctnum; }

inline unsigned long ADCHeader::GetLocalTrgTime() const { return locttime; }

#endif