R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_cointime(){
  
  gStyle -> SetOptStat(0);
  //TFile *f = new TFile("../prod_2nd/pwcoin_all.root");
   TCanvas *c1 = new TCanvas("c1", "Summed Histogram", 1300, 900);
 TH1D * h = new TH1D("h","",150,-1000,1000);
 c1->cd();
 gPad->SetLeftMargin(0.12);   // y축 여백 ← 핵심
 gPad->SetLogy();
 //TGaxis::SetMaxDigits(3);

// TString rawfile = "./prod_2nd/pwcoin_all_cut.root";
 TString rawfile = "../prod_2nd/pwcoin_all.root";
 TChain * t = new TChain("coin");
 t->Add(rawfile.Data());
 int tg;
 int tent= t->GetEntries();
 cout<<"tent "<<tent<<endl;
 // for(int i=0; i<10; i++){

    for(int i=0; i<tent; i++){
   t->GetEntry(i);
   tg = t->GetLeaf("timegap")->GetValue();
   // cout<<"tg "<<tg<<endl;
     h->Fill(tg);

   }

 
  h->GetYaxis()->SetTitle("Entries");
  h->GetXaxis()->SetTitle("dT[ns]");

  h->GetXaxis()->SetTitleFont(62);
  h->GetYaxis()->SetTitleFont(62);


  h->GetXaxis()->SetTitleSize(0.04);
  h->GetYaxis()->SetTitleSize(0.04);

 // h[0]->SetLineColor(kRed);
 h->SetLineColor(kBlack); 

// h[0]->SetLineColor(0);                    // 테두리선은 검정
// h[0]->SetFillColorAlpha(kGray, 0.3);       // 회색 + 알파값 (0.0 ~ 1.0)
 h->Draw("hist");


}
