R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_eff(){
  
  gStyle -> SetOptStat(0);
TFile *f = new TFile("./hist/eff/his2_210818_effset1_hv54.5_th350_10hr.root");
 TCanvas *c1 = new TCanvas("c1", "Summed Histogram", 1300, 900);

 c1->cd();
 gPad->SetLeftMargin(0.12);   // y축 여백 ← 핵심
 gPad->SetLogy();
TGaxis::SetMaxDigits(3);
  
 TH1D * h[2];

 h[0]= (TH1D*)f->Get("det_qsum");
 h[1]= (TH1D*)f->Get("det_qsum_sel"); 
//TH1D* h_sum = nullptr;
 for(int i=0; i<2; i++){
   //   h[i] = (TH1D*)f->Get(Form("D%dMChist",i));
   h[i]->Rebin(2);
  h[i]->GetXaxis()->SetRangeUser(1000, 100000);

  h[i]->GetXaxis()->SetTitle("Charge [ADC counts]");
  h[i]->GetYaxis()->SetTitle("Number of Events");

  h[i]->GetXaxis()->SetTitleFont(62);
  h[i]->GetYaxis()->SetTitleFont(62);


  h[i]->GetXaxis()->SetTitleSize(0.04);
  h[i]->GetYaxis()->SetTitleSize(0.04);
 }
 // h[0]->SetLineColor(kRed);
 h[1]->SetLineColor(kBlue); 

 h[0]->SetLineColor(0);                    // 테두리선은 검정
 h[0]->SetFillColorAlpha(kGray, 0.3);       // 회색 + 알파값 (0.0 ~ 1.0)
 h[0]->Draw("hist");


 h[0]->Draw();
 h[1]->Draw("SAME");
 

 TCanvas *c2 = new TCanvas("c2", "pannel 2d charge", 1100, 1000);

 c2->cd();
 // gPad->SetLeftMargin(0.12);   // y축 여백 ← 핵심
 // gPad->SetLogy();
 //TGaxis::SetMaxDigits(3);
  
 TH2D * ph;

 ph = (TH2D*)f->Get("pan_tb");

   ph->GetXaxis()->SetTitle("Lower pannel charge [ADC]");
   ph->GetYaxis()->SetTitle("Upper pannel charge [ADC]");

 //TH1D* h_sum = nullptr;

   //   h[i] = (TH1D*)f->Get(Form("D%dMChist",i));

   ph->GetXaxis()->SetRangeUser(1000, 13000);
   ph->GetYaxis()->SetRangeUser(1000, 15000);   


  ph->GetXaxis()->SetTitleFont(62);
  ph->GetYaxis()->SetTitleFont(62);


  ph->GetXaxis()->SetTitleSize(0.04);
  ph->GetYaxis()->SetTitleSize(0.04);
 // h[0]->SetLineColor(kRed);

 ph->Draw();


 
   //h_sum->Draw("hist");
}
