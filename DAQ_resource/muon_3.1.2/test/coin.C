double twolandau(double *v, double *p)
{
  double x = v[0];

  double a = p[0];
  double b = p[1];
  double c = p[2];
  double d = p[3];
  double e = p[4];
  double f = p[5];

  return a*TMath::Landau(x, b, c) + d*TMath::Landau(x, e, f);
}

void coin()
{
  //const int runnum = 287;
  const int runnum = 327;
  
  gStyle -> SetOptStat(0);
  
  TChain *wchain = new TChain("prd_wcd");
  wchain -> Add(Form("/data/kmseo/WCMD/%06d/prd_wcd_%06d_*.root", runnum, runnum));
  WCEvent *wevt = new WCEvent();
  wchain -> SetBranchAddress("WCEvent", &wevt);

  TChain *pchain[2];
  PSEvent *pevt[2];
  int pnevt[2];
  for(int i = 0; i < 2; i++){
    pchain[i] = new TChain("prd_psd");
    pchain[i] -> Add(Form("/data/kmseo/PSMD/%06d/prd_psd_daq%d_%06d_*.root", runnum, i+1, runnum));
    pevt[i] = new PSEvent();
    pchain[i] -> SetBranchAddress("PSEvent", &pevt[i]);

    pnevt[i] = pchain[i] -> GetEntries();
  }

  TH1D *his = new TH1D("his", "", 200, -1000, 1000);
  his -> SetXTitle("T_{D} [ns]");
  his -> SetYTitle("Number of Entries");
  TH1D *hQsumW = new TH1D("hQsumW", "", 250, 0, 100000);
  hQsumW -> SetXTitle("Qsum");
  hQsumW -> SetYTitle("Number of Entries");
  TH1D *hQsumW_iw = new TH1D("hQsumW_iw", "", 250, 0, 100000);
  TH1D *hQsumW_ow = new TH1D("hQsumW_ow", "", 250, 0, 100000);
  TH2D *h2tm = new TH2D("h2tm", "", 200, -1000, 1000, 48, 0, 48);
  h2tm -> SetXTitle("T_{D} [ns]");
  h2tm -> SetYTitle("Multiplicity");
  TH2D *h2tq = new TH2D("h2tq", "", 200, -1000, 1000, 500, 0, 100000);
  h2tq -> SetXTitle("T_{D} [ns]");
  h2tq -> SetYTitle("Qsum");

  TH1D *hqsumP[22];
  for(int i = 0; i < 22; i++){
    hqsumP[i] = new TH1D(Form("hqsumP_%d", i), "", 250, 0, 50000);
    hqsumP[i] -> SetXTitle("Qsum");
    hqsumP[i] -> SetYTitle("Number of Entries");
  }
  
  int stp = 0, stpp = 0;
  int pnevt_d[2] = {0};
  double wtime, ptime[2], wqsum, pqsum[22], q[4];
  int wnevt = wchain -> GetEntries();
  cout << wnevt << endl;
  for(int i = 0; i < wnevt; i++){
    wchain -> GetEntry(i);
    if(i != 0 && i%500000 == 0) cout << i << endl;

    int nchw = wevt -> GetN();
    if(nchw < 20) continue;

    wqsum = 0;
    for(int j = 0; j < nchw; j++){
      WCPmt *pmt = wevt -> Get(j);
      wqsum += pmt -> GetQtot();
    }
    
    stp = 0;
    stpp = 0;
    wtime = wevt -> GetTriggerTime();
    //cout << wtime/1e9 << endl;

    for(int j = 0; j < 2; j++){
      for(int k = pnevt_d[j]; k < pnevt[j]; k++){
	pchain[j] -> GetEntry(k);

	ptime[j] = pevt[j] -> GetTriggerTime();
	if(ptime[j]-wtime > 1e3){
	  pnevt_d[j] = k;
	  break;
	}

	if(TMath::Abs(ptime[j] - wtime) < 1e3){
	  stpp = 1;
	  
	  his -> Fill(ptime[j]-wtime);
	  h2tm -> Fill(ptime[j]-wtime, nchw);
	  h2tq -> Fill(ptime[j]-wtime, wqsum);

	  if(ptime[j]-wtime > 200 && ptime[j]-wtime < 400){
	    stp = 1;

	    int nchp = pevt[j] -> GetN();
	    for(int k = 0; k < nchp; k++){
	      PSHit *hit = pevt[j] -> Get(k);
	      int id = hit -> GetID();

	      if((id > 47  && id < 59) ||
		 (id > 112 && id < 124)){
		if(id < 60)
		  id -= 48;
		else
		  id = id-113+11;
	
		for(int k = 0; k < 4; k++)
		  q[k] = hit -> GetQ(k);
		
		pqsum[id] = hit -> GetQsum();
		if(pqsum[id] < 0 ||
		   q[0] < 0 || q[1] < 0 ||
		   q[2] < 0 || q[3] < 0){
		  continue;
		}

		hqsumP[id] -> Fill(pqsum[id]);
	      }
	    }
	  }
	  else
	    stp = 2;
	}
      }
    }
    
    if(stpp == 1)
      hQsumW -> Fill(wqsum);
    if(stp == 1)
      hQsumW_iw -> Fill(wqsum);
    else if(stp == 2)
      hQsumW_ow -> Fill(wqsum);
  }

  TF1 *func = new TF1("func", twolandau, 0, 100000, 6);
  func -> SetParameter(0, 115);
  func -> SetParameter(1, 11000);
  func -> SetParameter(2, 4000);
  func -> SetParameter(3, 200);
  func -> SetParameter(4, 25000);
  func -> SetParameter(5, 3500);

  hQsumW_iw -> Fit(func, "R0");
  
  TCanvas *can = new TCanvas("can", "", 800, 600);
  his -> Draw();
  can -> SetLogy();

  TCanvas *can_t = new TCanvas("can_t", "", 800, 1200);
  can_t -> Divide(1, 2);
  can_t -> cd(1);
  h2tm -> SetMarkerStyle(8);
  h2tm -> SetMarkerSize(0.5);
  h2tm -> Draw();
  can_t -> cd(2);
  h2tq -> SetMarkerStyle(8);
  h2tq -> SetMarkerSize(0.5);
  h2tq -> Draw();
  
  TCanvas *can_qsum = new TCanvas("can_qsum", "", 800, 600);
  hQsumW -> SetLineColor(kBlack);
  hQsumW -> Draw();
  hQsumW_iw -> SetLineColor(kRed);
  hQsumW_iw -> Draw("same");
  hQsumW_ow -> SetLineColor(kBlue);
  hQsumW_ow -> Draw("same");
  func -> Draw("same");
  can_qsum -> SetLogy();

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

  // TFile of("./hist/muonHist_bot_coinWCMD.root", "recreate");
  // for(int i = 0; i < 22; i++)
  //   hqsumP[i] -> Write();
  // of.Close();
}
