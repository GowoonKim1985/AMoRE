void draw_psmd_occupancy() {
    // 1. 파일 및 트리 가져오기
    TFile *f = new TFile("./prod_2nd/psmd_all.root", "READ");
    if (!f || f->IsZombie()) return;
    TTree *psmd = (TTree*)f->Get("psmd");

    // 2. 히스토그램 생성 (1~41 범위로 설정하여 숫자가 중앙에 오게 함)
    TH1F *h_occ = new TH1F("h_occ", ";Detector No.;Counts", 40, 1, 41);
    for (int i = 1; i <= 40; i++) {
        h_occ->GetXaxis()->SetBinLabel(i, Form("%d", i));
    }

    // 3. X축/Y축 스타일 설정
    h_occ->GetXaxis()->SetLabelSize(0.03);
    h_occ->GetXaxis()->SetTickLength(0); // X축 눈금 제거
    
    h_occ->GetYaxis()->SetLabelSize(0.035);
    h_occ->GetYaxis()->SetTickLength(0.015); // Y축 눈금 짧게
    
    h_occ->SetMinimum(0.8); // 로그 스케일 바닥 정돈
    h_occ->SetStats(0);

    // 4. 투명도 및 색상 설정 (핵심)
    // SetFillColorAlpha는 특정 색상에 투명도(0.0~1.0)를 직접 부여합니다.
    h_occ->SetFillColorAlpha(kBlue-7, 0.1); // 0.1로 아주 연하게 설정
    h_occ->SetLineColor(kBlue+1);
    h_occ->SetLineWidth(1);
    h_occ->SetFillStyle(1001); 

    // 5. 캔버스 및 로그 스케일 설정
    TCanvas *c1 = new TCanvas("c1", "Occupancy Canvas", 1000, 600);
    c1->SetLogy();
    c1->SetGridy();

    // 6. 데이터 그리기 (중요!)
    // "goff"를 넣어야 Draw 호출 시 자동으로 배경 히스토그램이 그려지는 것을 방지합니다.
    // 그래야 우리가 나중에 그리는 "hist"와 겹쳐서 진해지지 않습니다.
    psmd->Draw("bitsum >> h_occ", "", "goff");
    
    // 이제 딱 한 번만 그립니다.
    h_occ->Draw("hist");



        c1->SaveAs("./fig/fig_psmd_occupancy.pdf");
    c1->SaveAs("./fig/fig_psmd_occupancy.png");
}
