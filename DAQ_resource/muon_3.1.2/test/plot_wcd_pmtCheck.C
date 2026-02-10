void plot_wcd_pmtCheck()
{
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_wcd");
  chain -> Add("/data/kmseo/WCMD/000287/prd_wcd_000287_*.root");

  WCEvent *wevt = new WCEvent();
  chain -> SetBranchAddress("WCEvent", &wevt);

  TH2D *hhmap = new TH2D("hhmap_%d", "", 7, 0, 7, 7, 0, 7);
  hhmap -> SetXTitle("Row");
  hhmap -> SetYTitle("Column");
  TH1D *ht[2];
  TH1D *ht_sel[2];
  TH2D *htQ[2];
  TH2D *htQ_sel[2];
  for(int i = 0; i < 2; i++){
    ht[i] = new TH1D(Form("ht_%d", i), "", 48, 0, 48);
    ht[i] -> SetXTitle("Multiplicity");
    ht[i] -> SetYTitle("Number of Entries");

    ht_sel[i] = new TH1D(Form("ht_sel_%d", i), "", 48, 0, 48);
    ht_sel[i] -> SetXTitle("Multiplicity");
    ht_sel[i] -> SetYTitle("Number of Entries");

    htQ[i] = new TH2D(Form("htQ_%d", i), "", 48, 0, 48, 300, 0, 30000);
    htQ[i] -> SetXTitle("Multiplicity");
    htQ[i] -> SetYTitle("Qtot");

    htQ_sel[i] = new TH2D(Form("htQ_sel_%d", i), "", 48, 0, 48, 300, 0, 30000);
    htQ_sel[i] -> SetXTitle("Multiplicity");
    htQ_sel[i] -> SetYTitle("Qtot");
  }
  
  double qtot[48] = {0};
  double fmax[48] = {0};
  int id[48], stp;
  int nn[48] = {0};
  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);
    if(i != 0 && i%200000 == 0) cout << i << endl;

    stp = 0;
    int nch = wevt -> GetN();
    //if(nch < 20) continue;
    for(int j = 0; j < nch; j++){
      WCPmt *pmt = wevt -> Get(j);
      id[j] = pmt -> GetID() - 1;
      nn[id[j]] = 1;
      
      if(id[j] == 18 || id[j] == 29){
	if(id[j] == 18) id[j] = 0;
	else id[j] = 1;
	
	qtot[id[j]] = pmt -> GetQtot();
	//qtot[id[j]] = pmt -> GetFmax();

	ht[id[j]] -> Fill(nch);
	htQ[id[j]] -> Fill(nch, qtot[id[j]]);
	if(qtot[id[j]] > 2000 && nch < 20){
	  ht_sel[id[j]] -> Fill(nch);
	  htQ_sel[id[j]] -> Fill(nch, qtot[id[j]]);
	  
	  if(id[j] == 0)//see line 54
	    stp = 1;
	}
      }
    }
    
    if(stp == 1){
      for(int j = 0; j < 48; j++){
	if(nn[j] != 0){
	  hhmap -> Fill(7-j/7-1, 7-j%7-1);
	  nn[j] = 0;
	}
      }
      stp = 0;
    }
    else{
      for(int j = 0; j < 48; j++)
	nn[j] = 0;
    }
  }

  TCanvas *can_t = new TCanvas("can_t", "t", 1600, 600);
  can_t -> Divide(2, 1);
  TPad *ph[2];
  for(int i = 0; i < 2; i++){
    can_t -> cd(i+1);
    ph[i] = (TPad*)can_t -> cd(i+1);
    
    ht[i] -> Draw();
    ht_sel[i] -> SetLineColor(kRed);
    ht_sel[i] -> Draw("same");
    ph[i] -> SetLogy();
  }
  
  TCanvas *can_tQ = new TCanvas("can_tQ", "tQ", 1600, 600);
  can_tQ -> Divide(2, 1);
  for(int i = 0; i < 2; i++){
    can_tQ -> cd(i+1);

    htQ[i] -> SetMarkerStyle(8);
    htQ_sel[i] -> SetMarkerStyle(8);
    htQ[i] -> SetMarkerSize(0.5);
    htQ_sel[i] -> SetMarkerSize(0.5);

    htQ[i] -> Draw();
    //htQ_sel[i] -> SetMarkerColor(kRed);
    htQ_sel[i] -> Draw("same");
  }

  TCanvas *can_hmap = new TCanvas("can_hmap", "", 800, 800);
  hhmap -> Draw("colz");
}
