// Modified formula from previous calculations
// modified on Jan 2026 considering which q is near end

#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TRandom3.h>

#include <iostream>
#include <map>
#include <set>
#include <cmath>

struct DetInfo {
  double xc = 0.0;
  double yc = 0.0;
  double zc = 0.0;
  double minAsy = 0.0;
  double maxAsy = 0.0;
};

void hit_pos() {

  // ----------------------------
  // BAD DETECTOR LIST
  // ----------------------------
  std::set<int> badDet = {71,72,73};

  std::map<int,long long> removedPerDet;

  // ----------------------------
  // Load geometry
  // ----------------------------
  TFile *fGeom = TFile::Open("combine.root","READ");
  if(!fGeom || fGeom->IsZombie()){
    std::cerr<<"ERROR opening combine.root\n";
    return;
  }

  TTree *treeGeom = (TTree*)fGeom->Get("tree");

  int geom_detID;
  double geom_xc, geom_yc, geom_zc;
  double geom_minAsy, geom_maxAsy;

  treeGeom->SetBranchAddress("detID",&geom_detID);
  treeGeom->SetBranchAddress("xc",&geom_xc);
  treeGeom->SetBranchAddress("yc",&geom_yc);
  treeGeom->SetBranchAddress("zc",&geom_zc);
  treeGeom->SetBranchAddress("minAsy",&geom_minAsy);
  treeGeom->SetBranchAddress("maxAsy",&geom_maxAsy);

  std::map<int,DetInfo> detMap;

  Long64_t nGeom = treeGeom->GetEntries();

  for(Long64_t i=0;i<nGeom;i++){
    treeGeom->GetEntry(i);

    DetInfo info;
    info.xc = geom_xc;
    info.yc = geom_yc;
    info.zc = geom_zc;
    info.minAsy = geom_minAsy;
    info.maxAsy = geom_maxAsy;

    detMap[geom_detID] = info;
  }

  std::cout<<"Loaded geometry for "<<detMap.size()<<" detectors\n";


  // ----------------------------
  // INPUT DATA
  // ----------------------------
  TChain *chain = new TChain("tree");
  chain->Add("selected_mult2_cuts.root");

  Long64_t nEvent = chain->GetEntries();
  std::cout<<"Total entries: "<<nEvent<<"\n";

  int in_detID;
  double in_ptime,in_qsump,in_av_asy;
  double in_Q1,in_Q2,in_Q3,in_Q4;

  chain->SetBranchAddress("detID",&in_detID);
  chain->SetBranchAddress("ptime",&in_ptime);
  chain->SetBranchAddress("qsump",&in_qsump);
  chain->SetBranchAddress("av_asy",&in_av_asy);
  chain->SetBranchAddress("Q1",&in_Q1);
  chain->SetBranchAddress("Q2",&in_Q2);
  chain->SetBranchAddress("Q3",&in_Q3);
  chain->SetBranchAddress("Q4",&in_Q4);


  // ----------------------------
  // OUTPUT
  // ----------------------------
  TFile *fout = new TFile("hit_positions.root","RECREATE");

  TTree *outTree = new TTree("tree","Computed hit positions");

  int detID;
  double ptime,qsump,av_asy;
  double x_hit,y_hit,z_hit;
  double Q1,Q2,Q3,Q4;

  outTree->Branch("detID",&detID,"detID/I");
  outTree->Branch("ptime",&ptime,"ptime/D");
  outTree->Branch("qsump",&qsump,"qsump/D");
  outTree->Branch("av_asy",&av_asy,"av_asy/D");
  outTree->Branch("x_hit",&x_hit,"x_hit/D");
  outTree->Branch("y_hit",&y_hit,"y_hit/D");
  outTree->Branch("z_hit",&z_hit,"z_hit/D");
  outTree->Branch("Q1",&Q1,"Q1/D");
  outTree->Branch("Q2",&Q2,"Q2/D");
  outTree->Branch("Q3",&Q3,"Q3/D");
  outTree->Branch("Q4",&Q4,"Q4/D");


  const double L = 168.0;
  const double W = 30.5;
  const double T = 6.0;

  TRandom3 randGen(0);

  Long64_t kept=0;

  // ----------------------------
  // EVENT LOOP
  // ----------------------------
  for(Long64_t i=0;i<nEvent;i++){

    chain->GetEntry(i);

    detID = in_detID;

    // ----------------------------
    // REMOVE BAD DETECTORS
    // ----------------------------
    if(badDet.count(detID)){
      removedPerDet[detID]++;
      continue;
    }

    ptime = in_ptime;
    qsump = in_qsump;
    av_asy = in_av_asy;

    Q1 = in_Q1;
    Q2 = in_Q2;
    Q3 = in_Q3;
    Q4 = in_Q4;

    auto it = detMap.find(detID);
    if(it==detMap.end()) continue;

    const DetInfo &info = it->second;

    if(std::fabs(info.maxAsy-info.minAsy)<1e-9) continue;

    double dp = (info.maxAsy-av_asy)*L/(info.maxAsy-info.minAsy);

    if(dp<0) dp=0;
    if(dp>L) dp=L;


    if((detID>=65 && detID<=76) || (detID>=101 && detID<=112)){
      x_hit = (info.xc-0.5*L)+dp;
      y_hit = info.yc+(randGen.Uniform()-0.5)*T;
      z_hit = info.zc+(randGen.Uniform()-0.5)*W;
    }

    else if((detID>=0 && detID<=11) || (detID>=36 && detID<=47)){
      x_hit = (info.xc+0.5*L)-dp;
      y_hit = info.yc+(randGen.Uniform()-0.5)*T;
      z_hit = info.zc+(randGen.Uniform()-0.5)*W;
    }

    else if((detID>=12 && detID<=23) || (detID>=89 && detID<=100)){
      y_hit = (info.yc-0.5*L)+dp;
      x_hit = info.xc+(randGen.Uniform()-0.5)*T;
      z_hit = info.zc+(randGen.Uniform()-0.5)*W;
    }

    else if((detID>=24 && detID<=35) || (detID>=77 && detID<=88)){
      y_hit = (info.yc+0.5*L)-dp;
      x_hit = info.xc+(randGen.Uniform()-0.5)*T;
      z_hit = info.zc+(randGen.Uniform()-0.5)*W;
    }

    else if(detID>=113 && detID<=123){
      y_hit = (info.yc-0.5*L)+dp;
      x_hit = info.xc+(randGen.Uniform()-0.5)*W;
      z_hit = info.zc+(randGen.Uniform()-0.5)*T;
    }

    else{
      y_hit = (info.yc+0.5*L)-dp;
      x_hit = info.xc+(randGen.Uniform()-0.5)*W;
      z_hit = info.zc+(randGen.Uniform()-0.5)*T;
    }

    outTree->Fill();
    kept++;
  }

  fout->cd();
  outTree->Write();
  fout->Close();
  fGeom->Close();

  std::cout<<"\nHit position tree saved: hit_positions.root\n";
  std::cout<<"Kept entries: "<<kept<<"\n";

  std::cout<<"\nRemoved events per bad detector:\n";
  for(auto &p:removedPerDet)
    std::cout<<"detID "<<p.first<<" removed "<<p.second<<" events\n";
}
