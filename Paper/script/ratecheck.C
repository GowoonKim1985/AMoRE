R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void ratecheck()
{
  gStyle -> SetOptStat(0);

  int runnum = 356;
  //  TChain *chain = new TChain("tree");
  TChain *chain = new TChain("prd_wcd");  
  //  chain -> Add("/y/kmseo/data/WCMD/000216/oldtree/tree_WCMD_216_*.root");
  chain -> Add("/home/kkw/PROD/WCMD/000356/prd_wcd_000356_*.root");
  //  chain -> Add(Form("./prod/tree_WCMD_%03d_00000.root",runnum));
  int tent = chain->GetEntries();
  cout<<tent<<endl;
  double trgtime, evtnum;
  TH2D *rate = new TH2D("rate", "", 3000, 0, 300000000, 250000, 0, 2500000);
  rate -> SetXTitle("trgtime(sec)");
  rate -> SetYTitle("evtnum");

  
  for(int i=0; i<tent; i++){
    chain->GetEntry(i);
    evtnum = (double) chain->GetLeaf("fEventNumber")->GetValue();
    //    cout<<evtnum<<endl;
    trgtime = chain->GetLeaf("fTriggerTime")->GetValue();
    // cout<<trgtime<<endl;
    rate->Fill(trgtime,evtnum);
  }
  rate->Draw();
}
  
