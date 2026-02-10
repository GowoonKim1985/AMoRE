R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_wcmd_mul(){
  
  gStyle -> SetOptStat(0);
  //TFile *f = new TFile("./prod_2nd/pwcoin_all.root");
   TCanvas *c1 = new TCanvas("c1", "multiplicity", 1300, 900);
 TH1D * h = new TH1D("h","",35,10,45);
 c1->cd();
 gPad->SetLeftMargin(0.12);   // y축 여백 ← 핵심
 gPad->SetLogy();
 //TGaxis::SetMaxDigits(3);

// TString rawfile = "./prod_2nd/pwcoin_all_cut.root";
 TString name1 = "../hist/wcmd_tbit_all.root";
 TFile *f1 = new TFile(name1); 
 TH1D *h1 = (TH1D*)f1->Get("all_tbit");

 // h1->Draw();
 TString name2 = "../hist/wcmd_tbit_all_cut.root";
 TFile *f2 = new TFile(name2); 
 TH1D * h2 = (TH1D*)f2->Get("tbit_all_cut");
 // h2->Draw("SAME");
 
 TString name3 = "../hist/wcmd_tbit_coin_all.root";
 TFile *f3 = new TFile(name3); 
 TH1D * h3 = (TH1D*)f3->Get("tbit_coin_all");
 /*
 TString name3 = "../hist/wcmd_tbit_coin_bot.root";
 TFile *f3 = new TFile(name3); 
 TH1D * h3 = (TH1D*)f3->Get("bit_coin_bot");
 */
 

 
  h1->GetYaxis()->SetTitle("Events");
  h1->GetXaxis()->SetTitle("PMT multiplicity");

  h1->GetXaxis()->SetTitleFont(62);
  h1->GetYaxis()->SetTitleFont(62);


  h1->GetXaxis()->SetTitleSize(0.04);
  h1->GetYaxis()->SetTitleSize(0.04);

 // h[0]->SetLineColor(kRed);
 h1->SetLineColor(kBlack);
 h1->GetXaxis()->SetRangeUser(0,48);
 h1->GetYaxis()->SetRangeUser(10,10e6);
 h1->SetLineWidth(2);
 h1->SetLineStyle(2); 

 h1->SetStats(0);

 h2->SetLineColor(kBlack);
 h2->GetXaxis()->SetRangeUser(0,48); 
 h2->GetYaxis()->SetRangeUser(10,10e6);
 h2->SetLineWidth(2);
 h2->SetStats(0); 

 h3->SetLineColor(kRed);
 h3->GetXaxis()->SetRangeUser(0,48);
 h3->GetYaxis()->SetRangeUser(10,10e6);
 h3->SetLineWidth(2);
 h3->SetStats(0);



 // h[0]->SetLineColor(0);                    // 테두리선은 검정
// h[0]->SetFillColorAlpha(kGray, 0.3);       // 회색 + 알파값 (0.0 ~ 1.0)
 h1->Draw("");
 h2->Draw("SAME");
 h3->Draw("SAME");

}
