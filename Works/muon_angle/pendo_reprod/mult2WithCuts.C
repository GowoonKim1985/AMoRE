#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <iostream>
#include <unordered_map>
#include <map>
#include <cmath>
#include <iomanip>
#include <cstdint>

// Map original detID to [0..117]. Return -1 if detID not in allowed ranges.
static inline int MapDetID(int detID) {
  if (detID >= 0 && detID <= 58) return detID;
  if (detID >= 65 && detID <= 123) return detID - 6;
  return -1;
}

// Load qsump cuts from ROOT file into array indexed by mapped detIndex [0..117].
static bool LoadQsumpCuts(const char* cutFile,
                          const char* cutTreeName,
                          double qsump_cut_arr[118])
{
  for (int i = 0; i < 118; ++i) qsump_cut_arr[i] = -1.0;

  TFile *fc = TFile::Open(cutFile);
  if (!fc || fc->IsZombie()) {
    std::cerr << "ERROR: cannot open cut file: " << cutFile << "\n";
    return false;
  }

  TTree *tc = (TTree*)fc->Get(cutTreeName);
  if (!tc) {
    std::cerr << "ERROR: cannot find TTree '" << cutTreeName
              << "' in " << cutFile << "\n";
    fc->Close();
    return false;
  }

  Int_t detID_cut = 0;
  Double_t qsump_cut = 0.0;
  tc->SetBranchAddress("detID", &detID_cut);
  tc->SetBranchAddress("qsump_cut", &qsump_cut);

  Long64_t nc = tc->GetEntries();
  for (Long64_t i = 0; i < nc; ++i) {
    tc->GetEntry(i);
    int idx = MapDetID(detID_cut);
    if (idx < 0 || idx >= 118) continue;
    qsump_cut_arr[idx] = qsump_cut;
  }

  int missing = 0;
  for (int idx = 0; idx < 118; ++idx)
    if (qsump_cut_arr[idx] < 0) missing++;

  if (missing > 0) {
    std::cerr << "WARNING: " << missing
              << " mapped detectors missing qsump_cut in " << cutFile
              << " (qsump_cut_arr[idx] < 0)\n";
  }

  fc->Close();
  return true;
}

