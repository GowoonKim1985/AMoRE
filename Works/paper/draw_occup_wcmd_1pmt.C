R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(/home/cupsoft/muon_3.1.2/shlib/Linux5.14-GCC_11_3/libMuonObjs.so)

void draw_occup_wcmd_1pmt()
{
  gStyle -> SetOptStat(0);

  //327 sub 282, 330 sub 696

  //trg rate per hour
  int div = 24;
  int subrun[2] = {282, 696};
  const int n1 = subrun[0]/div;
  const int n2 = subrun[1]/div;
    int pmtnum[5] = {25, 4, 22, 28, 46};

  TChain * pchain1 = new TChain("wcmd");
  TChain * pchain2 = new TChain("wcmd");
 
  pchain1 -> Add("/home/kkw/Works/paper/prod_2nd/wcmd_000327.root");
  pchain2 -> Add("/home/kkw/Works/paper/prod_2nd/wcmd_000330.root");

  double trgtime1, trgtime2, qsum1, qsum2, qmax1, qmax2;
  int tbit1, tbit2;
  int ch1[48], ch2[48];
  
  pchain1->SetBranchAddress("trgtime", &trgtime1);
  pchain1->SetBranchAddress("tbit", &tbit1);
  pchain1->SetBranchAddress("ch", ch1);
  pchain1->SetBranchAddress("qsum", &qsum1);
  pchain1->SetBranchAddress("qmax", &qmax1);

  pchain2->SetBranchAddress("trgtime", &trgtime2);
  pchain2->SetBranchAddress("tbit", &tbit2);
  pchain2->SetBranchAddress("ch", ch2);
  pchain2->SetBranchAddress("qsum", &qsum2);
  pchain2->SetBranchAddress("qmax", &qmax2);
  
  Long64_t tent1 = pchain1->GetEntries();
  Long64_t tent2 = pchain2->GetEntries();

  // ===== 시작/끝 시간 =====
  double itime1, ftime1, itime2, ftime2, timesec;
  double qcut = 4500;
  
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
      cnt1[i][j] = 0; ccnt1[i][j] = 0; cocc1[i][j] = 0;
    }
  }
  for(int i = 0; i < n2; i++){
    time_div2[i] = 0;
    for(int j=0; j<5; j++){
      cnt2[i][j] = 0; ccnt2[i][j] = 0; cocc2[i][j] = 0;
    }
  }
  int hr_cnt;
  
  //run 327
  hr_cnt=1;
  for(Long64_t i = 0; i < tent1; i++){

    pchain1->GetEntry(i);
    timesec=trgtime1/1.e9;
    /*
    bool found = false;   
    for(int j = 0; j < tbit1; j++){
      if(ch1[j] == pmtnum[0]){ found = true; break; }
    }    
    if(found == true) {
      cnt1[hr_cnt-1][0]++;
      if((0.8 * qsum1 - 2100 > qmax1) && qsum1 > qcut){ ccnt1[hr_cnt-1][0]++;}
    }
    else{}
    */

    for(int j = 0; j < 5; j++){
      bool found = false;   
      for(int k = 0; k < tbit1; k++){
        if(ch1[k] == pmtnum[j]){ found = true; break; }
      }    
      if(found == true) {
        cnt1[hr_cnt-1][j]++;
        if((0.8 * qsum1 - 2100 > qmax1) && qsum1 > qcut){ ccnt1[hr_cnt-1][j]++; }
      }
    }
    
    if(timesec>(hr_cnt*3600*div)){

      time_div1[hr_cnt-1]=timesec/((double)div*3600); //1 day
      for(int j=0; j<5; j++){
	iocc1[hr_cnt-1][j]=cnt1[hr_cnt-1][j];
	cocc1[hr_cnt-1][j]=ccnt1[hr_cnt-1][j];
      }
      cout<<time_div1[hr_cnt-1]<< " hr : pmt no."<<pmtnum[0]<<" / occ : "<<iocc1[hr_cnt-1][0]<<"(w/ cut "<<cocc1[hr_cnt-1][0]<<")"<<endl;

      hr_cnt++;
      if(hr_cnt > n1) break;
    }
  }


  //run330
  hr_cnt=1;
  for(Long64_t i = 0; i < tent2; i++){

    pchain2->GetEntry(i);
    timesec=trgtime2/1.e9;
    /*
    bool found = false;   
    for(int j = 0; j < tbit2; j++){
      if(ch2[j] == pmtnum[0]){ found = true; break; }
    }    
    if(found == true) {
      cnt2[hr_cnt-1][0]++;
      if((0.8 * qsum2 - 2100 > qmax2) && qsum2 > qcut){ ccnt2[hr_cnt-1][0]++;}
    }
    else{}
*/

    for(int j = 0; j < 5; j++){
      bool found = false;   
      for(int k = 0; k < tbit2; k++){
        if(ch2[k] == pmtnum[j]){ found = true; break; }
      }    
      if(found == true) {
        cnt2[hr_cnt-1][j]++;
        if((0.8 * qsum2 - 2100 > qmax2) && qsum2 > qcut){ ccnt2[hr_cnt-1][j]++; }
      }
    }

    
    if(timesec>(hr_cnt*3600*div)){

      time_div2[hr_cnt-1]=timesec/((double)div*3600); //1 day

      for(int j=0; j<5; j++){
	iocc2[hr_cnt-1][j]=cnt2[hr_cnt-1][j];
	cocc2[hr_cnt-1][j]=ccnt2[hr_cnt-1][j];
      }
      cout<<time_div2[hr_cnt-1]<< " day : pmt no."<<pmtnum[0]<<" / occ : "<<iocc2[hr_cnt-1][0]<<"(w/ cut "<<cocc2[hr_cnt-1][0]<<")"<<endl;

      hr_cnt++;
      if(hr_cnt > n2) break;
    }
  }


  
    
    // ===== graph arrays =====
  //  const int n1 = 12;
  //  const int n2 = 30;
  const int ntot = n1 + n2 -1;

  double x[ntot], ex[ntot];
  /*
  double y_cr[ntot], ey_cr[ntot];

  for(int i = 0; i < ntot; i++){
    x[i] = 0;
    ex[i] = 0;
    y_cr[i] = 0;
    ey_cr[i] = 0;
  }

  */
  double y_cr[5][ntot], ey_cr[5][ntot];

  for(int j = 0; j < 5; j++){
    for(int i = 0; i < ntot; i++){
      y_cr[j][i]  = 0;
      ey_cr[j][i] = 0;
    }
  }
  for(int i = 0; i < ntot; i++){
    x[i] = 0;
    ex[i] = 0;
  }

  int npt = 0;
  
  double day_offset = 0.0;

  // run327
  for(int i=0; i<n1; i++){
    x[npt] = time_div1[i];
    ex[npt] = 0.0;

          double Ni, dt, cNi;
      dt   = time_div1[i];

    for(int j=0; j<5; j++){
      y_cr[j][npt] = cocc1[i][j];

    
      Ni   = cnt1[i][j];
      cNi = ccnt1[i][j];
      ey_cr[j][npt] = sqrt(cNi);
    }
    npt++;
  }

  day_offset = time_div1[n1-1];

  // run330
  for(int i=0; i<n2; i++){
  if(time_div2[i] <= 0) continue;
    //   if(fabs(time_div2[i] - 10.0) < 1e-6) continue;

  if(div==1&& i*div <= 621 && i*div >= 614)continue;
  if(div==24 && time_div2[i]>25.5 && time_div2[i]<26.5)continue; 
  x[npt] = day_offset + time_div2[i];
  ex[npt] = 0.0;

      double Ni, dt, cNi;
    
      dt   = time_div2[i];

      for(int j=0; j<5; j++){

    //   y_ir[npt] = iocc2[i][0];
	y_cr[j][npt] = cocc2[i][j];
	Ni   = cnt2[i][j];
	cNi   = ccnt2[i][j];
	//    ey_ir[k] = sqrt(Ni)/dt;
	//    ey_cr[k] = sqrt(Ntot)/ttot;
	//   ey_ir[npt] = sqrt(Ni);
	ey_cr[j][npt] = sqrt(cNi);
      }
      npt++;
  }
  /*
  // ===== draw =====
  TCanvas *c1 = new TCanvas("c1","rate",1300,900);
  c1->SetLeftMargin(0.15);
  TGraphErrors *g_cr = new TGraphErrors(npt, x, y_cr, ex, ey_cr);

  g_cr->SetTitle(";Runtime [H]; 1 PMT(no.25) response [counts/day]");

  
  g_cr->SetMarkerStyle(20);
  g_cr->SetMarkerSize(0.7);
  g_cr->SetMarkerColor(kRed);
  g_cr->SetLineColor(kRed);
  g_cr->SetLineWidth(2);
  
  gStyle->SetEndErrorSize(4);
  //  g_ir->Draw("APE");
  // g_cr->Draw("PE SAME");
  g_cr->Draw("APE");


  double ymax = 0;

for(int i=0; i<npt; i++){
  if(y_cr[i] > ymax) ymax = y_cr[i];
}

 cout<<"ymax" <<ymax<<endl;
g_cr->GetXaxis()->SetRangeUser(0, x[npt-1]);
g_cr->GetYaxis()->SetRangeUser(0.0, 1.0*ymax);
  //g_ir->GetYaxis()->SetRangeUser(0.0, 1.2*TMath::MaxElement(ntot, y_ir));
  TLegend *leg = new TLegend(0.65,0.75,0.88,0.88);
  // leg->AddEntry(g_ir,"All trigger events","lp");
  leg->AddEntry(g_cr,"Muon candidates","lp");
  leg->Draw();
  */


