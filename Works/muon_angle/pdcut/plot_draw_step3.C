#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TVector3.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;


// Muons = 283696, time = 29 dyas = 2,505,600, A = 483840 cm2
//(area of only side detectors without middle)
//muon flux = 2.34x10^-7muon/cm^2/sec = 202muons/m^2/day

void plot_draw_step3()
{
   //gStyle->SetOptFit(1);
  gStyle->SetOptStat(0);
  TGaxis::SetMaxDigits(3);

//      int nbins = 180;
        int nbins = 90;
    //int nbins = 36;  

  //TH2D *scatter1 = new TH2D("scatter1", "", 360, 0, 1, 360, 0, 360); // theta Vs phi
  //TH2D *scatter1 = new TH2D("scatter1", "", 300, 0, 360, 300, 0, 90); // theta Vs phi
  TH2D *scatter2 = new TH2D("scatter2", "", 360, 0, 1, 360, 0, 360); // phi Vs theta
  TH1D *phihis = new TH1D("phihis", "", nbins, 0, 360); 
  TH1D *thetahis = new TH1D("thetahis", "", nbins, 0, 1);  
  phihis->Sumw2();
  thetahis->Sumw2();

  TChain *chain = new TChain("tree");
  chain->Add("psmd_angle.root");// for all events
  //chain->Add("../smeared2/combined_SSSB.root"); // for coincidence with WCMD

  //chain->Add("side_bottom.root"); // smearedAll
  //chain->Add("side_side.root");  
 
  //chain->Add("../smeared2/side_bottom_zenith.root"); // smeared2
  //chain->Add("../smeared2/side_side.root");

  int id1, id2;
  double theta, phi, qsum1, qsum2;
  double Mf;
  double costheta;
  
  chain->SetBranchAddress("id1", &id1);
  chain->SetBranchAddress("id2", &id2);
  chain->SetBranchAddress("theta", &theta);
  chain->SetBranchAddress("phi", &phi);
  chain->SetBranchAddress("qsum1", &qsum1);
  chain->SetBranchAddress("qsum2", &qsum2);

  int nEntries = chain->GetEntries();
  cout << "Total entries: " << nEntries << endl;
  
  // //Mf = nEntries/(483840*2505600);
  //Mf = 8.81e-8;
  Mf = 54424; // total entries for 327 and 330 after appliying coincidence criteria with wcmd
  
  // cout << "muon flux = " << Mf << " muons/cm^2/sec" << endl;
  
  for (int i = 0; i < nEntries; i++) {
    chain->GetEntry(i);

    //cout << i << ": "<< phi << ": "<< theta << ": "<< sideHitID << ": "<< bottomHitID << endl;
    
    costheta = TMath::Cos(theta * TMath::Pi() / 180);
    
    scatter2->Fill(costheta, phi);
    //scatter1->Fill(phi, theta, Mf);

    if(qsum1>40000&&qsum2>40000){
      phihis->Fill(phi);
    }

    thetahis->Fill(costheta);

    //    if(costheta<=0.1 && phi<=55 && id_1==23 && id_2==11) cout << i << ": "<< phi << ": "<< theta << ": "<< id_1 << ": "<< id_2 << endl;
  }

  if (phihis->Integral("width") > 0) phihis->Scale(1.0 / phihis->Integral("width"));
  if (thetahis->Integral("width") > 0) thetahis->Scale(1.0 / thetahis->Integral("width"));
  
  TCanvas *can1 = new TCanvas("can1", "", 700, 700);
  can1->cd();
  //gPad->SetLogy();
  gStyle->SetPadRightMargin(0.2);
  gStyle->SetLabelSize(0.04, "Z");
  scatter2->GetXaxis()->SetTitle("Cos(#theta)");
  scatter2->GetYaxis()->SetTitle("Azimuthal angle #phi [#circ]");
  scatter2->SetZTitle("Normalized counts");
  scatter2->GetYaxis()->SetTitleOffset(1.4);
  scatter2->GetXaxis()->SetTitleOffset(1.3);
  scatter2->Scale(1.0 / scatter2->Integral("width"));
  scatter2->Draw("colz");
  gStyle->SetPalette(kBird); // or kBird, kRainBow, kViridis, etc.
  //can1->SaveAs("plots/scatter.png");

  TCanvas *can2 = new TCanvas("can2", "", 800, 700);
  can2->cd();
  //gPad->SetLogy();
  thetahis->SetLineColor(kBlue);
  thetahis->SetMarkerColor(kBlue);
  thetahis->SetMarkerStyle(20);
  thetahis->GetXaxis()->SetTitle("Cos(#theta)");
  thetahis->GetYaxis()->SetTitle("Normalized counts");
  thetahis->GetYaxis()->SetTitleOffset(1.3);
  thetahis->GetXaxis()->SetTitleOffset(1.3);
  thetahis->Scale(1.0 / thetahis->Integral("width"));
  thetahis->Draw();
  //can2->SaveAs("plots/theta.png");

  TCanvas *can3 = new TCanvas("can3", "", 800, 700);
  can3->cd();
  //gPad->SetLogy();
  phihis->SetLineColor(kBlue);
  phihis->SetMarkerColor(kBlue);
  phihis->SetMarkerStyle(20);
  phihis->GetXaxis()->SetTitle("Azimuthal angle #phi [#circ]");
  phihis->GetYaxis()->SetTitle("Normalized counts");
  phihis->GetYaxis()->SetTitleOffset(1.1);
  phihis->GetXaxis()->SetTitleOffset(1.3);
  phihis->Scale(1.0 / phihis->Integral("width"));
  //phihis->Sumw2();
  phihis->Draw();
  // thetahis->Draw("E1");
  
  //can3->SaveAs("plots/phi.png");
}
