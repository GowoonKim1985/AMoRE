void plot_coin()
{
  TH1D *hqsumP_tot[22];
  TH1D *hqsumP_psd[22];
  TH1D *hqsumP_wcd[22];
  TFile ft("./hist/muonHist_PSMD_all.root");
  TFile fp("./hist/muonHist_bot_coinPSMD.root");
  TFile fw("./hist/muonHist_bot_coinWCMD.root");
  int nn = 0;
  for(int i = 0; i < 130; i++){
    if((i > 47  && i < 59) ||
       (i > 112 && i < 124)){
      hqsumP_tot[nn] = (TH1D*)ft.Get(Form("hqsum_%d", i));
      hqsumP_tot[nn] -> SetDirectory(0);
      hqsumP_psd[nn] = (TH1D*)fp.Get(Form("hqsumP_%d", nn));
      hqsumP_psd[nn] -> SetDirectory(0);
      hqsumP_wcd[nn] = (TH1D*)fw.Get(Form("hqsumP_%d", nn));
      hqsumP_wcd[nn] -> SetDirectory(0);

      nn++;
    }
    else
      continue;
  }

  TGaxis::SetMaxDigits(4);
  TCanvas *can_p[2];
  for(int i = 0; i < 2; i++){
    can_p[i] = new TCanvas(Form("can_p%d", i+1), Form("PSMD Bot.%d", i+1), 1600, 900);
    can_p[i] -> Divide(4, 3);
    for(int j = 0; j < 11; j++){
      can_p[i] -> cd(j+1);

      hqsumP_tot[11*i+j] -> Rebin(5);
      hqsumP_psd[11*i+j] -> Rebin(5);
      hqsumP_wcd[11*i+j] -> Rebin(5);

      hqsumP_tot[11*i+j] -> GetXaxis() -> SetLabelSize(0.04);
      hqsumP_tot[11*i+j] -> GetYaxis() -> SetLabelSize(0.04);
      hqsumP_tot[11*i+j] -> GetXaxis() -> SetTitleSize(0.05);
      hqsumP_tot[11*i+j] -> GetYaxis() -> SetTitleSize(0.05);
      hqsumP_tot[11*i+j] -> GetYaxis() -> SetTitleOffset(0.9);
      
      hqsumP_tot[11*i+j] -> GetXaxis() -> SetRangeUser(0, 50000);
      hqsumP_tot[11*i+j] -> SetLineColor(kBlack);
      hqsumP_tot[11*i+j] -> SetLineWidth(1);
      hqsumP_tot[11*i+j] -> Draw();
      hqsumP_psd[11*i+j] -> SetLineColor(kRed);
      hqsumP_psd[11*i+j] -> SetLineWidth(1);
      hqsumP_psd[11*i+j] -> Draw("same");
      hqsumP_wcd[11*i+j] -> SetLineColor(kBlue);
      hqsumP_wcd[11*i+j] -> SetLineWidth(1);
      hqsumP_wcd[11*i+j] -> Draw("same");
      gPad -> SetLogy();
    }
  }
}
