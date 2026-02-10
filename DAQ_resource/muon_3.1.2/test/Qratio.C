void Qratio()
{
  double offset[22][2] = {
    {0.40, 0.50}, {0.40, 0.45}, {0.40, 0.50}, {0.50, 0.50},
    {0.60, 0.45}, {0.65, 0.40}, {0.50, 0.50}, {0.55, 0.50},
    {0.45, 0.45}, {0.55, 0.55}, {0.50, 0.50},

    {0.55, 0.50}, {0.50, 0.45}, {0.45, 0.50}, {0.55, 0.50},
    {0.60, 0.50}, {0.50, 0.45}, {0.50, 0.50}, {0.45, 0.55},
    {0.50, 0.45}, {0.45, 0.50}, {0.60, 0.60}
  };

  //TFile f("./hist/muonHist_Qratio_woQt.root");
  TFile f("./hist/muonHist_Qratio.root");

  TF1 *fu[22];
  TF1 *fl[22];
  TH1D *hQsum[22];
  TH1D *hQsum_sel[22];
  TH2D *hQrul[22];
  TH2D *hQrul_sel[22];
  TH2D *hQsR[22];
  TH2D *hQsR_sel[22];
  TH2D *hQmQs[22];
  TH2D *hQmQs_sel[22];
  for(int i = 0; i < 22; i++){
    hQsum[i] = (TH1D*)f.Get(Form("hQsum_%d", i));
    hQsum[i] -> SetDirectory(0);
    hQsum_sel[i] = (TH1D*)f.Get(Form("hQsum_sel_%d", i));
    hQsum_sel[i] -> SetDirectory(0);
    hQsum_sel[i] -> SetLineColor(kRed);
    hQsum[i] -> Rebin(2);
    hQsum_sel[i] -> Rebin(2);
    
    hQrul[i] = (TH2D*)f.Get(Form("hQrul_%d", i));
    hQrul[i] -> SetDirectory(0);
    hQrul_sel[i] = (TH2D*)f.Get(Form("hQrul_sel_%d", i));
    hQrul_sel[i] -> SetDirectory(0);
    hQrul_sel[i] -> SetMarkerSize(0.5);

    fu[i] = new TF1(Form("fu_%d", i), "x+[0]", -1, 1);
    fu[i] -> SetParameter(0, offset[i][0]);
    fl[i] = new TF1(Form("fl_%d", i), "x-[0]", -1, 1);
    fl[i] -> SetParameter(0, offset[i][1]);

    hQsR[i] = (TH2D*)f.Get(Form("hQsR_%d", i));
    hQsR[i] -> SetDirectory(0);
    hQsR_sel[i] = (TH2D*)f.Get(Form("hQsR_sel_%d", i));
    hQsR_sel[i] -> SetDirectory(0);
    hQsR_sel[i] -> SetMarkerSize(0.5);

    // hQmQs[i] = (TH2D*)f.Get(Form("hQmQs_%d", i));
    // hQmQs[i] -> SetDirectory(0);
    // hQmQs_sel[i] = (TH2D*)f.Get(Form("hQmQs_sel_%d", i));
    // hQmQs_sel[i] -> SetDirectory(0);
  }
  
  // TCanvas *can1_qsum = new TCanvas("can1_qsum", "psmd bottom 1", 1600, 900);
  // can1_qsum -> Divide(4, 3);
  // TPad *pad1_qsum[11];
  // for(int i = 0; i < 11; i++){
  //   can1_qsum -> cd(i+1);
  //   pad1_qsum[i] = (TPad*)can1_qsum -> cd(i+1);

  //   hQsum[i] -> Draw();
  //   hQsum_sel[i] -> Draw("same");
  //   pad1_qsum[i] -> SetLogy();
  // }

  // TCanvas *can2_qsum = new TCanvas("can2_qsum", "psmd bottom 2", 1600, 900);
  // can2_qsum -> Divide(4, 3);
  // TPad *pad2_qsum[11];
  // for(int i = 0; i < 11; i++){
  //   can2_qsum -> cd(i+1);
  //   pad2_qsum[i] = (TPad*)can2_qsum -> cd(i+1);
    
  //   hQsum[i+11] -> Draw();
  //   hQsum_sel[i+11] -> Draw("same");
  //   pad2_qsum[i] -> SetLogy();
  // }
  
  // TCanvas *can1 = new TCanvas("can1", "psmd bottom 1", 1600, 900);
  // can1 -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can1 -> cd(i+1);

  //   hQrul[i] -> Draw("colz");
  //   hQrul_sel[i] -> Draw("same");
  //   fu[i] -> Draw("same");
  //   fl[i] -> Draw("same");
  // }

  // TCanvas *can2 = new TCanvas("can2", "psmd bottom 2", 1600, 900);
  // can2 -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can2 -> cd(i+1);

  //   hQrul[i+11] -> Draw("colz");
  //   hQrul_sel[i+11] -> Draw("same");
  //   fu[i+11] -> Draw("same");
  //   fl[i+11] -> Draw("same");
  // }

  // TCanvas *can1_QsR = new TCanvas("can1_QsR", "psmd bottom 1", 1600, 900);
  // can1_QsR -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can1_QsR -> cd(i+1);

  //   hQsR[i] -> Draw("colz");
  //   hQsR_sel[i] -> Draw("same");
  // }

  // TCanvas *can2_QsR = new TCanvas("can2_QsR", "psmd bottom 2", 1600, 900);
  // can2_QsR -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can2_QsR -> cd(i+1);

  //   hQsR[i+11] -> Draw("colz");
  //   hQsR_sel[i+11] -> Draw("same");
  // }

  // TCanvas *can1_QmQs = new TCanvas("can1_QmQs", "psmd bottom 1", 1600, 900);
  // can1_QmQs -> Divide(4, 3);
  // for(int i = 0; i < 11; i++){
  //   can1_QmQs -> cd(i+1);

  //   hQmQs[i] -> Draw();
  // }

  TCanvas *can_fig = new TCanvas("can_fig", "", 1200, 900);
  can_fig -> Divide(2, 2);
  can_fig -> cd(1);
  hQrul[0] -> Draw("colz");
  hQrul_sel[0] -> Draw("same");
  fu[0] -> Draw("same");
  fl[0] -> Draw("same");

  can_fig -> cd(2);
  hQsR[0] -> Draw("colz");
  hQsR_sel[0] -> Draw("same");
  
  // can_fig -> cd(3);
  // hQmQs[0] -> Draw();
  // hQmQs_sel[0] -> Draw("same");

  can_fig -> cd(4);
  hQsum[0] -> Draw();
  hQsum_sel[0] -> Draw("same");
}
