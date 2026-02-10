void plot_wcd_ped(int idd = 18)
{
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_wcd");
  //chain -> Add("/data/kmseo/WCMD/000287/prd_wcd_000287_*.root");
  chain -> Add("/data/kmseo/WCMD/000303/prd_wcd_000303_*.root");

  WCEvent *wevt = new WCEvent();
  chain -> SetBranchAddress("WCEvent", &wevt);

  TH2D *hqped[48];
  TH2D *hqprms[48];
  TH2D *hprms[48];
  TH2D *hfxrms[48];
  TH1D *hrms[48];
  TH1D *hped[48];
  for(int i = 0; i < 48; i++){
    hqped[i] = new TH2D(Form("hqped_%d", i), "", 200, 0, 40000, 100, 0, 500);
    hqped[i] -> SetXTitle("Qtot");
    hqped[i] -> SetYTitle("Ped");
    hqped[i] -> SetMarkerStyle(8);
    hqped[i] -> SetMarkerSize(0.5);

    hqprms[i] = new TH2D(Form("hqprms_%d", i), "", 200, 0, 40000, 100, 0, 50);
    hqprms[i] -> SetXTitle("Qtot");
    hqprms[i] -> SetYTitle("RMS");
    hqprms[i] -> SetMarkerStyle(8);
    hqprms[i] -> SetMarkerSize(0.5);

    hprms[i] = new TH2D(Form("hprms_%d", i), "", 100, 100, 200, 100, 0, 50);
    hprms[i] -> SetXTitle("Ped");
    hprms[i] -> SetYTitle("RMS");
    hprms[i] -> SetMarkerStyle(8);
    hprms[i] -> SetMarkerSize(0.5);

    hfxrms[i] = new TH2D(Form("hfxrms_%d", i), "", 100, 0, 1008, 100, 0, 50);
    hfxrms[i] -> SetXTitle("Fmaxx");
    hfxrms[i] -> SetYTitle("RMS");
    hfxrms[i] -> SetMarkerStyle(8);
    hfxrms[i] -> SetMarkerSize(0.5);

    hrms[i] = new TH1D(Form("hrms_%d", i), "", 100, 0, 50);
    hrms[i] -> SetXTitle("RMS");
    hrms[i] -> SetYTitle("Number of Entries");
    // hrms[i] -> GetXaxis() -> SetLabelSize(0.1);
    // hrms[i] -> GetYaxis() -> SetLabelSize(0.1);
    
    hped[i] = new TH1D(Form("hped_%d", i), "", 100, 100, 200);
    hped[i] -> SetXTitle("Pedestal");
    hped[i] -> SetYTitle("Number of Entries");
    // hped[i] -> GetXaxis() -> SetLabelSize(0.1);
    // hped[i] -> GetYaxis() -> SetLabelSize(0.1);
  }
  
  double qtot[48] = {0};
  double ped[48] = {0};
  double rms[48] = {0};
  double fmaxx[48] = {0};
  int id[48];
  
  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);
    if(i != 0 && i%2000000 == 0) cout << i << endl;

    int nch = wevt -> GetN();
    //cout << nch << endl;
    for(int j = 0; j < nch; j++){
      WCPmt *pmt = wevt -> Get(j);
      id[j] = pmt -> GetID();
      //if(id[j] != idd) continue;
      
      qtot[id[j]] = pmt -> GetQtot();
      if(qtot[id[j]] < 0) continue;
      ped[id[j]] = pmt -> GetPedMean();
      rms[id[j]] = pmt -> GetPedRMS();
      fmaxx[id[j]] = pmt -> GetFmaxx();

      
      hqped[id[j]] -> Fill(qtot[id[j]], ped[id[j]]);
      hqprms[id[j]] -> Fill(qtot[id[j]], rms[id[j]]);
      hprms[id[j]] -> Fill(ped[id[j]], rms[id[j]]);
      hfxrms[id[j]] -> Fill(fmaxx[id[j]], rms[id[j]]);

      hrms[id[j]] -> Fill(rms[id[j]]);
      hped[id[j]] -> Fill(ped[id[j]]);
    }
  }

  // TCanvas *can = new TCanvas("can", "", 1500, 1000);
  // can -> Divide(3, 2);
  // can -> cd(1);
  // hqped[idd] -> Draw();
  // can -> cd(2);
  // hqprms[idd] -> Draw();
  // can -> cd(3);
  // hprms[idd] -> Draw();
  // can -> cd(4);
  // hfxrms[idd] -> Draw();
  // can -> cd(5);
  // hped[idd] -> Draw();
  // gPad -> SetLogy();
  // can -> cd(6);
  // hrms[idd] -> Draw();
  // gPad -> SetLogy();

  TCanvas *can_ped = new TCanvas("can_ped", "ped", 1500, 1000);
  can_ped -> Divide(7, 7);
  for(int i = 0; i < 47; i++){
    can_ped -> cd(i+1);
    
    hped[i] -> Draw();
    gPad -> SetLogy();
  }
  TCanvas *can_rms = new TCanvas("can_rms", "rms", 1500, 1000);
  can_rms -> Divide(7, 7);
  for(int i = 0; i < 47; i++){
    can_rms -> cd(i+1);
    
    hrms[i] -> Draw();
    gPad -> SetLogy();
  }
}
