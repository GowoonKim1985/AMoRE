void draw_psmd_iqcom() {

    TFile *f1 = TFile::Open("./prod_2nd/psmd_all.root");
    TTree *t1 = (TTree*)f1->Get("psmd");
    //    TFile *f2 = TFile::Open("./prod_2nd/pwcoin_all.root");
    //    TTree *t2 = (TTree*)f2->Get("coin");
    TFile *f2 = TFile::Open("./prod_2nd/pwcoin_all_pbot_only.root");
    TTree *t2 = (TTree*)f2->Get("coin");

    // -------------------------------
    // 1) 첫 번째: 2D hist + COLZ
    // -------------------------------
    TH2F *h1 = new TH2F("h1", "all event",
                        500, 0, 50000,   // 필요에 맞게 범위 지정
                        100, 0, 10);

    double bitsum1, qsum1[120], iq1[120][4];
    int nent1 = t1->GetEntries();
    cout<<"1st plot entries "<<nent1<<endl;
    //    for (int i=0; i<nent1; i++) {
    for (int i=0; i<100000; i++) {      
      if(i%50000==0){cout<<i<<"/"<<nent1<<endl;}
      t1->GetEntry(i);
      bitsum1 = t1->GetLeaf("bitsum")->GetValue();
      for(int j=0; j<bitsum1; j++){
	qsum1[j] = t1 ->GetLeaf("qsum")->GetValue(j);
	for(int k=0; k<4; k++){
	  iq1[j][k] = t1->GetLeaf("iq")->GetValue(j*4+k);
	}
	h1->Fill(qsum1[j], (iq1[j][0]+iq1[j][1])/(iq1[j][2]+iq1[j][3])) ;
      }

    }

    TCanvas *c = new TCanvas("c", "c", 1200, 800);
    h1->Draw("COLZ");

    // -------------------------------
    // 2) 두 번째: TGraph (빨간 속빈 동그라미)
    // -------------------------------
    vector<double> vx, vy;

    double bitsum2, qsum2[120], iq2[120][4];
    int nent2 = t2->GetEntries();
    cout<<"2nd plot entries "<<nent2<<endl;

    for (int i=0; i<nent2; i++) {
      
      t2->GetEntry(i);
      bitsum2 = t2->GetLeaf("pbitsum")->GetValue();
      for(int j=0; j<bitsum2; j++){
	qsum2[j] = t2 ->GetLeaf("pqsum")->GetValue(j);
	for(int k=0; k<4; k++){
	  iq2[j][k] = t2->GetLeaf("piq")->GetValue(j*4+k);
	}
	vx.push_back(qsum2[j]);
        vy.push_back((iq2[j][0]+iq2[j][1])/(iq2[j][2]+iq2[j][3]));
      }
    }
   

    TGraph *gr = new TGraph(vx.size(), &vx[0], &vy[0]);
    gr->SetMarkerStyle(24);   // 속 빈 원
    gr->SetMarkerColor(kRed);
    gr->SetMarkerSize(1.2);

    gr->Draw("P SAME");

TFile *fout = new TFile("./hist/psmd_iqcom.root", "RECREATE");
h1->Write();     // 2D 히스토그램 저장
gr->Write();     // 오버레이한 그래프 저장
c->Write();      // 그림이 그려진 캔버스 전체 저장
fout->Close();    

}
