R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libMuonObjs)

void plot_Qa()
{
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_psd");
  chain -> Add("/y/kmseo/data/prd_psd_000109_wQx_v2.root");
  // chain -> Add("/y/kmseo/data/prd_psd_000110_wQx_v2.root");
  // chain -> Add("/y/kmseo/data/prd_psd_000111_wQx_v2.root");

  PSEvent *pevt = new PSEvent();
  chain -> SetBranchAddress("PSEvent", &pevt);

  const int col[5] = {1, 2, 4, 6, 8};
  double qcut[10] = { 300, 1000, 1450, 1850, 2100,
                     2200, 2400, 2400, 2400, 2400};

  
  TH1D *hqa[10];
  TH2D *hqax[10][4];
  TH2D *hqqx[10][4];
  for(int i = 0; i < 10; i++){
    hqa[i] = new TH1D(Form("hqa_%d", i), "", 200, 0, 20000);
    hqa[i] -> SetXTitle("Qsum");
    hqa[i] -> SetYTitle("Number fo Entries");

    for(int j = 0; j < 4; j++){
      hqax[i][j] = new TH2D(Form("hqax_%d_%d", i, j), "", 200, 0, 20000,
			    20, 20, 40);
      hqax[i][j] -> SetXTitle("Qsum");
      hqax[i][j] -> SetYTitle("Qx");

      hqqx[i][j] = new TH2D(Form("hqqx_%d_%d", i, j), "", 200, 0, 20000,
			    200, 0, 500);
      hqqx[i][j] -> SetXTitle("Qsum");
      hqqx[i][j] -> SetYTitle("Qsum/Qx");
}
  }
  TH1D *hqsum = new TH1D("hqsum", "", 200, 0, 20000);
  hqsum -> SetXTitle("Qsum");
  hqsum -> SetYTitle("Number of Entries");
  
  double nmuon[2][22] = {0};
  double qsum;
  double q[4], qa[4][10], qasum[10], qm, qmqs, time, p1, p2;
  int nch, id, did, chpsd, multi, qx[4];
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

      PSHit *hit = pevt -> Get(j);

      id = hit -> GetID();
      // if(id == 0)
      // 	cout << id << endl;

      if(id != 48) continue;
      
      if((id > 47  && id < 59) ||
	 (id > 112 && id < 124)){
	if(id < 60)
	  id -= 48;
	else
	  id = id-113+11;

	
	for(int k = 0; k < 4; k++){
	  q[k] = hit -> GetQ(k);
	  qx[k] = hit -> GetQx(k);
	  for(int l = 0; l < 10; l++){
	    qa[k][l] = hit -> GetQa(k, l);
	  }
	}
	qsum = hit -> GetQsum();
	if(q[0] > 2000 || q[1] > 2000 ||
	   q[2] > 2000 || q[3] > 2000){
	  hqsum -> Fill(qsum);
	}

	
	for(int l = 0; l < 10; l++){
	  qasum[l] = hit -> GetQasum(l);
	  //cout << id << " " << l << " " << qasum[l] << endl;
	  if(qasum[l] > qcut[l]){
	     hqa[l] -> Fill(qasum[l]);
	     for(int m = 0; m < 4; m++){
	       hqax[l][m] -> Fill(qa[m][l], qx[m]);
	       hqqx[l][m] -> Fill(qa[m][l], qa[m][l]/qx[m]);
	     }
	  }
	}
      }
    }
  }

  double rtime = 58453.62+343964.74+523412.83;
  double tmuon[2] = {0};
  double area = 1.67*0.3*22;
  
  TCanvas *can = new TCanvas("can", "", 800, 600);
  hqsum -> SetLineWidth(2);
  hqsum -> SetMaximum(1e5);
  hqsum -> Draw();
  for(int i = 0; i < 10; i++){
    hqa[i] -> SetLineColor(i+1);
    if(i == 9)
      hqa[i] -> SetLineColor(i+2);

    hqa[i] -> Draw("same");
  }
  can -> SetLogy();

  TCanvas *can_hqax = new TCanvas("can_hqax", "", 1200, 900);
  can_hqax -> Divide(2, 2);
  for(int i = 0; i < 4; i++){
    can_hqax -> cd(i+1);

    hqqx[9][i] -> Draw("colz");
  }
}
