void check_qhis() {
    // 1. 파일 열기 (파일명은 실제 경로에 맞게 수정하세요)

  int grp = 10;
  TFile *f1 = TFile::Open(Form("/home/kkw/Works/paper/hist/qhist_psmd_grp%i.root", grp));
  TFile *f2 = TFile::Open(Form("/home/kkw/Works/paper/hist/qhist_psmd_coin_grp%i.root", grp));
  TFile *f3 = TFile::Open(Form("./hist/qhist_psmd_2hit_grp%i.root", grp));

    if (!f1 || !f2 || !f3) {
        printf("파일을 열 수 없습니다.\n");
        return;
    }

    TCanvas *c1 = new TCanvas("c1", "Histogram Comparison", 900, 600);

        // Y축을 로그 스케일로 설정 (0: 선형, 1: 로그)
    c1->SetLogy(1);
    
    int idet, fdet;
    if(grp==1){idet=0; fdet=11;}
    if(grp==2){idet=12; fdet=23;}
    if(grp==3){idet=24; fdet=35;}
    if(grp==4){idet=36; fdet=47;}
    if(grp==5){idet=65; fdet=76;}
    if(grp==6){idet=77; fdet=88;}
    if(grp==7){idet=89; fdet=100;}
    if(grp==8){idet=101; fdet=112;}
    if(grp==9){idet=48; fdet=58;}
    if(grp==10){idet=113; fdet=123;}

    for (int i = idet; i <= fdet; ++i) {
        TString histName = Form("hqsum_%d", i);

        // 2. 히스토그램 가져오기
        TH1F *h1 = (TH1F*)f1->Get(histName);
        TH1F *h2 = (TH1F*)f2->Get(histName);
        TH1F *h3 = (TH1F*)f3->Get(histName);

        if (!h1 || !h2 || !h3) {
            printf("%s 가 존재하지 않습니다. 건너뜁니다.\n", histName.Data());
            continue;
        }

        // 3. 빈(bin) 수 반으로 줄이기
        h1->Rebin(2);
        h2->Rebin(2);
        h3->Rebin(2);
        h1->GetXaxis()->SetRangeUser(0, 50000);
        // 4. 색상 및 스타일 설정
        h1->SetLineColor(kBlack);
        h2->SetLineColor(kRed);
        h3->SetLineColor(kBlue);

        h1->SetLineWidth(2);
        h2->SetLineWidth(2);
        h3->SetLineWidth(2);

        // 5. 그리기
        h1->SetTitle(Form("Comparison: %s;Value;Entries", histName.Data()));
        h1->Draw("hist");       // 첫 번째는 그냥 Draw
        h2->Draw("hist same");  // 나머지는 same 옵션
        h3->Draw("hist same");

        c1->Update();
        
        // 6. 확인 및 대기
        printf("%d번 히스토그램 출력 중... (아무 키나 누르고 화면을 클릭하면 다음으로 넘어갑니다)\n", i);
        c1->WaitPrimitive(); // 사용자가 캔버스를 클릭하거나 키를 누를 때까지 대기
    }

    printf("모든 히스토그램 확인 완료.\n");
}
