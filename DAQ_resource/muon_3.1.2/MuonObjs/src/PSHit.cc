#include "/home/kmseo/Works/muon_3.1.2/MuonObjs/MuonObjs/PSHit.hh"

ClassImp(PSHit)

PSHit::PSHit()
    : TObject()
{
  fID = 0;
  fNCh = 0;
  fQsum = 0;
  memset(fQ, 0, 4 * sizeof(double));
  memset(fPt, 0, 4 * sizeof(double));
  memset(fQx, 0, 4 * sizeof(int));
  memset(fQmax, 0, sizeof(fQmax));
  memset(fQmaxsum, 0, 10*sizeof(double));
}

PSHit::PSHit(int id)
    : TObject()
{
  fID = id;
  fNCh = 0;
  fQsum = 0;
  memset(fQ, 0, 4 * sizeof(double));
  memset(fPt, 0, 4 * sizeof(double));
  memset(fQx, 0, 4 * sizeof(int));
  memset(fQmax, 0, sizeof(fQmax));
  memset(fQmaxsum, 0, 10*sizeof(double));
}

PSHit::PSHit(const PSHit & hit)
    : TObject(hit)
{
  fID = hit.GetID();
  fNCh = hit.GetNCh();
  fQsum = hit.GetQsum();
  for(int i = 0; i < 10; i++)
    fQmaxsum[i] = hit.GetQmaxsum(i);
    
  for (int i = 0; i < fNCh; i++){
    fQ[i] = hit.GetQ(i);
    fPt[i] = hit.GetPt(i);
    fQx[i] = hit.GetQx(i);
    for(int j = 0; j < 10; j++){
      fQmax[i][j] = hit.GetQmax(i, j);
    }
  }
}

PSHit::~PSHit() {}