int colors[5] = {kRed, kBlue, kGreen+2, kOrange+1, kViolet+1};

  TCanvas *c1 = new TCanvas("c1","rate",1300,900);
  c1->SetLeftMargin(0.15);

  TGraphErrors *g_cr[5];
  for(int j = 0; j < 5; j++){
    g_cr[j] = new TGraphErrors(npt, x, y_cr[j], ex, ey_cr[j]);
    g_cr[j]->SetMarkerStyle(20);
    g_cr[j]->SetMarkerSize(0.7);
    g_cr[j]->SetMarkerColor(colors[j]);
    g_cr[j]->SetLineColor(colors[j]);
    g_cr[j]->SetLineWidth(2);
  }

  
  g_cr[0]->GetXaxis()->SetTitleFont(62);
  g_cr[0]->GetYaxis()->SetTitleFont(62);


  g_cr[0]->GetXaxis()->SetTitleSize(0.04);
  g_cr[0]->GetYaxis()->SetTitleSize(0.04);


  g_cr[0]->SetTitle(";Runtime [day]; PMT response [counts/day]");
  gStyle->SetEndErrorSize(4);
  g_cr[0]->Draw("APE");

  double ymax = 0;
  for(int j = 0; j < 5; j++)
    for(int i = 0; i < npt; i++)
      if(y_cr[j][i] > ymax) ymax = y_cr[j][i];

  cout<<"ymax: "<<ymax<<endl;
  g_cr[0]->GetXaxis()->SetRangeUser(0, x[npt-1]);
  //  g_cr[0]->GetYaxis()->SetRangeUser(0.0, 1.0*ymax);
  g_cr[0]->GetYaxis()->SetRangeUser(0.0, 4500.);

  for(int j = 1; j < 5; j++)
    g_cr[j]->Draw("PE SAME");

  //  TLegend *leg = new TLegend(0.65,0.65,0.88,0.88);
  TLegend *leg = new TLegend(0.65,0.15,0.88,0.38);
  for(int j = 0; j < 5; j++){
    leg->AddEntry(g_cr[j], Form("PMT no.%d", pmtnum[j]), "lp");
  }
  leg->Draw();
  
  c1->Update();
  

}
