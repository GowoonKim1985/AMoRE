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

void plot_phi()
{
   //gStyle->SetOptFit(1);
  gStyle->SetOptStat(0);
  TGaxis::SetMaxDigits(3);

  TH1D *phihis = new TH1D("phihis", "", 60, 0, 360); 
  //  phihis->Sumw2();

  TChain *chain = new TChain("tree");
  chain->Add("psmd_angle.root");// for all events

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
    
    //    scatter2->Fill(costheta, phi);
    //scatter1->Fill(phi, theta, Mf);

    if(qsum1>0&&qsum2>0){
      phihis->Fill(phi);
    }

    //   thetahis->Fill(costheta);

    //    if(costheta<=0.1 && phi<=55 && id_1==23 && id_2==11) cout << i << ": "<< phi << ": "<< theta << ": "<< id_1 << ": "<< id_2 << endl;
  }

  //  if (phihis->Integral("width") > 0) phihis->Scale(1.0 / phihis->Integral("width"));
  //  if (thetahis->Integral("width") > 0) thetahis->Scale(1.0 / thetahis->Integral("width"));

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
  phihis->Scale(1.0 / phihis->Integral());

  
  //phihis->Sumw2();
  phihis->Draw("EP");
  // thetahis->Draw("E1");

      TFile *outf = TFile::Open("plot_phi.root", "RECREATE");

     outf->cd();

    // hThetaData->Write();
     phihis->Write();
    // hCosData->Write();
    // h2D_ThetaPhi->Write();

     outf->Close();

  //can3->SaveAs("plots/phi.png");
}
