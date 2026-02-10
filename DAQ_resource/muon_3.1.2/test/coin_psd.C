void coin_psd(int runnum = 327)
{
  TChain *pchain[2];
  PSEvent *pevt[2];
  int nevt_d1[2] = {0, 0};
  int pnevt[2];
  for(int i = 0; i < 2; i++){
    pchain[i] = new TChain("prd_psd");
    pchain[i] -> Add(Form("/data/kmseo/PSMD/%06d/prd_psd_daq%d_%06d_*.root", runnum, i+1, runnum));
    pevt[i] = new PSEvent();
    pchain[i] -> SetBranchAddress("PSEvent", &pevt[i]);
    
    pnevt[i] = pchain[i] -> GetEntries();
  }

  TH1D *hqsumP[22];
  for(int i = 0; i < 22; i++){
    hqsumP[i] = new TH1D(Form("hqsumP_%d", i), "", 250, 0, 50000);
    hqsumP[i] -> SetXTitle("Qsum");
    hqsumP[i] -> SetYTitle("Number of Entries");
  }
  
  int nevt[2] = {static_cast<int>(pchain[0] -> GetEntries())/1,
                 static_cast<int>(pchain[1] -> GetEntries())/1};
  // int nevt[2] = {50000, 50000};
  int range_id[2][2]  = {{0, 48}, {65, 113}};
  int range_idb[2][2] = {{47, 59}, {112, 124}};
  bool flag_side;
  bool flag_coin = false;
  double ptime, qsum;
  for(int i = 0; i < 2; i++){
    cout << nevt[i] << endl;
    
    for(int j = 0; j < nevt[i]; j++){
      if(j != 0 && j%3000000 == 0) cout << j << endl;
      
      flag_side = false;
      pchain[i] -> GetEntry(j);

      int nch = pevt[i] -> GetN();
      ptime = pevt[i] -> GetTriggerTime();
      for(int k = 0; k < nch; k++){
	qsum = 0;
	
	PSHit *hit = pevt[i] -> Get(k);
	if(!hit) continue;
	
	for(int l = 0; l < 4; l++)
	  qsum += hit -> GetQ(l);
	if(qsum < 5000) continue;
	
	int id = hit -> GetID();
	if(id > range_id[i][0] && id < range_id[i][1]){
	  flag_side = true;
	  //cout << Form("side PSMD %3d is fired", id) << endl;
	}
	if(flag_side && id > range_idb[i][0] && id < range_idb[i][1]){
	  //cout << Form("bottom PSMD %3d is fired", id) << endl;
	  if(id < 60)
	    hqsumP[id-48] -> Fill(qsum);
	  else
	    hqsumP[id-113+11] -> Fill(qsum);
	  
	  flag_side = false;
	}
      }
    }
  }

 TH1D *hqsumP_tot[22];
  TFile f("./hist/muonHist_PSMD_all.root");
  int nn = 0;
  for(int i = 0; i < 130; i++){
    if((i > 47  && i < 59) ||
       (i > 112 && i < 124)){
      hqsumP_tot[nn] = (TH1D*)f.Get(Form("hqsum_%d", i));
      hqsumP_tot[nn] -> SetDirectory(0);

      nn++;
    }
    else
      continue;
  }

  TCanvas *can_p[2];
  for(int i = 0; i < 2; i++){
    can_p[i] = new TCanvas(Form("can_p%d", i+1), Form("PSMD Bot.%d", i+1), 1600, 900);
    can_p[i] -> Divide(4, 3);
    for(int j = 0; j < 11; j++){
      can_p[i] -> cd(j+1);
      
      hqsumP_tot[11*i+j] -> GetXaxis() -> SetRangeUser(0, 50000);
      hqsumP_tot[11*i+j] -> SetLineColor(kBlack);
      hqsumP_tot[11*i+j] -> Draw();
      hqsumP[11*i+j] -> SetLineColor(kRed);
      hqsumP[11*i+j] -> Draw("same");
      gPad -> SetLogy();
    }
  }

  // TFile of("./hist/muonHist_bot_coinPSMD.root", "recreate");
  // for(int i = 0; i < 22; i++)
  //   hqsumP[i] -> Write();
  // of.Close();
}
