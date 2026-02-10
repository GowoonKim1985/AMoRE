R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libMuonObjs)

void plot_qtot()
{
  TStopwatch st;
  st.Start();

  gStyle -> SetOptStat(0);

  const int nthreads = 20;
  ROOT::EnableImplicitMT(nthreads);
  
  TChain *chain = new TChain("prd_psd");
  chain -> Add("/y/kmseo/data/PSMD/prd_psd_000109.root");
  chain -> Add("/y/kmseo/data/PSMD/prd_psd_000110.root");
  chain -> Add("/y/kmseo/data/PSMD/prd_psd_000111_1.root");
  chain -> Add("/y/kmseo/data/PSMD/prd_psd_000111_2.root");

  PSEvent *pevt = new PSEvent();
  chain -> SetBranchAddress("PSEvent", &pevt);

  const int col[5] = {1, 2, 4, 6, 8};
  const int npanel = 22;
  
  int cut_qsum[22] =
    {7000, 7000, 8000, 7000, 7000, 7000,
     7000, 7000, 6000, 7000, 7000,
     7000, 7000, 8000, 8000, 6000, 7000,
     7000, 7000, 7000, 7000, 6000
    };

  TH1D *hqsum[22];
  TH1D *hqsum_sel[22];
  TH1D *hqtot[22][4];
  TH1D *hqtot_sel[22][4];
  for(int i = 0; i < 22; i++){
    hqsum[i] = new TH1D(Form("hqsum_%d", i), "", 200, 0, 50000);
    hqsum_sel[i] = new TH1D(Form("hqsum_sel_%d", i), "", 200, 0, 50000);
    hqsum_sel[i] -> SetLineColor(kRed);
    
    for(int j = 0; j < 4; j++){
      hqtot[i][j] = new TH1D(Form("hqtot_%d_%d", i, j), "", 200, 0, 10000);
      hqtot[i][j] -> SetXTitle(Form("Qtot of %d", j+1));
      hqtot[i][j] -> SetYTitle("Number of Entries");

      hqtot_sel[i][j] = new TH1D(Form("hqtot_sel_%d_%d", i, j), "", 200, 0, 10000);
      hqtot_sel[i][j] -> SetXTitle(Form("Qtot of %d", j+1));
      hqtot_sel[i][j] -> SetYTitle("Number of Entries");

      hqtot_sel[i][j] -> SetLineColor(kRed);
    }
    if(i < 11){
      hqsum[i] -> SetXTitle(Form("PSD.%d Qsum", i+48));
    }
    else{
      hqsum[i] -> SetXTitle(Form("PSD.%d Qsum", i+48+65-10));
    }
    hqsum[i] -> SetYTitle("Number of Entries");
  }

  double qsum[22] = {0};
  double q[4];
  int ntot[22] = {0};
  int ntot_sel[22] = {0};
  int nch, id, did;
  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);
    if(i != 0 && i%1000000 == 0){
      cout << i << " | ";
      st.Print();
      st.Start();
    }
    nch = pevt -> GetN();
    //cout << nch << endl;
    for(int j = 0; j < nch; j++){
      PSHit *hit = pevt -> Get(j);

      id = hit -> GetID();

      if((id > 47  && id < 59) ||
	 (id > 112 && id < 124)){
	if(id < 60)
	  id -= 48;
	else
	  id = id-113+11;

	//if(id != 0) continue;
	
	qsum[id] = hit -> GetQsum();
	for(int k = 0; k < 4; k++){
	  q[k] = hit -> GetQ(k);
	  hqtot[id][k] -> Fill(q[k]);
	}

	if(qsum[id] < 0 ||
	   q[0] < 0 || q[1] < 0 ||
	   q[2] < 0 || q[3] < 0){
	  continue;
	}

	ntot[id]++;
	hqsum[id] -> Fill(qsum[id]);
	if(qsum[id] > cut_qsum[id]){
	  ntot_sel[id]++;
	  for(int k = 0; k < 4; k++)
	    hqtot_sel[id][k] -> Fill(q[k]);
	}
	if(q[0] > 600 && q[1] > 600 &&
	   q[2] > 600 && q[3] > 600)
	  hqsum_sel[id] -> Fill(qsum[id]);
      }
    }
  }

  TLine *l[22];
  for(int i = 0; i < 22; i++){
    l[i] = new TLine(cut_qsum[i], 0, cut_qsum[i], hqsum[i] -> GetMaximum());
    l[i] -> SetLineColor(kBlue);
    l[i] -> SetLineWidth(2);

    cout << (double)ntot_sel[i]/ntot[i]*100 << endl;
  }

  TGaxis::SetMaxDigits(4);
  TCanvas *can[22];
  for(int n = 0; n < 22; n++){
    can[n] = new TCanvas(Form("can_%d", n), Form("can %d", n+1), 1800, 800);
    can[n] -> Divide(3, 2);
    TPad *pad[6];
    for(int i = 0; i < 6; i++){
      if(i == 3) continue;
      
      can[n] -> cd(i+1);
      pad[i] = (TPad*)can[n] -> cd(i+1);
      
      if(i == 0){
	hqsum[n] -> Draw();
	hqsum_sel[n] -> Draw("same");
	l[n] -> Draw("same");
      }
      else{
	if(i < 3){
	  hqtot[n][i-1] -> Draw();
	  hqtot_sel[n][i-1] -> Draw("same");
	}
	else{
	  hqtot[n][i-2] -> Draw();
	  hqtot_sel[n][i-2] -> Draw("same");
	}
      }
      pad[i] -> SetLogy();
    }
    //can[n] -> Print(Form("./figure/20230914/muonDist_%02d.png", n+1));
  }

  st.Stop();
}
