R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TGraphErrors.h>

void fit_hist()
{

  TFile * outfile = new TFile("./plot/fit_ratio_exp.root","RECREATE");

  const int dp = 15;

  double x[dp], y[dp], ex[dp], ey[dp];

  TTree * fitpara = new TTree("fitpara","fitpara");
  double p0, p1, err0, err1;
  TCanvas * c0 = new TCanvas("c0","c0", 1600,900);
  TCanvas * cr = new TCanvas("cr","cr", 800,600);
  c0->Divide(4,3);

  fitpara->Branch("p0",&p0,"p0/D");
  fitpara->Branch("err0",&err0,"err0/D");
  //  fitpara->Branch("p1",&p1,"p1/D");  
  //  fitpara->Branch("err1",&err1,"err1/D");  

  /*
  int run[dp]={287, 294, 303, 304, 304, 318, 330, 330, 343, 356, 361, 368};
  int div[dp]={0, 0, 19, 0, 30, 12, 1, 26, 0, 22, 0, 12};
  int day[dp]={0, 29, 53, 84, 113, 144, 174, 201, 238, 266, 299, 317};
  */
  
  int run[dp]={287, 294, 303, 304, 304, 311, 315, 318, 318, 330, 330, 343, 356, 361, 368};
  int div[dp]={0, 0, 19, 0, 30, 4, 4, 4, 12, 1, 26, 0, 22, 0, 12};
  int day[dp]={0, 29, 53, 84, 91, 98, 105, 113, 144, 174, 201, 238, 266, 299, 317};

    TF1 *fitFunc = new TF1("fitFunc", "exp(-x/[0])", 3, 7);
  //  TF1 *fitFunc = new TF1("fitFunc", "exp((-x/[0])+[1])", 3, 7);  
  TH1F *hist[dp];

  for(int i=0; i<dp; i++){
    
    TFile *file = TFile::Open(Form("./plot/run%i_ratio_24hr_exp.root",run[i]));
    hist[i] = (TH1F*)file->Get(Form("ratio%i",div[i]));

    TCanvas *c1 = new TCanvas("c1", "Fit example", 800, 600);
    hist[i]->Draw("p");
    

  //  TF1 *fitFunc = new TF1("fitFunc", "[0]/(x*x)", 1.5, 6);
  //    fitFunc->SetParameter(0, 100);

  fitFunc->SetParameter(0, 1);    
  //  fitFunc->SetParameter(1, -1);

  
  /*
   TF1 *fitFunc = new TF1("fitFunc", "(exp([0]+x*[1]))*[2]/(x*x)", 0.5, 8);        fitFunc->SetParameter(0, -1);    
  fitFunc->SetParameter(1, -1);
  fitFunc->SetParameter(2, 100);        
  */





  hist[i]->SetMinimum(0);
  hist[i]->SetMaximum(0.6);  
  hist[i]->Fit("fitFunc","R");
  p0 = fitFunc->GetParameter(0);
  err0 = fitFunc->GetParError(0);
  //  p1 = fitFunc->GetParameter(1);
  //  err1 = fitFunc->GetParError(1);

  fitpara->Fill();
  x[i]=i+1;
  ex[i]=0;
  y[i]=p0;
  ey[i]=err0;
  
  hist[i]->SetName(Form("dp%i",i));
  outfile->cd();
  hist[i]->Write();

  c0->cd(i+1);
  hist[i]->Draw("P");
    //    c1->Update();
  }

  TGraphErrors *rambda = new TGraphErrors(dp, x, y, ex, ey);
  rambda->SetTitle("rambda");
  rambda->SetMarkerStyle(21);
  rambda->GetXaxis()->SetLimits(0,16);
  rambda->SetMinimum(0);
  rambda->SetMaximum(5);

  cr->cd();
  rambda->Draw("AP");
  
  fitpara->Write();
  rambda->Write("rambda");
  c0->Update();
  c0->Write();
  cr->Update();
  cr->Write();
  outfile->Close();

}
