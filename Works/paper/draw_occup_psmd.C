R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(/home/cupsoft/muon_3.1.2/shlib/Linux5.14-GCC_11_3/libMuonObjs.so)

void draw_occup_psmd()
{
  gStyle -> SetOptStat(0);

  //327 sub 282, 330 sub 696

  //trg rate per hour
  int div = 24;
  //  int subrun[2] = {282, 672};
  int subrun[2] = {282, 696};
  const int n1 = subrun[0]/div;
  const int n2 = subrun[1]/div;
  //  int pmtnum[5] = {25, 4, 22,28, 46};
  int pmtnum[5] = {52, 4, 22,28, 46};

  TChain * pchain1 = new TChain("psmd");
  TChain * pchain2 = new TChain("psmd");
 
  pchain1 -> Add("/home/kkw/Works/paper/prod_2nd/psmd_000327.root");
  pchain2 -> Add("/home/kkw/Works/paper/prod_2nd/psmd_000330.root");

  double trgtime1, trgtime2, qsum1[120], qsum2[120], qmax1[120], qmax2[120], iq1[120][4], iq2[120][4];
  int tbit1, tbit2;
  int ch1[120], ch2[120];
  
  pchain1->SetBranchAddress("trgtime", &trgtime1);
  pchain1->SetBranchAddress("bitsum", &tbit1);
  pchain1->SetBranchAddress("trgdet", ch1);
  pchain1->SetBranchAddress("qsum", &qsum1);
  pchain1->SetBranchAddress("iq", &iq1);
  pchain1->SetBranchAddress("qmax", &qmax1);

  pchain2->SetBranchAddress("trgtime", &trgtime2);
  pchain2->SetBranchAddress("bitsum", &tbit2);
  pchain2->SetBranchAddress("trgdet", ch2);
  pchain2->SetBranchAddress("qsum", &qsum2);
  pchain2->SetBranchAddress("iq", &iq2);
  pchain2->SetBranchAddress("qmax", &qmax2);
  
  Long64_t tent1 = pchain1->GetEntries();
  Long64_t tent2 = pchain2->GetEntries();

  // ===== 시작/끝 시간 =====
  double itime1, ftime1, itime2, ftime2, timesec;

  pchain1->GetEntry(0);
  itime1 = trgtime1 / 1e9;
  pchain1->GetEntry(tent1 - 1);
  ftime1 = trgtime1 / 1e9;

  pchain2->GetEntry(0);
  itime2 = trgtime2 / 1e9;
  pchain2->GetEntry(tent2 - 1);
  ftime2 = trgtime2 / 1e9;

  // ===== 카운트 배열 초기화 =====
  // cnt[bin]: 구간별 all-trigger 카운트 (누적 아님, 구간별 raw)
  // ccnt[bin]: 구간별 muon cut 카운트
  int cnt1[n1][5],  ccnt1[n1][5];
  int cnt2[n2][5],  ccnt2[n2][5];
  double time_div1[n1], time_div2[n2];
  double iocc1[n1][5], cocc1[n1][5], iocc2[n2][5], cocc2[n2][5];
  
  for(int i = 0; i < n1; i++){
    time_div1[i] = 0;
    for(int j=0; j< 5; j++){
      cnt1[i][j] = 0; ccnt1[i][j] = 0; 
    }
  }
  for(int i = 0; i < n2; i++){
    time_div2[i] = 0;
    for(int j=0; j<5; j++){
      cnt2[i][j] = 0; ccnt2[i][j] = 0; 
    }
  }
  int hr_cnt, ich;
  
  //run 327
  hr_cnt=1;
  for(Long64_t i = 0; i < tent1; i++){

    pchain1->GetEntry(i);
    timesec=trgtime1/1.e9;

    bool found = false;   
    for(int j = 0; j < tbit1; j++){
      if(ch1[j] == pmtnum[0]){ ich=j; found = true; break; }
    }    
    if(found == true) {
      cnt1[hr_cnt-1][0]++;
      //      if((0.8 * qsum1 - 2100 > qmax1) && qsum1 > 4500){ ccnt1[hr_cnt-1][0]++;}
      if(qsum1[ich]>12000&&iq1[ich][0]>0&&iq1[ich][1]>0&&iq1[ich][2]>0&&iq1[ich][3]>0){ ccnt1[hr_cnt-1][0]++;}
    }
    else{}
    
    if(timesec>(hr_cnt*3600*div)){

      time_div1[hr_cnt-1]=timesec/((double)div*3600); //1 day

      iocc1[hr_cnt-1][0]=cnt1[hr_cnt-1][0];
      cocc1[hr_cnt-1][0]=ccnt1[hr_cnt-1][0];

      cout<<time_div1[hr_cnt-1]<< " day : pmt no."<<pmtnum[0]<<" "<<cnt1[hr_cnt-1][0]<<" evts"<<endl;
      cout<<"occ : "<<iocc1[hr_cnt-1][0]<<"(w/ cut "<<cocc1[hr_cnt-1][0]<<")"<<endl;

      hr_cnt++;
      if(hr_cnt > n1) break;
    }
  }


  //run330
  hr_cnt=1;
  for(Long64_t i = 0; i < tent2; i++){

    pchain2->GetEntry(i);
    timesec=trgtime2/1.e9;

    bool found = false;   
    for(int j = 0; j < tbit2; j++){
      if(ch2[j] == pmtnum[0]){ ich=j; found = true; break; }
    }    
    if(found == true) {
      cnt2[hr_cnt-1][0]++;
      //      if((0.8 * qsum2 - 2100 > qmax2) && qsum2 > 4500){ ccnt2[hr_cnt-1][0]++;}
      if(qsum2[ich]>12000&&iq2[ich][0]>0&&iq2[ich][1]>0&&iq2[ich][2]>0&&iq2[ich][3]>0){ ccnt2[hr_cnt-1][0]++;}

    }
    else{}
    
    if(timesec>(hr_cnt*3600*div)){

      time_div2[hr_cnt-1]=timesec/((double)div*3600); //1 day

      iocc2[hr_cnt-1][0]=cnt2[hr_cnt-1][0];
      cocc2[hr_cnt-1][0]=ccnt2[hr_cnt-1][0];

      cout<<time_div2[hr_cnt-1]<< " day : pmt no."<<pmtnum[0]<<" "<<cnt2[hr_cnt-1][0]<<" evts"<<endl;
      cout<<"occ : "<<iocc2[hr_cnt-1][0]<<"(w/ cut "<<cocc2[hr_cnt-1][0]<<")"<<endl;

      hr_cnt++;
      if(hr_cnt > n2) break;
    }
  }


  
    
    // ===== graph arrays =====
  //  const int n1 = 12;
  //  const int n2 = 30;
  const int ntot = n1 + n2;

  double x[ntot], ex[ntot];
  double y_ir[ntot], ey_ir[ntot];
  double y_cr[ntot], ey_cr[ntot];

  double day_offset = 0.0;

  // run327
  for(int i=0; i<n1; i++){
    x[i] = time_div1[i];
    ex[i] = 0.0;

    y_ir[i] = iocc1[i][0];
    y_cr[i] = cocc1[i][0];

    double Ni, dt, cNi;

    Ni   = cnt1[i][0];
    dt   = time_div1[i];
    cNi = ccnt1[i][0];

    ey_ir[i] = sqrt(Ni);
    ey_cr[i] = sqrt(cNi);
  }

  day_offset = time_div1[n1-1];

  // run330
  for(int i=0; i<n2; i++){
    int k = n1 + i;

    x[k] = day_offset + time_div2[i];
    ex[k] = 0.0;

    y_ir[k] = iocc2[i][0];
    y_cr[k] = cocc2[i][0];

    double Ni, dt, cNi;

      Ni   = cnt2[i][0];
      cNi   = ccnt2[i][0];
      dt   = time_div2[i];
    //    ey_ir[k] = sqrt(Ni)/dt;
    //    ey_cr[k] = sqrt(Ntot)/ttot;
    ey_ir[k] = sqrt(Ni);
    ey_cr[k] = sqrt(cNi);
  }

  // ===== draw =====
  TCanvas *c1 = new TCanvas("c1","rate",900,600);

  TGraphErrors *g_ir = new TGraphErrors(ntot, x, y_ir, ex, ey_ir);
  TGraphErrors *g_cr = new TGraphErrors(ntot, x, y_cr, ex, ey_cr);

  g_ir->SetTitle(";Day;PMT response(counts/day)");
  g_ir->SetMarkerStyle(20);
  g_ir->SetMarkerSize(0.7);
  g_ir->SetMarkerColor(kBlue);
  g_ir->SetLineColor(kBlue);
  g_ir->SetLineWidth(2);
  
  g_cr->SetMarkerStyle(20);
  g_cr->SetMarkerSize(0.7);
  g_cr->SetMarkerColor(kRed);
  g_cr->SetLineColor(kRed);
  g_cr->SetLineWidth(2);
  
  gStyle->SetEndErrorSize(4);
  g_ir->Draw("APE");
  g_cr->Draw("PE SAME");

  g_ir->GetYaxis()->SetRangeUser(0.0, 1.2*TMath::MaxElement(ntot, y_ir) > 1.2*TMath::MaxElement(ntot, y_cr) ? 1.2*TMath::MaxElement(ntot, y_ir) : 1.2*TMath::MaxElement(ntot, y_cr));
  //g_ir->GetYaxis()->SetRangeUser(0.0, 1.2*TMath::MaxElement(ntot, y_ir));
  TLegend *leg = new TLegend(0.65,0.75,0.88,0.88);
  leg->AddEntry(g_ir,"all trigger events","lp");
  leg->AddEntry(g_cr,"Muon","lp");
  leg->Draw();

  c1->Update();
  

}
