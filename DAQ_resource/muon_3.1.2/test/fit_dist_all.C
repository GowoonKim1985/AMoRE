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

void fit_dist_all()
{
  const int npanel = 130;
  
  TFile f("./hist/muonHist_PSMD_all.root");
  TH1D *h[npanel];

  int d[2];
  int nmuon[npanel] = {0};
  int cut_qsum[npanel] = {0};
  double cut_qsum_ff[npanel] = {0};
  double tmuon = 0;
  double nmuonR[npanel] = {0};
  bool tag[npanel] = {false};
  int nnpanel = 0;
  
  TF1 *fitfunc[npanel], *bExp[npanel], *mLand[npanel], *dExp;
  for(int i = 0; i < npanel; i++){
    //h[i] = (TH1D*)f.Get(Form("hqsum_%d", i));
    h[i] = (TH1D*)f.Get(Form("hqsum_sel_%d", i));
    h[i] -> SetDirectory(0);
    h[i] -> Rebin(4);
    h[i] -> SetLineColor(kBlack);
    if(h[i] -> Integral() < 500){
      //cout << i << endl;
      tag[i] = true;
      nnpanel++;
      continue;
    }
    double range[2];
    int mbin = h[i] -> GetMaximumBin();
    int mbincont = h[i] -> GetMaximum();
    bool ttt = false;
    double mpv = 0;
    double mpva = 0;
    for(int j = mbin+1; j < mbin+100; j++){
      d[0] = h[i] -> GetBinContent(j-1);
      d[1] = h[i] -> GetBinContent(j);

      if(d[1] < mbincont*0.7 && ttt == false){
	range[0] = h[i] -> GetBinCenter(j);
	ttt = true;
      }
      
      if(d[1]-d[0] > 0){
	mpv = h[i] -> GetBinCenter(j);
	mpva = h[i] -> GetBinContent(j);
	break;
      }
    }
    range[1] = 200000;
    
    dExp = new TF1("dExp", "[0]*TMath::Exp([1]*x)", range[0], range[0]*2);
    dExp -> SetParameter(0, h[i] -> GetMaximum()*0.8);
    dExp -> SetParameter(0, -0.003);
    h[i] -> Fit(dExp, "R0Q");

    fitfunc[i] = new TF1(Form("fitfunc_%d", i), func,
			 range[0], range[1], 5);
    fitfunc[i] -> FixParameter(0, dExp -> GetParameter(0));
    fitfunc[i] -> FixParameter(1, dExp -> GetParameter(1));
    fitfunc[i] -> SetParameter(2, mpva);
    fitfunc[i] -> SetParLimits(2, mpva*0.8, mpva*10);
    fitfunc[i] -> SetParameter(3, mpv);
    fitfunc[i] -> SetParLimits(3, mpv*0.8, mpv*2);
    fitfunc[i] -> SetParameter(4, 1000);
    fitfunc[i] -> SetParLimits(4, 500, 10000);

    h[i] -> Fit(fitfunc[i], "R0Q");
    //h[i] -> Fit(fitfunc[i], "R0Q");

    bExp[i] = new TF1(Form("bExp_%d", i), "[0]*TMath::Exp([1]*x)",
		      range[0], range[1]);
    bExp[i] -> SetParameter(0, fitfunc[i] -> GetParameter(0));
    bExp[i] -> SetParameter(1, fitfunc[i] -> GetParameter(1));
    
    mLand[i] = new TF1(Form("mLand_%d", i), "[0]*TMath::Landau(x, [1], [2])",
		       0, 200000);
    mLand[i] -> SetParameter(0, fitfunc[i] -> GetParameter(2));
    mLand[i] -> SetParameter(1, fitfunc[i] -> GetParameter(3));
    mLand[i] -> SetParameter(2, fitfunc[i] -> GetParameter(4));

    cut_qsum_ff[i] = mLand[i] -> GetMaximumX();
    nmuon[i] = h[i] -> Integral(h[i] -> FindBin(cut_qsum_ff[i]), h[i] -> GetNbinsX());
    double rLand = mLand[i] -> Integral(cut_qsum_ff[i], 200000)/mLand[i] -> Integral(0, 200000);
    nmuonR[i] = nmuon[i]/rLand;
    cout << i << " " << nmuon[i] << " "
	 << rLand << " " << nmuonR[i] << endl;

    if((i > 48 && i < 59) ||
       (i > 113 && i < 124))
    tmuon += nmuonR[i];
    dExp -> Clear();
  }

  double area = 167.*30.*22.;
  cout << npanel-nnpanel << endl;
  //double area = 167.*30.*(double)(npanel-nnpanel);
  double rtime = 1016872.93954499;
  // cout << endl;
  cout << tmuon << " " << tmuon/area/rtime << endl;
  // TLine *l[npanel];
  // TLine *l_ff[npanel];
  // for(int i = 0; i < npanel; i++){
  //   l[i] = new TLine(cut_qsum[i], 0, cut_qsum[i], h[i] -> GetMaximum());
  //   l[i] -> SetLineColor(15);
  //   l[i] -> SetLineWidth(2);

  //   l_ff[i] = new TLine(cut_qsum_ff[i], 0,
  // 			cut_qsum_ff[i], h[i] -> GetMaximum());
  //   l_ff[i] -> SetLineColor(4);
  //   l_ff[i] -> SetLineWidth(2);
  // }
  
  TGaxis::SetMaxDigits(4);
  int npad[10] = {12, 12, 12, 12, 11,
                  12, 12, 12, 12, 11};
  int nn = 0;
  TCanvas *can[10];
  TPad *pad[npanel];
  for(int i = 0; i < 10; i++){
    can[i] = new TCanvas(Form("can_%d", i), Form("%d", i+1), 1600, 900);
    can[i] -> Divide(4, 3);

    if(i == 5) nn += 6;
    for(int j = 0; j < npad[i]; j++){
      can[i] -> cd(j+1);
      pad[nn] = (TPad*)can[i] -> cd(j+1);
      pad[nn] -> SetLogy();

      h[nn] -> Draw();
      if(tag[nn] == true){
	nn++;
	continue;
      }
      // l[nn] -> Draw();
      // l_ff[nn] -> Draw();
      fitfunc[nn] -> Draw("same");
      bExp[nn] -> SetLineColor(kGreen);
      bExp[nn] -> SetLineStyle(7);
      bExp[nn] -> Draw("same");
      mLand[nn] -> SetLineColor(kBlue);
      mLand[nn] -> SetLineStyle(7);
      mLand[nn] -> Draw("same");

      nn++;
    }
  }
}
