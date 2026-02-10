TChain *pchain[2];
PSEvent *pevt[2];
int nevtC[2][2] = {0};
int idC;
double qsumC[2];

int CheckCoinBot(int bdaq, int sdaq, double ptime)
{
  int range_idd[2][2] = {{48, 58}, {113, 123}};
  int nevt = pchain[sdaq] -> GetEntries();
  for(int i = nevtC[bdaq][sdaq]; i < nevt; i++){
    pchain[sdaq] -> GetEntry(i);
    
    double ptime_d = pevt[sdaq] -> GetTriggerTime();
    if(ptime_d < ptime) continue;
    nevtC[bdaq][sdaq] = i+1;
    
    int nch = pevt[sdaq] -> GetN();
    for(int j = 0; j < nch; j++){
      PSHit *hit = pevt[sdaq] -> Get(j);

      for(int l = 0; l < 4; l++)
	qsumC[sdaq] += hit -> GetQ(l);
     
      int id = hit -> GetID();
      if(id >= range_idd[sdaq][0] && id <= range_idd[sdaq][1]){
	idC = id;

	return 1;
      }
    }
    
    return 0;
  }

  return 0;
}

void coin_psd2(int runnum = 327)
{
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
  
  // int nevt[2] = {static_cast<int>(pchain[0] -> GetEntries())/1,
  //                static_cast<int>(pchain[1] -> GetEntries())/1};
  int nevt[2] = {30000, 30000};
  //cout << nevt[0] << " " << nevt[1] << endl;
  int range_id[2][2] = {{0, 48}, {65, 113}};
  bool flag_side = false;
  bool flag_coin = false;
  double ptime, qsum;
  for(int i = 0; i < 2; i++){
    cout << nevt[i] << endl;

    for(int j = 0; j < nevt[i]; j++){
      if(j != 0 && j%1000000 == 0) cout << j << endl;
      
      pchain[i] -> GetEntry(j);

      int nch = pevt[i] -> GetN();
      ptime = pevt[i] -> GetTriggerTime();
      for(int k = 0; k < nch; k++){
	qsum = 0;
	
	PSHit *hit = pevt[i] -> Get(k);
	if(!hit) continue;
	
	for(int l = 0; l < 4; l++)
	  qsum += hit -> GetQ(l);
	if(qsum < 20000) continue;
	
	int id = hit -> GetID();
	if(id > range_id[i][0] && id < range_id[i][1]){
	  idC = 0;
	  qsumC[0] = 0;
	  qsumC[1] = 0;
	  
	  int cstamp1 = CheckCoinBot(i, 0, ptime);
	  int cstamp2 = CheckCoinBot(i, 1, ptime);

	  if(cstamp1+cstamp2 > 0){
	    cout << Form("coincide event detected from daq%d, id: %3d, evt: %5d, time: %5.2f qsum: %5.0f | idC: %3d, qsumC1: %5.0f, qsumC2: %5.0f", i+1, id, j, ptime/1e9, qsum, idC, qsumC[0], qsumC[1]) << endl;

	    if(idC < 60){
	      if(qsumC[0] != 0)
		hqsumP[idC-48] -> Fill(qsumC[0]);
	    }
	    else{
	      if(qsumC[1] != 0)
		hqsumP[idC-113+11] -> Fill(qsumC[1]);
	    }
	  }
	  // else
	  //   cout << "not coin!!" << endl;
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
}
