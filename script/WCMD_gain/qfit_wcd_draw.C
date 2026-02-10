#include <TCanvas.h>  

void qfit_wcd_draw()
{
  int runnum[18] ={
    359, 362, 367, 374, 378,
    380, 393, 400, 402, 408,
    412, 414, 417, 425, 427,
    434, 437, 444};

int run=runnum[17];

  //  run=437;
  //  run =434;
  int mb, mb_sp;
  double max, maxx, minx, min, sig, max_sp, maxx_sp;
  int stamp = 0;
  double mean_sp = 0;
  double err_sp = 0;
  int np_sp = 0;

  double p[6]={0};
  double id, chi2, q, q_err;
  /*  
  TTree * tree = new TTree("tree","tree");

  tree->Branch("id", &id, "id/D");
  tree->Branch("p0", &p[0], "p0/D");
  tree->Branch("p1", &p[1], "p1/D");
  tree->Branch("p2", &p[2], "p2/D");
  tree->Branch("p3", &p[3], "p3/D");
  tree->Branch("p4", &p[4], "p4/D");
  tree->Branch("p5", &p[5], "p5/D");
  tree->Branch("chi2", &chi2, "chi2/D");
  tree->Branch("q", &q, "q/D");
    tree->Branch("q_err", &q_err, "q_err/D");
  */
  
  TCanvas *tcan = new TCanvas("tcan", "tcan", 1100, 800);
  //  tcan->Divide(7,7);
  tcan->SetLogy();


 char a;
    TH1D *his;
  TF1 *ffunc;
  TFile *f;

  f = new TFile(Form("./hist/QmaxDist_%06d.root", run));    
    
  //  for(int j = 5; j < 47; j++){

  int j=4;
  id=j+1;
    cout<<"pmt "<<j+1<<endl;
    his = (TH1D*)f -> Get(Form("qmax_%d", j+1));
    his -> SetDirectory(0);
    his->SetStats(0);

    mb = his -> GetMaximumBin();
    maxx = his -> GetBinCenter(mb);
    max = his -> GetBinContent(mb);

    cout<<"max bin "<<mb<<"/ value "<<maxx<<"/ height"<< max<<endl;

    int nbinsx = his -> GetNbinsX();
      double lw = maxx-10 < 0 ? 0 : maxx-10;
      double up = maxx+10;
     
      TF1 *func = new TF1("func", "[0]*TMath::Gaus(x, [1], [2])", lw, up);
      func -> SetParameter(0, max);
      func -> SetParameter(1, maxx);
      func -> SetParLimits(1, 0, 1000);
      func -> SetParameter(2, 10);
      //      his[j] -> Fit(func, "R");
      his -> Fit(func, "R0Q");      

      maxx = func -> GetParameter(1);
      sig = func -> GetParameter(2);
      sig = TMath::Abs(sig);

      TF1 *fpol = new TF1("fpol", "pol7", maxx+sig, maxx+10*sig);
      his -> Fit(fpol, "R0Q");
      double lw_sp = 0;
      for(double k = maxx+sig; k < maxx+10*sig; k = k+0.1){
	//cout << k << " " << fpol -> Derivative(k) << endl;
	if(fpol -> Derivative(k) > -0.1){
	  lw_sp = k;
	  break;
	}}
      cout << "lw_sp "<<lw_sp << endl;

	    
      mb_sp = 0;
      maxx_sp = 0;
      max_sp = 0;
      int lw_spbin = his -> FindBin(lw_sp);
      for(int k = lw_spbin; k < nbinsx; k++){
	if(his -> GetBinContent(k+1) > max_sp){
	  max_sp = his -> GetBinContent(k+1);
	  maxx_sp = his -> GetBinCenter(k);
	}
      }

      cout<<"lw_sp "<<lw_sp<<endl;
      cout <<"maxx_sp" << maxx_sp << endl;
      cout<<"max_sp "<<max_sp<<endl;
      cout<<"max*0.01 "<<max*0.01<<endl;
      //      if(i > 7) lw_sp *= 2;// hv > 1200+700, 1900
      /*
      if(lw_sp == 0 || maxx_sp < lw_sp || max_sp < max*0.01){
	cout << "SP peak was not idenfied for " << j+1 <<endl;
	continue;
      }
      else{
	cout << "SP peak is positioned at " << maxx_sp << " for " << j+1 << endl;
	np_sp[j]++;
      }
      */
      stamp = 1;
      ffunc = new TF1(Form("ffunc_%d", j),
			    "[0]*TMath::Gaus(x, [1], [2])+[3]*TMath::Gaus(x, [4], [5])", 0, maxx_sp*2);

      cout<<"fitting range 0~"<<maxx_sp*2<<endl;
      ffunc -> SetParameter(0, max);
      ffunc -> SetParameter(1, maxx);
      ffunc -> SetParLimits(1, 0, 1000);
      ffunc -> SetParameter(2, 10);
      ffunc -> SetParameter(3, max_sp);
      ffunc -> SetParameter(4, maxx_sp);
      ffunc -> SetParLimits(4, 0, 1000);
	    //      ffunc[j] -> SetParLimits(4, lw_sp, 100);      
	    //      ffunc[j] -> SetParameter(5, 10);
      ffunc -> SetParameter(5, 10);      
      tcan->cd();
      
      tcan->cd(j+1);
      gPad->SetLogy();
      his -> Fit(ffunc, "R");

      for(int ipa=0; ipa<6; ipa++){
	p[ipa]=ffunc->GetParameter(ipa);
      }
      
      his->GetXaxis()->SetRangeUser(0,200);

  his->GetYaxis()->SetTitle("Events");
  his->GetXaxis()->SetTitle("Charge [ADC counts]");

  his->GetXaxis()->SetTitleFont(62);
  his->GetYaxis()->SetTitleFont(62);


  his->GetXaxis()->SetTitleSize(0.04);
  his->GetYaxis()->SetTitleSize(0.04);



      
      //      can->cd();
      his->Draw();
      tcan->Update();
      


      
      TF1 *g2 = new TF1("g2", "[0]*TMath::Gaus(x,[1],[2])", 10, 100);
      g2->SetParameter(0,p[3]);
      g2->SetParameter(1,p[4]);
      g2->SetParameter(2,p[5]);      


g2->SetLineColor(kRed);
 g2->SetLineStyle(2); // dashed optional
 // g2->Draw("SAME");
 tcan->Update();
      
 cout<<""<<endl;
      TFitResultPtr fitResult = his->Fit(ffunc, "S"); // "S" 옵션: 결과 저장
std::cout << "Chi2: " << fitResult->Chi2() << std::endl;
std::cout << "NDF: " << fitResult->Ndf() << std::endl;
std::cout << "Reduced Chi2: " << fitResult->Chi2() / fitResult->Ndf() << std::endl;
 cout<<""<<endl;


 chi2=(fitResult->Chi2()) / (fitResult->Ndf());
      double charge = (2500e-3/4096.)*(2e-9/50.)*ffunc -> GetParameter(4);
      charge /= 1.6e-19;
      cout<<"charge " <<charge<<endl;

      //(DynamicRange [mV]/Resolution)*(BinSize [ns]/Resistance [ohm])*ADC
      double chargeE = (2500e-3/4096.)*(2e-9/50.)*ffunc -> GetParError(4);
      chargeE /= 1.6e-19;
      cout<<"chargeE " <<chargeE<<endl;      

      q=charge; q_err=chargeE;
      cout<<"run " <<run<<" / pmt "<<j+1<<endl;
      
      //   cin>>a;
      //      tree->Fill();
}
    //  }
  

