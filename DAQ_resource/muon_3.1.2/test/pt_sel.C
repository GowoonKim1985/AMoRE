void pt_sel()
{
  TFile f("./hist/PulseTime_PSMD48.root");
  
  double par[4][3] = {{25.8, 0.0006, 25.2}, {25.8, 0.0006, 25.2},
		      {25.8, 0.0006, 25.2}, {25.8, 0.0006, 25.2}};
  TH2D *hpt[4];
  TF1 *func_sel[4];
  TLine *l[4];
  for(int i = 0; i < 4; i++){
    hpt[i] = (TH2D*)f.Get(Form("hpt_%d", i));
    hpt[i] -> SetDirectory(0);

    l[i] = new TLine(0, par[i][0], 5000, par[i][0]);
    func_sel[i] = new TF1(Form("func_sel_%d", i), "[0]*x+[1]", 0, 5000);
    func_sel[i] -> SetParameter(0, par[i][1]);
    func_sel[i] -> SetParameter(1, par[i][2]);

    l[i] -> SetLineColor(kRed);
    l[i] -> SetLineWidth(1);
    func_sel[i] -> SetLineColor(kRed);
    func_sel[i] -> SetLineWidth(1);
  }

  TCanvas *can = new TCanvas("can", "", 1200, 900);
  can -> Divide(2, 2);
  for(int i = 0; i < 4; i++){
    can -> cd(i+1);
    hpt[i] -> Draw();
    l[i] -> Draw("same");
    func_sel[i] -> Draw("same");
  }

}
