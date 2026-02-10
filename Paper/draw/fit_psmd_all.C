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

void fit_psmd_all()
{
  //  TFile f("/home/kkw/muon_3.1.2/test/hist/muonHist.root");
  //  TFile f("./hist/qhist_psmd_bot.root");
  //    TFile f("../hist/hist_psmd_all.root");
     TFile f("../hist/qhist_psmd_all.root");      

  int cut_qsum = 16000;
  /*
  int cut_qsum_fully[22] =
    {10000, 15000, 12000, 18000,
     14000, 11000,  9000, 10000,
     15000, 11000, 10000,
     10000, 15000, 15000, 14000,
      8000, 10000,  8000, 10000,
     15000, 14000,  5000};
  */
  int cut_qsum_fully = 11000;


  int cut_qsum_ff = 0;
  int mean = 20000;

  //  double range[2]  = {10000, 50000};
  double range[2]  = {10000, 50000};  

  TCanvas *can = new TCanvas("can", "one hist", 1300, 900);
  can -> SetLogy();
  can->cd();

  
  int nmuon = 0;
  int nmuon_fully = 0;
  double nmuonR = 0;
  double tmuon = 0;
  double tmuon_fly = 0;
  double mpv, chi2;
  
  TF1 *fitfunc, *bexp, *mLand;
  
    TH1D * h = (TH1D*)f.Get("hqsum_bot");
    h -> SetDirectory(0);
    //    h[i] -> Rebin(2);
    //      h->GetXaxis()->SetRangeUser(range[0], range[1]);
    //  h->GetXaxis()->SetRangeUser(5000, 50000);
  h->GetXaxis()->SetRangeUser(5000, 150000);  

  h->GetXaxis()->SetTitle("Charge [ADC counts]");
  h->GetYaxis()->SetTitle("Number of Events");

  h->GetXaxis()->SetTitleFont(62);
  h->GetYaxis()->SetTitleFont(62);


  h->GetXaxis()->SetTitleSize(0.04);
  h->GetYaxis()->SetTitleSize(0.04);



  h->Draw("hist");

  fitfunc = new TF1("fitfunc", func, range[0], range[1], 5);
    fitfunc -> SetParameter(0, 20);
    fitfunc -> SetParameter(1, -0.003);
    fitfunc -> SetParLimits(1, -0.007, -0.001);
    fitfunc -> SetParameter(2, 10);
    fitfunc -> SetParLimits(2, 0, 10000);
    fitfunc -> SetParameter(3, mean);
    fitfunc -> SetParLimits(3, mean*0.8, mean*1.2);
    fitfunc -> SetParameter(4, 500);
    fitfunc -> SetParLimits(4, 100, 4000);

    //    can->cd();
    //    h->Draw("hist");
    h -> Fit(fitfunc, "R");


    //to get error
    TVirtualFitter *fitter = TVirtualFitter::GetFitter();
    int npar = fitfunc->GetNpar();

    std::vector<double> par(npar);
    for (int i = 0; i < npar; i++)
      par[i] = fitfunc->GetParameter(i);


    double *cov = fitter->GetCovarianceMatrix();
   

////
    
    bexp = new TF1("bexp", "[0]*TMath::Exp([1]*x)",
		      range[0], range[1]);
    bexp -> SetParameter(0, fitfunc -> GetParameter(0));
    bexp -> SetParameter(1, fitfunc -> GetParameter(1));
    
    mLand = new TF1("mLand", "[0]*TMath::Landau(x, [1], [2])",
		       5000, 50000);
    mLand -> SetParameter(0, fitfunc -> GetParameter(2));
    mLand -> SetParameter(1, fitfunc -> GetParameter(3));
    mLand -> SetParameter(2, fitfunc -> GetParameter(4));

    //cout << mLand[i] -> GetMaximumX() << endl;
    cut_qsum_ff = mLand -> GetMaximumX();


    nmuon = h -> Integral(h -> FindBin(cut_qsum_ff), 50000);
    nmuon_fully = h -> Integral(h -> FindBin(cut_qsum_fully), 50000);
    double rLand = mLand -> Integral(cut_qsum_ff, 50000)/mLand -> Integral(0, 50000);
    nmuonR = nmuon/rLand;
    //    cout << i << " " << nmuon[i] << " " << rLand << " " << nmuonR[i] << " " << nmuon_fully[i] << endl;
    cout<<""<<endl;

    cout<<"mpv "<<fitfunc->GetParameter(3)<<endl;
    mpv=fitfunc->GetParameter(3);

    cout<<"chi2 "<<fitfunc->GetChisquare() / fitfunc->GetNDF()<<endl;
    chi2=fitfunc->GetChisquare() / fitfunc->GetNDF();
    //i : integral cut to 50000 : from landau, cut-50000 cnt/0-50000 cnt ratio : : total muon including before cut(assume landau) :  
    tmuon += nmuonR;
    tmuon_fly += nmuon_fully;

cout << "mLand integral (0-50000) = "
     << mLand->Integral(0, 50000) << endl;

cout << "mLand integral (" << cut_qsum_ff << "-50000) = "
     << mLand->Integral(18000, 50000) << endl;

//  double area = 1.67*0.3*22;//cm^2
  double area = 168*31*22;//cm^2  
  double rtime_327 =1016876.8;//sec
  double rtime_330 = 2507658.3;  
  //  double rtime = 58453.62+343964.74+523412.83;
  double rtime = rtime_327 + rtime_330;  


  double bw = 250.;
  double allsum = (mLand->Integral(0,50000))/bw; //0~50000 fit sum
  double divsum1 = (mLand->Integral(0,25000))/bw; //0~50000 fit sum
  double divsum2 =  h->Integral(h->FindBin(25000),h->FindBin(150000));
  //  double divsum3 =  h->Integral(h->FindBin(25000),h->FindBin(50000));
  double divsum_err = sqrt(divsum1+divsum2);
 
  cout<<"divsum1 "<<divsum1<<endl;
  cout<<"divsum2 "<<divsum2<<endl;
  cout<<"divsum_err "<<divsum_err<<endl;  
  cout<<"h sum 25000 - 50000 : " <<h->Integral(h->FindBin(25000),h->FindBin(50000))<<endl;
  cout<<"h sum 25000 - 150000 : " <<h->Integral(h->FindBin(25000),h->FindBin(150000))<<endl;  
  
  cout<<"mLand integral (all) / area / time(sec) " << allsum/(rtime)/(area)<<endl; 

  cout<<"mLand integral (div sum) / area / time(sec) " << (divsum1+divsum2)/(rtime)/(area)<<endl;
  cout<<"mLand integral (div sum) / area / time(sec) err " << divsum_err/(rtime)/(area)<<endl;
  
  cout<<"hist integral (20000-150000) / area / time(sec) " << (h->Integral(h->FindBin(20000),h->FindBin(150000)))/(rtime)/(area)<<endl;

  cout<<"mLand integral 2 / area / time(sec) " << (h->Integral(h->FindBin(18000),h->FindBin(50000)))/(rtime)/(area)<<endl;
  cout<<"mLand integral 2 / area / time(sec) " << (h->Integral(h->FindBin(18000),h->FindBin(50000)))/(rtime)/(area)<<endl;
 
cout << tmuon << " " << tmuon/area/(rtime/86400.) << " " << tmuon/area/(rtime/86400.)/328 << endl;
  cout << tmuon_fly/rtime << endl;
  TLine *l;
  TLine *l_ff;

    l = new TLine(cut_qsum, 0, cut_qsum, h -> GetMaximum());
    l -> SetLineColor(15);
    l -> SetLineWidth(2);

    l_ff = new TLine(cut_qsum_ff, 0,
			cut_qsum_ff, h -> GetMaximum());
    l_ff -> SetLineColor(4);
    l_ff -> SetLineWidth(2);

  
  TGaxis::SetMaxDigits(4);
/*
TCanvas *can_f1 = new TCanvas("can_f1", "Muon dist 1", 1400, 900);
  can_f1 -> Divide(4, 3);
  TPad *pad_f1[11];
  for(int i = 0; i < 11; i++){
    can_f1 -> cd(i+1);
    pad_f1[i] = (TPad*)can_f1 -> cd(i+1);

    h[i] -> Draw("hist");
*/
    // l[i] -> Draw();
    // l_ff[i] -> Draw();
    
    fitfunc -> Draw("same");

bexp -> SetLineColor(kGreen);
    bexp -> SetLineStyle(7);
    bexp -> Draw("same");
    mLand -> SetLineColor(kBlue);
    mLand -> SetLineStyle(7);
mLand->SetFillStyle(3003);    // 점 찍은 듯한 채움 (dot pattern)
mLand->SetFillColor(kBlue-9);   // 점 색상


    mLand -> Draw("same");


/*
  
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
*/
  
  // TCanvas *can = new TCanvas("can", "", 800, 600);
  // h[0] -> Draw();
  // fitfunc[0] -> Draw("same");
  // bexp[0] -> Draw("same");
  // mLand[0] -> Draw("same");
  // can -> SetLogy();
}
