
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TMath.h>
#include <vector>
#include <cmath>
#include <iostream>

struct HitRec {
  int    id;
  double t;      // ptime
  double x, y, z;
  double q;      // qsump
};

// ---- Side membership helpers (EDIT only if your ID layout changes) ----
static inline bool is_side_id(int id) {
  return ( (id >= 0   && id <= 11)  || (id >= 101 && id <= 112) ||   // side 1
           (id >= 12  && id <= 23)  || (id >= 24  && id <= 35 ) ||   // side 2
           (id >= 36  && id <= 47)  || (id >= 65  && id <= 76 ) ||   // side 3
           (id >= 77  && id <= 88)  || (id >= 89  && id <= 100) );   // side 4
}

static inline int side_group(int id) {
  if ((id >= 0   && id <= 11)  || (id >= 101 && id <= 112)) return 1; // side 1
  if ((id >= 12  && id <= 23)  || (id >= 24  && id <= 35 )) return 2; // side 2
  if ((id >= 36  && id <= 47)  || (id >= 65  && id <= 76 )) return 3; // side 3
  if ((id >= 77  && id <= 88)  || (id >= 89  && id <= 100)) return 4; // side 4
  return -1;
}

void Side_Side_pair(double time_equal_tol = 1e-9)
{
  // ---------- Input ----------
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
  std::cout << "[INFO] Total hits: " << n << "\n";
  if (n == 0) return;

  // ---------- Output ----------
  TFile outFile("side_sidePair.root","RECREATE");
  TTree outTree("tree","Muon direction from two side hits (down-going)");

  double theta=0, phi=0;
  int    id_side1=0, id_side2=0;        // side1: higher z   side2: lower z
  double x1=0,y1=0,z1=0, x2=0,y2=0,z2=0;
  double q_side1=0, q_side2=0;
  double t_side1=0, t_side2=0;
  double ptime_pair=0, qsum_pair=0;

  outTree.Branch("theta",&theta);
  outTree.Branch("phi",&phi);
  outTree.Branch("id_side1",&id_side1);
  outTree.Branch("id_side2",&id_side2);
  outTree.Branch("x1",&x1); outTree.Branch("y1",&y1); outTree.Branch("z1",&z1);
  outTree.Branch("x2",&x2); outTree.Branch("y2",&y2); outTree.Branch("z2",&z2);
  outTree.Branch("q_side1",&q_side1);
  outTree.Branch("q_side2",&q_side2);
  outTree.Branch("t_side1",&t_side1);
  outTree.Branch("t_side2",&t_side2);
  outTree.Branch("ptime_pair",&ptime_pair);
  outTree.Branch("qsum_pair",&qsum_pair);

  // ---------- Grouping by exact ptime ----------
  Long64_t i = 0;
  Long64_t n_groups=0, n_with2=0, n_written=0, n_sameSide=0, n_upward=0;

  while (i < n) {
    chain.GetEntry(i);
    const double t0 = ptime;

    // collect all hits with ptime == t0 (within tiny tolerance)
    std::vector<HitRec> group;
    while (i < n) {
      chain.GetEntry(i);
      if (std::fabs(ptime - t0) > time_equal_tol) break;
      if (is_side_id(detID)) {
        group.push_back({detID, ptime, x_hit, y_hit, z_hit, qsump});
      }
      ++i;
    }
    ++n_groups;
    if (group.size() != 2) continue;       // strictly 2 side hits at this time
    ++n_with2;

    // Must be from different sides
    int g1 = side_group(group[0].id);
    int g2 = side_group(group[1].id);
    if (g1 == g2 || g1==-1 || g2==-1) { ++n_sameSide; continue; }

    // Down-going: set side1 as higher-z, side2 as lower-z
    HitRec s1 = group[0];
    HitRec s2 = group[1];
    if (s1.z < s2.z) std::swap(s1, s2);    // ensure s1.z >= s2.z
    if (!(s1.z > s2.z)) { ++n_upward; continue; } // equal z or upward → skip

    // Direction from side1 (higher z) to side2 (lower z)
    const double dx = s1.x - s2.x;
    const double dy = s1.y - s2.y;
    const double dz = s1.z - s2.z;
    const double r  = std::sqrt(dx*dx + dy*dy + dz*dz);
    if (r <= 0) continue;

    theta = std::acos(dz / r) * 180.0 / TMath::Pi();
    phi   = std::atan2(dy, dx) * 180.0 / TMath::Pi();
    if (phi < 0) phi += 360.0;

    // Fill outputs
    id_side1 = s1.id;  id_side2 = s2.id;
    x1=s1.x; y1=s1.y; z1=s1.z;
    x2=s2.x; y2=s2.y; z2=s2.z;
    q_side1  = s1.q;   q_side2  = s2.q;
    t_side1  = s1.t;   t_side2  = s2.t;
    ptime_pair = t0;                       // common trigger time
    qsum_pair  = q_side1 + q_side2;

    outTree.Fill();
    ++n_written;
  }

  outFile.cd();
  outTree.Write();
  outFile.Close();

  std::cout << "[DONE] time-groups: " << n_groups
            << "  groups with exactly 2 side hits: " << n_with2
            << "  written: " << n_written << "\n"
            << "  rejected (same side): " << n_sameSide
            << "  rejected (not down-going): " << n_upward
            << "  tol=" << time_equal_tol << "\n";
}
