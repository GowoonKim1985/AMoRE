void wplot_vortex()
{
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_wcd");
  int ntime = 0;
  for(int i = 0; i < 137; i++){
    ntime++;
    chain -> Add(Form("/data/kmseo/WCMD/000303/prd_wcd_000303_%05d.root", i));
  }
  //chain -> Add("/data/kmseo/WCMD/000287/prd_wcd_000287_*.root");

  WCEvent *wevt = new WCEvent();
  chain -> SetBranchAddress("WCEvent", &wevt);
  
  TH2D *hhitmap = new TH2D("hhitmap", "", 7, 0, 7, 7, 0, 7);
  hhitmap -> SetXTitle("Row of PMT map");
  hhitmap -> SetYTitle("Column of PMT map");
  TH1D *hhmap_px, *hhmap_py;
  TH2D *hv_by2D = new TH2D("hv_by2D", "", 7, 0, 7, 7, 0, 7);
  hv_by2D -> SetXTitle("Row of PMT map");
  hv_by2D -> SetYTitle("Column of PMT map");
  TH2D *hv_bypxy = new TH2D("hv_bypxy", "", 7, 0, 7, 7, 0, 7);
  hv_bypxy -> SetXTitle("Row of PMT map");
  hv_bypxy -> SetYTitle("Column of PMT map");
  TH2D *hv_bywa = new TH2D("hv_bywa", "", 35, 0, 7, 35, 0, 7);
  hv_bywa -> SetXTitle("Row of PMT map");
  hv_bywa -> SetYTitle("Column of PMT map");
  
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

  char aaa;
  TCanvas *can = new TCanvas("can", "", 1000, 1000);
  can -> Divide(2, 2);
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
      
      fmax[id[j]] = pmt -> GetFmax();
      rms[id[j]] = pmt -> GetPedRMS();
      if(fmax[id[j]] < 50 || rms[id[j]] > 3) continue;
      
      qtot[id[j]] = pmt -> GetQtot();

      abitsum++;
      nn[id[j]] = 1;
      qsum += qtot[id[j]];
    }

    if(abitsum > 7 && qsum > 20000){
      for(int k = 0; k < 48; k++){
	if(nn[k] == 1)
	  hhitmap -> SetBinContent(7-k/7, 7-k%7, qtot[k]/qsum);
	else{
	  hhitmap -> SetBinContent(k+1, 0);
	  continue;
	}
	qsum_sel += qtot[k];
      }
      int maxbin = hhitmap -> GetMaximumBin();
      int x, y, z, px, py;
      hhitmap -> GetBinXYZ(maxbin, x, y, z);
      hhmap_px = (TH1D*)hhitmap -> ProjectionX("hhmap_px", 0, 7);
      hhmap_py = (TH1D*)hhitmap -> ProjectionY("hhmap_py", 0, 7);
      px = hhmap_px -> GetMaximumBin();
      py = hhmap_py -> GetMaximumBin();

      double x_wa = 0;
      double y_wa = 0;
      double sx_wa = 0;
      double sy_wa = 0;
      for(int l = 0; l < 7; l++){
	x_wa += (l+1)*hhmap_px -> GetBinContent(l+1);
	y_wa += (l+1)*hhmap_py -> GetBinContent(l+1);
	sx_wa += hhmap_px -> GetBinContent(l+1);
	sy_wa += hhmap_py -> GetBinContent(l+1);
      }

      cout << " Maximum position (x, y): " << x << ",   " << y << "   by hitmap" << endl;
      can -> cd(1);
      hhitmap -> Draw("lego2");
      can -> cd(3);
      hhmap_px -> Draw();
      can -> cd(4);
      hhmap_py -> Draw();
      cout << " Maximum position (x, y): " << hhmap_px -> GetMaximumBin() << ",   " << hhmap_py -> GetMaximumBin() << "   by projection Hist." << endl;
      cout << "Weighted position (x, y): " << Form("%.1f, %.1f", x_wa/sx_wa, y_wa/sy_wa) << " by weighted mean of projected Hist. " << endl;
      can -> Modified();
      can -> Update();

      cout << "next?" << endl;
      cin >> aaa;
      if(aaa == 'q') return 0;

      hv_by2D -> Fill(x-1, y-1);
      hv_bypxy -> Fill(px-1, py-1);
      hv_bywa -> Fill(x_wa/sx_wa, y_wa/sy_wa);
    }
  }

  hv_by2D -> Scale(1./36);
  hv_bypxy -> Scale(1./36);
  hv_bywa -> Scale(1./36);
  
  for(int i = 0; i < 7; i++){
    for(int j = 0; j < 7; j++){
      if(i == 0 || i == 6 || j == 0 || j == 6){
	hv_by2D -> SetBinContent(i+1, j+1, hv_by2D -> GetBinContent(i+1, j+1)/(3*0.85));
	hv_bypxy -> SetBinContent(i+1, j+1, hv_bypxy -> GetBinContent(i+1, j+1)/(3*0.85));

      }
    }
  }

  TH1D *hv_px[2], *hv_py[2];
  TCanvas *can_vortex = new TCanvas("can_vortex", "", 1500, 1000);
  can_vortex -> Divide(3, 2);
  can_vortex -> cd(1);
  hv_by2D -> Draw("colz");
  hv_px[0] = (TH1D*)hv_by2D -> ProjectionX("hv_px_0", 0, 7);
  hv_py[0] = (TH1D*)hv_by2D -> ProjectionY("hv_py_0", 0, 7);
  can_vortex -> cd(2);
  hv_px[0] -> Draw();
  can_vortex -> cd(3);
  hv_py[0] -> Draw();

  can_vortex -> cd(4);
  hv_bypxy -> Draw("colz");
  hv_px[1] = (TH1D*)hv_bypxy -> ProjectionX("hv_px_1", 0, 7);
  hv_py[1] = (TH1D*)hv_bypxy -> ProjectionY("hv_py_1", 0, 7);
  can_vortex -> cd(5);
  hv_px[1] -> Draw();
  can_vortex -> cd(6);
  hv_py[1] -> Draw();
}
