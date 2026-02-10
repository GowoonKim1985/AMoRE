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

void fit_wcmd_muon()
{
  TFile f("./hist/his_wcmd.root");

  int nmuon[22] = {0};
  int nmuon_fully[22] = {0};
  double nmuonR[22] = {0};
  double tmuon = 0;
  double tmuon_fly = 0;
  
  //  TF1 *fitfunc[22], *bexp[22], *mLand[22];

  TCanvas * can = new TCanvas("can", "wcmd muon", 1200, 1000);
  
  TH1D * whis = (TH1D*)f.Get("qsum");
  whis -> SetDirectory(0);
  can->cd();
  whis->Draw();

  int t_range[2]={10000,50000};
  //  int b_range[2]={10000,}
    fitfunc = new TF1("fitfunc", func, range[0], range[1], 5);

    fitfunc -> SetParameter(0, 20);
    fitfunc -> SetParameter(1, -0.003);
    //    fitfunc -> SetParLimits(1, -0.007, -0.001);
    fitfunc -> SetParameter(2, 10);
    //    fitfunc -> SetParLimits(2, 0, 300);
    fitfunc -> SetParameter(3, 30000);
    //    fitfunc -> SetParLimits(3, mean[i]*0.8, mean[i]*1.2);
    fitfunc -> SetParameter(4, 500);
    //    fitfunc -> SetParLimits(4, 100, 2000);
    
    h -> Fit(fitfunc, "R0Q");

    //background exp func (get par from fitfunc)
    TF1 * bexp = new TF1("bexp", "[0]*TMath::Exp([1]*x)", range[0], range[1]);
    bexp -> SetParameter(0, fitfunc -> GetParameter(0));
    bexp -> SetParameter(1, fitfunc -> GetParameter(1));

    //landau func (get par from fitfunc)    

    TF1 * mLand = new TF1("mLand", "[0]*TMath::Landau(x, [1], [2])", range[0], range[1]);
    mLand -> SetParameter(0, fitfunc -> GetParameter(2));
    mLand -> SetParameter(1, fitfunc -> GetParameter(3));
    mLand -> SetParameter(2, fitfunc -> GetParameter(4));

    fitfunc ->SetLineColor(kRed);
    fitfunc -> Draw("same");
    bexp -> SetLineColor(kGreen);
    bexp -> SetLineStyle(7);
    bexp -> Draw("same");
    mLand -> SetLineColor(kBlue);
    mLand -> SetLineStyle(7);
    mLand -> Draw("same");

    
    //cout << mLand[i] -> GetMaximumX() << endl;
    cut_qsum_ff[i] = mLand[i] -> GetMaximumX();


    nmuon[i] = h[i] -> Integral(h[i] -> FindBin(cut_qsum_ff[i]), 50000);
    nmuon_fully[i] = h[i] -> Integral(h[i] -> FindBin(cut_qsum_fully[i]), 50000);
    double rLand = mLand[i] -> Integral(cut_qsum_ff[i], 50000)/mLand[i] -> Integral(0, 50000);
    nmuonR[i] = nmuon[i]/rLand;
    cout << i << " " << nmuon[i] << " " << rLand << " " << nmuonR[i] << " " << nmuon_fully[i] << endl;
    tmuon += nmuonR[i];
    tmuon_fly += nmuon_fully[i];
  }

  double area = 1.67*0.3*22;
  double rtime = 58453.62+343964.74+523412.83;
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

    h[i] -> Draw();

    // l[i] -> Draw();
    // l_ff[i] -> Draw();
    

    pad_f1[i] -> SetLogy();
  }
  
  TCanvas *can_f2 = new TCanvas("can_f2", "Muon dist 2", 1400, 900);
  can_f2 -> Divide(4, 3);
  TPad *pad_f2[11];
  for(int i = 11; i < 22; i++){
    can_f2 -> cd(i-10);
    pad_f2[i] = (TPad*)can_f2 -> cd(i-10);

    h[i] -> Draw();

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
  

  // TCanvas *can = new TCanvas("can", "", 800, 600);
  // h[0] -> Draw();
  // fitfunc[0] -> Draw("same");
  // bexp[0] -> Draw("same");
  // mLand[0] -> Draw("same");
  // can -> SetLogy();
}
