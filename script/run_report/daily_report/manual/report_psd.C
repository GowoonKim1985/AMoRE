#include <stdio.h>
//#include "../MuonObjs/MuonObjs/WCPmt.hh"
#include <TText.h>

void report_psd()
{
  gStyle -> SetOptStat(0);

  FILE *runinfo;
  runinfo = fopen("t_rate.txt","r");
      
//    int run = 483;
//    int subrun = 4;

  int run, subrun;
  fscanf(runinfo, "%d", &run);
  fscanf(runinfo, "%d", &subrun);  
  subrun = subrun-1;
  fclose(runinfo);

  //recent file check
  FILE *fin_file1;
  FILE *fin_file2;
  TString fin_filename1 = Form("/data/amore2test/PROD/PSMD/%06d/prd_psd_daq1_%06d_%05d.root",run,run,subrun);
  TString fin_filename2 = Form("/data/amore2test/PROD/PSMD/%06d/prd_psd_daq2_%06d_%05d.root",run,run,subrun);
  fin_file1 = fopen(fin_filename1, "r");
  fin_file2 = fopen(fin_filename2, "r");  

  if(fin_file1 ==NULL){
  fin_filename1 = Form("/data/amore2test/PROD/PSMD/%06d/prd_psd_daq1_%06d_%05d.root",run,run,subrun-1);
  fin_filename2 = Form("/data/amore2test/PROD/PSMD/%06d/prd_psd_daq2_%06d_%05d.root",run,run,subrun-1);  
  }
  //  cout<<run<<endl;
  //  cout<<subrun<<endl;
  TChain *pchain = new TChain("prd_psd");
  //  wchain -> Add(Form("/home/kkw/Muon/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,subrun));
  //  wchain -> Add(Form("/home/kkw/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,subrun));
  pchain -> Add(fin_filename1);
  pchain -> Add(fin_filename2);    
  /*
  WCEvent *wevt = new WCEvent();
  wchain -> SetBranchAddress("WCEvent", &wevt);
  */
  
  int nent = pchain -> GetEntries();
  //  cout << nent << endl;
  
  //hit map
  int hitcnt[130];
  int maxcnt=0;
  TH2D *hitmap[5];

  hitmap[0]= new TH2D(Form("hitmap_1"),"Side1(Door/E)",2,0,2,12,0,12);
  hitmap[1]= new TH2D(Form("hitmap_2"),"Side2(N)",2,0,2,12,0,12);
  hitmap[2]= new TH2D(Form("hitmap_3"),"Side3(Wall/W)",2,0,2,12,0,12);
  hitmap[3]= new TH2D(Form("hitmap_4"),"Side4(S)",2,0,2,12,0,12);    

  hitmap[4] = new TH2D("hitmap_5","Bottom",11,0,11,2,0,2);

  
  
  char text[458];
  
  for(int i=0; i<124; i++){
    sprintf(text,"fColl.fID==%i",i);
    hitcnt[i] = pchain->GetEntries(text);
    //    cout<<"id "<< i<<"cnt : "<<hitcnt[i]<<endl;
    if(maxcnt<hitcnt[i]){maxcnt=hitcnt[i];}
    }
  //    cout<<"max cnt : "<<maxcnt<<endl;
  /*
    for(int i=0; i<124; i++){
    if(i>=0&&i<=11){hitmap[0]->SetBinContent(1, i+1, hitcnt[i]);}
    if(i>=12&&i<=23){hitmap[0]->SetBinContent(2, i-11, hitcnt[i]);}
    if(i>=24&&i<=35){hitmap[1]->SetBinContent(1, i-23, hitcnt[i]);}
    if(i>=36&&i<=47){hitmap[1]->SetBinContent(2, i-35, hitcnt[i]);}
    if(i>=65&&i<=76){hitmap[2]->SetBinContent(1, i-64, hitcnt[i]);}
    if(i>=77&&i<=88){hitmap[2]->SetBinContent(2, i-76, hitcnt[i]);}            
    if(i>=89&&i<=100){hitmap[3]->SetBinContent(1, i-88, hitcnt[i]);}
    if(i>=101&&i<=112){hitmap[3]->SetBinContent(2, i-100, hitcnt[i]);}            

    if(i>=48&&i<=58){hitmap[4]->SetBinContent(i-47, 2, hitcnt[i]);}
    if(i>=113&&i<=123){hitmap[4]->SetBinContent(i-112, 1, hitcnt[i]);}            
   
    }
  */
    for(int i=0; i<124; i++){
    if(i>=0&&i<=11){hitmap[0]->SetBinContent(2, i+1, hitcnt[i]);}
    if(i>=12&&i<=23){hitmap[1]->SetBinContent(1, i-11, hitcnt[i]);}
    if(i>=24&&i<=35){hitmap[1]->SetBinContent(2, i-23, hitcnt[i]);}
    if(i>=36&&i<=47){hitmap[2]->SetBinContent(1, i-35, hitcnt[i]);}
    if(i>=65&&i<=76){hitmap[2]->SetBinContent(2, i-64, hitcnt[i]);}
    if(i>=77&&i<=88){hitmap[3]->SetBinContent(1, i-76, hitcnt[i]);}            
    if(i>=89&&i<=100){hitmap[3]->SetBinContent(2, i-88, hitcnt[i]);}
    if(i>=101&&i<=112){hitmap[0]->SetBinContent(1, i-100, hitcnt[i]);}            

    if(i>=48&&i<=58){hitmap[4]->SetBinContent(i-47, 2, hitcnt[i]);}
    if(i>=113&&i<=123){hitmap[4]->SetBinContent(i-112, 1, hitcnt[i]);}            
   
    }

  
  double ymax=(double)maxcnt;
  //  cout<<"test "<<ymax<<endl;
    
  for(int i=0;i<4;i++){
  hitmap[i]->GetXaxis()->SetNdivisions(2,0,0);
  hitmap[i]->GetYaxis()->SetNdivisions(12,0,0);
  hitmap[i]->GetZaxis()->SetLabelSize(0.05);
  hitmap[i]->SetMinimum(0);
  hitmap[i]->SetMaximum(ymax);

  }
  hitmap[4]->GetXaxis()->SetNdivisions(11,0,0);
  hitmap[4]->GetYaxis()->SetNdivisions(2,0,0);
  hitmap[4]->SetMinimum(0);
  hitmap[4]->SetMaximum(maxcnt*1.2);


    
  TCanvas *hcan = new TCanvas("hcan", "Canvas", 1000, 1000);

  TPad *padTop = new TPad("padTop", "Top Pad", 0.0, 0.25, 1.0, 1.0);//top 75% area
  TPad *padBot = new TPad("padBot", "Bottom Pad", 0.0, 0.0, 1.0, 0.25); //bot 25% area

  gStyle->SetTitleSize(0.08, "t");
  TText t1, t2;
 
  padTop->Draw();
  padBot->Draw();

  padTop->Divide(4,1);

  padTop->cd(1); gPad->SetRightMargin(0.25); hitmap[0]->Draw("COLZ");
  for(int i=0; i<12; i++){
    double y = hitmap[0]->GetYaxis()->GetBinCenter(i+1);
    t1.DrawText(0.5, y, Form("%d",i+101));//101~112
    t2.DrawText(1.5, y, Form("%d",i));//0~11
  }
  
  padTop->cd(2); gPad->SetRightMargin(0.25); hitmap[1]->Draw("COLZ");
  for(int i=0; i<12; i++){
    double y = hitmap[1]->GetYaxis()->GetBinCenter(i+1);
    t1.DrawText(0.5, y, Form("%d",i+12));
    t2.DrawText(1.5, y, Form("%d",i+24));
  }
  
  padTop->cd(3); gPad->SetRightMargin(0.25); hitmap[2]->Draw("COLZ");
  for(int i=0; i<12; i++){
    double y = hitmap[2]->GetYaxis()->GetBinCenter(i+1);
    t1.DrawText(0.5, y, Form("%d",i+36));
    t2.DrawText(1.5, y, Form("%d",i+65));
  }

  padTop->cd(4); gPad->SetRightMargin(0.25); hitmap[3]->Draw("COLZ");
  for(int i=0; i<12; i++){
    double y = hitmap[3]->GetYaxis()->GetBinCenter(i+1);
    t1.DrawText(0.5, y, Form("%d",i+77));
    t2.DrawText(1.5, y, Form("%d",i+89));
  }

  
  padBot->cd();
  hitmap[4]->Draw("COLZ");
  for(int i=0; i<11; i++){
    double x = hitmap[4]->GetYaxis()->GetBinCenter(i+1);
    t1.DrawText(x, 0.5, Form("%d",i+113));
    t2.DrawText(x, 1.5, Form("%d",i+48));
  }


  
  //fid numbering

    



  // 전체 캔버스로 돌아가 타이틀을 추가
hcan->cd(); // 캔버스로 돌아가기
hcan->SetTopMargin(0.08);  // 캔버스 상단 여백을 줄임
/*
// TPaveText로 타이틀 추가
TPaveText *pt = new TPaveText(0.1, 0.95, 0.9, 0.99, "NDC");  // NDC 좌표에서 상단 중앙에 텍스트 박스 생성
pt->AddText("PSMD Hitmap");  // 타이틀 텍스트 추가
pt->SetTextAlign(22);  // 중앙 정렬
pt->SetTextSize(0.04);  // 텍스트 크기 설정
pt->SetFillColor(0);  // 배경을 투명하게 설정
 pt->SetBorderSize(0);
 pt->SetShadowColor(0); 
 pt->Draw();  // 텍스트 그리기
  */


TLatex *lt = new TLatex();
lt->SetNDC();                // NDC 좌표 사용
lt->SetTextAlign(22);        // 가운데 정렬 (h=2, v=2)
lt->SetTextSize(0.04);       // 텍스트 크기
lt->DrawLatex(0.5, 0.96, "PSMD Hitmap");  // (x,y,text)


hcan->Update();

 hcan->Print(Form("./figure/psmd_hmap.png"));
 
}
