void qfit_wcd(int fhv = -1)
{
  int low, upp;
  if(fhv >= 0 && fhv < 9){
    low = fhv;
    upp = fhv+1;
  }
  else{
    low = 0;
    upp = 9;
  }
  
  int mb, mb_sp;
  double max, maxx, sig, max_sp, maxx_sp;
  int stamp[9][47] = {0};
  double mean_sp[9][47] = {0};
  double err_sp[9][47] = {0};
  int np_sp[47] = {0};
  
  TGraphErrors *grp[47];
  for(int i = 0; i < 47; i++)
    grp[i] = new TGraphErrors();
  TH1D *his[9][47];
  TF1 *ffunc[9][47];
  TFile *f;
  for(int i = low; i < upp; i++){
    f = new TFile(Form("./hist/QmaxDist_%dV.root", 1200+i*100));
    
    for(int j = 0; j < 47; j++){
      his[i][j] = (TH1D*)f -> Get(Form("qmax_%d", j+1));
      if(i > 6)
	his[i][j] -> Rebin(5);
      his[i][j] -> SetDirectory(0);

      mb = his[i][j] -> GetMaximumBin();
      maxx = his[i][j] -> GetBinCenter(mb);
      max = his[i][j] -> GetBinContent(mb);
      int nbinsx = his[i][j] -> GetNbinsX();
      double lw = maxx-10 < 0 ? 0 : maxx-10;
      double up = maxx+10;
      TF1 *func = new TF1("func", "[0]*TMath::Gaus(x, [1], [2])", lw, up);
      func -> SetParameter(0, max);
      func -> SetParameter(1, maxx);
      func -> SetParLimits(1, 0, 1000);
      func -> SetParameter(2, 10);
      his[i][j] -> Fit(func, "R0Q");

      maxx = func -> GetParameter(1);
      sig = func -> GetParameter(2);
      sig = TMath::Abs(sig);

      TF1 *fpol = new TF1("fpol", "pol7", maxx+sig, maxx+10*sig);
      his[i][j] -> Fit(fpol, "R0Q");
      double lw_sp = 0;
      for(double k = maxx+sig; k < maxx+10*sig; k = k+0.1){
	//cout << k << " " << fpol -> Derivative(k) << endl;
	if(fpol -> Derivative(k) > -0.1){
	  lw_sp = k;
	  break;
	}
      }
      //cout << lw_sp << endl;
      
      mb_sp = 0;
      maxx_sp = 0;
      max_sp = 0;
      int lw_spbin = his[i][j] -> FindBin(lw_sp);
      for(int k = lw_spbin; k < nbinsx; k++){
	if(his[i][j] -> GetBinContent(k+1) > max_sp){
	  max_sp = his[i][j] -> GetBinContent(k+1);
	  maxx_sp = his[i][j] -> GetBinCenter(k);
	}
      }
      //cout << maxx_sp << endl;

      if(i > 7) lw_sp *= 2;
      
      if(lw_sp == 0 || maxx_sp < lw_sp || max_sp < max*0.01){
	//cout << "SP peak was not idenfied for " << j+1 << " with HV " << 1200+i*100 << endl;
	continue;
      }
      else{
	//cout << "SP peak is positioned at " << maxx_sp << " for " << j+1 << endl;
	np_sp[j]++;
      }
      stamp[i][j] = 1;
      ffunc[i][j] = new TF1(Form("ffunc_%d_%d", i, j),
			    "[0]*TMath::Gaus(x, [1], [2])+[3]*TMath::Gaus(x, [4], [5])", 0, maxx_sp*2);
      ffunc[i][j] -> SetParameter(0, max);
      ffunc[i][j] -> SetParameter(1, maxx);
      ffunc[i][j] -> SetParLimits(1, 0, 1000);
      ffunc[i][j] -> SetParameter(2, 10);
      ffunc[i][j] -> SetParameter(3, max_sp);
      ffunc[i][j] -> SetParameter(4, maxx_sp);
      ffunc[i][j] -> SetParLimits(4, 0, 1000);
      ffunc[i][j] -> SetParameter(5, 10);
      his[i][j] -> Fit(ffunc[i][j], "R0Q");

      double charge = (2500e-3/4096.)*(2e-9/50.)*ffunc[i][j] -> GetParameter(4);
      charge /= 1.6e-19;
      //(DynamicRange [mV]/Resolution)*(BinSize [ns]/Resistance [ohm])*ADC
      double chargeE = (2500e-3/4096.)*(2e-9/50.)*ffunc[i][j] -> GetParError(4);
      chargeE /= 1.6e-19;
      
      grp[j] -> SetPoint(np_sp[j]-1, 1200+i*100, charge);
      grp[j] -> SetPointError(np_sp[j]-1, 0, chargeE);
    }
  }
  // cout << "37: " << 4.81838e+01*(2500e-3/4096.)*(2e-9/50.)/1.6e-19 << endl;
  // cout << "38: " << 4.49652e+01*(2500e-3/4096.)*(2e-9/50.)/1.6e-19 << endl;
  TF1 *func_exp[47];
  if(fhv >= 0 && fhv < 9){
    TCanvas *can = new TCanvas("can", "", 1500, 1000);
    can -> Divide(7, 7);
    TPad *pad[47];
    for(int i = 0; i < 47; i++){
      can -> cd(i+1);
      pad[i] = (TPad*)can -> cd(i+1);
      pad[i] -> SetLogy();
      his[fhv][i] -> GetXaxis() -> SetRangeUser(0, 800);
      his[fhv][i] -> Draw();
      if(stamp[fhv][i] != 1)
	continue;
      ffunc[fhv][i] -> Draw("same");
    }
  }
  else{
    TCanvas *can_grp = new TCanvas("can_grp", "", 1500, 1000);
    can_grp -> Divide(7, 7);
    for(int i = 0; i < 47; i++){
      func_exp[i] = new TF1(Form("func_exp_%d", i), "expo(x)", 0, 2100);
      grp[i] -> Fit(func_exp[i], "R0Q");
      grp[i] -> Fit(func_exp[i], "R0Q");

      can_grp -> cd(i+1);
      grp[i] -> Draw("APL");
      func_exp[i] -> Draw("same");
      grp[i] -> GetXaxis() -> SetLabelSize(0.07);
    }
  }

  // TCanvas *can_grp_one = new TCanvas("can_grp_one", "", 800, 600);
  // grp[36] -> Draw("AP");
  // func_exp[36] -> Draw("same");
  // grp[36] -> GetXaxis() -> SetTitle("Voltage [V]");
  // grp[36] -> GetYaxis() -> SetTitle("Gain");
  
  // double gain[47] = {0};
  // for(int i = 0; i < 47; i++){
  //   for(int j = 1200; j < 2000; j++){
  //     if(func_exp[i] -> Eval(j) > 6e6){
  // 	gain[i] = (double)j;
  // 	break;
  //     }
  //   }
  //   cout << Form("6e6 gain HV of PMT %2d: %.0f V", i+1, gain[i]) << endl;

  //   cout << ((gain[i]*1.6e-19)/(2500e-3/4096.)/(2e-9/50.)) << endl;
  //   //double charge = (2500e-3/4096.)*(2e-9/50.)*ffunc[i][j] -> GetParameter(4);
  //   //charge /= 1.6e-19;

  // }
}
