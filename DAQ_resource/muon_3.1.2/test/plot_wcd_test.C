R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libMuonObjs)

void plot_wcd_test()
{
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_wcd");
  chain -> Add("./test_wcd_000227.root");

  TH1D *hid = new TH1D("hid", "", 47, 0, 47);
  TH1D *his[47];
  TH1D *hPed[47];
  TH1D *hRms[47];
  for(int i = 0; i < 47; i++){
    his[i] = new TH1D(Form("his_%d", i), "", 200, 0, 1000);
    hPed[i] = new TH1D(Form("hPed_%d", i), "", 100, 0, 0);
    hRms[i] = new TH1D(Form("hRms_%d", i), "", 200, 0, 200);
  }
  WCEvent *wevt = new WCEvent();
  chain -> SetBranchAddress("WCEvent", &wevt);

  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);
    if(i != 0 && i%1000000 == 0) cout << i << endl;

    int nch = wevt -> GetN();
    double time = wevt -> GetTriggerTime();
    // cout << "nch: " << nch << endl;
    // cout << "ttrg: " << time/1e9 << " s" << endl;
    for(int j = 0; j < nch; j++){
      WCPmt *hit = wevt -> Get(j);
      int id = hit -> GetID();
      hid -> Fill(id);
      //cout << id << endl;

      his[id] -> Fill(hit -> GetFmax());
      hPed[id] -> Fill(hit -> GetPedMean());
      hRms[id] -> Fill(hit -> GetPedRMS());

      //cout << "\t Q: " << hit -> GetFmax() << " from id " << id << endl;
    }
  }

  TCanvas *can = new TCanvas("can", "", 1200, 1000);
  can -> Divide(7, 7);
  TPad *pad[47];
  for(int i = 0; i < 47; i++){
    can -> cd(i+1);
    pad[i] = (TPad*)can -> cd(i+1);
    
    his[i] -> Draw();
    pad[i] -> SetLogy();
  }
  
  // TCanvas *can_ped = new TCanvas("can_ped", "", 1200, 1000);
  // can_ped -> Divide(7, 7);
  // TPad *pad_ped[47];
  // for(int i = 0; i < 47; i++){
  //   can_ped -> cd(i+1);
  //   pad_ped[i] = (TPad*)can_ped -> cd(i+1);
    
  //   hPed[i] -> Draw();
  //   pad_ped[i] -> SetLogy();
  // }
  
  TCanvas *can_rms = new TCanvas("can_rms", "", 1200, 1000);
  can_rms -> Divide(7, 7);
  TPad *pad_rms[47];
  for(int i = 0; i < 47; i++){
    can_rms -> cd(i+1);
    pad_rms[i] = (TPad*)can_rms -> cd(i+1);
    
    hRms[i] -> Draw();
    pad_rms[i] -> SetLogy();
  }
}
