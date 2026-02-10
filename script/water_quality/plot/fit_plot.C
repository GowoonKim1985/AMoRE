void fit_plot()
{
  TFile f("./plot/run272_ratio_24hr.root");

  TH1D *h;
  TF1 *fitfunc = new TF1("fitfunc","[0]*exp([1]*x)/(x*x)+[2]",1,7);
  h=(TH1D*)f.Get("ratio0");

  
  
  TF1 *fitfunc[22], *bexp[22], *mLand[22];
  for(int i = 0; i < 22; i++){
    h[i] = (TH1D*)f.Get(Form("hqsum_%d", i));
    h[i] -> SetDirectory(0);
    h[i] -> Rebin(2);

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
    else if(i > 4 && i != 16){
      fitfunc[i] -> SetParLimits(4, 100, 4000);
    }
    else if(i == 16){
      fitfunc[i] -> SetParLimits(4, 100, 2500);
    }
    
    h[i] -> Fit(fitfunc[i], "R0Q");
    if(i == 1)
      h[i] -> Fit(fitfunc[i], "R0Q");
    else
      h[i] -> Fit(fitfunc[i], "R0Q");      
    
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
  }

  // TCanvas *can = new TCanvas("can", "", 800, 600);
  // h[0] -> Draw();
  // fitfunc[0] -> Draw("same");
  // bexp[0] -> Draw("same");
  // mLand[0] -> Draw("same");
  // can -> SetLogy();
}
