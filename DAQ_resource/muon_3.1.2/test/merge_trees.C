R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libMuonObjs)

void merge_trees()
{
  const int nthreads = 10;
  ROOT::EnableImplicitMT(nthreads);

  TStopwatch s;
  s.Start();
  
  int runnum = 216;
  int subrun = 1;

  TFile *f1 = new TFile(Form("/y/kmseo/data/PSMD/%06d/prd_psd_daq1_%06d_%05d.root", runnum, runnum, subrun));
  TTree *tPSMD1 = (TTree*)f1 -> Get("prd_psd");
  TFile *f2 = new TFile(Form("/y/kmseo/data/PSMD/%06d/prd_psd_daq2_%06d_%05d.root", runnum, runnum, subrun));
  TTree *tPSMD2 = (TTree*)f2 -> Get("prd_psd");
  TFile *f3 = new TFile(Form("/y/kmseo/data/WCMD/%06d/tree_WCMD_%05d.root", runnum, subrun));
  TTree *tWCMD = (TTree*)f3 -> Get("tree");
  TList *list = new TList();
  list -> Add(tPSMD1);
  list -> Add(tPSMD2);
  list -> Add(tWCMD);

  TFile of("./testTree.root", "recreate");  
  TTree *newtree = TTree::MergeTrees(list);
  newtree -> SetName("newtree");
  newtree -> Write();
  of.Close();

  s.Stop();
  s.Print();
}
