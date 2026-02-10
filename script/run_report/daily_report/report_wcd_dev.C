#include <stdio.h>
#include "../MuonObjs/MuonObjs/WCPmt.hh"

void report_wcd_dev()
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
  //  cout << nent << endl;

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
  can_chbit -> Print("./test/wcmd_chbit.png");
  
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

  
  //qtot + rms mean get
  TH1D *hqtot[48];
  TH1D *hqtot_sel[48];

  for(int i = 0; i < 48; i++){
    hqtot[i] = new TH1D(Form("hqtot_%d", i), "", 200, 0, 40000);
    hqtot[i] -> SetXTitle(Form("Fmax of PMT %2d", i+1));
    //hqtot[i] -> SetXTitle(Form("Qtot of PMT %2d", i+1));
    hqtot[i] -> SetYTitle("Number of Entries");
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
  double rmssum[48]={0}; double pedsum[48]={0}; double nped[48]={8};
  double wtime[2];

  //get trg time range
  wchain ->GetEntry(0);
  wtime[0]=wchain->GetLeaf("fTriggerTime")->GetValue();
  wchain ->GetEntry(nent-1);
  wtime[1]=wchain->GetLeaf("fTriggerTime")->GetValue();
  cout<<"wtime start "<<wtime[0]/1000000000.<<endl;
  cout<<"wtime end "<<wtime[0]/1000000000.<<endl;  
  

  //  for(int i = 0; i < 10; i++){
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
      //      cout<<"rms "<<rms[id[j]]<<endl;
      rmssum[id[j]] = rmssum[id[j]]+rms[id[j]];
      //      cout<<"rmssum "<<rmssum[id[j]]<<endl;
      pedsum[id[j]] = pedsum[id[j]]+ped[id[j]];
      nped[id[j]]=nped[id[j]]+1;
      
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

  
  // ped@48ch

  TH2D * ped_time[48];
  TCanvas *can_ped_time = new TCanvas("can_ped_time", "can_ped_time", 400, 400);
  can_ped_time->cd();

  int hitcheck=0;
  FILE *dummy, *target;
  char buffer[1024];
  size_t bytesRead;

  
  //hist ped vs time
  double xrange[2], yrange[2];
  xrange[0]=wtime[0]/(60.*1000000000.); //min
  xrange[1]=wtime[1]/(60.*1000000000.);
  
  for(int i=0; i<=48; i++){
    cout<<"mean pedmean "<<pedsum[i]/nped[i]<<endl;
    cout<<"mean pedrms "<<rmssum[i]/nped[i]<<endl;    
    yrange[0]=pedsum[i]/nped[i] -(3*(rmssum[i]/nped[i]));
    yrange[1]=pedsum[i]/nped[i] + (3*(rmssum[i]/nped[i]));
    
    //    hitcheck =(wchain->GetEntries(Form("fID==%i",i+1)));
    hitcheck =(wchain->GetEntries(Form("fID==%i",i)));    
    if(hitcheck>0){
      wchain->Draw(Form("fPedMean:fTriggerTime>>ped_time[%i]("title",50,wtime[0],wtime[1],50,yrange[0],yrange[1])",i),Form("fID==%i",i));
      can_ped_time -> Print(Form("./test/wcmd_ped_time_%i.png",i+1));
    }
    else{
      dummy = fopen("./figure/empty.png", "rb");
      target = fopen(Form("./test/wcmd_ped_time_%i.png",i+1),"wb");
      while ((bytesRead = fread(buffer, 1, sizeof(buffer), dummy)) > 0) {
        fwrite(buffer, 1, bytesRead, target);
      }
      fclose(dummy);
      fclose(target);
    }
  }



  //multiplicity + hitmap

  for(int i = 0; i < 47; i++){
    hhitmap -> SetBinContent(7-i/7, 7-i%7, a[i]/nn[i]);  
  }

  TCanvas *can_mul = new TCanvas("can_mul", "multiplicity", 800, 600);
  can_mul -> cd();
  htbit -> Draw();
  can_mul ->SetLogy();
  can_mul -> Print("./test/wcmd_mul.png");

  TCanvas *can_hmap1 = new TCanvas("can_hmap1", "All", 600, 600);
  can_hmap1 -> cd();
  hhitmap -> Draw("colz");
  can_hmap1 -> Print("./test/wcmd_hmap1.png");

  TCanvas *can_hmap2 = new TCanvas("can_hmap2", "All", 600, 600);
  can_hmap2 -> cd();
  hhitmap_cnt -> Draw("colz");
  can_hmap2 -> Print("./test/wcmd_hmap2.png");

  
  TCanvas *can_sum = new TCanvas("can_sum", "", 800, 600);
  hqsum ->GetXaxis()->SetRangeUser(0,70000);
  hqsum -> Draw();
  hqsum_sel -> SetLineColor(kRed);
  hqsum_sel -> Draw("same");
  can_sum -> SetLogy();
  //  can_sum -> Print("./figure/20230919/WCMD_qsum.png");
  can_sum -> Print("./test/wcmd_qsum.png");


  
  // qtot @48ch
  
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
      can_qtot -> Print("./test/wcmd_qtot.png");
  
}
