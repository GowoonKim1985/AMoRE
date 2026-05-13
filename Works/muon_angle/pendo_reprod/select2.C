#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <iostream>
#include <algorithm>
using namespace std;

void select2() {

  // ----------------------------
  // 1) Input chain
  // ----------------------------
  TChain *chain = new TChain("tree");
  chain->Add("All330.root");
  chain->Add("All327.root");

  // Input branches
  int detID = -1, max_ch = -1;
  double qsump = 0, ptime = 0, qmax = 0, Q1 = 0, Q2 = 0, Q3 = 0, Q4 = 0;

  chain->SetBranchAddress("detID", &detID);
  chain->SetBranchAddress("ptime", &ptime);
  chain->SetBranchAddress("qsump", &qsump);
  chain->SetBranchAddress("qmax", &qmax);
  chain->SetBranchAddress("Q1", &Q1);
  chain->SetBranchAddress("Q2", &Q2);
  chain->SetBranchAddress("Q3", &Q3);
  chain->SetBranchAddress("Q4", &Q4);
  chain->SetBranchAddress("max_ch", &max_ch);

  // ----------------------------
  // 2) Load cuts from qsumCoinCut.root
  //    (tree branches: idp, minCut)
  // ----------------------------
  const int MAX_DETID = 124;          // detID runs up to 123
  double minCutByDetID[MAX_DETID];
  for (int i = 0; i < MAX_DETID; i++) minCutByDetID[i] = -1e99;  // default: no cut

  TFile *fcut = TFile::Open("qsumCoinCut.root", "READ");
  if (!fcut || fcut->IsZombie()) {
    cout << "ERROR: cannot open qsumCoinCut.root\n";
    return;
  }

  TTree *tcut = (TTree*)fcut->Get("tree");
  if (!tcut) {
    cout << "ERROR: cannot find tree 'tree' in qsumCoinCut.root\n";
    fcut->Close();
    return;
  }

  int idp = -1;
  double minCut = 0;
  tcut->SetBranchAddress("idp", &idp);
  tcut->SetBranchAddress("minCut", &minCut);

  int ncuts = tcut->GetEntries();
  cout << "Loaded cut entries: " << ncuts << endl;

  for (int i = 0; i < ncuts; i++) {
    tcut->GetEntry(i);
    if (idp >= 0 && idp < MAX_DETID) {
      minCutByDetID[idp] = minCut;   // detID == idp (same IDs)
    }
  }
  fcut->Close();

  // ----------------------------
  // 3) Output file + output tree
  // ----------------------------
  TFile *outf = new TFile("selected.root", "RECREATE");
  if (!outf || outf->IsZombie()) {
    cout << "ERROR: cannot create selected.root\n";
    return;
  }

  TTree *outTree = new TTree("tree", "Selected events after qsump cuts");

  // Output branches (same variables)
  outTree->Branch("detID",  &detID,  "detID/I");
  outTree->Branch("ptime",  &ptime,  "ptime/D");
  outTree->Branch("qsump",  &qsump,  "qsump/D");
  outTree->Branch("qmax",   &qmax,   "qmax/D");
  outTree->Branch("Q1",     &Q1,     "Q1/D");
  outTree->Branch("Q2",     &Q2,     "Q2/D");
  outTree->Branch("Q3",     &Q3,     "Q3/D");
  outTree->Branch("Q4",     &Q4,     "Q4/D");
  outTree->Branch("max_ch", &max_ch, "max_ch/I");

  // ----------------------------
  // 4) Event loop + apply cuts
  // ----------------------------
  Long64_t n = chain->GetEntries();
  cout << "Total entries: " << n << endl;

  Long64_t kept = 0, rejected = 0, skipped = 0;

  for (Long64_t i = 0; i < n; i++) {
    chain->GetEntry(i);

    //  valid detIDs: 0-58 and 65-123
    bool valid = (detID >= 0 && detID <= 58) || (detID >= 65 && detID <= 123);
    if (!valid) { skipped++; continue; }

    double cut = minCutByDetID[detID];

    // Apply cut: keep only qsump >= minCut
    if (qsump < cut) { rejected++; continue; }

    outTree->Fill();
    kept++;
  }

  cout << "Kept:     " << kept << endl;
  cout << "Rejected: " << rejected << endl;
  cout << "Skipped (invalid detID): " << skipped << endl;

  // ----------------------------
  // 5) output
  // ----------------------------
  outf->cd();
  outTree->Write();
  outf->Close();
}
