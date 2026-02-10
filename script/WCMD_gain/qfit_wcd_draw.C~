#include <TCanvas.h>  

void qfit_wcd(int run = 434)
{
  int runnum[18] ={
    359, 362, 367, 374, 378,
    380, 393, 400, 402, 408,
    412, 414, 417, 425, 427,
    434, 437, 444};

 run=runnum[17];
  //  run=437;
  //  run =434;
  int mb, mb_sp;
  double max, maxx, minx, min, sig, max_sp, maxx_sp;
  int stamp[47] = {0};
  double mean_sp[47] = {0};
  double err_sp[47] = {0};
  int np_sp[47] = {0};

  double p[6]={0};
  double id, chi2, q, q_err;
  
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

  
  TCanvas *tcan = new TCanvas("tcan", "tcan", 1400, 1000);
  tcan->Divide(7,7);
  tcan->SetLogy();



 char a;
    TH1D *his[47];
  TF1 *ffunc[47];
  TFile *f;

  f = new TFile(Form("./hist/QmaxDist_%06d.root", run));    
    
  for(int j = 0; j < 47; j++){

    if(j<47){
      //    if(j==3){
      id=j+1;
    cout<<"pmt "<<j+1<<endl;
    his[j] = (TH1D*)f -> Get(Form("qmax_%d", j+1));
    his[j] -> SetDirectory(0);

    mb = his[j] -> GetMaximumBin();
    maxx = his[j] -> GetBinCenter(mb);
    max = his[j] -> GetBinContent(mb);

    cout<<"max bin "<<mb<<"/ value "<<maxx<<"/ height"<< max<<endl;

    int nbinsx = his[j] -> GetNbinsX();
      double lw = maxx-10 < 0 ? 0 : maxx-10;
      double up = maxx+10;


      
      TF1 *func = new TF1("func", "[0]*TMath::Gaus(x, [1], [2])", lw, up);
      func -> SetParameter(0, max);
      func -> SetParameter(1, maxx);
      func -> SetParLimits(1, 0, 1000);
      func -> SetParameter(2, 10);
      //      his[j] -> Fit(func, "R");
            his[j] -> Fit(func, "R0Q");      

      maxx = func -> GetParameter(1);
      sig = func -> GetParameter(2);
      sig = TMath::Abs(sig);

      TF1 *fpol = new TF1("fpol", "pol7", maxx+sig, maxx+10*sig);
      his[j] -> Fit(fpol, "R0Q");
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
      int lw_spbin = his[j] -> FindBin(lw_sp);
      for(int k = lw_spbin; k < nbinsx; k++){
	if(his[j] -> GetBinContent(k+1) > max_sp){
	  max_sp = his[j] -> GetBinContent(k+1);
	  maxx_sp = his[j] -> GetBinCenter(k);
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
      stamp[j] = 1;
      ffunc[j] = new TF1(Form("ffunc_%d", j),
			    "[0]*TMath::Gaus(x, [1], [2])+[3]*TMath::Gaus(x, [4], [5])", 0, maxx_sp*2);

      cout<<"fitting range 0~"<<maxx_sp*2<<endl;
      ffunc[j] -> SetParameter(0, max);
      ffunc[j] -> SetParameter(1, maxx);
      ffunc[j] -> SetParLimits(1, 0, 1000);
      ffunc[j] -> SetParameter(2, 10);
      ffunc[j] -> SetParameter(3, max_sp);
      ffunc[j] -> SetParameter(4, maxx_sp);
            ffunc[j] -> SetParLimits(4, 0, 1000);
	    //      ffunc[j] -> SetParLimits(4, lw_sp, 100);      
	    //      ffunc[j] -> SetParameter(5, 10);
      ffunc[j] -> SetParameter(5, 10);      



      
	    if(j==1){ffunc[j] -> SetParLimits(4, 10, 100);};//pmt2
	    if(j==2){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt2	    
    	    if(j==2){ffunc[j] -> SetParameter(5, 2);};//pmt4
	    if(j==3){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt4   
	    if(j==5){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt6
	    if(j==6){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt7
	    if(j==6){ffunc[j] -> SetParameter(5, 2);};//pmt22
	    if(j==7){ffunc[j] -> SetParLimits(4, 10, 100);};//pmt8
	    if(j==9){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt22	    
	    if(j==9){ffunc[j] -> SetParameter(5, 2);};//pmt22


	    //	    if(j==9){ffunc[j] -> SetParameter(5, 2);};//pmt22
	    
	    
	    if(j==13){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt15
	    if(j==13){ffunc[j] -> SetParameter(5, 2);};//pmt22	    
	    if(j==14){ffunc[j] -> SetParLimits(4, 25, 80);};//pmt15



	    if(j==16){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt15	    
	    if(j==18){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt15	    
	    if(j==18){ffunc[j] -> SetParameter(5, 2);};//pmt22	    
	    
	    if(j==20){ffunc[j] -> SetParLimits(4, 35, 100);};//pmt21 
	    if(j==20){ffunc[j] -> SetParameter(5, 2);};//pmt21
	    if(j==21){ffunc[j] -> SetParLimits(4, 30, 100);};//pmt22
	    if(j==23){ffunc[j] -> SetParLimits(4, 20, 80);};//pmt24
	    if(j==25){ffunc[j] -> SetParLimits(4, 20, 70);};//pmt26
	    if(j==27){ffunc[j] -> SetParLimits(4, 20, 50);};//pmt26	    

	    if(j==30){ffunc[j] -> SetParLimits(4, 35, 70);};//pmt31
	    if(j==32){ffunc[j] -> SetParameter(5, 15);};//pmt31

	    if(j==34){ffunc[j] -> SetParLimits(4, 35, 70);};//pmt35
	    if(j==34){ffunc[j] -> SetParameter(5, 2);};//pmt35
	    if(j==35){ffunc[j] -> SetParLimits(4, 40, 80);};//pmt36
	    if(j==38){ffunc[j] -> SetParLimits(4, 35, 80);};//pmt39	   
	    if(j==38){ffunc[j] -> SetParameter(5, 2);};//pmt39	   
	    if(j==39){ffunc[j] -> SetParLimits(4, 35, 80);};//pmt40
	    if(j==39){ffunc[j] -> SetParameter(5, 2);};//pmt22


	    if(j==41){ffunc[j] -> SetParLimits(4, 35, 80);};//pmt40
	    if(j==41){ffunc[j] -> SetParameter(5, 2);};//pmt22
	    if(j==44){ffunc[j] -> SetParLimits(4, 25, 60);};//pmt40
	    if(j==44){ffunc[j] -> SetParameter(5, 2);};//pmt22

   

	    if(j==46){ffunc[j] -> SetParLimits(4, 25, 55);};//pmt40
	    if(j==46){ffunc[j] -> SetParameter(5, 2);};//pmt22	      	    	   
	    
	    if(run==427){
	      if(j==21){ffunc[j] -> SetParameter(5, 2);};//pmt22
	      if(j==32){ffunc[j] -> SetParameter(5, 2);};//pmt22	      
	    }
	    if(run==417){
	      if(j==0){ffunc[j] -> SetParameter(5, 2);};//pmt22
	    }
	    if(run==414){
	      if(j==0){ffunc[j] -> SetParameter(5, 2);};//pmt22
	      if(j==33){ffunc[j] -> SetParameter(5, 2);};//pmt22
	    }
	    if(run==402){
	      if(j==3){ffunc[j] -> SetParameter(5, 2);};//pmt22
	    }

	    if(run==400){
	      if(j==33){ffunc[j] -> SetParLimits(4, 35, 80);};//pmt33
	      if(j==33){ffunc[j] -> SetParameter(5, 2);};//pmt35
	      if(j==39){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt40
	      if(j==45){ffunc[j] -> SetParameter(5, 2);};//pmt22
	    }

	    if(run==393){
	      if(j==5){ffunc[j] -> SetParLimits(4, 25, 60);};//pmt6
	      if(j==6){ffunc[j] -> SetParLimits(5, 1, 20);};//pmt33	      
	      if(j==10){ffunc[j] -> SetParLimits(4, 20, 60);};//pmt22
	      if(j==12){ffunc[j] -> SetParLimits(4, 10, 60);};//pmt22
	      if(j==15){ffunc[j] -> SetParLimits(4, 25, 60);};//pmt15      
	      if(j==25){ffunc[j] -> SetParLimits(4, 30, 70);};//pmt26	    
	      if(j==32){ffunc[j] -> SetParameter(5, 20);};//pmt22
	    }

      	    if(run==380){
	      if(j==1){ffunc[j] -> SetParLimits(4, 10, 50);};//pmt26
	      if(j==4){ffunc[j] -> SetParameter(5, 20);};//pmt22
	      if(j==8){ffunc[j] -> SetParameter(5, 20);};//pmt22
	      if(j==15){ffunc[j] -> SetParLimits(4, 18, 60);};//pmt26
	      if(j==15){ffunc[j] -> SetParameter(5, 5);};//pmt22
	      if(j==27){ffunc[j] -> SetParLimits(4, 15, 50);};//pmt26
	    }

	    if(run==378){
	      if(j==23){ffunc[j] -> SetParLimits(4, 40, 80);};//pmt26
	      if(j==23){ffunc[j] -> SetParameter(5, 5);};//pmt22
	    }


	    if(run==374){
	      if(j==1){ffunc[j] -> SetParLimits(4, 8, 40);};//pmt26
	      if(j==23){ffunc[j] -> SetParLimits(4, 40, 80);};//pmt26
	      if(j==23){ffunc[j] -> SetParameter(5, 5);};//
	      if(j==27){ffunc[j] -> SetParLimits(4, 15, 50);};//pmt26
	      if(j==32){ffunc[j] -> SetParLimits(4, 30, 80);};//pmt26
	      if(j==32){ffunc[j] -> SetParameter(5, 20);};//
	    }

	    
	    if(run==367){
	      if(j==38){ffunc[j] -> SetParameter(5, 20);};//
	      if(j==46){ffunc[j] -> SetParLimits(4, 20, 55);};//pmt26
	      if(j==46){ffunc[j] -> SetParameter(5, 5);};//
	    }
	    

	    if(run==362){
	      if(j==15){ffunc[j] -> SetParLimits(4, 38, 50);};//pmt26
	      if(j==15){ffunc[j] -> SetParameter(5, 2);};//
	    }

	    if(run==425){
	      if(j==0){ffunc[j] -> SetParameter(5, 15);};//
	      if(j==4){ffunc[j] -> SetParameter(5, 15);};//
	      if(j==6){ffunc[j] -> SetParameter(5, 15);};//	      	      
	    }
	    
	    
	    
	    
      tcan->cd();
      
	    tcan->cd(j+1);
	    gPad->SetLogy();
      his[j] -> Fit(ffunc[j], "R");

      for(int ipa=0; ipa<6; ipa++){
	p[ipa]=ffunc[j]->GetParameter(ipa);
      }
      
      his[j]->GetXaxis()->SetRangeUser(0,200);
      //      can->cd();
      his[j]->Draw();
      tcan->Update();
      

 cout<<""<<endl;
      TFitResultPtr fitResult = his[j]->Fit(ffunc[j], "S"); // "S" 옵션: 결과 저장
std::cout << "Chi2: " << fitResult->Chi2() << std::endl;
std::cout << "NDF: " << fitResult->Ndf() << std::endl;
std::cout << "Reduced Chi2: " << fitResult->Chi2() / fitResult->Ndf() << std::endl;
 cout<<""<<endl;


 chi2=(fitResult->Chi2()) / (fitResult->Ndf());
      double charge = (2500e-3/4096.)*(2e-9/50.)*ffunc[j] -> GetParameter(4);
      charge /= 1.6e-19;
      cout<<"charge " <<charge<<endl;

      //(DynamicRange [mV]/Resolution)*(BinSize [ns]/Resistance [ohm])*ADC
      double chargeE = (2500e-3/4096.)*(2e-9/50.)*ffunc[j] -> GetParError(4);
      chargeE /= 1.6e-19;
      cout<<"chargeE " <<chargeE<<endl;      

      q=charge; q_err=chargeE;
      cout<<"run " <<run<<" / pmt "<<j+1<<endl;
      
      //   cin>>a;
      tree->Fill();
    }
  }
  TFile of(Form("./hist/GainCal_%06d.root", run), "recreate");
  for(int i = 0; i < 47; i++){his[i] -> Write();}
  tcan->Write();
  tree->Write();
  of.Close();

  
}
