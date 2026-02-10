void comp()
{
  TFile *f[2];

  TH1D *hqsum[2][22];

  for(int i = 0; i < 2; i++){
    if(i == 0)
      f[i] = new TFile("./hist/muonHist.root");
    else
      f[i] = new TFile("./hist/muonHist_227_daq1.root");
    
    for(int j = 0; j < 22; j++){
      if(i == 1 && j > 10)
	f[i] = new TFile("./hist/muonHist_227_daq2.root");
      hqsum[i][j] = (TH1D*)f[i] -> Get(Form("hqsum_%d", j));
      hqsum[i][j] -> SetDirectory(0);

      if(i == 1)
	hqsum[i][j] -> SetLineColor(kRed);
    }
  }
  

  TCanvas *can = new TCanvas("", "", 1600, 1000);
  can -> Divide(6, 4);
  TPad *pad[22];
  for(int i = 0; i < 22; i++){
    can -> cd(i+1);
    pad[i] = (TPad*)can -> cd(i+1);

    hqsum[0][i] -> Draw();
    hqsum[1][i] -> Draw("same");

    pad[i] -> SetLogy();
  }





}
