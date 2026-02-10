R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libMuonObjs)

void plot_psd()
{
  int runnum = 318;
  
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_psd");
  for(int i = 0; i < 100; i++)
    chain -> Add(Form("/data/kmseo/PSMD/%06d/prd_psd_daq2_%06d_%05d.root", runnum, runnum, i));
  //chain -> Add(Form("/data/kmseo/PSMD/%06d/prd_psd_daq2_%06d_*.root", runnum, runnum));
  
  PSEvent *pevt = new PSEvent();
  chain -> SetBranchAddress("PSEvent", &pevt);

  const int col[5] = {1, 2, 4, 6, 8};

  int cut_qsum[22] =
    {13000, 18000, 17000, 22000,
     17000, 16000, 13000, 14000,
     19000, 16000, 16000,
     16000, 16000, 18000, 17000,
     10000, 13000, 13000, 14000,
     19000, 17000, 10500};
  double cut_qmqs[22] =
    {0.4, 0.4, 0.4, 0.4,
     0.4, 0.4, 0.4, 0.4,
     0.4, 0.4, 0.4,
     0.4, 0.4, 0.4, 0.4,
     0.4, 0.4, 0.4, 0.4,
     0.4, 0.4, 0.4};

  TH1D *hUDR[22];
  TH1D *hUDR_sel[22];
  TH2D *hUD[22];
  TH2D *hUD_sel[22];
  TH1D *hqsum[22];
  TH1D *hqsum_sel[22];
  TH2D *hscat[22];
  TH1D *hqmqs[22][5];
  for(int i = 0; i < 22; i++){
    hqsum[i] = new TH1D(Form("hqsum_%d", i), "", 200, 0, 50000);
    hqsum_sel[i] = new TH1D(Form("hqsum_sel_%d", i), "", 200, 0, 50000);

    hscat[i] = new TH2D(Form("hscat_%d", i), "", 500, 0, 50000, 500, 0.25, 1);
    hscat[i] -> SetYTitle("Qmax/Qtot");

    hUDR[i] = new TH1D(Form("hUDR_%d", i), "", 200, 0, 10);
    hUDR[i] -> SetXTitle("(Q1+Q2)/(Q3+Q4)");
    hUDR[i] -> SetYTitle("Number fo Entries");

    hUDR_sel[i] = new TH1D(Form("hUDR_sel_%d", i), "", 200, 0, 10);
    hUDR_sel[i] -> SetXTitle("(Q1+Q2)/(Q3+Q4)");
    hUDR_sel[i] -> SetYTitle("Number fo Entries");
    hUDR_sel[i] -> SetLineColor(kRed);
    
    hUD[i] = new TH2D(Form("hUD_%d", i), "", 300, 0, 30000, 300, 0, 30000);
    hUD[i] -> SetXTitle("Q1+Q2");
    hUD[i] -> SetYTitle("Q3+Q4");

    hUD_sel[i] = new TH2D(Form("hUD_sel_%d", i), "", 300, 0, 30000, 300, 0, 30000);
    hUD_sel[i] -> SetXTitle("Q1+Q2");
    hUD_sel[i] -> SetYTitle("Q3+Q4");
    hUD_sel[i] -> SetMarkerStyle(4);
    hUD_sel[i] -> SetMarkerColor(kRed);
    if(i < 11){
      hqsum[i] -> SetXTitle(Form("PSD.%d Qsum", i+48));
      for(int j = 0; j < 5; j++){
	hqmqs[i][j] = new TH1D(Form("hqmqs_%d_%d", i, j), "", 200, 0, 1);
	hqmqs[i][j] -> SetXTitle(Form("PSD.%d Qmax/Qtot", i+48));
	hqmqs[i][j] -> SetLineColor(col[j]);
	hscat[i] -> SetXTitle(Form("Qsum of Det.%d", i+48));
      }
    }
    else{
      hqsum[i] -> SetXTitle(Form("PSD.%d Qsum", i+48+65-10));
      for(int j = 0; j < 5; j++){
	hqmqs[i][j] = new TH1D(Form("hqmqs_%d_%d", i, j), "", 200, 0, 1);
	hqmqs[i][j] -> SetXTitle(Form("PSD.%d Qmax/Qtot", i+48+65-10));
	hqmqs[i][j] -> SetLineColor(col[j]);
	hscat[i] -> SetXTitle(Form("Qsum of Det.%d", i+48+65-10));
      }
    }
    hqsum[i] -> SetYTitle("Number of Entries");

    hqsum[i] -> SetLineColor(kBlack);
    hqsum_sel[i] -> SetLineColor(kRed);
  }

  TH1D *hoccu  = new TH1D("hoccu", "", 22, 0, 22);
  hoccu -> SetXTitle("ID");
  hoccu -> SetYTitle("Number of Entries");
  TH1D *hmulti = new TH1D("hmulti", "", 22, 0, 22);
  hmulti -> SetXTitle("Multiplicity");
  hmulti -> SetYTitle("Number of Entries");

  double nmuon[22] = {0};
  double qsum[22] = {0};
  double q[4], qm, qmqs, time;
  int nch, id, did, chpsd, multi;
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

      if((id > 47  && id < 59) ||
	 (id > 112 && id < 124)){
	chpsd = 0;
	if(id < 60)
	  id -= 48;
	else
	  id = id-113+11;
	
	for(int k = 0; k < 4; k++){
	  q[k] = hit -> GetQ(k);
	  
	  if(q[k] > qm){
	    qm = q[k];
	    chpsd = k;
	  }
	}
	qsum[id] = hit -> GetQsum();
	if(qsum[id] < 0 ||
	   q[0] < 0 || q[1] < 0 ||
	   q[2] < 0 || q[3] < 0){
	  continue;
	}
	qmqs = qm/qsum[id];
	// cout << qsum[id] << " "
	//      << q[0] << " " << q[1] << " "
	//      << q[2] << " " << q[3] << " "
	//      << qm << endl;

	hqsum[id] -> Fill(qsum[id]);
	hqmqs[id][0] -> Fill(qmqs);
	hqmqs[id][chpsd+1] -> Fill(qmqs);
	hUD[id] -> Fill(q[0]+q[1], q[2]+q[3]);
	hUDR[id] -> Fill((q[0]+q[1])/(q[2]+q[3]));	
	hscat[id] -> Fill(qsum[id], qmqs);
	if(qsum[id] > cut_qsum[id] && qmqs < cut_qmqs[id]){
	  nmuon[id]++;
	  
	  multi++;
	  hoccu -> Fill(id);

	  hUD_sel[id] -> Fill(q[0]+q[1], q[2]+q[3]);
	  hUDR_sel[id] -> Fill((q[0]+q[1])/(q[2]+q[3]));	
	}
      }
    }
    if(multi != 0)
      hmulti -> Fill(multi);
  }

  double rtime = 58453.62+343964.74+523412.83;
  //double rtime = 523412.83;
  double tmuon = 0;
  double area = 1.67*0.3*22;
  TLine *l[22];
  for(int i = 0; i < 22; i++){
    //cout << i << " " << nmuon[i] << endl;
    //tmuon += nmuon[i]/(1.67*0.3);
    tmuon += nmuon[i];

    l[i] = new TLine(cut_qsum[i], 0, cut_qsum[i], hqsum[i] -> GetMaximum());
    l[i] -> SetLineColor(kBlue);
    l[i] -> SetLineWidth(2);
  }
  //cout << tmuon << " " << tmuon/(rtime/86400.) << endl;
  cout << tmuon << " "
       // << tmuon/area << " "
       // << tmuon/(rtime/86400.) << " "
       << tmuon/area/(rtime/86400.) << endl;

  TGaxis::SetMaxDigits(4);
  // TCanvas *can_m1 = new TCanvas("can_m1", "Qsum1", 1600, 900);
  // can_m1 -> Divide(4, 3);
  // TPad *pm1[11];
  // for(int i = 0; i < 11; i++){
  //   can_m1 -> cd(i+1);
  //   pm1[i] = (TPad*)can_m1 -> cd(i+1);
    
  //   hqsum[i] -> Draw();
  //   //hqsum_sel[i] -> Draw("same");

  //   l[i] -> Draw("same");
  //   pm1[i] -> SetLogy();
  // }

  TCanvas *can_m2 = new TCanvas("can_m2", "Qsum2", 1600, 900);
  can_m2 -> Divide(4, 3);
  TPad *pm2[11];
  for(int i = 11; i < 22; i++){
    can_m2 -> cd(i-10);
    pm2[i-11] = (TPad*)can_m2 -> cd(i-10);
    
    hqsum[i] -> Draw();
    //hqsum_sel[i] -> Draw("same");
    l[i] -> Draw("same");
    pm2[i-11] -> SetLogy();
  }
  
  // TCanvas *can_scat1 = new TCanvas("can_scat1", "scat1", 1600, 900);
  // can_scat1 -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can_scat1 -> cd(i+1);
    
  //   hscat[i] -> Draw("colz");
  // }

  // TCanvas *can = new TCanvas("can", "", 800, 600);
  // hscat[0] -> Draw();

  // TCanvas *can_scat2 = new TCanvas("can_scat2", "scat2", 1600, 900);
  // can_scat2 -> Divide(4, 3);
  // for(int i = 11; i < 22; i++){
  //   can_scat2 -> cd(i-10);
    
  //   hscat[i] -> Draw("colz");
  // }

  // TPad *pad_multi;
  // TCanvas *can_occu = new TCanvas("can_occu", "", 1600, 600);
  // can_occu -> Divide(2, 1);
  // can_occu -> cd(1);
  // hoccu -> Sumw2();
  // hoccu -> Draw();
  // can_occu -> cd(2);
  // pad_multi = (TPad*)can_occu -> cd(2);
  // hmulti -> Draw();
  // pad_multi -> SetLogy();

  // TLine *l_qmqs[22];
  // TCanvas *can_qmqs1 = new TCanvas("can_qmqs1", "QmQs1", 1600, 900);
  // can_qmqs1 -> Divide(4, 3);
  // TPad *pqmqs1[11];
  // for(int i = 0; i < 11; i++){
  //   can_qmqs1 -> cd(i+1);
  //   pqmqs1[i] = (TPad*)can_qmqs1 -> cd(i+1);

  //   hqmqs[i][0] -> Draw();
  //   for(int j = 1; j < 5; j++)
  //     hqmqs[i][j] -> Draw("same");

  //   l_qmqs[i] = new TLine(cut_qmqs[i], 0,
  // 			  cut_qmqs[i], hqmqs[i][0] -> GetMaximum()*1.1);
  //   l_qmqs[i] -> SetLineColor(kBlue);
  //   l_qmqs[i] -> SetLineWidth(2);
  //   l_qmqs[i] -> Draw("same");

  //   pqmqs1[i] -> SetLogy();
  // }
  
  // TCanvas *can_qmqs2 = new TCanvas("can_qmqs2", "QmQs2", 1600, 900);
  // can_qmqs2 -> Divide(4, 3);
  // TPad *pqmqs2[11];
  // for(int i = 11; i < 22; i++){
  //   can_qmqs2 -> cd(i-10);
  //   pqmqs2[i-11] = (TPad*)can_qmqs2 -> cd(i-10);
    
  //   hqmqs[i][0] -> Draw();
  //   for(int j = 1; j < 5; j++)
  //     hqmqs[i][j] -> Draw("same");
   
  //   pqmqs2[i-11] -> SetLogy();

  //   l_qmqs[i] = new TLine(cut_qmqs[i], 0,
  // 			  cut_qmqs[i], hqmqs[i][0] -> GetMaximum()*1.1);
  //   l_qmqs[i] -> SetLineColor(kBlue);
  //   l_qmqs[i] -> SetLineWidth(2);
  //   l_qmqs[i] -> Draw("same");
  // }

  // TPad *pUDR1[11];
  // TCanvas *can_qUDR1 = new TCanvas("can_qUDR1", "UDR 1", 1600, 900);
  // can_qUDR1 -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can_qUDR1 -> cd(i+1);
  //   pUDR1[i] = (TPad*)can_qUDR1 -> cd(i+1);
    
  //   hUDR[i] -> Draw();
  //   hUDR_sel[i] -> Draw("same");

  //   pUDR1[i] -> SetLogy();
  // }

  // TPad *pUDR2[11];
  // TCanvas *can_qUDR2 = new TCanvas("can_qUDR2", "UDR 2", 1600, 900);
  // can_qUDR2 -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can_qUDR2 -> cd(i+1);
  //   pUDR2[i] = (TPad*)can_qUDR2 -> cd(i+1);
    
  //   hUDR[i] -> Draw();
  //   hUDR_sel[i] -> Draw("same");

  //   pUDR2[i] -> SetLogy();
  // }

  // TCanvas *can_qUD1 = new TCanvas("can_qUD1", "UD 1", 1600, 900);
  // can_qUD1 -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can_qUD1 -> cd(i+1);
    
  //   hUD[i] -> Draw();
  //   hUD_sel[i] -> Draw("same");
  // }

  // TCanvas *can_qUD2 = new TCanvas("can_qUD2", "UD 2", 1600, 900);
  // can_qUD2 -> Divide(4, 3);
  // for(int i = 11; i < 22; i++){
  //   can_qUD2 -> cd(i-10);
    
  //   hUD[i] -> Draw();
  //   hUD_sel[i] -> Draw("same");
  // }
  
  // TFile of("./hist/muonHist.root", "recreate");
  // for(int i = 0; i < 22; i++){
  //   hqsum[i] -> Write();
  //   hUDR[i] -> Write();
  //   hUDR_sel[i] -> Write();
  // }
  // of.Close();
}
