R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libMuonObjs)

void plot_pt()
{
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_psd");
  chain -> Add("/y/kmseo/data/prd_psd_000109.root");
  //chain -> Add("/y/kmseo/data/prd_psd_000110.root");

  PSEvent *pevt = new PSEvent();
  chain -> SetBranchAddress("PSEvent", &pevt);

  const int col[5] = {1, 2, 4, 6, 8};
  double par[4][3] = {{25.8, 0.0006, 25.2}, {25.8, 0.0006, 25.2},
		      {25.8, 0.0006, 25.2}, {25.8, 0.0006, 25.2}};
  TH2D *hpt[4];
  TF1 *func_sel[4];
  TLine *l[4];
  for(int i = 0; i < 4; i++){
    hpt[i] = new TH2D(Form("hpt_%d", i), "", 500, 0, 5000, 100, 20, 30);
    hpt[i] -> SetXTitle("Q");
    hpt[i] -> SetYTitle("Pulse time");

    l[i] = new TLine(0, par[i][0], 5000, par[i][0]);
    func_sel[i] = new TF1(Form("func_sel_%d", i), "[0]*x+[1]", 0, 5000);
    func_sel[i] -> SetParameter(0, par[i][1]);
    func_sel[i] -> SetParameter(1, par[i][2]);

    l[i] -> SetLineColor(kRed);
    l[i] -> SetLineWidth(1);
    func_sel[i] -> SetLineColor(kRed);
    func_sel[i] -> SetLineWidth(1);
  }
  TH1D *hqsum = new TH1D("hqsum", "", 500, 0, 50000);
  hqsum -> SetXTitle("Qsum");
  hqsum -> SetYTitle("Number of Entries");
  TH1D *hqsum_sel = new TH1D("hqsum_sel", "", 500, 0, 50000);
  hqsum_sel -> SetXTitle("Qsum");
  hqsum_sel -> SetYTitle("Number of Entries");

  double nmuon[22] = {0};
  double qsum[22] = {0};
  double q[4], qm, qmqs, time, pt[4], qsumsel;
  int nch, id, did, chpsd, multi, nn;
  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);
    if(i != 0 && i%1000000 == 0) cout << i << endl;

    nch = pevt -> GetN();
    //cout << nch << endl;
    time = pevt -> GetTriggerTime();

    multi = 0;
    for(int j = 0; j < nch; j++){
      qm = 0;
      qsumsel = 0;
      nn = 0;
      PSHit *hit = pevt -> Get(j);

      id = hit -> GetID();
      // if(id == 0)
      // 	cout << id << endl;

      if((id > 47  && id < 59) ||
	 (id > 112 && id < 124)){
	chpsd = 0;
	if(id < 60)
	  id -= 48;
	else
	  id = id-113+11;

	if(id != 0) continue;
	
	for(int k = 0; k < 4; k++){
	  q[k] = hit -> GetQ(k);
	  pt[k] = hit -> GetPt(k);
	  
	  if(q[k] > qm){
	    qm = q[k];
	    chpsd = k;
	  }
	}
	qsum[id] = hit -> GetQsum();
	if(qsum[id] < 0 ||
	   q[0] < 0 || q[1] < 0 ||
	   q[2] < 0 || q[3] < 0 ||
	   pt[0] < 0 || pt[1] < 0 ||
	   pt[2] < 0 || pt[3] < 0){
	  continue;
	}

	hqsum -> Fill(qsum[id]);
	//cout << Form("PulseTime: %.3f, %.3f, %.3f, %.3f with Qsum=%.1f", pt[0], pt[1], pt[2], pt[3], qsum[id]) << endl;
	for(int k = 0; k < 4; k++){
	  hpt[k] -> Fill(q[k], pt[k]);

	  if(pt[k] > par[k][0] &&
	     pt[k] < q[k]*par[k][1] + par[k][2] &&
	     nn == 0){
	    nn++;
	    hqsum_sel -> Fill(qsum[id]);
	  }
	}
      }
    }
  }

  TCanvas *can_pt = new TCanvas("can_pt", "", 1200, 900);
  can_pt -> Divide(2, 2);
  for(int i = 0; i < 4; i++){
    can_pt -> cd(i+1);
    hpt[i] -> Draw();
    l[i] -> Draw("same");
    func_sel[i] -> Draw("same");
  }

  TCanvas *can_qsum = new TCanvas("can_qsum", "", 800, 600);
  hqsum -> Draw();
  hqsum_sel -> SetLineColor(kRed);
  hqsum_sel -> Draw("same");
  
  // TFile of("./hist/PulseTime_PSMD48.root", "RECREATE");
  // for(int i = 0; i < 4; i++)
  //   hpt[i] -> Write();
  // of.Close();
}
