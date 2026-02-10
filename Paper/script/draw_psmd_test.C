R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_psmd_test(){
  
  TFile *f = new TFile("./hist/anaM_210903_box20_hv54_th500_30min_qa2.root");

    gStyle -> SetOptStat(0);
  TCanvas *can[6];
  for(int i=0; i<4;i++){
    can[i] = new TCanvas(Form("can%i",i), Form("qhis%i",i), 1300, 900);
  gPad->SetLeftMargin(0.12);   
  }
  can[4] = new TCanvas("can4", "2d plot", 1000, 900);
  //  c1->cd();

  gPad->SetLeftMargin(0.16);
    gPad->SetRightMargin(0.15);   
  can[5] = new TCanvas("can5", "qhis all", 1300, 900);
    gPad->SetLeftMargin(0.12);   
  //  TGaxis::SetMaxDigits(3);

 TH1D * h[4];

//TH1D* h_sum = nullptr;
 for(int i=0; i<4; i++){

   h[i] = (TH1D*)f->Get(Form("D0cal%i",i));
   h[i]->GetListOfFunctions()->Clear();  
//   h[i]->GetXaxis()->SetRangeUser(5000, 15000);

   h[i]->GetXaxis()->SetTitle("Charge [ADC counts]");
   h[i]->GetYaxis()->SetTitle("Number of Events");
   
   h[i]->GetXaxis()->SetTitleFont(62);
   h[i]->GetYaxis()->SetTitleFont(62);
   h[i]->GetXaxis()->SetTitleSize(0.04);
   h[i]->GetYaxis()->SetTitleSize(0.04);
   h[i]->SetLineColor(kBlack); 
   can[i]->cd();
   h[i]->Draw();

 }

    h[0]->SetLineColor(kBlack);
    h[1]->SetLineColor(kRed);
    h[2]->SetLineColor(kBlue); 
    h[3]->SetLineColor(kGreen); 

          can[5]->cd();
	  h[0]->Draw();
    for(int i=0; i<4; i++){
      h[i]->Draw("SAME");
      
    }
 TH2D * his2d;

 his2d = (TH2D*)f->Get("D0MC2d");
 his2d->SetXTitle("Bottom panel Charge");
 his2d->SetYTitle("Top panel Charge");
   
 his2d->GetXaxis()->SetTitleFont(62);
 his2d->GetYaxis()->SetTitleFont(62);

 his2d->GetXaxis()->SetTitleSize(0.04);
 his2d->GetYaxis()->SetTitleSize(0.04);

 can[4]->cd();
 his2d->Draw("colz");

 


 
   //h_sum->Draw("hist");
}
