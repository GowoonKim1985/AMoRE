double func(double *v, double *p)
{
  double x = v[0];

  double a = p[0];
  double b = p[1];
  double c = p[2];
  double d = p[3];
  double e = p[4];
  
  return a*TMath::Exp(b*x)+c*TMath::Landau(x, d, e);
}

void fit_psmd_muon()
{
  //  TFile f("/home/kkw/muon_3.1.2/test/hist/muonHist.root");
  //  TFile f("./hist/qhist_psmd_bot.root");
  TFile f("./hist/qhist_psmd_bot.root");    

  TH1D *h[22];

  int cut_qsum[22] =
    {13000, 18000, 17000, 22000,
     17000, 16000, 13000, 14000,
     19000, 16000, 16000,
     16000, 16000, 18000, 17000,
     10000, 13000, 13000, 14000,
     19000, 17000, 10500};
  /*
  int cut_qsum_fully[22] =
    {10000, 15000, 12000, 18000,
     14000, 11000,  9000, 10000,
     15000, 11000, 10000,
     10000, 15000, 15000, 14000,
      8000, 10000,  8000, 10000,
     15000, 14000,  5000};
  */
  int cut_qsum_fully[22] =
    {10000, 15000, 12000, 18000,
     14000, 11000,  9000, 10000,
     15000, 11000, 12000,
     10000, 15000, 15000, 14000,
      8000, 10000,  8000, 10000,
     15000, 14000,  5000};


  int cut_qsum_ff[22] = {0};

  /*  
  int mean[22] = {
    17000, 25000, 23000, 25000,
    25000, 20000, 17000, 17000,
    23000, 20000, 21000,
    17000, 20000, 22000, 20000,
    15000, 21000, 15000, 17000,
    21000, 20000, 15000
  };
  */
  int mean[22] = {
    20000, 25000, 23000, 25000,
    23000, 20000, 17000, 17000,
    20000, 17000, 28000,
    23000, 20000, 22000, 20000,
    15000, 15000, 15000, 17000,
    21000, 20000, 15000
  };

  /*
  double range[22][2]//fit range
    = {{10000, 50000}, {10000, 50000}, {10000, 50000}, {12000, 50000},
       {10000, 50000}, {10000, 50000}, {10000, 50000}, {10000, 50000},
       {10000, 50000}, {10000, 50000}, {10000, 50000},
       {10000, 50000}, {10000, 50000}, {10000, 50000}, {10000, 50000},
       { 8000, 50000}, {10000, 50000}, {10000, 50000}, {10000, 50000},
       {10000, 50000}, {10000, 50000}, { 8000, 50000}};
  */
  double range[22][2]//fit range
    = {{12000, 50000}, {12000, 50000}, {12000, 50000}, {12000, 50000},
       {10000, 50000}, {10000, 50000}, {10000, 50000}, {10000, 50000},
       {10000, 50000}, {8000, 50000}, {13000, 50000},
       {12000, 50000}, {12000, 50000}, {12000, 50000}, {10000, 50000},
       { 8000, 50000}, {8000, 50000}, {10000, 50000}, {10000, 50000},
       {10000, 50000}, {10000, 50000}, { 8000, 50000}};

  TCanvas *can = new TCanvas("can", "one hist", 800, 600);
  can -> SetLogy();

  
  
  int nmuon[22] = {0};
  int nmuon_fully[22] = {0};
  double nmuonR[22] = {0};
  double tmuon = 0;
  double tmuon_fly = 0;
  double mpv[22], chi2[22];
  
  TF1 *fitfunc[22], *bexp[22], *mLand[22];
  for(int i = 0; i < 22; i++){
    h[i] = (TH1D*)f.Get(Form("hqsum_%d", i));
    h[i] -> SetDirectory(0);
    //    h[i] -> Rebin(2);

    fitfunc[i] = new TF1(Form("fitfunc_%d", i), func,
			 range[i][0], range[i][1], 5);
    fitfunc[i] -> SetParameter(0, 20);
    fitfunc[i] -> SetParameter(1, -0.003);
    fitfunc[i] -> SetParLimits(1, -0.007, -0.001);
    fitfunc[i] -> SetParameter(2, 10);
    fitfunc[i] -> SetParLimits(2, 0, 300);
    fitfunc[i] -> SetParameter(3, mean[i]);
    fitfunc[i] -> SetParLimits(3, mean[i]*0.8, mean[i]*1.2);
    fitfunc[i] -> SetParameter(4, 500);
    fitfunc[i] -> SetParLimits(4, 100, 2000);

    if(i==0){
    fitfunc[i] -> SetParLimits(4, 100, 4000);
    }
    if(i == 1){
      fitfunc[i] -> SetParameter(1, -0.003);
      fitfunc[i] -> SetParLimits(1, -0.004, -0.0005);
      fitfunc[i] -> SetParLimits(4, 100, 4000);
    }
    else if(i == 2){
      fitfunc[i] -> SetParLimits(4, 100, 4000);
    }
    else if(i == 3){
      fitfunc[i] -> SetParameter(1, -0.001);
      fitfunc[i] -> SetParLimits(1, -0.004, -0.0001);
      fitfunc[i] -> SetParLimits(4, 100, 4000);
    }
    if(i==4){
    fitfunc[i] -> SetParLimits(4, 100, 4000);
    }

    else if(i > 4 && i != 16){
      fitfunc[i] -> SetParLimits(4, 100, 4000);
    }
    else if(i == 16){
      fitfunc[i] -> SetParLimits(4, 100, 2500);
    }
    if(i==8){
      h[i]->SetBinError(59, 0);
    }
    
    if(i==10){
          fitfunc[i] -> SetParameter(0, 20);
          fitfunc[i] -> SetParLimits(0, 0, 1e10);	  
      //  fitfunc[i] -> SetParLimits(1, -0.007, -0.0007);
	  //    fitfunc[i] -> SetParLimits(3, 25000, 30000);
    }
    if(i==0||i==1||i==2||i==11||i==12||i==13){
          fitfunc[i] -> SetParameter(0, 20);
          fitfunc[i] -> SetParLimits(0, 0, 1e10);	  
    }


    
    h[i] -> Fit(fitfunc[i], "R0Q");
    if(i == 16){
      can->cd();
      h[i] -> Fit(fitfunc[i], "R");
    }      
    else{
      h[i] -> Fit(fitfunc[i], "R0Q");
    }
    
    bexp[i] = new TF1(Form("bexp_%d", i), "[0]*TMath::Exp([1]*x)",
		      range[i][0], range[i][1]);
    bexp[i] -> SetParameter(0, fitfunc[i] -> GetParameter(0));
    bexp[i] -> SetParameter(1, fitfunc[i] -> GetParameter(1));
    
    mLand[i] = new TF1(Form("mLand_%d", i), "[0]*TMath::Landau(x, [1], [2])",
		       0, 50000);
    mLand[i] -> SetParameter(0, fitfunc[i] -> GetParameter(2));
    mLand[i] -> SetParameter(1, fitfunc[i] -> GetParameter(3));
    mLand[i] -> SetParameter(2, fitfunc[i] -> GetParameter(4));

    //cout << mLand[i] -> GetMaximumX() << endl;
    cut_qsum_ff[i] = mLand[i] -> GetMaximumX();


    nmuon[i] = h[i] -> Integral(h[i] -> FindBin(cut_qsum_ff[i]), 50000);
    nmuon_fully[i] = h[i] -> Integral(h[i] -> FindBin(cut_qsum_fully[i]), 50000);
    double rLand = mLand[i] -> Integral(cut_qsum_ff[i], 50000)/mLand[i] -> Integral(0, 50000);
    nmuonR[i] = nmuon[i]/rLand;
    //    cout << i << " " << nmuon[i] << " " << rLand << " " << nmuonR[i] << " " << nmuon_fully[i] << endl;
    cout<<""<<endl;
    cout<<i<<endl;
    cout<<"mpv "<<fitfunc[i]->GetParameter(3)<<endl;
    mpv[i]=fitfunc[i]->GetParameter(3);

    cout<<"chi2 "<<fitfunc[i]->GetChisquare() / fitfunc[i]->GetNDF()<<endl;
    chi2[i]=fitfunc[i]->GetChisquare() / fitfunc[i]->GetNDF();
    //i : integral cut to 50000 : from landau, cut-50000 cnt/0-50000 cnt ratio : : total muon including before cut(assume landau) :  
    tmuon += nmuonR[i];
    tmuon_fly += nmuon_fully[i];
  }


  double area = 1.67*0.3*22;//cm^2
  double rtime_327 =1016876.8;//sec
  double rtime_330 = 2507658.3;  
  //  double rtime = 58453.62+343964.74+523412.83;
  double rtime = rtime_327 + rtime_330;  

  cout << endl;
  cout << tmuon << " " << tmuon/area/(rtime/86400.) << " " << tmuon/area/(rtime/86400.)/328 << endl;
  cout << tmuon_fly/rtime << endl;
  TLine *l[22];
  TLine *l_ff[22];
  for(int i = 0; i < 22; i++){
    l[i] = new TLine(cut_qsum[i], 0, cut_qsum[i], h[i] -> GetMaximum());
    l[i] -> SetLineColor(15);
    l[i] -> SetLineWidth(2);

    l_ff[i] = new TLine(cut_qsum_ff[i], 0,
			cut_qsum_ff[i], h[i] -> GetMaximum());
    l_ff[i] -> SetLineColor(4);
    l_ff[i] -> SetLineWidth(2);
  }
  
  TGaxis::SetMaxDigits(4);
  TCanvas *can_f1 = new TCanvas("can_f1", "Muon dist 1", 1400, 900);
  can_f1 -> Divide(4, 3);
  TPad *pad_f1[11];
  for(int i = 0; i < 11; i++){
    can_f1 -> cd(i+1);
    pad_f1[i] = (TPad*)can_f1 -> cd(i+1);

    h[i] -> Draw("hist");

    // l[i] -> Draw();
    // l_ff[i] -> Draw();
    
    fitfunc[i] -> Draw("same");
    bexp[i] -> SetLineColor(kGreen);
    bexp[i] -> SetLineStyle(7);
    bexp[i] -> Draw("same");
    mLand[i] -> SetLineColor(kBlue);
    mLand[i] -> SetLineStyle(7);
    mLand[i] -> Draw("same");

    pad_f1[i] -> SetLogy();
  }
  
  TCanvas *can_f2 = new TCanvas("can_f2", "Muon dist 2", 1400, 900);
  can_f2 -> Divide(4, 3);
  TPad *pad_f2[11];
  for(int i = 11; i < 22; i++){
    can_f2 -> cd(i-10);
    pad_f2[i] = (TPad*)can_f2 -> cd(i-10);

    h[i] -> Draw("hist");

    // l[i] -> Draw();
    // l_ff[i] -> Draw();
    
    fitfunc[i] -> Draw("same");
    bexp[i] -> SetLineColor(kGreen);
    bexp[i] -> SetLineStyle(7);
    bexp[i] -> Draw("same");
    mLand[i] -> SetLineColor(kBlue);
    mLand[i] -> SetLineStyle(7);
    mLand[i] -> Draw("same");
    
    pad_f2[i] -> SetLogy();
  }

  cout<<"mpv"<<endl;
  for(int i=0; i<22;i++){
    printf("h%d %.3f,  \n", i, mpv[i]);
//   cout<<"h"<<i<<" "<<mpv[i]<<endl;
  }
  
  // TCanvas *can = new TCanvas("can", "", 800, 600);
  // h[0] -> Draw();
  // fitfunc[0] -> Draw("same");
  // bexp[0] -> Draw("same");
  // mLand[0] -> Draw("same");
  // can -> SetLogy();
}
