#ifndef PSHit_hh
#define PSHit_hh

#include "TObject.h"

const int kNDP = 62;

class PSHit : public TObject {
public:
  PSHit();
  PSHit(int id);
  PSHit(const PSHit & PSHit);
  virtual ~PSHit();

  void SetID(int id);
  void SetQ(int n, double q);
  void SetVariables(double q, int tbin, double *qa, double pt);
  //void SetQx(int n, int tbin);

  int GetID() const;
  int GetNCh() const;
  int GetQx(int n) const;
  double GetQ(int n) const;
  double GetPt(int n) const;
  double GetQmax(int n, int m) const;
  double GetQsum() const;
  double GetQmaxsum(int n) const;

  void SetWaveform(unsigned short * wave) {
    memcpy(fWave, wave, kNDP*sizeof(unsigned short));
  }

  unsigned short * GetWaveform() const {
    return fWave;
  }

  void SetBit(int bit) { fBit = bit; }
  int GetBit() const { return fBit; }

  private:
  int fID;
  int fNCh;
  int fQx[4];
  double fQ[4];
  double fPt[4];
  double fQmax[4][10];
  double fQsum;
  double fQmaxsum[10];
  int fBit;
  int fNDP;
  unsigned short * fWave; // [kNDP]
  
  ClassDef(PSHit, 1)
};

inline void PSHit::SetID(int id) { fID = id; }
inline void PSHit::SetQ(int n, double q)
{
  fQ[n] = q;
  fQsum += q;
  fNCh += 1;
}

inline void PSHit::SetVariables(double q, int tbin, double *qa, double pt)
{
  fQ[fNCh] = q;
  fPt[fNCh] = pt;
  fQx[fNCh] = tbin;
  fQsum += q;
  for(int i = 0; i < 10; i++){
    fQmax[fNCh][i] = qa[i];
    fQmaxsum[i] += qa[i];
  }
  fNCh += 1;
}

inline int PSHit::GetID() const { return fID; }
inline int PSHit::GetNCh() const { return fNCh; }
inline int PSHit::GetQx(int n) const { return fQx[n]; }
inline double PSHit::GetQ(int n) const { return fQ[n]; }
inline double PSHit::GetPt(int n) const { return fPt[n]; }
inline double PSHit::GetQmax(int n, int m) const { return fQmax[n][m]; }
inline double PSHit::GetQsum() const { return fQsum; }
inline double PSHit::GetQmaxsum(int n) const { return fQmaxsum[n]; }

#endif
