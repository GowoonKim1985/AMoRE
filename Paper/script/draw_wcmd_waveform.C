R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_wcmd_waveform(){
  
  //  TFile *f = new TFile("./hist/wcmd_wave_sample.root");
  TFile *f = new TFile("./hist/wcmd_flasher_sample.root");  

    gStyle -> SetOptStat(0);

    TCanvas * can = new TCanvas("can", "wcmd waveform", 1300, 900);
    //can.cd();

  gPad->SetLeftMargin(0.12);
    //  gPad->SetRightMargin(0.15);   
  //  can[5] = new TCanvas("can5", "qhis all", 1300, 900);
  //    gPad->SetLeftMargin(0.12);   
  //  TGaxis::SetMaxDigits(3);

 TH1D * h;

//TH1D* h_sum = nullptr;


//   h = (TH1D*)f->Get("Waveform_ch019");
   h = (TH1D*)f->Get("Waveform_ch039");   //flasher
   //   h[i]->GetListOfFunctions()->Clear();  
//   h[i]->GetXaxis()->SetRangeUser(5000, 15000);
   TAxis* xaxis = h->GetXaxis();

   int nbins = xaxis->GetNbins();
   double xmin = xaxis->GetXmin();

// 원래 bin width가 1 ns였다면 → 2 ns로 바꾸려면:
   double new_xmin = xmin * 2;
   double new_xmax = new_xmin + nbins * 2; // 2 ns × nbins

   xaxis->SetLimits(new_xmin, new_xmax);
   
   h->GetXaxis()->SetTitle("Time [ns]");
   h->GetYaxis()->SetTitle("ADC");
   
   h->GetXaxis()->SetTitleFont(62);
   h->GetYaxis()->SetTitleFont(62);
   h->GetXaxis()->SetTitleSize(0.04);
   h->GetYaxis()->SetTitleSize(0.04);
   h->SetLineColor(kBlack); 
   // can->cd();
   //   can.cd();
   h->Draw();

   


}
