R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libRawObjs.so)

void draw_wcmd_qmax_qsum(){
  
  gStyle -> SetOptStat(0);
  //TFile *f = new TFile("./prod_2nd/pwcoin_all.root");
   TCanvas *c1 = new TCanvas("c1", "Summed Histogram", 1300, 900);
   //   TCanvas *c2 = new TCanvas("c2", "qmax vs qsum", 1200, 1000);   

   TH1D * h1 = new TH1D("h","",500,3000,70000);
   TH2D * h2 = new TH2D("h2","",1000,0,80000,1000,0,35000);
   TF1 * line = new TF1("line","0.8*x-2100",0,80000);



  // Gaussian 피팅 추가
   //  TF1 *gausFit = new TF1("gausFit", "gaus", 13800, 22000);  // 범위는 조정 가능


   c1->cd();
   gPad->SetLeftMargin(0.12);   // y축 여백 ← 핵심
   //   gPad->SetLogy();
 //TGaxis::SetMaxDigits(3);

 TString rawfile = "./prod_2nd/wcmd_all.root";
 TChain * t = new TChain("wcmd");
 t->Add(rawfile.Data());
 int tg;
 double qmax, qsum;
 int tent= t->GetEntries();
 cout<<"tent "<<tent<<endl;
 // for(int i=0; i<10000; i++){
 if(i%100000==0){cout<<i<<"/"<<tent<<}
 for(int i=0; i<tent; i++){
   t->GetEntry(i);
   qmax=t->GetLeaf("qmax")->GetValue();
   qsum=t->GetLeaf("qsum")->GetValue();     

   h2->Fill(qsum,qmax);
   if(qmax<0.8*qsum-2100){
     h1->Fill(qsum);
   }
 }
 
  h1->GetXaxis()->SetTitle("Charge [ADC counts]");
  h1->GetYaxis()->SetTitle("Number of Events");

  h1->GetXaxis()->SetTitleFont(62);
  h1->GetYaxis()->SetTitleFont(62);


  h1->GetXaxis()->SetTitleSize(0.04);
  h1->GetYaxis()->SetTitleSize(0.04);

 // h[0]->SetLineColor(kRed);
 h1->SetLineColor(kBlack); 

// h[0]->SetLineColor(0);                    // 테두리선은 검정
// h[0]->SetFillColorAlpha(kGray, 0.3);       // 회색 + 알파값 (0.0 ~ 1.0)
 c1->cd();
 h1->Draw();

 //   gausFit->SetLineStyle(2);  // 2: 점선 (dashed)
 //  gausFit->SetLineColor(kRed);     // 피팅선 색상
  //  gausFit->SetLineWidth(2);

 //  h1->Fit(gausFit, "R");  // "R"은 지정한 x범위에서만 피팅
 //  gausFit->Draw("same");           // h1 위에 피팅선 추가로 그림

  /* 
 h2->GetXaxis()->SetTitle("All PMT Charge [ADC]");
 h2->GetYaxis()->SetTitle("PMT_{max} Charge [ADC]");

 h2->GetXaxis()->SetTitleFont(62);
 h2->GetYaxis()->SetTitleFont(62);


 h2->GetXaxis()->SetTitleSize(0.04);
 h2->GetYaxis()->SetTitleSize(0.04);
 c2->cd();
    gPad->SetLeftMargin(0.16);   // y축 여백 ← 핵심
 h2->Draw();
 line->Draw("SAME");
  */
}
