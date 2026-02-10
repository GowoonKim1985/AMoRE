#include <stdio.h>
#include "../MuonObjs/MuonObjs/WCPmt.hh"
#include <TText.h>

void report_wcd()
{
  gStyle -> SetOptStat(0);

  FILE *runinfo;
  runinfo = fopen("t_rate.txt","r");
      
  //  int run = 330;
  //  int subrun = 0;

  int run, subrun;
  fscanf(runinfo, "%d", &run);
  fscanf(runinfo, "%d", &subrun);  
  subrun = subrun-1;
  fclose(runinfo);

  //recent file check
  FILE *fin_file;
  TString fin_filename = Form("/home/kkw/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,subrun);
  fin_file = fopen(fin_filename, "r");
  if(fin_file ==NULL){
  fin_filename = Form("/home/kkw/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,subrun-1);
  }
  //  cout<<run<<endl;
  //  cout<<subrun<<endl;
  TChain *wchain = new TChain("prd_wcd");
  //  wchain -> Add(Form("/home/kkw/Muon/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,subrun));
  //  wchain -> Add(Form("/home/kkw/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,subrun));
  wchain -> Add(fin_filename);  

  WCEvent *wevt = new WCEvent();
  wchain -> SetBranchAddress("WCEvent", &wevt);

  int nent = wchain -> GetEntries();
  cout << nent << endl;
  /*
  wchain->GetEntry(0);
  double itime = wchain->GetLeaf(fTriggerTime)->GetValue();
  wchain->GetEntry(nent-1);
  double ftime = wchain->GetLeaf(fTriggerTime)->GetValue();
  */
 
  // Rate

  // tbit@ch
  TH1D *chbit = new TH1D("chbit", "Channel tbit", 48, 0, 48);
  TCanvas *can_chbit = new TCanvas("can_chbit", "", 800, 600);
  can_chbit->cd();
  wchain->Draw("fID>>chbit");
  chbit -> SetXTitle("PMT_Ch");
  chbit -> SetYTitle("Tbit");
  chbit -> SetTitle("WCMD Ch tbit");
  can_chbit -> Print("./figure/wcmd_chbit.png");

  
  //multiplicity
  TH1D *htbit = new TH1D("htbit", "", 48, 0, 48);
  htbit -> SetXTitle("Multiplicty");
  htbit -> SetYTitle("Number fo Entries");
  htbit ->SetTitle("Multiplicity");
  
  //hit map
  TH2D *hhitmap = new TH2D("hhitmap", "", 7, 0, 7, 7, 0, 7);
  hhitmap -> SetXTitle("Row of PMT map");
  hhitmap -> SetYTitle("Column of PMT map");
  hhitmap ->SetTitle("Hit Map");
  TH2D *hhitmap_cnt = new TH2D("hhitmap_cnt", "", 7, 0, 7, 7, 0, 7);
  hhitmap_cnt -> SetXTitle("Row of PMT map");
  hhitmap_cnt -> SetYTitle("Column of PMT map");
  hhitmap_cnt -> SetTitle("Hit Map (>20mul)");
  
  // ped@48ch
  TH2D * ped_time[48];
  TCanvas *can_ped_time = new TCanvas("can_ped_time", "can_ped_time", 500, 500);
  can_ped_time->SetGrid(0,1);
  //  can_ped_time->cd();

  int hitcheck=0;
  FILE *dummy, *target;
  char buffer[1024];
  size_t bytesRead;


  //get mean of rms (for get 6 sigma)

  int wtent = wchain->GetEntries();
  double pedrms[48] = {0};
  double pedmean[48] = {0};
  double ftrgtime[2];
  int fid[48];
  double ftcnt[48]={0};
  //  cout<<"wtent "<<wtent<<endl;
  for(int i=0; i<wtent; i++){
    wchain->GetEntry(i);
    //    cout<<i<<endl;
    int wnch = wchain->GetLeaf("fColl.fPedRMS")->GetNdata();
    for(int j=0; j<wnch; j++){
      fid[j]=wchain->GetLeaf("fColl.fID")->GetValue(j);
      ftcnt[fid[j]] = ftcnt[fid[j]]+1;
      //      pedrms[fid[j]]= pedrms[fid[j]] + (wchain->GetLeaf("fColl.fPedRMS")->GetValue(j));
      pedmean[fid[j]]=pedmean[fid[j]] + wchain->GetLeaf("fColl.fPedMean")->GetValue(j);      
    }
  }

  wchain->GetEntry(0);
  ftrgtime[0]=wchain->GetLeaf("fTriggerTime")->GetValue();

  wchain->GetEntry(wtent-1);
  ftrgtime[1]=wchain->GetLeaf("fTriggerTime")->GetValue();

  TH2D *pedhis[48];
  double ped_xrange[2], ped_yrange[2];
  
    //  for(int i=0; i<=48; i++){
  for(int i=0; i<48; i++){
    pedmean[i]= pedmean[i]/ftcnt[i];
    ped_xrange[0]=ftrgtime[0]/1000000000;
    ped_xrange[1]=ftrgtime[1]/1000000000;    
    ped_yrange[0]=pedmean[i]-2.5;
    ped_yrange[1]=pedmean[i]+2.5;    
    //    cout<<"y range "<<ped_yrange[0]<<" ~ "<<ped_yrange[1]<<endl;
    pedhis[i] = new TH2D(Form("ped_%i", i), Form("Ped Mean %i",i), 50,ped_xrange[0],ped_xrange[1],50,ped_yrange[0],ped_yrange[1]);    
      pedhis[i]->SetMarkerStyle(2);
      pedhis[i]->SetMarkerSize(1.5);
      pedhis[i]->SetXTitle("TrgTime[sec]");
    
    hitcheck =(wchain->GetEntries(Form("fID==%i",i)));
    if(hitcheck>0){
//      wchain->Draw(Form("fPedMean:fTriggerTime>>ped_time[%i]",i),Form("fID==%i",i+1));
//      wchain->Draw(Form("fPedMean:fTriggerTime>>ped_time[%i]",i),Form("fID==%i",i));
//      wchain->Draw(Form("fPedMean:(fTriggerTime/1000000000)>>pedhis[%i]",i),Form("fID==%i",i));
      wchain->Project(Form("ped_%i",i),"fPedMean:(fTriggerTime/1000000000)",Form("fID==%i",i));
      can_ped_time->cd();
      pedhis[i]->Draw();
      can_ped_time -> Print(Form("./figure/wcmd_ped_time_%i.png",i+1));
    }
    else{
      dummy = fopen("./figure/empty.png", "rb");
      target = fopen(Form("./figure/wcmd_ped_time_%i.png",i+1),"wb");
      while ((bytesRead = fread(buffer, 1, sizeof(buffer), dummy)) > 0) {
        fwrite(buffer, 1, bytesRead, target);
      }
      fclose(dummy);
      fclose(target);
    }
  }

 
  //qtot
  TH1D *hqtot[48];
  TH1D *hqtot_sel[48];
  for(int i = 0; i < 48; i++){
    hqtot[i] = new TH1D(Form("hqtot_%d", i), "", 200, 0, 40000);
    hqtot[i] -> SetXTitle(Form("Fmax of PMT %2d", i+1));
    //hqtot[i] -> SetXTitle(Form("Qtot of PMT %2d", i+1));
    hqtot[i] -> SetYTitle("Number of Entries");
    /*
    hqtot[i] -> GetXaxis() -> SetTitleSize(0.09);
    hqtot[i] -> GetXaxis() -> SetTitleOffset(0.5);
    hqtot[i] -> GetXaxis() -> SetLabelSize(0.07);
    hqtot[i] -> GetYaxis() -> SetTitleSize(0.09);
    hqtot[i] -> GetYaxis() -> SetTitleOffset(0.5);
    hqtot[i] -> GetYaxis() -> SetLabelSize(0.07);
    */
    hqtot_sel[i] = new TH1D(Form("hqtot_sel_%d", i), "", 200, 0, 40000);
    //hqtot_sel[i] -> SetXTitle(Form("Fmax of PMT %2d", i+1));
    hqtot[i] -> SetXTitle(Form("Qtot of PMT %2d", i+1));
    hqtot_sel[i] -> SetYTitle("Number of Entries");
  }

  
  TH1D *hqsum = new TH1D("hqsum", "", 500, 0, 100000);
  hqsum -> SetXTitle("Qsum");
  hqsum -> SetYTitle("Number of Entries");
  hqsum ->SetTitle("Qsum hist");
  TH1D *hqsum_sel = new TH1D("hqsum_sel", "", 500, 0, 100000);
  hqsum_sel -> SetXTitle("Qsum");
  hqsum_sel -> SetYTitle("Number of Entries");
  hqsum_sel -> SetTitle("Qsum hist (w/ cut)");

    /*
    TH2D *htbQ = new TH2D("htbQ", "", 48, 0, 48, 500, 0, 100000);
  htbQ -> SetXTitle("Multiplicity");
  htbQ -> SetYTitle("Qsum");
    */
  
  double qtot[48] = {0};
  double a[48] = {0};
  double ped[48] = {0};
  double rms[48] = {0};
  double fmax[48] = {0};
  int id[48];
  int nn[48] = {0};
  int nn_over[48] = {0};
  int nn_max[48] = {0};
  int tbit[48], tbitsum;
  double qsum, qsum_sel;
  
  /*
  wchain->GetEntry(0);
  int itime = Wchain->GetLeaf(fTriggerTime)->GetValue();
  wchain->GetEntry(nevt-1);
  int ftime = Wchain->GetLeaf(fTriggerTime)->GetValue();
  */
  
  for(int i = 0; i < nent; i++){
    wchain -> GetEntry(i);
    if(i != 0 && i%2000000 == 0) cout << i << endl;

    qsum = 0;
    qsum_sel = 0;
    tbitsum = 0;

    int nch = wevt -> GetN();
    //cout << nch << endl;
    for(int j = 0; j < nch; j++){
      WCPmt *pmt = wevt -> Get(j);
      id[j] = pmt -> GetID();
      
      qtot[id[j]] = pmt -> GetQtot();
      fmax[id[j]] = pmt -> GetFmax();
      ped[id[j]] = pmt -> GetPedMean();
      rms[id[j]] = pmt -> GetPedRMS();

      if(rms[id[j]] > 10) continue;

      //tbitsum++;
      nn[id[j]]++;
      qsum += qtot[id[j]];
      hqtot[id[j]] -> Fill(qtot[id[j]]);

      //cout << j << " " << id[j] << " " << qtot[id[j]] << endl;
      if(nch > 20){
	hhitmap_cnt -> Fill(7-id[j]/7-1, 7-id[j]%7-1);
	hqtot_sel[id[j]] -> Fill(qtot[id[j]]);

	qsum_sel += qtot[id[j]];
	a[id[j]] += qtot[id[j]];
	nn[id[j]]++;
      }
    }
    
    hqsum -> Fill(qsum);
    if(nch > 20 || qsum > 6000) hqsum_sel -> Fill(qsum_sel);
    htbit -> Fill(nch);
    //    htbQ -> Fill(nch, qsum);
  }

  for(int i = 0; i < 47; i++){
    hhitmap -> SetBinContent(7-i/7, 7-i%7, a[i]/nn[i]);  
  }

  TCanvas *can_mul = new TCanvas("can_mul", "multiplicity", 800, 600);
  can_mul -> cd();
  htbit -> Draw();
  can_mul ->SetLogy();
  can_mul -> Print("./figure/wcmd_mul.png");

  
  /*
  TCanvas *can_tbit = new TCanvas("can_tbit", "", 1600, 600);
  can_tbit -> Divide(2, 1);
  can_tbit -> cd(1);
  TPad *pad_tb = (TPad*)can_tbit -> cd(1);
  htbit -> Draw();
  pad_tb -> SetLogy();
  can_tbit -> cd(2);
  htbQ -> Draw();
  //can_tbit -> Print("./figure/20230919/WCMD_tbit.png");
  */
  

  TCanvas *can_hmap1 = new TCanvas("can_hmap1", "All", 600, 600);
  can_hmap1 -> cd();
  hhitmap -> Draw("colz");

  TText t;
  t.SetTextSize(0.02);
  t.SetTextAlign(22);

  for(int i = 0; i < 47; i++){
    double x = hhitmap->GetXaxis()->GetBinCenter(7-i/7);
    double y = hhitmap->GetYaxis()->GetBinCenter(7-i%7);    
    t.DrawText(x,y,Form("%d",i)); 
  }
  can_hmap1->Update();

  can_hmap1 -> Print("./figure/wcmd_hmap1.png");

  TCanvas *can_hmap2 = new TCanvas("can_hmap2", "All", 600, 600);
  can_hmap2 -> cd();
  hhitmap_cnt -> Draw("colz");

  for(int i = 0; i < 47; i++){
    double x = hhitmap_cnt->GetXaxis()->GetBinCenter(7-i/7);
    double y = hhitmap_cnt->GetYaxis()->GetBinCenter(7-i%7);    
    t.DrawText(x,y,Form("%d",i)); 
  }
  can_hmap2->Update();


  can_hmap2 -> Print("./figure/wcmd_hmap2.png");


  /*  
  TCanvas *can_hmap = new TCanvas("can_hmap", "All", 1200, 600);
  can_hmap -> Divide(2, 1);
  can_hmap -> cd(1);
  hhitmap -> Draw("colz");
  can_hmap -> cd(2);
  hhitmap_cnt -> Draw("colz");
  //can_hmap -> Print("./figure/20230919/WCMD_hmap.png");
  */
  
  TCanvas *can_sum = new TCanvas("can_sum", "", 800, 600);
  hqsum ->GetXaxis()->SetRangeUser(0,70000);
  hqsum -> Draw();
  hqsum_sel -> SetLineColor(kRed);
  hqsum_sel -> Draw("same");
  can_sum -> SetLogy();
  //  can_sum -> Print("./figure/20230919/WCMD_qsum.png");
  can_sum -> Print("./figure/wcmd_qsum.png");

  // qtot @48ch
  /*
  TCanvas *can_qtot = new TCanvas("can_qtot", "can_qtot", 800, 600);
  can_qtot->cd();
    
  for(int i=0; i<48; i++){
    hqtot[i]->Draw();
    hqtot_sel[i] -> SetLineColor(kRed);
    hqtot_sel[i] -> Draw("same");
    can_qtot -> SetLogy();
    can_qtot -> Print(Form("./figure/wcmd_qtot_%i.png",i+1));

  }
  */
  
  TCanvas *can_qtot = new TCanvas("can_qtot", "", 1600, 1600);
  can_qtot -> Divide(7, 7);
  TPad *pad_qtot[48];
  for(int i = 0; i < 47; i++){
    can_qtot -> cd(i+1);
    pad_qtot[i] = (TPad*)can_qtot -> cd(i+1);
    
    hqtot[i] -> Draw();
    hqtot_sel[i] -> SetLineColor(kRed);
    hqtot_sel[i] -> Draw("same");
    pad_qtot[i] -> SetLogy();
  }
  can_qtot->Update();
      can_qtot -> Print("./figure/wcmd_qtot.png");
  //can_qtot -> Print("./figure/20230919/WCMD_qtot.png");
  /*
  TFile of("./hist/wcmdhist_000272.root", "recreate");
  hqsum->Write();
  for(int i = 0; i < 48; i++){
    hqtot[i]->Write();
  //   hqsum[i] -> Write();
  //   hUDR[i] -> Write();
  //   hUDR_sel[i] -> Write();
   }
   of.Close();

*/
  
}
