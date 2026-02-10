R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

#include <TFile.h>
#include <TH1F.h>
#include <TF1.h>
#include <TCanvas.h>
#include "TGraphErrors.h"

void draw_rambda()
{

  TFile * outfile = new TFile("./plot/rambda_ratio_exp.root","RECREATE");
  const int dp = 15;
  const int set = 2;
  double x[set][dp];
  double y[set][dp];
  double ex[set][dp];
  double ey[set][dp];

    double x[0][i]=i;

    double y[0][0]=1.86;    double y[0][0]=0.68;
    double y[0][1]=1.86;    double y[0][1]=0.68;
    double y[0][2]=1.86;    double y[0][2]=0.68;
    double y[0][3]=1.86;    double y[0][3]=0.68;
    double y[0][4]=1.86;    double y[0][4]=0.68;
    double y[0][5]=1.86;    double y[0][5]=0.68;
    double y[0][6]=1.86;    double y[0][6]=0.68;
    double y[0][7]=1.86;    double y[0][7]=0.68;
    double y[0][8]=1.86;    double y[0][8]=0.68;
    double y[0][9]=1.86;    double y[0][9]=0.68;
    double y[0][10]=1.86;    double y[0][10]=0.68;
    double y[0][11]=1.86;    double y[0][11]=0.68;
    double y[0][12]=1.86;    double y[0][12]=0.68;
    double y[0][13]=1.86;    double y[0][13]=0.68;
    double y[0][14]=1.86;    double y[0][14]=0.68;





    
  TTree * fitpara = new TTree("fitpara","fitpara");
  double p0, p1, err0, err1;
  TCanvas * c0 = new TCanvas("c0","c0", 1600,900);
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


  hist[i]->SetName(Form("dp%i",i));
  outfile->cd();
  hist[i]->Write();

  c0->cd(i+1);
  hist[i]->Draw("P");
    //    c1->Update();
  }   
  c0->Update();
  fitpara->Write();
  c0->Update();
  c0->Write();
  outfile->Close();

}
