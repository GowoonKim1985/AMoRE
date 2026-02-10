#ifndef WCPmt_hh
#define WCPmt_hh

#include "TObject.h"

class WCPmt : public TObject {
public:
  WCPmt();
  WCPmt(int id);
  WCPmt(const WCPmt & WCPmt);
  virtual ~WCPmt();

  void SetID(int id);
  void SetPedMean(double ped);
  void SetPedRMS(double rms);
  void SetQtot(double q);
  void SetQmax(double qa);
  void SetFmax(double f);
  void SetFmaxx(int x);
  void SetIBin(int x);
  void SetVariables(double qtot, double qmax, double fmax, int qmaxx, unsigned short ped, double rms);
  
  int GetID() const;
  double GetPedMean() const;
  double GetPedRMS() const; 
  double GetQtot() const;
  double GetQmax() const;
  double GetFmax() const;
  int GetFmaxx() const;
  int GetIBin() const;

private:
  int fID;
  double fPedMean;
  double fPedRMS;
  double fQtot;
  double fQmax;
  double fFmax;
  int fFmaxx;
  int fIBin;

  ClassDef(WCPmt, 1)
};

inline void WCPmt::SetID(int id) { fID = id; }
inline void WCPmt::SetPedMean(double q) { fPedMean = q; }
inline void WCPmt::SetPedRMS(double q) { fPedRMS = q; }
inline void WCPmt::SetQtot(double q) { fQtot = q; }
inline void WCPmt::SetQmax(double q) { fQmax = q; }
inline void WCPmt::SetFmax(double q) { fFmax = q; }
inline void WCPmt::SetFmaxx(int q) { fFmaxx = q; }
inline void WCPmt::SetIBin(int q) { fIBin = q; }

inline void WCPmt::SetVariables(double qtot, double qmax, double fmax, int fmaxx, unsigned short ped, double rms)
{
  fQtot = qtot;
  fQmax = qmax;
  fFmax = fmax;
  fFmaxx = fmaxx;
  fPedMean = ped;
  fPedRMS = rms;
}

inline int WCPmt::GetID() const { return fID; }
inline double WCPmt::GetPedMean() const { return fPedMean; }
inline double WCPmt::GetPedRMS() const { return fPedRMS; }
inline double WCPmt::GetQtot() const { return fQtot; }
inline double WCPmt::GetQmax() const { return fQmax; }
inline double WCPmt::GetFmax() const { return fFmax; }
inline int WCPmt::GetFmaxx() const { return fFmaxx; }
inline int WCPmt::GetIBin() const { return fIBin; }

#endif
