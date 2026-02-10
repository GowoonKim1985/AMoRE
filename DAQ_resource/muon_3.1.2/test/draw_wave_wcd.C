void draw_wave_wcd()
{
  gStyle -> SetOptStat(0);
  
  TFile f("./hist/AccWaves_000288.root");
  TH1D *hwave[48];
  for(int i = 0; i < 47; i++){
    hwave[i] = (TH1D*)f.Get(Form("his_%d", i));
    hwave[i] -> SetDirectory(0);
  }
  
  TCanvas *can = new TCanvas("can", "", 1500, 1000);
  can -> Divide(7, 7);
  for(int i = 0; i < 47; i++){
    can -> cd(i+1);
    hwave[i] -> GetYaxis() -> SetLabelSize(0.1);
    hwave[i] -> GetXaxis() -> SetLabelSize(0.1);
    hwave[i] -> Draw("hist");
  }
}
