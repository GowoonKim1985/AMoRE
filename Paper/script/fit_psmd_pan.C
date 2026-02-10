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

void fit_psmd_pan()
{
  //  TFile f("/home/kkw/muon_3.1.2/test/hist/muonHist.root");
  //  TFile f("./hist/qhist_psmd_bot.root");
  //  TFile f("./hist/hist_psmd_all.root");    

  //  TFile *f = new TFile("./hist/eff/his2_210810_eff_hv54.3_th250_10hr.root");  
  TFile *f = new TFile("./hist/eff/his2_210818_effset1_hv54.5_th350_10hr.root");  

  int cut_qsum = 5000;
  int cut_qsum_fully = 1000;

  int cut_qsum_ff = 0;
  int mean = 8000;

  double range[2]  = {1500, 11000};

  TCanvas *can = new TCanvas("can", "one hist", 1300, 900);
  //  can -> SetLogy();
  can->cd();
  gStyle->SetOptStat(0);    // 전체 stat 박스 비활성화
  gPad->SetLeftMargin(0.12);

  
  int nmuon = 0;
  int nmuon_fully = 0;
  double nmuonR = 0;
  double tmuon = 0;
  double tmuon_fly = 0;
  double mpv, chi2;
  
  TF1 *fitfunc, *bexp, *mLand;
  
    TH1D * h = (TH1D*)f->Get("pan_b");
    h -> SetDirectory(0);
       h -> Rebin(2);
    h->GetXaxis()->SetRangeUser(1000, 25000);
    //    h->GetYaxis()->SetRangeUser(0, 2000);
    h->GetXaxis()->SetTitle("Charge [ADC counts]");
    h->GetYaxis()->SetTitle("Number of Events");
    
    h->GetXaxis()->SetTitleFont(62);
    h->GetYaxis()->SetTitleFont(62);


    h->GetXaxis()->SetTitleSize(0.04);
    h->GetYaxis()->SetTitleSize(0.04);
    h->SetLineColor(kBlack);
    h->Draw("hist");
    

    TF1 * expfit = new TF1("expfit", "expo", 5000,7000);
    h->Fit(expfit,"R");
    //     expfit->Draw("same");  // 피팅 함수 수동으로 덧그림
    
        
    fitfunc = new TF1("fitfunc", func, range[0], range[1], 5);
    fitfunc -> SetParameter(0, 20000);
    fitfunc -> SetParLimits(0, 1000, 50000);
    fitfunc -> SetParameter(1, -0.0008);
    fitfunc -> SetParLimits(1, -0.001, -0.0001);
    
    fitfunc -> SetParameter(2, 100000);
    fitfunc -> SetParLimits(2, 0, 500000);
    fitfunc -> SetParLimits(2, 50000, 500000);    

    fitfunc -> SetParameter(3, mean);
    fitfunc -> SetParLimits(3, mean*0.8, mean*1.2);
    fitfunc -> SetParameter(4, 500);
    fitfunc -> SetParLimits(4, 100, 1500);

    //    can->cd();
    //    h->Draw("hist");
    h -> Fit(fitfunc, "R");
    
    bexp = new TF1("bexp", "[0]*TMath::Exp([1]*x)",
		      range[0], range[1]);
    bexp -> SetParameter(0, fitfunc -> GetParameter(0));
    bexp -> SetParameter(1, fitfunc -> GetParameter(1));
    
    mLand = new TF1("mLand", "[0]*TMath::Landau(x, [1], [2])",
		      range[0], 25000);

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



  double area = 1.67*0.3*22;//cm^2
  double rtime_327 =1016876.8;//sec
  double rtime_330 = 2507658.3;  
  //  double rtime = 58453.62+343964.74+523412.83;
  double rtime = rtime_327 + rtime_330;  

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

  
    //  TGaxis::SetMaxDigits(4);
    
    fitfunc -> Draw("same");

bexp -> SetLineColor(kGreen);
    bexp -> SetLineStyle(7);
    bexp -> Draw("same");
    mLand -> SetLineColor(kBlue);
    mLand -> SetLineStyle(7);

    //mLand->SetFillStyle(3003);    // 점 찍은 듯한 채움 (dot pattern)
    //mLand->SetFillColor(kBlue-9);   // 점 색상

    mLand -> Draw("same");


    TF1* f_diff = new TF1("f_diff", "[0]*TMath::Exp([1]*x) - [2]*TMath::Landau(x, [3], [4])", 2000, 20000);
f_diff->SetParameters(
  fitfunc->GetParameter(0),
  fitfunc->GetParameter(1),
  fitfunc->GetParameter(2),
  fitfunc->GetParameter(3),
  fitfunc->GetParameter(4)
);

double x_cross = f_diff->GetX(0, 2000, 20000);
cout << x_cross << endl;
      
}
