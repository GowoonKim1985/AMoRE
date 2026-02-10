void plot_wcd()
{
  int runnum = 339;
  //int runnum = 315;

  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_wcd");
  int ntime = 0;
  for(int i = 0; i < 1; i++){
    ntime++;
    chain -> Add(Form("/data/kmseo/WCMD/%06d/prd_wcd_%06d_%05d.root", runnum, runnum, i));
  }
  //chain -> Add("/data/kmseo/WCMD/000287/prd_wcd_000287_*.root");

  WCEvent *wevt = new WCEvent();
  chain -> SetBranchAddress("WCEvent", &wevt);

  TF1 *funcu = new TF1("funcu", "pol3", 5, 48);
  funcu -> SetParameter(0, 248.43);
  funcu -> SetParameter(1, 744.18);
  funcu -> SetParameter(2, 163.77);
  funcu -> SetParameter(3, -2.312);
  TF1 *funcl = new TF1("funcl", "pol3", 5, 48);
  funcl -> SetParameter(0, -607);
  funcl -> SetParameter(1, 226.34);
  funcl -> SetParameter(2, 30.98);
  funcl -> SetParameter(3, -0.033);
  double *pu = funcu -> GetParameters();
  double *pl = funcl -> GetParameters();
  //cout << pu[0] << endl;
  
  TH1D *hqsum = new TH1D("hqsum", "", 500, 0, 100000);
  hqsum -> SetXTitle("Qsum");
  hqsum -> SetYTitle("Number of Entries");
  TH1D *hqsum_sel = new TH1D("hqsum_sel", "", 500, 0, 100000);
  hqsum_sel -> SetXTitle("Qsum");
  hqsum_sel -> SetYTitle("Number of Entries");

  TH1D *hqtot[48];
  TH1D *hqtot_sel[48];
  TH1D *hfmax[48];
  TH1D *hfmax_sel[48];
  TH2D *hqtp[48];
  TH2D *hqtr[48];
  for(int i = 0; i < 48; i++){
    hqtot[i] = new TH1D(Form("hqtot_%d", i), "", 200, 0, 40000);
    hqtot[i] -> SetXTitle(Form("Qtot of PMT %2d", i+1));
    hqtot[i] -> SetYTitle("Number of Entries");

    hqtot[i] -> GetXaxis() -> SetTitleSize(0.09);
    hqtot[i] -> GetXaxis() -> SetTitleOffset(0.5);
    hqtot[i] -> GetXaxis() -> SetLabelSize(0.07);
    hqtot[i] -> GetYaxis() -> SetTitleSize(0.09);
    hqtot[i] -> GetYaxis() -> SetTitleOffset(0.5);
    hqtot[i] -> GetYaxis() -> SetLabelSize(0.07);

    hqtot_sel[i] = new TH1D(Form("hqtot_sel_%d", i), "", 200, 0, 40000);
    hqtot[i] -> SetXTitle(Form("Qtot of PMT %2d", i+1));
    hqtot_sel[i] -> SetYTitle("Number of Entries");

    hfmax[i] = new TH1D(Form("hfmax_%d", i), "", 400, 0, 4000);
    hfmax[i] -> SetXTitle(Form("Fmax of PMT %2d", i+1));
    hfmax[i] -> SetYTitle("Number of Entries");

    hfmax[i] -> GetXaxis() -> SetTitleSize(0.09);
    hfmax[i] -> GetXaxis() -> SetTitleOffset(0.5);
    hfmax[i] -> GetXaxis() -> SetLabelSize(0.07);
    hfmax[i] -> GetYaxis() -> SetTitleSize(0.09);
    hfmax[i] -> GetYaxis() -> SetTitleOffset(0.5);
    hfmax[i] -> GetYaxis() -> SetLabelSize(0.07);

    hfmax_sel[i] = new TH1D(Form("hfmax_sel_%d", i), "", 400, 0, 4000);
    hfmax[i] -> SetXTitle(Form("Fmax of PMT %2d", i+1));
    hfmax_sel[i] -> SetYTitle("Number of Entries");

    hqtp[i] = new TH2D(Form("hqtp_%d", i), "", 200, 0, 40000, 200, 80, 220);
    hqtp[i] -> SetXTitle("Qtot");
    hqtp[i] -> SetYTitle("Ped");
    hqtp[i] -> SetMarkerStyle(8);
    hqtp[i] -> SetMarkerSize(0.5);
    hqtp[i] -> GetXaxis() -> SetLabelSize(0.07);
    hqtp[i] -> GetYaxis() -> SetLabelSize(0.07);
    
    hqtr[i] = new TH2D(Form("hqtr_%d", i), "", 200, 0, 40000, 100, 0, 10);
    hqtr[i] -> SetXTitle("Qtot");
    hqtr[i] -> SetYTitle("Ped");
    hqtr[i] -> SetMarkerStyle(8);
    hqtr[i] -> SetMarkerSize(0.5);
    hqtr[i] -> GetXaxis() -> SetLabelSize(0.07);
    hqtr[i] -> GetYaxis() -> SetLabelSize(0.07);
  }
  TH1D *htbit = new TH1D("htbit", "", 48, 0, 48);
  htbit -> SetXTitle("Multiplicity");
  htbit -> SetYTitle("Number fo Entries");
  TH2D *htbQ = new TH2D("htbQ", "", 48, 0, 48, 500, 0, 100000);
  htbQ -> SetXTitle("Multiplicity");
  htbQ -> SetYTitle("Qsum");
  TH2D *htbQ2 = new TH2D("htbQ2", "", 48, 0, 48, 500, 0, 100000);
  htbQ2 -> SetXTitle("Multiplicity");
  htbQ2 -> SetYTitle("Qsum");
  TH2D *hhitmap = new TH2D("hhitmap", "", 7, 0, 7, 7, 0, 7);
  hhitmap -> SetXTitle("Row of PMT map");
  hhitmap -> SetYTitle("Column of PMT map");
  TH2D *hhitmap_cnt = new TH2D("hhitmap_cnt", "", 7, 0, 7, 7, 0, 7);
  hhitmap_cnt -> SetXTitle("Row of PMT map");
  hhitmap_cnt -> SetYTitle("Column of PMT map");

  double qtot[48] = {0};
  double a[48] = {0};
  double ped[48] = {0};
  double rms[48] = {0};
  double fmax[48] = {0};
  int id[48];
  int nn[48] = {0};
  int nn_over[48] = {0};
  int nn_max[48] = {0};
  int abitsum;
  double qsum, qsum_sel;
  
  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);
    if(i != 0 && i%2000000 == 0) cout << i << endl;

    qsum = 0;
    qsum_sel = 0;
    abitsum = 0;
    
    int nch = wevt -> GetN();
    //cout << nch << endl;
    for(int j = 0; j < nch; j++){
      WCPmt *pmt = wevt -> Get(j);
      id[j] = pmt -> GetID();

      nn[id[j]] = 0;
      // if(id[j] == 29 || id[j] == 30 ||
      // 	 id[j] == 35 || id[j] == 36 || id[j] == 37 || id[j] == 38 ||
      // 	 id[j] == 42 || id[j] == 43 || id[j] == 44) continue;

      fmax[id[j]] = pmt -> GetFmax();
      rms[id[j]] = pmt -> GetPedRMS();
      if(fmax[id[j]] < 50) continue;
      if(rms[id[j]] > 3) continue;
      
      qtot[id[j]] = pmt -> GetQtot();
      fmax[id[j]] = pmt -> GetFmax();
      ped[id[j]] = pmt -> GetPedMean();

      abitsum++;
      nn[id[j]] = 1;
      qsum += qtot[id[j]];
      hqtot[id[j]] -> Fill(qtot[id[j]]);
      hfmax[id[j]] -> Fill(fmax[id[j]]);
      
      hqtp[id[j]] -> Fill(qtot[id[j]], ped[id[j]]);
      hqtr[id[j]] -> Fill(qtot[id[j]], rms[id[j]]);
    }
    hqsum -> Fill(qsum);

    htbit -> Fill(abitsum);
    htbQ -> Fill(abitsum, qsum);

    if(abitsum > 5){
      for(int k = 0; k < 48; k++){
	if(nn[k] != 1) continue;
	
	hhitmap_cnt -> Fill(7-k/7-1, 7-k%7-1);
	hqtot_sel[k] -> Fill(qtot[k]);
	hfmax_sel[k] -> Fill(fmax[k]);
	
	qsum_sel += qtot[k];
	a[k] += qtot[k];
	nn_over[k]++;
      }

      double ul = 0;
      double ll = 0;
      for(int l = 0; l < 4; l++){
	ul += TMath::Power(abitsum, l)*pu[l];
	ll += TMath::Power(abitsum, l)*pl[l];
      }
      
      if(qsum_sel > ul || qsum_sel < ll) continue;
      hqsum_sel -> Fill(qsum_sel);
    }
  }

  double rtime = ntime*3600;
  for(int i = 0; i < 47; i++)
    hhitmap -> SetBinContent(7-i/7, 7-i%7, a[i]/nn_over[i]);  
  cout << hqsum_sel -> Integral()/(600*600)/rtime << endl;

  TCanvas *can_tbit = new TCanvas("can_tbit", "", 1600, 600);
  can_tbit -> Divide(2, 1);
  can_tbit -> cd(1);
  TPad *pad_tb = (TPad*)can_tbit -> cd(1);
  //htbit -> Scale(1./rtime);
  
  htbit -> Draw("hist");
  pad_tb -> SetLogy();
  can_tbit -> cd(2);
  htbQ -> SetMarkerStyle(8);
  htbQ -> SetMarkerSize(0.5);
  htbQ -> Draw();
  funcu -> Draw("same");
  funcl -> Draw("same");
  //can_tbit -> Print("./figure/20231215/WCMD_tbit_000303.png");
  
  TCanvas *can_hmap = new TCanvas("can_hmap", "All", 600, 600);
  hhitmap_cnt -> Draw("colz");
  //can_hmap -> Print(Form("./figure/20240216/WCMD_hmap_%06d.png", runnum));

  TCanvas *can_sum = new TCanvas("can_sum", "", 800, 600);
  hqsum -> SetLineColor(kBlack);
  hqsum -> Draw();
  hqsum_sel -> SetLineColor(kBlue);
  hqsum_sel -> Draw("same");
  can_sum -> SetLogy();
  //can_sum -> Print(Form("./figure/20240216/WCMD_qsum_%06d.png", runnum));

  TCanvas *can_qtot = new TCanvas("can_qtot", "", 1600, 800);
  can_qtot -> Divide(7, 7);
  TPad *pad_qtot[48];
  for(int i = 0; i < 47; i++){
    can_qtot -> cd(i+1);
    pad_qtot[i] = (TPad*)can_qtot -> cd(i+1);

    // hqtot[i] -> Scale(1./rtime);
    // hqtot_sel[i] -> Scale(1./rtime);

    hqtot[i] -> Draw("hist");
    hqtot_sel[i] -> SetLineColor(kRed);
    hqtot_sel[i] -> Draw("hist, same");
    pad_qtot[i] -> SetLogy();
  }
  //can_qtot -> Print(Form("./figure/20240216/WCMD_qtot_%06d.png", runnum));

  // TCanvas *can_fmax = new TCanvas("can_fmax", "", 1600, 800);
  // can_fmax -> Divide(7, 7);
  // TPad *pad_fmax[48];
  // for(int i = 0; i < 47; i++){
  //   can_fmax -> cd(i+1);
  //   pad_fmax[i] = (TPad*)can_fmax -> cd(i+1);

  //   hfmax[i] -> Scale(1./rtime);
  //   hfmax[i] -> GetXaxis() -> SetRangeUser(0, 1000);
  //   hfmax_sel[i] -> Scale(1./rtime);

  //   hfmax[i] -> Draw("hist");
  //   hfmax_sel[i] -> SetLineColor(kRed);
  //   hfmax_sel[i] -> Draw("hist, same");
  //   pad_fmax[i] -> SetLogy();
  // }
  // //can_fmax -> Print("./figure/20231215/WCMD_fmax_000287.png");

  // TCanvas *can_qtp = new TCanvas("can_qtp", "QtPed", 1600, 800);
  // can_qtp -> Divide(7, 7);
  // for(int i = 0; i < 47; i++){
  //   can_qtp -> cd(i+1);

  //   hqtp[i] -> Draw();
  // }
  // //can_qtp -> Print("./figure/20231215/WCMD_qtp_000287.png");

  // TCanvas *can_qtr = new TCanvas("can_qtr", "QtRms", 1600, 800);
  // can_qtr -> Divide(7, 7);
  // for(int i = 0; i < 47; i++){
  //   can_qtr -> cd(i+1);

  //   hqtr[i] -> Draw();
  // }
  // //can_qtr -> Print("./figure/20231215/WCMD_qtr_000287.png");
}
