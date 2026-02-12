void draw_qhis_comparison() {
    // 1. 파일 열기 (파일명은 실제 경로에 맞게 수정하세요)

  int grp = 9;
  int idet = 52;
  TFile *f1 = TFile::Open(Form("/home/kkw/Works/paper/hist/qhist_psmd_grp%i.root", grp));
  TFile *f2 = TFile::Open(Form("/home/kkw/Works/paper/hist/qhist_psmd_coin_grp%i.root", grp));

    TCanvas *c1 = new TCanvas("c1", "Histogram Comparison", 900, 600);

        // Y축을 로그 스케일로 설정 (0: 선형, 1: 로그)
    c1->SetLogy(1);
    
    TString histName = Form("hqsum_%d", idet);

        // 2. 히스토그램 가져오기
        TH1F *h1 = (TH1F*)f1->Get(histName);
        TH1F *h2 = (TH1F*)f2->Get(histName);

        // 3. 빈(bin) 수 반으로 줄이기
	//        h1->Rebin(2);
	//      h2->Rebin(2);
        h1->GetXaxis()->SetRangeUser(0, 50000);
        // 4. 색상 및 스타일 설정
        h1->SetLineColor(kBlack);
        h2->SetLineColor(kRed);

	//        h1->SetLineWidth(2);
	//    h2->SetLineWidth(2);
        // 5. 그리기
        h1->SetTitle(";Charge [ADC Counts];Number of Entries");
  h1->GetXaxis()->SetTitleFont(62);
  h1->GetYaxis()->SetTitleFont(62);
h1->GetXaxis()->SetMaxDigits(3);
  
        h1->Draw("hist");       // 첫 번째는 그냥 Draw
        h2->Draw("hist same");  // 나머지는 same 옵션
   
        
       //cut line
double cut_x = 12000; // 원하는 컷 위치 (예: 21050)
double y_max = h1->GetMaximum(); // h1은 히스토그램 이름

TLine *line = new TLine(cut_x, 0, cut_x, y_max * 1.8);

// 2. 선 스타일 설정
line->SetLineColor(kRed);      // 빨간색
 line->SetLineWidth(3);         // 선 굵기
 line->SetLineStyle(2);         // 점선 (2번 스타일이 Dashed입니다)

// 3. 그리기
 line->Draw("same"); // "same"을 써야 히스토그램 위에 겹쳐 그려집니다.
 c1->Update();
       

}
