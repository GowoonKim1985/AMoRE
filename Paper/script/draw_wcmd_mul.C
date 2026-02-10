R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_wcmd_mul(){
  
  gStyle -> SetOptStat(0);
  //TFile *f = new TFile("./prod_2nd/pwcoin_all.root");
   TCanvas *c1 = new TCanvas("c1", "Summed Histogram", 1100, 1100);
   //   TCanvas *c2 = new TCanvas("c2", "qmax vs qsum", 1200, 1000);   

   TH1D * h0 = new TH1D("h0","",38,10,48);
   TH1D * h = new TH1D("h","",38,10,48);
   TH1D * hcut = new TH1D("hcut","",38,10,48);   

   c1->cd();
   gPad->SetLeftMargin(0.12);   // y축 여백 ← 핵심
      gPad->SetLogy();
 //TGaxis::SetMaxDigits(3);

 TString rawfile = "./prod_2nd/wcmd_all.root";
 TChain * t = new TChain("value");
 t->Add(rawfile.Data());
 int tbit;
 double qmax, qsum;

 int tent= t->GetEntries();
 cout<<"tent "<<tent<<endl;
 // for(int i=0; i<100000; i++){

       for(int i=0; i<tent; i++){
   t->GetEntry(i);
   qmax=t->GetLeaf("qmax")->GetValue();
   qsum=t->GetLeaf("qsum")->GetValue();
   tbit=t->GetLeaf("tbit")->GetValue();        
   if(qsum>3000){
   h0->Fill(tbit);

   if(qmax<0.8*qsum-2100){ h->Fill(tbit);   }
   else{hcut->Fill(tbit);}
   }
 }
 
  h0->GetXaxis()->SetTitle("Multiplicity");
  h0->GetYaxis()->SetTitle("Number of Events");

  h->GetXaxis()->SetTitleFont(62);
  h->GetYaxis()->SetTitleFont(62);


  h->GetXaxis()->SetTitleSize(0.04);
  h->GetYaxis()->SetTitleSize(0.04);

 // h[0]->SetLineColor(kRed);
 h0->SetLineColor(kBlack);
 // h0->SetLineStyle(2);
  h0->SetLineWidth(2);
 h->SetLineColor(kRed);
   h->SetLineWidth(2);
  h->SetFillStyle(3004);
  //  h->SetFillColorAlpha(kRed,0.1);
 h->SetLineStyle(2);
   hcut->SetLineColor(kBlue); 
  hcut->SetLineWidth(2);
  hcut->SetFillStyle(3004);
  //  hcut->SetFillColorAlpha(kBlue,0.1);
 hcut->SetLineStyle(2);

  // h[0]->SetLineColor(0);
// 테두리선은 검정
//h0->SetFillStyle(1001);                    // 채우기 스타일 (기본 실선 채움)
//  h0->SetFillColorAlpha(kGray, 0.8);       // 회색 + 알파값 (0.0 ~ 1.0)
//  h0->SetFillColorAlpha(kWhite, 0.1);       // 회색 + 알파값 (0.0 ~ 1.0)  

 c1->cd();
 h0->Draw();
 hcut->Draw("SAME"); 
 h->Draw("SAME");

 //   gausFit->SetLineStyle(2);  // 2: 점선 (dashed)
}
