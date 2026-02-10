R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_wcmd_hitmap_by_mul()
{
  gStyle->SetOptStat(0);   // stat box 끄기
  TString rawfile = "../prod_2nd/pwcoin_all.root";
  TChain * tree = new TChain("coin");
  tree->Add(rawfile.Data());
  
  int wqmax_ch, wbitsum, wtrgdet[48];
  double wqsum;
  tree->SetBranchAddress("wbitsum", &wbitsum);
  //  tree->SetBranchAddress("wtrgdet", &wtrgdet);
  tree->SetBranchAddress("wqmax_ch", &wqmax_ch);
  tree->SetBranchAddress("wqsum", &wqsum);  

 TH2I *h_high = new TH2I("h_high", "wtrgdet >= 40;col;row", 7,0,7, 7,0,7);
  TH2I *h_low  = new TH2I("h_low",  "wtrgdet < 40;col;row",  7,0,7, 7,0,7);

  int tent = tree->GetEntries();
  //  cout<<"test"<<endl;
   for (int i = 0; i < tent; i++) {
     //   for (int i = 0; i < 10; i++) {
      //    cout<<"entry "<<i<<endl;

    //      if(i%10000==0){      cout<<i<<"/"<<tent<<endl;}
    tree->GetEntry(i);
    //    if (wqmax_ch < 0 || wqmax_ch > 48) continue;
    if (wqsum<3000) continue;    

    
    int row = wqmax_ch / 7;
    int col = wqmax_ch % 7;

    if(wbitsum >= 40) {h_high->Fill(col, 6-row);}
    else {h_low->Fill(col, 6-row);}
    //    hitmap->Fill(col, 6 - row);
  }

  // 캔버스 1: wtrgdet >= 40
  TCanvas *c1 = new TCanvas("c_high", "wtrgdet >= 40", 600, 600);
  c1->SetRightMargin(0.18);
h_high->GetXaxis()->SetLabelSize(0);
h_high->GetYaxis()->SetLabelSize(0);
h_high->GetXaxis()->SetTitle("");
h_high->GetYaxis()->SetTitle("");
//h_high->GetXaxis()->SetNdivisions(7, false);
//h_high->GetYaxis()->SetNdivisions(7, false);
 h_high->GetXaxis()->SetTickLength(0);
h_high->GetYaxis()->SetTickLength(0);
h_high->SetMinimum(0);
h_high->SetMaximum(1000);
h_high->SetContour(10);

h_high->SetMaximum(800);         // 예: 상위 70% 잘라내기
 
  h_high->Draw("COLZ");
  //  h_high->Draw("COLZ TEXT");  

TLatex latex;
latex.SetTextAlign(22);
latex.SetTextSize(0.022);
latex.SetTextColor(kBlack);

for (int row = 0; row < 7; row++) {
  for (int col = 0; col < 7; col++) {
    int pmt = row*7 + col;
    if(pmt<47)
    latex.DrawLatex(col+0.5, 6-row+0.5, Form("PMT %d", pmt+1));
  }
}
/*
for (int i = 0; i <= 7; i++) {
  // 세로선
  TLine *lv = new TLine(i, 0, i, 7);
  lv->SetLineStyle(2);   // 점선
  lv->SetLineWidth(1);
  lv->SetLineColor(kBlack);
  lv->Draw();

  // 가로선
  TLine *lh = new TLine(0, i, 7, i);
  lh->SetLineStyle(2);   // 점선
  lh->SetLineWidth(1);
  lh->SetLineColor(kBlack);
  lh->Draw();
}
*/

 
  // 캔버스 2: wtrgdet < 40
  TCanvas *c2 = new TCanvas("c_low", "wtrgdet < 40", 600, 600);
  c2->SetRightMargin(0.18);
h_low->GetXaxis()->SetLabelSize(0);
h_low->GetYaxis()->SetLabelSize(0);
h_low->GetXaxis()->SetTitle("");
h_low->GetYaxis()->SetTitle("");

 h_low->GetXaxis()->SetTickLength(0);
h_low->GetYaxis()->SetTickLength(0);
//h_low->GetXaxis()->SetNdivisions(7, false);
//h_low->GetYaxis()->SetNdivisions(7, false);

h_low->SetMinimum(0);
h_low->SetMaximum(1000);
h_low->SetContour(10);
h_low->SetMaximum(800);         // 예: 상위 70% 잘라내
 h_low->Draw("COLZ");

 // h_low->Draw("COLZ TEXT");  
  
  //TLatex latex;
latex.SetTextAlign(22);
latex.SetTextSize(0.022);
latex.SetTextColor(kBlack);

for (int row = 0; row < 7; row++) {
  for (int col = 0; col < 7; col++) {
    int pmt = row*7 + col;
    if(pmt<47) latex.DrawLatex(col+0.5, 6-row+0.5, Form("PMT %d", pmt+1));
  }
 }
/*
for (int i = 0; i <= 7; i++) {
  // 세로선
  TLine *lv = new TLine(i, 0, i, 7);
  lv->SetLineStyle(2);   // 점선
  lv->SetLineWidth(1);
  lv->SetLineColor(kBlack);
  lv->Draw();

  // 가로선
  TLine *lh = new TLine(0, i, 7, i);
  lh->SetLineStyle(2);   // 점선
  lh->SetLineWidth(1);
  lh->SetLineColor(kBlack);
  lh->Draw();
}
*/



}
