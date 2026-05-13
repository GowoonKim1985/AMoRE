#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TCanvas.h>
#include <TStyle.h>

void draw_phi()
{
  TFile *f = new TFile("angle_ps_coincidence_1000run_top2.root","READ");
  TTree *tree = (TTree*)f->Get("tree");

  TCanvas *c1 = new TCanvas("c1","",800,600);

  TH1F *hred = new TH1F("hred",
                        "phi distribution;phi [deg];Counts",
                        60,0,360);

  TH1F *hgreen = new TH1F("hgreen",
                          "phi distribution;phi [deg];Normalized Counts",
                          60,0,360);

  tree->Draw("phi>>hred","","goff");

  tree->Draw("phi>>hgreen","Sum$(psveto_id<125 && psveto_id!=71 && psveto_id!=73)==npsveto_id","goff");

  hred->SetMarkerStyle(20);
  hred->SetMarkerSize(0.5);
  hred->SetMarkerColor(kRed);
  hred->SetLineColor(kRed);

  hgreen->SetMarkerStyle(20);
  hgreen->SetMarkerSize(0.8);
  hgreen->SetMarkerColor(kGreen+2);
  hgreen->SetLineColor(kGreen+2);
  hgreen->Scale(1.0 / hgreen->Integral());
  gStyle->SetErrorX(0);

  //  hred->Draw("EP");
  //  hgreen->Draw("EP SAME");
  hgreen->Draw("EP");

      TFile *outf = TFile::Open("plot_phi.root", "RECREATE");
 
     outf->cd();

     hgreen->Write();

     outf->Close();


  
}