void mult2WithCuts(const char* inputFile  = "selected.root",
                  const char* outputFile = "selected_mult2_cuts.root",
                  const char* treeName   = "tree",
                  const char* cutFile    = "qsump_cuts.root",
                  const char* cutTree    = "cuts")
{
  // ---------------- Input ----------------
  TChain *chain = new TChain(treeName);
  chain->Add(inputFile);

  Long64_t n = chain->GetEntries();
  if (n <= 0) {
    std::cout << "ERROR: no entries found in " << inputFile
              << " (tree=" << treeName << ")\n";
    return;
  }

  // Load detector-specific qsump cuts
  double qsump_cut_arr[118];
  if (!LoadQsumpCuts(cutFile, cutTree, qsump_cut_arr)) {
    std::cerr << "ERROR: failed to load qsump cuts.\n";
    return;
  }

  int    detID = 0;
  double ptime = 0, qsump = 0;
  double qmax = 0, Q1 = 0, Q2 = 0, Q3 = 0, Q4 = 0;

  chain->SetBranchAddress("detID", &detID);
  chain->SetBranchAddress("ptime", &ptime);
  chain->SetBranchAddress("qsump", &qsump);
  chain->SetBranchAddress("qmax", &qmax);
  chain->SetBranchAddress("Q1", &Q1);
  chain->SetBranchAddress("Q2", &Q2);
  chain->SetBranchAddress("Q3", &Q3);
  chain->SetBranchAddress("Q4", &Q4);

  std::cout << "Total hits (entries) in input: " << n << "\n";

  // ---------------- Pass 1: RAW multiplicity per exact ptime(ns) ----------------
  // No cuts here (as you requested)
  std::unordered_map<Long64_t, uint8_t> mult;
  mult.reserve((size_t)std::min<Long64_t>(n / 4, 30'000'000));

  for (Long64_t i = 0; i < n; i++) {
    chain->GetEntry(i);

    Long64_t key = (Long64_t) llround(ptime);
    auto &c = mult[key];
    if (c < 3) c++; // cap at 3

    if (i != 0 && i % 5'000'000 == 0) {
      std::cout << "Pass1: processed " << i
                << " entries, unique trigger-times ~ " << mult.size() << "\n";
    }
  }

  // multiplicity summary
  std::map<int, Long64_t> multiplicity_counts;
  for (const auto &kv : mult) multiplicity_counts[(int)kv.second]++;

  // ---------------- Output ----------------
  TFile fout(outputFile, "RECREATE");
  if (fout.IsZombie()) {
    std::cout << "ERROR: cannot create output file: " << outputFile << "\n";
    return;
  }

  TTree tout("tree", "mult==2 (raw) then detID+qsump cuts + av_asy");

  double av_asy = 0.0;

  tout.Branch("detID",  &detID,  "detID/I");
  tout.Branch("ptime",  &ptime,  "ptime/D");
  tout.Branch("qsump",  &qsump,  "qsump/D");
  tout.Branch("qmax",   &qmax,   "qmax/D");
  tout.Branch("Q1",     &Q1,     "Q1/D");
  tout.Branch("Q2",     &Q2,     "Q2/D");
  tout.Branch("Q3",     &Q3,     "Q3/D");
  tout.Branch("Q4",     &Q4,     "Q4/D");
  tout.Branch("av_asy", &av_asy, "av_asy/D");

  // ---------------- Pass 2: require raw mult==2, THEN apply cuts ----------------
  Long64_t saved_hits = 0;
  Long64_t skipped_mult = 0;
  Long64_t skipped_det  = 0;
  Long64_t skipped_qcut = 0;
  Long64_t skipped_zero = 0;

  for (Long64_t i = 0; i < n; i++) {
    chain->GetEntry(i);

    // raw multiplicity==2 check FIRST
    Long64_t key = (Long64_t) llround(ptime);
    auto it = mult.find(key);
    if (it == mult.end()) continue;
    if (it->second != 2) { skipped_mult++; continue; }

    // detID acceptance
    int detIndex = MapDetID(detID);
    if (detIndex < 0) { skipped_det++; continue; }

    // per-detector qsump cut
    double qcut = qsump_cut_arr[detIndex];
    if (qcut >= 0.0 && qsump <= qcut) { skipped_qcut++; continue; }

    // av_asy computation
    double qlow_dif      = Q1 - Q2;
    double qlow_dif_opp  = Q2 - Q1;
    double qupp_dif      = Q3 - Q4;
    double qupp_dif_opp  = Q4 - Q3;
    double qlow          = Q1 + Q2;
    double qupp          = Q3 + Q4;

    if (qlow == 0.0 || qupp == 0.0) { skipped_zero++; continue; }

    double A = qlow_dif     / qlow;
    double B = qupp_dif     / qupp;
    double C = qlow_dif_opp / qlow;
    double D = qupp_dif_opp / qupp;

    if ((detID >= 12 && detID <= 35) || (detID >= 77 && detID <= 100))
      av_asy = 0.5 * (A + B);
    else
      av_asy = 0.5 * (C + D);

    tout.Fill();
    saved_hits++;

    if (i != 0 && i % 5'000'000 == 0) {
      std::cout << "Pass2: processed " << i
                << " entries, saved_hits=" << saved_hits << "\n";
    }
  }

  fout.cd();
  tout.Write();
  fout.Close();

  // ---------------- Summary ----------------
  std::cout << "\n===== Summary =====\n";
  std::cout << "Input file                 : " << inputFile << "\n";
  std::cout << "Cut file                   : " << cutFile << " (tree=" << cutTree << ")\n";
  std::cout << "Tree                       : " << treeName << "\n";
  std::cout << "Total hits (entries)       : " << n << "\n";
  std::cout << "Unique trigger-times (keys): " << mult.size() << "\n";
  std::cout << "Multiplicity counts (#keys):\n";
  for (const auto &kv : multiplicity_counts) {
    int m = kv.first;
    Long64_t ng = kv.second;
    if (m == 3) std::cout << "  mult>=3 : " << ng << "\n";
    else        std::cout << "  mult=" << m << "  : " << ng << "\n";
  }
  std::cout << "Skipped (mult!=2)          : " << skipped_mult << "\n";
  std::cout << "Skipped (bad detID)        : " << skipped_det << "\n";
  std::cout << "Skipped (qsump<=cut)       : " << skipped_qcut << "\n";
  std::cout << "Skipped (qlow/qupp=0)      : " << skipped_zero << "\n";
  std::cout << "Saved hits (mult==2)       : " << saved_hits << "\n";
  std::cout << "Saved groups (mult==2)     : " << (saved_hits / 2) << "\n";
  std::cout << "Output file                : " << outputFile << "\n";
  std::cout << "===================\n\n";
}

