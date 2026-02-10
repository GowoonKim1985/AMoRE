void occu_WCMD()
{
  gStyle -> SetOptStat(0);
  
  double rtime[2] = {337706.20, 42*3600};
  int runnum[2] = {287, 303};
  TH1D *hoccu[2];

  for(int i = 0; i < 2; i++){
    hoccu[i] = new TH1D(Form("hoccu_%d", i), "", 48, 0, 48);
    hoccu[i] -> SetXTitle("Channel");
    hoccu[i] -> SetYTitle("Number of Entries [/Sec.]");
    
    TChain *chain = new TChain("prd_wcd");
    chain -> Add(Form("/data/kmseo/WCMD/%06d/prd_wcd_%06d_*.root", runnum[i], runnum[i]));

    WCEvent *wevt = new WCEvent();
    chain -> SetBranchAddress("WCEvent", &wevt);

    int nevt = chain -> GetEntries();
    cout << nevt << endl;
    for(int j = 0; j < nevt; j++){
      chain -> GetEntry(j);
      
      int nch = wevt -> GetN();
      for(int k = 0; k < nch; k++){
	WCPmt *pmt = wevt -> Get(k);
	
	hoccu[i] -> Fill(pmt -> GetID()-1);
      }
    }
    hoccu[i] -> Scale(1./rtime[i]);
  }

  TCanvas *can = new TCanvas("can", "", 800, 600);
  hoccu[1] -> SetLineColor(kRed);
  hoccu[1] -> Draw("hist");
  hoccu[0] -> Draw("hist, same");
}
