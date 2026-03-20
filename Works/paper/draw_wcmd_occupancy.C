void draw_wcmd_occupancy() {
    // 1. 파일 및 트리 가져오기 (wcmd_all.root / wcmd 트리)
    TFile *f = new TFile("./prod_2nd/wcmd_all.root", "READ");
    if (!f || f->IsZombie()) {
        printf("wcmd 파일을 열 수 없습니다.\n");
        return;
    }
    TTree *wcmd = (TTree*)f->Get("wcmd");

    // 2. 히스토그램 생성 (1~41 범위, 40개 Bin)
    TH1F *h_occ = new TH1F("h_occ_wcmd", ";Detector No.;Counts", 47, 1, 48);
    for (int i = 1; i <= 47; i++) {
        h_occ->GetXaxis()->SetBinLabel(i, Form("%d", i));
    }

    // 3. 축 스타일 설정
    h_occ->GetXaxis()->SetLabelSize(0.03);
    h_occ->GetXaxis()->SetTickLength(0);
    
    h_occ->GetYaxis()->SetLabelSize(0.035);
    h_occ->GetYaxis()->SetTickLength(0.02);
    // Y축 눈금을 10의 승수 단위 위주로 정리
    h_occ->GetYaxis()->SetNdivisions(10, kFALSE); 
    
    h_occ->SetMinimum(0.8); 
    h_occ->SetStats(0);

    // 4. 색상 및 반투명도 설정 (붉은색 계통)
    // kRed-7 색상에 투명도 0.1 (10%) 적용
    h_occ->SetFillColorAlpha(kRed-7, 0.1); 
    h_occ->SetLineColor(kRed+1); // 외곽선은 조금 더 진한 빨강
    h_occ->SetLineWidth(1);
    h_occ->SetFillStyle(1001);

    // 5. 캔버스 및 로그 스케일 설정
    TCanvas *c1 = new TCanvas("c2", "wcmd Occupancy", 1000, 600);
    c1->SetLogy();
    c1->SetGridy();

    // 6. 데이터 그리기 (bitsum 리프 사용)]
        TCut myCut = "qmax < (qsum * 0.8 - 2100) &&qsum>3000";
    wcmd->Draw("tbit >> h_occ_wcmd", myCut, "goff");
    h_occ->Draw("hist");

    // 7. PDF 파일 저장
    gSystem->mkdir("./fig", kTRUE);
    c1->SaveAs("./fig/fig_wcmd_occupancy.pdf");
    
    printf("wcmd 오큐판시가 ./fig/fig_wcmd_occupancy.pdf 로 저장되었습니다.\n");
}
