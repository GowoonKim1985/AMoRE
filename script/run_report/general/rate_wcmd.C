R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)

//#include <stdio.h>
//#include "/home/kkw/muon_3.1.2/MuonObjs/MuonObjs/WCPmt.hh"
//#include <TText.h>

void rate_wcmd()
{
  gStyle -> SetOptStat(0);

  // get current run number, subrun number
  /*
  FILE *runinfo;
  runinfo = fopen("t_rate.txt","r");    
  int run, subrun;
  fscanf(runinfo, "%d", &run);
  fscanf(runinfo, "%d", &subrun);  
  subrun = subrun-1;
  fclose(runinfo);
  */
  int run = 465;
  const int subrun = 100;


  // trg rate plot for all period (cumulative, interval)
  double nevt1[subrun], time1[subrun], time_min1[subrun], rate1[subrun], tent1[subrun];
  TChain *wchain[subrun]; 
  
  for(int isub=0; isub<subrun; isub++){
    wchain[isub] = new TChain("prd_wcd");
    wchain[isub] -> Add(Form("/data/amore2test/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,isub));
    tent1[isub]=wchain[isub] -> GetEntries();
    wchain[isub]->GetEntry(tent1[isub]-1);
    nevt1[isub] = wchain[isub]->GetLeaf("fEventNumber")->GetValue();
    time1[isub] = (wchain[isub]->GetLeaf("fTriggerTime")->GetValue())/1000000000.;
    rate1[isub] = nevt1[isub]/time1[isub];
    if(time1[isub]==0){time1[isub]=time1[isub-1]+3600; rate1[isub]=0;}//case @ file error, t time=0
    //    cout<<"tot evt num "<<t_evt[isub]<<endl;
    //    cout<<"fin trg time "<<t_time[isub]<<endl;
    //    cout<<"tot rate "<<t_rate[isub]<<endl;
    time_min1[isub]=time1[isub] / 60.;
  }

  TCanvas *c_rate1 = new TCanvas("c_rate1", "WCMD Rate vs Time", 1600, 400);
  c_rate1->SetGridy();           
  gStyle->SetGridStyle(3);  
  gStyle->SetGridWidth(1);
  gStyle->SetGridColor(kGray+1);
  c_rate1->Update();

  TGraph *gr = new TGraph(subrun, time_min1, rate1);
  gr->SetTitle("WCMD Rate vs Time;DAQ Time [min];Rate [Hz]");
  gr->SetMarkerStyle(20);
  gr->SetMarkerSize(0.7);
  gr->SetMarkerColor(kBlue);
  gr->SetLineColor(kBlue);
  gr->SetLineWidth(2);   
  gPad->SetGridy();      
  gr->Draw("APL"); 

  // trg rate plot for current 2hr onging.....(cumulative, interval)
  int csub, inum;
  if(subrun<2){csub=0;}
  else{csub=subrun-2;}
  double nevt2[6], time2[6], time_min2[6], irate2[6], crate2[6];

  for(int isub=csub; isub<subrun; isub++){
    int ient = tent1[isub]/3; 
    for(int idiv=0; idiv<3; idiv++){
      inum = 3*(isub-csub)+idiv;//0~6 numbering
      //      cout<<"sub "<<isub<<" / div "<<idiv<<"("<<inum<<")"<<endl;
      wchain[isub]->GetEntry(ient*(idiv+1)-1);
      //      cout<<"entry "<<ient*(idiv+1)-1<<"/"<<tent1[isub]<<endl;
      nevt2[inum] = wchain[isub]->GetLeaf("fEventNumber")->GetValue();
      time2[inum] = (wchain[isub]->GetLeaf("fTriggerTime")->GetValue())/1000000000.;
      //c rate
      crate2[inum] = nevt2[inum]/time2[inum];
      //i rate
      if(inum==0){irate2[inum]=(nevt2[inum]-nevt1[isub-1])/(time2[inum]-time1[isub-1]);}
      else{irate2[inum] = (nevt2[inum]-nevt2[inum-1])/(time2[inum]-time2[inum-1]);}
      if(time2[inum]==0){time2[inum]=time1[isub-1]+1200*(idiv+1); crate2[inum]=0; irate2[inum]=0;}//case @ file error, t time=0
      time_min2[inum]=time2[inum] / 60.;
      //      cout<<"inum "<<inum<<endl;
      //      cout<<"c rate"<<crate2[inum]<<endl;
      //      cout<<"i rate"<<irate2[inum]<<endl;      
       
    }
   
  }

  int dp = inum+1;
  TCanvas *c_rate2 = new TCanvas("c_rate2", "WCMD Rate vs Time (2hr)", 800, 400);
  c_rate2->SetGridy();           
  gStyle->SetGridStyle(3);  
  gStyle->SetGridWidth(1);
  gStyle->SetGridColor(kGray+1);
  c_rate2->Update();
  
  TGraph *g_cr = new TGraph(dp, time_min2, crate2);
  TGraph *g_ir = new TGraph(dp, time_min2, irate2);  

  // --- Y축 범위 계산 (둘 다 포함) ---
  double min_cr = TMath::MinElement(dp, crate2);
  double max_cr = TMath::MaxElement(dp, crate2);
  double min_ir = TMath::MinElement(dp, irate2);
  double max_ir = TMath::MaxElement(dp, irate2);
  
  double ymin = std::min(min_cr, min_ir);
  double ymax = std::max(max_cr, max_ir);
  double dy = ymax - ymin;

  if (dy <= 0) { dy = (ymax == 0 ? 1.0 : 0.1 * std::abs(ymax)); }
  ymin -= 0.05 * dy;   // 아래 여유
  ymax += 0.10 * dy;   // 위 여유
  cout<<"ymin " <<ymin<<endl;
  cout<<"ymax " <<ymax<<endl;  
  
  g_cr->SetMarkerStyle(20);
  g_cr->SetMarkerSize(0.7);
  g_cr->SetMarkerColor(kBlue);
  g_cr->SetLineColor(kBlue);
  g_cr->SetLineWidth(2);   

  g_ir->SetMarkerStyle(20);
  g_ir->SetMarkerSize(0.7);
  g_ir->SetMarkerColor(kRed);
  g_ir->SetLineColor(kRed);
  g_ir->SetLineWidth(2);
  g_ir->SetMinimum(ymin);
  g_ir->SetMaximum(ymax);
  
  g_ir->SetTitle("WCMD Rate vs Time (2hr);DAQ Time [min];Rate [Hz]");
  g_ir->Draw("APL"); 
  g_cr->Draw("PL SAME"); 
  c_rate2->Update();
  

}
