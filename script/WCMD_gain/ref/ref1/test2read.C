R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libFramework)
       
double twogaus(double *v, double *p)  // fPoly funct
{
  double x = v[0];

  double a = p[0];
  double m1 = p[1];
  double s1 = p[2];
  double b = p[3];
  double m2 = p[4];
  double s2 = p[5];

  double y1 = TMath::Gaus(x, m1, s1, true);
  double y2 = TMath::Gaus(x, m2, s2, true);

  return a*y1 + b*y2;
}

void test2read()
{

  gStyle->SetOptFit(1);
  
  TStopwatch sw;
  sw.Start();
    
  int nch = 48;
  int id[nch];
  double qi[nch], q2[nch];
  double hmax[nch], hmaxx[nch], pedm[nch], pedrms[nch];
  double qsum, qsumsel1;
  double trigtime, t1, tf;
  double qmax, R;
  int maxid, mtp, mtp2;

  double gain[nch], gain2[nch], gainf[nch];
 
  TH1D *his[48], *Qhis[48], *qhis2[nch];
  TH1D *qhis[48];
  TH1D *hmaxhis[48];

  TH1D *qsumhis, *qmax_mtp47, *qsum_mtp47, *qsumG;
  TH1D *qsumsel;
  TH1D *qmaxhis;
  TH1D *maxidhis;
  TH1D *mtphis, *mtp2his;              // ocupancy plot = maxidhis
  TH1D *qsumsel1his, *qsumsel2his, *qsumsel3his;
  TH1D *Rhis;
  TF1 *f1;

  TChain *chain = new TChain("prod");
  chain->Add("testfit2_20230810_MTH10RL16TH5.root");
  
  chain->SetBranchAddress("qi", qi);
  chain->SetBranchAddress("hmax", hmax);
  chain->SetBranchAddress("hmaxx", hmaxx);
  chain->SetBranchAddress("pedm", pedm);
  chain->SetBranchAddress("pedrms", pedrms);
  chain->SetBranchAddress("id", id);
  chain->SetBranchAddress("trigtime", &trigtime);
 
  for(int i = 0; i < 48; i++) {
    qhis[i] = new TH1D(Form("qhis_%d", i+1), "", 100, -50, 250);
    qhis2[i] = new TH1D(Form("qhis2_%d", i+1), "", 100, -50, 250); 
  }

   int mb, mb_sp;
   double max, maxx, sig, sig2, max_sp, maxx_sp;
  
  int nent = chain->GetEntries();
  //int nent = chain->GetEntries();
  cout << "Tatal number of entries: " << nent << endl;

  for(int i = 0; i < nent; i++) {
    chain->GetEntry(i);

     if (i > 0 && i % 500000 == 0)
      cout << Form("%6d events processed ......", i) << endl;

     for(int j = 0; j < nch; j++) {
       
       //cout << i << ": " << qi[j] << ": " << hmax[j] << ": " << hmaxx[j] << ": " << id[j]<< endl;

	 qhis[j]->Fill(qi[j]);  

       }
       
  }

  TCanvas *can2 = new TCanvas("can2", "", 1700, 900);
  can2->Divide(7, 7);
  for(int j = 0; j < 48; j++) {
    can2->cd(j+1);
    gPad->SetLogy();
    // qhis[j]->Draw();
    // }
  //----------------------------------------------------------First fitting---------------------------------------------------------------
  
  
  //for(int j = 0; j < nch; j++) {
    double maxh = qhis[j]->GetMaximum();
    int maxbin = qhis[j]->GetMaximumBin();
    double maxx = qhis[j]->GetBinCenter(maxbin);
    double std = qhis[j]->GetStdDev();
    // cout << "Std" << std << endl;
    //-------------------------------------------------------------------------------
    int nbinsx = qhis[j] -> GetNbinsX();
      double lw = maxx-10 < 0 ? 0 : maxx-10;
      double up = maxx+10;
      TF1 *func = new TF1("func", "[0]*TMath::Gaus(x, [1], [2])", lw, up);
      func -> SetParameter(0, maxh);
      func -> SetParameter(1, maxx);
      func -> SetParLimits(1, 0, 1000);
      func -> SetParameter(2, 10);
      qhis[j] -> Fit(func, "R0Q");

      maxh = func -> GetParameter(0);
      maxx = func -> GetParameter(1);
      sig = func -> GetParameter(2);
      sig = TMath::Abs(sig);

      TF1 *fpol = new TF1("fpol", "pol7", maxx+sig, maxx+10*sig);
      qhis[j] -> Fit(fpol, "R0Q");
      double lw_sp = 0;
      for(double k = maxx+sig; k < maxx+10*sig; k = k+0.1){
	//cout << k << " " << fpol -> Derivative(k) << endl;
	if(fpol -> Derivative(k) > -0.1){
	  lw_sp = k;
	  break;
	}
      }
      mb_sp = 0;
      maxx_sp = 0;
      max_sp = 0;
      int lw_spbin = qhis[j] -> FindBin(lw_sp);
      for(int k = lw_spbin; k < nbinsx; k++){
	if(qhis[j] -> GetBinContent(k+1) > max_sp){
	  max_sp = qhis[j] -> GetBinContent(k+1);
	  maxx_sp = qhis[j] -> GetBinCenter(k);
	}
      }
      TF1 *f1 = new TF1("f1",twogaus, 0, maxx_sp*2, 6);
      f1->SetParameter(0, maxh);
      f1->SetParameter(1, maxx);
      f1->SetParLimits(1, 0, 1000);
      f1->SetParameter(2, 5);
      f1->SetParameter(3, max_sp);
      f1->SetParameter(4, maxx_sp);
      f1->SetParLimits(4, 0, 1000);
      f1->SetParameter(5, 5);

      qhis[j]->Fit(f1, "RQ");
      
      gain[j] = f1->GetParameter(4);
      //cout << id[j] << ": " << gain[j] << endl;
  }

  //------------------------start of second fit-----------------------------------

  for(int i = 0; i < nent; i++) {
    chain->GetEntry(i);
    
    for(int j = 0; j < nch; j++) {
      q2[j] = qi[j]*gain[0]/gain[j];
       
      qhis2[j]->Fill(q2[j]);     
     }
   }

   TCanvas *can3 = new TCanvas("can3", "", 1700, 900);
  can3->Divide(7, 7);
  for(int j = 0; j < 48; j++) {
    can3->cd(j+1);
    gPad->SetLogy();
    // qhis[j]->Draw();
    // } 
    //-------------------------------------------------------------------------------
  //for(int j = 0; j < nch; j++) {
    double maxh2 = qhis2[j]->GetMaximum();
    int maxbin2 = qhis2[j]->GetMaximumBin();
    double maxx2 = qhis2[j]->GetBinCenter(maxbin2);
    double std2 = qhis2[j]->GetStdDev();
    // cout << "Std" << std << endl;
    //-------------------------------------------------------------------------------
    int nbinsx2 = qhis2[j] -> GetNbinsX();
      double lw2 = maxx2-10 < 0 ? 0 : maxx2-10;
      double up2 = maxx2+10;
      TF1 *func2 = new TF1("func2", "[0]*TMath::Gaus(x, [1], [2])", lw2, up2);
      func2 -> SetParameter(0, maxh2);
      func2 -> SetParameter(1, maxx2);
      func2 -> SetParLimits(1, 0, 1000);
      func2 -> SetParameter(2, 10);
      qhis2[j] -> Fit(func2, "R0Q");

      maxh2 = func2 -> GetParameter(0);
      maxx2 = func2 -> GetParameter(1);
      sig2 = func2 -> GetParameter(2);
      sig2 = TMath::Abs(sig2);

      TF1 *fpol2 = new TF1("fpol2", "pol7", maxx2+sig2, maxx2+10*sig2);
      qhis2[j] -> Fit(fpol2, "R0Q");
      double lw_sp2 = 0;
      for(double k = maxx2+sig2; k < maxx2+10*sig2; k = k+0.1){
	//cout << k << " " << fpol -> Derivative(k) << endl;
	if(fpol2 -> Derivative(k) > -0.1){
	  lw_sp2 = k;
	  break;
	}
      }
      mb_sp = 0;
      maxx_sp = 0;
      max_sp = 0;
      int lw_spbin2 = qhis2[j] -> FindBin(lw_sp2);
      for(int k = lw_spbin2; k < nbinsx2; k++){
	if(qhis2[j] -> GetBinContent(k+1) > max_sp){
	  max_sp = qhis2[j] -> GetBinContent(k+1);
	  maxx_sp = qhis2[j] -> GetBinCenter(k);
	}
      }
      TF1 *f2 = new TF1("f2",twogaus, 0, maxx_sp*2, 6);
      f2->SetParameter(0, maxh2);
      f2->SetParameter(1, maxx2);
      f2->SetParLimits(1, 0, 1000);
      f2->SetParameter(2, 5);
      f2->SetParameter(3, max_sp);
      f2->SetParameter(4, maxx_sp);
      f2->SetParLimits(4, 0, 1000);
      f2->SetParameter(5, 5);

      qhis2[j]->Fit(f2, "RQ");
      
      gainf[j] = f2->GetParameter(4);
      
      cout << id[j] << ": " << gain[j] << ": " << gainf[j] << endl;
  }



    //--------------------------end of secondfit---------------------------------------
  /*
    double upper = maxx + 10;
    double lower = maxx - 10;

    TF1 *f1 = new TF1("f1", "gaus", lower, upper);
    Qhis[j]->Fit(f1, "RQ");
    double fitpar = f1->GetParameter(1);
    gain[j] = fitpar;
    //gain2[j] = gain[j]*gain[12]/gain[j];
    
  }
  
  //------------------------------------------------------------Second fitting--------------------------------------------------------

   for(int i = 0; i < nent; i++) {
     chain->GetEntry(i);
    
     for(int j = 0; j < nch; j++) {
       q[j] = q4[j]*gain[12]/gain[j];
       
       Qhis2[j]->Fill(q[j]);     
     }
   }

   for(int j = 0; j < nch; j++) {
    int max2bin = Qhis2[j]->GetMaximumBin();
    double max2x = Qhis2[j]->GetBinCenter(max2bin);
    double std2 = Qhis2[j]->GetStdDev();
    // cout << "Std2" << std << endl;

    double upper2 = max2x + std2;
    double lower2 = max2x - std2;

    TF1 *f2 = new TF1("f2", "gaus", lower2, upper2);
    Qhis2[j]->Fit(f2, "RQ");
    double fitpar2 = f2->GetParameter(1);
    gainf[j] = fitpar2;
    
  }

   for(int j = 0; j < nch; j++) {
     cout << id[j] << ": " << gain[j] << ": " << gainf[j]<< endl;
   }
   
   //-----------------------------------------------End------------------------------------------------------------------------
   
  TCanvas *c[4];
  for(int k = 0; k < 4; k++) {
    c[k] = new TCanvas(Form("c%d", k),  "", 1800, 900);
    c[k]->Divide(4,3);
    for(int i = 0; i < 12; i++) {
	c[k]->cd(i+1);
	//gPad->SetLogy();
	Qhis2[12*k+1]->Draw();
    }
  }
  
  TCanvas *c2[4];
  for(int k = 0; k < 4; k++) {
    c2[k] = new TCanvas(Form("c2%d", k),  "", 1800, 900);
    c2[k]->Divide(4,3);
    for(int i = 0; i < 12; i++) {
	c2[k]->cd(i+1);
	//gPad->SetLogy();
	Qhis2[12*k+1]->Draw();
	//Qhis[12*k+i]->Fit(f1, "R");
    }
  }
  */ 
  
  sw.Stop();

  cout << "Total time: " << sw.RealTime()/60 << " min " << endl;
     
}
