R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_wcmd_qsum_qmax(){
  
  gStyle -> SetOptStat(0);
  //  TFile *f = new TFile("./prod_2nd/wcmd_000330.root");
  TCanvas *c1 = new TCanvas("c2", "qmax vs qsum", 1100, 1000);
  TH2D * qh = new TH2D("qh","",500,0,50000,500,0,50000);

 TString rawfile = "./prod_2nd/wcmd_000330.root";
 TChain * t = new TChain("value");
 t->Add(rawfile.Data());
 int tent= t->GetEntries();
 double qsum, qmax, tch, ich;
  for(int i=0; i<tent; i++){
    //  for(int i=0; i<10000; i++){
   t->GetEntry(i);
   tch=t->GetLeaf("ch")->GetNdata();

   for(int j=0; j<tch; j++){
     qsum=t->GetLeaf("qsum")->GetValue(j);
     qmax=t->GetLeaf("qmax")->GetValue(j);
     ich=t->GetLeaf("ch")->GetValue(j);
     /*
     if(ich==9||(qmax<(qsum*0.93-500))){qh->Fill(qsum,qmax);}
     else if(ich==10||(qmax<(qsum*0.94-1000))){qh->Fill(qsum,qmax);}
     else if(ich==11||(qmax<(qsum*0.94-500))){qh->Fill(qsum,qmax);}
     else if(ich==15||(qmax<(qsum*0.93-500))){qh->Fill(qsum,qmax);}
     else if(ich==16||(qmax<(qsum*0.94-1000))){qh->Fill(qsum,qmax);}
     else if(ich==17||(qmax<(qsum*0.93-1000))){qh->Fill(qsum,qmax);}     
     else if(ich==18||(qmax<(qsum*0.92-1000))){qh->Fill(qsum,qmax);}
     else if(ich==19||(qmax<(qsum*0.93-500))){qh->Fill(qsum,qmax);}
     else if(ich==22||(qmax<(qsum*0.93-800))){qh->Fill(qsum,qmax);}
     else if(ich==23||(qmax<(qsum*0.94-1000))){qh->Fill(qsum,qmax);}
     else if(ich==24||(qmax<(qsum*0.92-1100))){qh->Fill(qsum,qmax);}
     else if(ich==25||(qmax<(qsum*0.92-1100))){qh->Fill(qsum,qmax);}
     else if(ich==26||(qmax<(qsum*0.93-700))){qh->Fill(qsum,qmax);}     
     else if(ich==29||(qmax<(qsum*0.93-1000))){qh->Fill(qsum,qmax);}
     else if(ich==30||(qmax<(qsum*0.93-800))){qh->Fill(qsum,qmax);}
     else if(ich==31||(qmax<(qsum*0.93-500))){qh->Fill(qsum,qmax);}
     else if(ich==32||(qmax<(qsum*0.84-1200))){qh->Fill(qsum,qmax);}
     else {qh->Fill(qsum,qmax);}
  
   */
     if(qmax<(qsum*0.8-2100)){qh->Fill(qsum,qmax);}
     }  

 }


 c1->cd();
  qh->GetXaxis()->SetTitle("Qsum");
  qh->GetYaxis()->SetTitle("Qmax");

 //TH1D* h_sum = nullptr;

   //   h[i] = (TH1D*)f->Get(Form("D%dMChist",i));
   // gPad->SetLeftMargin(0.12);   // y축 여백 ← 핵심

 //ph->GetXaxis()->SetRangeUser(1000, 13000);
 // ph->GetYaxis()->SetRangeUser(1000, 15000);   


 qh->GetXaxis()->SetTitleFont(62);
 qh->GetYaxis()->SetTitleFont(62);


 qh->GetXaxis()->SetTitleSize(0.04);
 qh->GetYaxis()->SetTitleSize(0.04);
 // h[0]->SetLineColor(kRed);

 qh->Draw();


 }
