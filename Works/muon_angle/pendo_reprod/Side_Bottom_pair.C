
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TMath.h>
#include <vector>
#include <cmath>
#include <iostream>

struct HitRec {
  int    id;
  double t;      // ptime for that hit
  double x, y, z;
  double q;      // qsump (charge) for that hit
};

// EDIT THESE if your ID layout differs
static inline bool is_side(int id) {
  // vertical walls
  return (id < 48) || (id >= 65 && id < 113);
}
static inline bool is_bottom(int id) {
  // bottom pads
  return ( (id >= 48 && id <= 58) || (id >= 113 && id <= 123) );
}

void Side_Bottom_pair(double time_equal_tol = 1e-9)
{
  // -------- input --------
  TChain chain("tree");
  chain.Add("hit_positions.root");

  int    detID;
  double ptime, qsump;
  double x_hit, y_hit, z_hit;

  chain.SetBranchAddress("detID", &detID);
  chain.SetBranchAddress("ptime", &ptime);
  chain.SetBranchAddress("qsump", &qsump);
  chain.SetBranchAddress("x_hit", &x_hit);
  chain.SetBranchAddress("y_hit", &y_hit);
  chain.SetBranchAddress("z_hit", &z_hit);

  Long64_t n = chain.GetEntries();
  std::cout << "[INFO] Total hits in input: " << n << "\n";
  if (n == 0) return;

  // -------- output --------
  TFile outFile("side_bottomPair.root","RECREATE");
  TTree outTree("tree","Side-Bottom down-going pairs");

  // Pair kinematics (side -> bottom)
  double theta=0, phi=0;

  // Per-hit info
  int    id_side=0, id_bottom=0;
  double x1=0,y1=0,z1=0;   // side
  double x2=0,y2=0,z2=0;   // bottom
  double q_side=0, q_bottom=0;
  double t_side=0, t_bottom=0;

  // Convenience
  double ptime_pair=0;                // in your DAQ should equal t_side == t_bottom
  double qsum_pair=0;                 // q_side + q_bottom

  outTree.Branch("theta",&theta);
  outTree.Branch("phi",&phi);
  outTree.Branch("id_side",&id_side);
  outTree.Branch("id_bottom",&id_bottom);
  outTree.Branch("x1",&x1); outTree.Branch("y1",&y1); outTree.Branch("z1",&z1);
  outTree.Branch("x2",&x2); outTree.Branch("y2",&y2); outTree.Branch("z2",&z2);
  outTree.Branch("q_side",&q_side);
  outTree.Branch("q_bottom",&q_bottom);
  outTree.Branch("t_side",&t_side);
  outTree.Branch("t_bottom",&t_bottom);
  outTree.Branch("ptime_pair",&ptime_pair);
  outTree.Branch("qsum_pair",&qsum_pair);

  // -------- grouping loop by EXACT (or near-exact) ptime --------
  Long64_t i = 0;
  Long64_t n_groups=0, n_exactSB=0, n_written=0, n_reject_up=0, n_reject_mult=0;

  while (i < n) {
    chain.GetEntry(i);
    const double t0 = ptime;

    // collect all hits with ptime == t0 (within tiny tolerance, just in case)
    std::vector<HitRec> group;
    while (i < n) {
      chain.GetEntry(i);
      if (std::fabs(ptime - t0) > time_equal_tol) break; // next event
      group.push_back({detID, ptime, x_hit, y_hit, z_hit, qsump});
      ++i;
    }
    ++n_groups;
    if (group.empty()) continue;

    // separate side and bottom
    std::vector<HitRec> sides, bottoms;
    for (const auto& h : group) {
      if (is_side(h.id))   sides.push_back(h);
      if (is_bottom(h.id)) bottoms.push_back(h);
    }

    // Require EXACTLY one side and EXACTLY one bottom in the group
    if (sides.size() != 1 || bottoms.size() != 1) { ++n_reject_mult; continue; }
    ++n_exactSB;

    const HitRec& s = sides[0];
    const HitRec& b = bottoms[0];

    // Enforce strictly down-going: side is higher Z than bottom
    if (!(s.z > b.z)) { ++n_reject_up; continue; }

    // Build direction from side -> bottom
    const double dx = s.x - b.x;
    const double dy = s.y - b.y;
    const double dz = s.z - b.z;
    const double r  = std::sqrt(dx*dx + dy*dy + dz*dz);
    if (r <= 0) continue;

    theta = std::acos(dz / r) * 180.0 / TMath::Pi();
    phi   = std::atan2(dy, dx) * 180.0 / TMath::Pi();
    if (phi < 0) phi += 360.0;

    // Fill per-hit branches
    id_side   = s.id;      id_bottom = b.id;
    x1=s.x; y1=s.y; z1=s.z;
    x2=b.x; y2=b.y; z2=b.z;
    q_side    = s.q;       q_bottom  = b.q;
    t_side    = s.t;       t_bottom  = b.t;

    // Convenience
    ptime_pair = t0;                 // should match both t_side/t_bottom in your DAQ
    qsum_pair  = q_side + q_bottom;

    outTree.Fill();
    ++n_written;
  }

  outFile.cd();
  outTree.Write();
  outFile.Close();

  std::cout << "[DONE] groups: " << n_groups
            << "  exact SB: " << n_exactSB
            << "  written: " << n_written
            << "  rejected (upward): " << n_reject_up
            << "  rejected (≠1 side or ≠1 bottom): " << n_reject_mult
            << "  tol=" << time_equal_tol << "\n";
}
