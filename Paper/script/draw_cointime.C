R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libRawObjs.so)

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
 TString rawfile = "./prod_2nd/pwcoin_all.root";
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

    //cut line
double cut_x1 = 200; // 원하는 컷 위치 (예: 21050)
double y_max1 = h->GetMaximum(); // h1은 히스토그램 이름

TLine *line1 = new TLine(cut_x1, 0, cut_x1, y_max1 * 1.8);

// 2. 선 스타일 설정
line1->SetLineColor(kRed);      // 빨간색
line1->SetLineWidth(3);         // 선 굵기
line1->SetLineStyle(2);         // 점선 (2번 스타일이 Dashed입니다)

// 3. 그리기
line1->Draw("same"); // "same"을 써야 히스토그램 위에 겹쳐 그려집니다.

//cut line
double cut_x2 = 400; // 원하는 컷 위치 (예: 21050)
double y_max2 = h->GetMaximum(); // h1은 히스토그램 이름

TLine *line2 = new TLine(cut_x2, 0, cut_x2, y_max2 * 1.8);

// 2. 선 스타일 설정
line2->SetLineColor(kRed);      // 빨간색
line2->SetLineWidth(3);         // 선 굵기
line2->SetLineStyle(2);         // 점선 (2번 스타일이 Dashed입니다)

// 3. 그리기
line2->Draw("same"); // "same"을 써야 히스토그램 위에 겹쳐 그려집니다.
}
