R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(/home/nyandapendo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/cupsoft/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <cmath>
#include <iostream>
#include <map>      // <-- added
#include <climits>  // <-- for INT_MIN
using namespace std;

// ---------- Main Processing Function ----------
void Raw_All() {
 
  TChain *pchain;
  PSEvent *pevt;
  int pnevt;
  /*
  for (int i = 0; i < 2; i++) {
    pchain[i] = new TChain("prd_psd");
    //pchain[i]->Add(Form("/data/nyandapendo/PSMD2/000327/prd_psd_daq%d_000327_*.root", i+1));
    pchain[i]->Add(Form("/data/nyandapendo/PSMD2/000330/prd_psd_daq%d_000330_*.root", i+1));
    pevt[i] = new PSEvent();
    pchain[i]->SetBranchAddress("PSEvent", &pevt[i]);
    pnevt[i] = pchain[i]->GetEntries();
  }
  */
  int runnum= 330;
    pchain = new TChain("prd_psd");
    //pchain[i]->Add(Form("/data/nyandapendo/PSMD2/000327/prd_psd_daq%d_000327_*.root", i+1));
    pchain->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/merge/prd_psd_%06d_*.root", runnum, runnum));
    pevt = new PSEvent();
    pchain->SetBranchAddress("PSEvent", &pevt);
    pnevt = pchain->GetEntries();

    cout<<pnevt<<endl;
    TFile *outfile = new TFile(Form("All%03d.root",runnum), "RECREATE");
  TTree *tree = new TTree("tree", "Selected PSMD Hits");

  int detID;
  double ptime;
  double qsump;
  double q[4];
  double Q1, Q2, Q3, Q4;
  double qmax;
  int max_ch;

  tree->Branch("detID", &detID, "detID/I");
  tree->Branch("ptime", &ptime, "ptime/D");
  tree->Branch("qsump", &qsump, "qsump/D");
  tree->Branch("qmax", &qmax, "qmax/D");
  tree->Branch("Q1",&Q1); tree->Branch("Q2",&Q2); tree->Branch("Q3",&Q3); tree->Branch("Q4",&Q4);
  tree->Branch("max_ch", &max_ch, "max_ch/I");

  // ---- counter per detector id
  std::map<int, Long64_t> detID_counts;

  int pnevt_d[2] = {0};
  
  //  for (int k = pnevt_d[j]; k < pnevt[j]; k++) { //k
    for (int k = 0; k<pnevt; k++) { //k
  //  for (int k = 0; k<10; k++) { //k
    pchain->GetEntry(k);
	
      if(k != 0 && k%1000000 == 0) cout << k << endl;
	
      ptime = pevt->GetTriggerTime();

      int nchp = pevt->GetN();
	
      for (int d = 0; d < nchp; d++) {
	PSHit *hit = pevt->Get(d);
	detID = hit->GetID();
	if (!(detID < 59 || (detID > 64 && detID < 124))) continue;

	qsump = hit->GetQsum();

	qmax = INT_MIN;
	//	cout<<detID<<endl;
	//	cout<<"qsum "<<qsump<<endl;
	for (int b = 0; b < 4; b++) {
	  q[b] = hit->GetQ(b);
	      
	  Q1=q[0];
	  Q2=q[1];
	  Q3=q[2];
	  Q4=q[3];

	  if(q[b] > qmax){
	    qmax = q[b];
	    max_ch = b;
	  }
	}

	tree->Fill();
	detID_counts[detID]++;
      }
    }//k

  // ---- added: print summary
  cout << "\n========== Detector ID Summary ==========\n";
  cout << "detID\tEntries (qsump events)\n";
  cout << "----------------------------------------\n";
  for (const auto &entry : detID_counts) {
    cout << entry.first << "\t" << entry.second << endl;
  }
  cout << "========================================\n";
  
  outfile->cd();
  tree->Write();
  outfile->Close();
}
