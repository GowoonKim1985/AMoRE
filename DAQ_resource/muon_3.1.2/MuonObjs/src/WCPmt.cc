#include "/home/kmseo/Works/muon_3.1.2/MuonObjs/MuonObjs/WCPmt.hh"

ClassImp(WCPmt)

WCPmt::WCPmt()
    : TObject()
{
  fID = 0;
  fPedMean = 0;
  fPedRMS = 0;
  fQtot = 0;
  fQmax = 0;
  fFmax = 0;
  fFmaxx = 0;
  fIBin = 0;
}

WCPmt::WCPmt(int id)
    : TObject()
{
  fID = id;
  fPedMean = 0;
  fPedRMS = 0;
  fQtot = 0;
  fQmax = 0;
  fFmax = 0;
  fFmaxx = 0;
  fIBin = 0;
}

WCPmt::WCPmt(const WCPmt & hit)
    : TObject(hit)
{
  fID = hit.GetID();
  fPedMean = hit.GetPedMean();
  fPedRMS = hit.GetPedRMS();
  fQtot = hit.GetQtot();
  fQmax = hit.GetQmax();
  fFmax = hit.GetFmax();
  fFmaxx = hit.GetFmaxx();
  fIBin = hit.GetIBin();
}

WCPmt::~WCPmt() {}
