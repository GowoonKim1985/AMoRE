R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libMuonObjs)

void plot_Qratio()
{
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_psd");
  chain -> Add("/y/kmseo/data/prd_psd_000109_wQx.root");
  chain -> Add("/y/kmseo/data/prd_psd_000110_wQx.root");
  //chain -> Add("/y/kmseo/data/prd_psd_000111_wQx.root");

  PSEvent *pevt = new PSEvent();
  chain -> SetBranchAddress("PSEvent", &pevt);

  const int col[5] = {1, 2, 4, 6, 8};

  double qcut[22] = {
    2000, 2500, 2500, 3500,
    2500, 2500, 2000, 2000,
    2500, 2500, 2500,
    2000, 2500, 2500, 2500,
    1500, 2000, 2000, 2000,
    2500, 2500, 1500
  };
  // double qcut[22] = {
  //   500, 500, 500, 500,
  //   500, 500, 500, 500,
  //   500, 500, 500,
  //   500, 500, 500, 500,
  //   500, 500, 500, 500,
  //   500, 500, 500
  // };
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

  TF1 *fu_p1p2_sel[22];
  TF1 *fl_p1p2_sel[22];
  double offset[22][2] = {
    {0.40, 0.50}, {0.40, 0.45}, {0.40, 0.50}, {0.50, 0.50},
    {0.60, 0.45}, {0.65, 0.40}, {0.50, 0.50}, {0.55, 0.50},
    {0.45, 0.45}, {0.55, 0.55}, {0.50, 0.50},

    {0.55, 0.50}, {0.50, 0.45}, {0.45, 0.50}, {0.55, 0.50},
    {0.60, 0.50}, {0.50, 0.45}, {0.50, 0.50}, {0.45, 0.55},
    {0.50, 0.45}, {0.45, 0.50}, {0.60, 0.60}
  };

  TH1D *hQsum[22];
  TH1D *hQsum_sel[22];
  TH1D *hUDR[22];
  TH1D *hUDR_sel[22];
  TH2D *hUD[22];
  TH2D *hUD_sel[22];
  TH2D *h2R[22];
  TH2D *h2R_sel[22];
  TH2D *hQsR[22];
  TH2D *hQsR_sel[22];
  TH1D *hQru[22];
  TH1D *hQru_sel[22];
  TH1D *hQrl[22];
  TH1D *hQrl_sel[22];
  TH2D *hQrul[22];
  TH2D *hQrul_sel[22];
  TH2D *hQsRul[22];
  TH2D *hQsRul_sel[22];
  TH2D *hQmQsRul[22];
  TH2D *hQmQsRul_sel[22];
  TH2D *hQmQs[22];
  TH2D *hQmQs_sel[22];
  for(int i = 0; i < 22; i++){
    hQsum[i] = new TH1D(Form("hQsum_%d", i), "", 250, 0, 50000);
    hQsum[i] -> SetXTitle("Qsum");
    hQsum[i] -> SetYTitle("Number of Entries");

    hQsum_sel[i] = new TH1D(Form("hQsum_sel_%d", i), "", 250, 0, 50000);
    hQsum_sel[i] -> SetXTitle("Qsum");
    hQsum_sel[i] -> SetYTitle("Number of Entries");
    
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

    h2R[i] = new TH2D(Form("h2R_%d", i), "", 200, 0, 10, 100, 0, 1);
    h2R[i] -> SetXTitle("(Q1+Q2)/(Q3+Q4)");
    h2R[i] -> SetYTitle("QmQs");

    h2R_sel[i] = new TH2D(Form("h2R_sel_%d", i), "", 200, 0, 10, 100, 0, 1);
    h2R_sel[i] -> SetXTitle("(Q1+Q2)/(Q3+Q4)");
    h2R_sel[i] -> SetYTitle("QmQs");
    h2R_sel[i] -> SetMarkerStyle(4);
    h2R_sel[i] -> SetMarkerColor(kRed);

    hQsR[i] = new TH2D(Form("hQsR_%d", i), "", 200, 0, 50000, 200, 0, 10);
    hQsR[i] -> SetXTitle("Qsum");
    hQsR[i] -> SetYTitle("(Q1+Q2)/(Q3+Q4)");

    hQsR_sel[i] = new TH2D(Form("hQsR_sel_%d", i), "", 200, 0, 50000, 200, 0, 10);
    hQsR_sel[i] -> SetXTitle("Qsum");
    hQsR_sel[i] -> SetYTitle("(Q1+Q2)/(Q3+Q4)");
    hQsR_sel[i] -> SetMarkerStyle(4);
    hQsR_sel[i] -> SetMarkerColor(kRed);

    hQru[i] = new TH1D(Form("hQru_%d", i), "", 200, -1, 1);
    hQru[i] -> SetXTitle("p1");
    hQru[i] -> SetYTitle("Number of Entries");

    hQru_sel[i] = new TH1D(Form("hQru_sel_%d", i), "", 200, -1, 1);
    hQru_sel[i] -> SetXTitle("p1");
    hQru_sel[i] -> SetYTitle("Number of Entries");
    hQru_sel[i] -> SetLineColor(kRed);

    hQrl[i] = new TH1D(Form("hQrl_%d", i), "", 200, -1, 1);
    hQrl[i] -> SetXTitle("p2");
    hQrl[i] -> SetYTitle("Number of Entries");

    hQrl_sel[i] = new TH1D(Form("hQrl_sel_%d", i), "", 200, -1, 1);
    hQrl_sel[i] -> SetXTitle("p2");
    hQrl_sel[i] -> SetYTitle("Number of Entries");
    hQrl_sel[i] -> SetLineColor(kRed);
    
    hQrul[i] = new TH2D(Form("hQrul_%d", i), "",
			200, -1, 1, 200, -1, 1);
    hQrul[i] -> SetXTitle("p1");
    hQrul[i] -> SetYTitle("p2");

    hQrul_sel[i] = new TH2D(Form("hQrul_sel_%d", i), "",
			    200, -1, 1, 200, -1, 1);
    hQrul_sel[i] -> SetXTitle("p1");
    hQrul_sel[i] -> SetYTitle("p2");
    hQrul_sel[i] -> SetMarkerStyle(4);
    hQrul_sel[i] -> SetMarkerColor(kRed);

    fu_p1p2_sel[i] = new TF1(Form("fu_p1p2_sel_%d", i), "x+[0]", -1, 1);
    fu_p1p2_sel[i] -> SetParameter(0, offset[i][0]);
    fl_p1p2_sel[i] = new TF1(Form("fl_p1p2_sel_%d", i), "x-[0]", -1, 1);
    fl_p1p2_sel[i] -> SetParameter(0, offset[i][1]);
    
    hQsRul[i] = new TH2D(Form("hQsRul_%d", i), "",
			500, 0, 50000, 200, -20, 20);
    hQsRul[i] -> SetXTitle("Qsum");
    hQsRul[i] -> SetYTitle("p1/p2");

    hQsRul_sel[i] = new TH2D(Form("hQsRul_sel_%d", i), "",
			 500, 0, 50000, 200, -20, 20);
    hQsRul_sel[i] -> SetXTitle("Qsum");
    hQsRul_sel[i] -> SetYTitle("p1/p2");
    hQsRul_sel[i] -> SetMarkerStyle(4);
    hQsRul_sel[i] -> SetMarkerColor(kRed);

    hQmQsRul[i] = new TH2D(Form("hQmQsRul_%d", i), "",
			   500, 0, 1, 200, -20, 20);
    hQmQsRul[i] -> SetXTitle("QmQs");
    hQmQsRul[i] -> SetYTitle("p1/p2");

    hQmQsRul_sel[i] = new TH2D(Form("hQmQsRul_sel_%d", i), "",
			       500, 0, 1, 200, -20, 20);
    hQmQsRul_sel[i] -> SetXTitle("QmQs");
    hQmQsRul_sel[i] -> SetYTitle("p1/p2");
    hQmQsRul_sel[i] -> SetMarkerStyle(4);
    hQmQsRul_sel[i] -> SetMarkerColor(kRed);

    hQmQs[i] = new TH2D(Form("hQmQs_%d", i), "",
			500, 0, 50000, 200, 0, 1);
    hQmQs[i] -> SetYTitle("QmQs");
    hQmQs[i] -> SetXTitle("Qsum");

    hQmQs_sel[i] = new TH2D(Form("hQmQs_sel_%d", i), "",
			    500, 0, 50000, 200, 0, 1);
    hQmQs_sel[i] -> SetYTitle("QmQs");
    hQmQs_sel[i] -> SetXTitle("Qsum");
    hQmQs_sel[i] -> SetMarkerStyle(4);
    hQmQs_sel[i] -> SetMarkerColor(kRed);
  }

  double nmuon[2][22] = {0};
  double qsum[22] = {0};
  double q[4], qm, qmqs, time, p1, p2;
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

      //if(id != 48) continue;
      
      if((id > 47  && id < 59) ||
	 (id > 112 && id < 124)){
	chpsd = 0;
	if(id < 60)
	  id -= 48;
	else
	  id = id-113+11;

	
	for(int k = 0; k < 4; k++){
	  q[k] = hit -> GetQ(k);

	  //cout << k << " " << pevt -> GetTriggerTime() << endl;
	  
	  if(q[k] > qm){
	    qm = q[k];
	    chpsd = k;
	  }
	}
	
	qsum[id] = hit -> GetQsum();
	if(qsum[id] < 0 ||
	   q[0] < qcut[id] || q[1] < qcut[id] ||
	   q[2] < qcut[id] || q[3] < qcut[id]){
	  continue;
	}
	qmqs = qm/qsum[id];
	// cout << qsum[id] << " "
	//      << q[0] << " " << q[1] << " "
	//      << q[2] << " " << q[3] << " "
	//      << qm << endl;
	p1 = (q[0]-q[1])/(q[0]+q[1]);
	p2 = (q[2]-q[3])/(q[2]+q[3]);

	hQsum[id] -> Fill(qsum[id]);
	
	hUD[id] -> Fill(q[0]+q[1], q[2]+q[3]);
	hUDR[id] -> Fill((q[0]+q[1])/(q[2]+q[3]));
	h2R[id] -> Fill((q[0]+q[1])/(q[2]+q[3]), qmqs);
	hQsR[id] -> Fill(qsum[id], (q[0]+q[1])/(q[2]+q[3]));	

	hQru[id] -> Fill(p1);
	hQrl[id] -> Fill(p2);
	hQrul[id] -> Fill(p1, p2);
	hQsRul[id] -> Fill(qsum[id], p1/p2);
	hQmQsRul[id] -> Fill(qmqs, p1/p2);
	hQmQs[id] -> Fill(qsum[id], qmqs);
	if(p1 < p2 + offset[id][0] &&
	   p1 > p2 - offset[id][1] &&
	   (q[0]+q[1])/(q[2]+q[3]) < 3 &&
	   qmqs < cut_qmqs[id]){
	  hQsum_sel[id] -> Fill(qsum[id]);

	  nmuon[0][id]++;
	}
	if(qsum[id] > cut_qsum[id] && qmqs < cut_qmqs[id] &&
	   p1 < p2 + offset[id][0] &&
	   p1 > p2 - offset[id][1] &&
	   (q[0]+q[1])/(q[2]+q[3]) < 3){
	  nmuon[1][id]++;
	  
	  hUD_sel[id] -> Fill(q[0]+q[1], q[2]+q[3]);
	  hUDR_sel[id] -> Fill((q[0]+q[1])/(q[2]+q[3]));
	  h2R_sel[id] -> Fill((q[0]+q[1])/(q[2]+q[3]), qmqs);
	  hQsR_sel[id] -> Fill(qsum[id], (q[0]+q[1])/(q[2]+q[3]));	

	  hQru_sel[id] -> Fill(p1);
	  hQrl_sel[id] -> Fill(p2);
	  hQrul_sel[id] -> Fill(p1, p2);
	  hQsRul_sel[id] -> Fill(qsum[id], p1/p2);
	  hQmQsRul_sel[id] -> Fill(qmqs, p1/p2);

	  hQmQs_sel[id] -> Fill(qsum[id], qmqs);
  	}
      }
    }
  }

  double rtime = 58453.62+343964.74+523412.83;
  double tmuon[2] = {0};
  double area = 1.67*0.3*22;
  // for(int i = 0; i < 22; i++){
  //   cout << i << " " << nmuon[1][i] << " " << nmuon[0][i]<< endl;
  //   tmuon[1] += nmuon[1][i];
  //   tmuon[0] += nmuon[0][i];
  // }
  cout << tmuon[1] << " "
       // << tmuon[1]/area << " "
       // << tmuon[1]/(rtime/86400.) << " "
       << tmuon[1]/area/(rtime/86400.) << endl;
  cout << tmuon[0] << " "
       // << tmuon[0]/area << " "
       // << tmuon[0]/(rtime/86400.) << " "
       << tmuon[0]/area/(rtime/86400.) << endl;
  
  TCanvas *can = new TCanvas("can", "", 1200, 1000);
  can -> Divide(2, 2);
  TPad *pad[2];
  can -> cd(1);
  pad[0] = (TPad*)can -> cd(1);
  hQru[0] -> Draw();
  hQru_sel[0] -> Draw("same");
  pad[0] -> SetLogy();
  can -> cd(2);
  pad[1] = (TPad*)can -> cd(2);
  hQrl[0] -> Draw();
  hQrl_sel[0] -> Draw("same");
  pad[1] -> SetLogy();
  can -> cd(3);
  hQrul[0] -> Draw("colz");
  hQrul_sel[0] -> Draw("same");
  fu_p1p2_sel[0] -> Draw("same");
  fl_p1p2_sel[0] -> Draw("same");
  can -> cd(4);
  hQmQsRul[0] -> Draw("colz");
  hQmQsRul_sel[0] -> Draw("same");
  // hQsRul[0] -> Draw("colz");
  // hQsRul_sel[0] -> Draw("same");

  
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

  // TCanvas *can_q2R1 = new TCanvas("can_q2R1", "2R 1", 1600, 900);
  // can_q2R1 -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can_q2R1 -> cd(i+1);
    
  //   h2R[i] -> Draw("colz");
  //   h2R_sel[i] -> Draw("same");
  // }

  // TCanvas *can_q2R2 = new TCanvas("can_q2R2", "2R 2", 1600, 900);
  // can_q2R2 -> Divide(4, 3);
  // for(int i = 11; i < 22; i++){
  //   can_q2R2 -> cd(i-10);
    
  //   h2R[i] -> Draw("colz");
  //   h2R_sel[i] -> Draw("same");
  // }

  // TCanvas *can_QsR1 = new TCanvas("can_QsR1", "QsR 1", 1600, 900);
  // can_QsR1 -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can_QsR1 -> cd(i+1);
    
  //   hQsR[i] -> Draw("colz");
  //   hQsR_sel[i] -> Draw("same");
  // }

  // TCanvas *can_QsR2 = new TCanvas("can_QsR2", "QsR 2", 1600, 900);
  // can_QsR2 -> Divide(4, 3);
  // for(int i = 11; i < 22; i++){
  //   can_QsR2 -> cd(i-10);
    
  //   hQsR[i] -> Draw("colz");
  //   hQsR_sel[i] -> Draw("same");
  // }

  // TFile of("./hist/muonHist_Qratio.root", "recreate");
  // for(int i = 0; i < 22; i++){
  //   hQsum[i] -> Write();
  //   hQsum_sel[i] -> Write();

  //   hQsR[i] -> Write();
  //   hQsR_sel[i] -> Write();
    
  //   hQru[i] -> Write();
  //   hQrl[i] -> Write();
  //   hQrul[i] -> Write();
  //   hQsRul[i] -> Write();
  //   hQmQsRul[i] -> Write();
  //   hQmQs[i] -> Write();
    
  //   hQru_sel[i] -> Write();
  //   hQrl_sel[i] -> Write();
  //   hQrul_sel[i] -> Write();
  //   hQsRul_sel[i] -> Write();
  //   hQmQsRul_sel[i] -> Write();
  //   hQmQs_sel[i] -> Write();
  // }
  // of.Close();
}
