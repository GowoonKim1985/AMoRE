R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(/home/cupsoft/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)


#include "TVector3.h"
#include "TFile.h"
#include "TTree.h"
#include "TChain.h"
#include <cmath>
#include <iostream>
#include <map>
#include <climits>
using namespace std;

// ---------- helper: process one event ----------
void ProcessOneEvent(PSEvent* evt,
                     TTree* tree,
                     std::map<int, Long64_t>& detID_counts,
                     int& detID,
                     double& ptime,
                     double& qsump,
                     double& qmax,
                     int& max_ch,
                     double& Q1,
                     double& Q2,
                     double& Q3,
                     double& Q4)
{
  if (!evt) return;

  ptime = evt->GetTriggerTime();
  int nchp = evt->GetN();

  for (int d = 0; d < nchp; d++) {
    PSHit *hit = evt->Get(d);
    if (!hit) continue;

    detID = hit->GetID();

    if (!(detID < 59 || (detID > 64 && detID < 124))) continue;

    qsump = hit->GetQsum();

    qmax = INT_MIN;
    max_ch = -1;

    double q[4];
    for (int b = 0; b < 4; b++) {
      q[b] = hit->GetQ(b);

      if (q[b] > qmax) {
        qmax = q[b];
        max_ch = b;
      }
    }

    Q1 = q[0];
    Q2 = q[1];
    Q3 = q[2];
    Q4 = q[3];

    tree->Fill();
    detID_counts[detID]++;
  }
}


// ---------- Main Processing Function ----------
void Raw_Process()
{
  TChain *pchain[2];
  PSEvent *pevt[2];
  Long64_t pnevt[2];

  for (int i = 0; i < 2; i++) {
    pchain[i] = new TChain("prd_psd");
    //pchain[i]->Add(Form("/data/nyandapendo/PSMD2/000327/prd_psd_daq%d_000327_*.root", i+1));
    //    pchain[i]->Add(Form("/data/nyandapendo/PSMD2/000330/prd_psd_daq%d_000330_*.root", i+1));
    pchain[i]->Add(Form("/home/kkw/Muon/PROD/PSMD/000327/prd_psd_daq%d_000327_*.root", i+1));

    pevt[i] = new PSEvent();
    pchain[i]->SetBranchAddress("PSEvent", &pevt[i]);
    pnevt[i] = pchain[i]->GetEntries();

    cout << "DAQ" << i+1 << " entries: " << pnevt[i] << endl;
  }

  TFile *outfile = new TFile("All327PerTrg.root", "RECREATE");
  TTree *tree = new TTree("tree", "Selected PSMD Hits");

  int detID;
  double ptime;
  double qsump;
  double Q1, Q2, Q3, Q4;
  double qmax;
  int max_ch;

  tree->Branch("detID", &detID, "detID/I");
  tree->Branch("ptime", &ptime, "ptime/D");
  tree->Branch("qsump", &qsump, "qsump/D");
  tree->Branch("qmax", &qmax, "qmax/D");
  tree->Branch("Q1", &Q1, "Q1/D");
  tree->Branch("Q2", &Q2, "Q2/D");
  tree->Branch("Q3", &Q3, "Q3/D");
  tree->Branch("Q4", &Q4, "Q4/D");
  tree->Branch("max_ch", &max_ch, "max_ch/I");

  // ---- counter per detector id
  std::map<int, Long64_t> detID_counts;

  // -----------------------------------------
  // Merge DAQ1 and DAQ2 by trigger time
  // -----------------------------------------
  Long64_t idx[2] = {0, 0};

  // preload first event from each DAQ
  bool hasEvt[2] = {false, false};
  double trig[2] = {0.0, 0.0};

  for (int j = 0; j < 2; j++) {
    if (idx[j] < pnevt[j]) {
      pchain[j]->GetEntry(idx[j]);
      hasEvt[j] = true;
      trig[j] = pevt[j]->GetTriggerTime();
    }
  }

  // If trigger time is integer-valued in ns, using llround is safer than raw double compare
  while (hasEvt[0] || hasEvt[1]) {

    // progress print
    if ((idx[0] + idx[1]) != 0 && ((idx[0] + idx[1]) % 1000000 == 0)) {
      cout << "Processed total events = " << (idx[0] + idx[1])
           << "  (DAQ1=" << idx[0] << ", DAQ2=" << idx[1] << ")" << endl;
    }

    // case 1: only DAQ1 still has events
    if (hasEvt[0] && !hasEvt[1]) {
      ProcessOneEvent(pevt[0], tree, detID_counts, detID, ptime, qsump, qmax, max_ch, Q1, Q2, Q3, Q4);
      idx[0]++;
      if (idx[0] < pnevt[0]) {
        pchain[0]->GetEntry(idx[0]);
        trig[0] = pevt[0]->GetTriggerTime();
      } else {
        hasEvt[0] = false;
      }
      continue;
    }

    // case 2: only DAQ2 still has events
    if (!hasEvt[0] && hasEvt[1]) {
      ProcessOneEvent(pevt[1], tree, detID_counts, detID, ptime, qsump, qmax, max_ch, Q1, Q2, Q3, Q4);
      idx[1]++;
      if (idx[1] < pnevt[1]) {
        pchain[1]->GetEntry(idx[1]);
        trig[1] = pevt[1]->GetTriggerTime();
      } else {
        hasEvt[1] = false;
      }
      continue;
    }

    // both DAQs have events
    Long64_t t0 = llround(trig[0]);
    Long64_t t1 = llround(trig[1]);

    if (t0 < t1) {
      // process DAQ1 event
      ProcessOneEvent(pevt[0], tree, detID_counts, detID, ptime, qsump, qmax, max_ch, Q1, Q2, Q3, Q4);
      idx[0]++;
      if (idx[0] < pnevt[0]) {
        pchain[0]->GetEntry(idx[0]);
        trig[0] = pevt[0]->GetTriggerTime();
      } else {
        hasEvt[0] = false;
      }
    }
    else if (t1 < t0) {
      // process DAQ2 event
      ProcessOneEvent(pevt[1], tree, detID_counts, detID, ptime, qsump, qmax, max_ch, Q1, Q2, Q3, Q4);
      idx[1]++;
      if (idx[1] < pnevt[1]) {
        pchain[1]->GetEntry(idx[1]);
        trig[1] = pevt[1]->GetTriggerTime();
      } else {
        hasEvt[1] = false;
      }
    }
    else {
      // same trigger time -> process BOTH together
      ProcessOneEvent(pevt[0], tree, detID_counts, detID, ptime, qsump, qmax, max_ch, Q1, Q2, Q3, Q4);
      ProcessOneEvent(pevt[1], tree, detID_counts, detID, ptime, qsump, qmax, max_ch, Q1, Q2, Q3, Q4);

      idx[0]++;
      idx[1]++;

      if (idx[0] < pnevt[0]) {
        pchain[0]->GetEntry(idx[0]);
        trig[0] = pevt[0]->GetTriggerTime();
      } else {
        hasEvt[0] = false;
      }

      if (idx[1] < pnevt[1]) {
        pchain[1]->GetEntry(idx[1]);
        trig[1] = pevt[1]->GetTriggerTime();
      } else {
        hasEvt[1] = false;
      }
    }
  }

  // ---- print summary
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
