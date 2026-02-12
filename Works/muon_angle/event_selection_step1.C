#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>
#include <iostream>
#include <map>
#include <iomanip> 
#include <fstream>

void event_selection_step1() {

  //qcut txt read
  const int tdet = 124;
  int det[tdet];
  double qcut[tdet];

    std::ifstream file("cut.txt");



    for (int i = 0; i < tdet; ++i) {
        // 파일에서 두 숫자를 순서대로 읽어 x와 y 어레이에 저장
        if (!(file >> det[i] >> qcut[i])) break; 
    }

    file.close();

    // 확인용 출력 (첫 번째 데이터)
    //    printf("First data: x[0] = %i, y[0] = %f\n", idet[115], qcut[115]);


    
  
  // Open geometry file
  TFile *fGeom = TFile::Open("coordinate.root");
  TTree *treeGeom = (TTree*)fGeom->Get("tree");

  int detID, outID;
  double outXc, outYc, outZc;
  double xc, yc, zc, A_min, A_max;

  double ix[127]={0}; double iy[127]={0}; double iz[127]={0};
  double minA[127]={0}; double  maxA[127]={0};
  int idet[127];
  
  int nGeom = treeGeom->GetEntries();
  for (int i = 0; i < nGeom; ++i) {
    treeGeom->GetEntry(i);
    //   cout<<"idet "<<detID<<endl;
    detID=treeGeom->GetLeaf("detID")->GetValue();
    xc=treeGeom->GetLeaf("xc")->GetValue();
    yc=treeGeom->GetLeaf("yc")->GetValue();
    zc=treeGeom->GetLeaf("zc")->GetValue();
    A_min=treeGeom->GetLeaf("A_min")->GetValue();
    A_max=treeGeom->GetLeaf("A_max")->GetValue();    
	idet[detID]=detID;
    ix[detID]=xc;
    iy[detID]=yc;
    iz[detID]=zc;
    minA[detID]=A_min;
    maxA[detID]=A_max;
  }

  //  cout<<idet[0]<<" "<<ix[0]<<" "<<iy[0]<<" "<<iz[0]<<" "<<minA[0]<<" "<<maxA[0];
  //  cout<<idet[10]<<" : "<<ix[10]<<": "<<iy[10]<<": "<<iz[10]<<": "<<minA[10]<<": "<<maxA[10]<<endl;

  
  
  TChain *chain = new TChain("phit");
  chain->Add("psmd_2hit.root");

  int trgdet[2], trp[2];
  double qsum[2];
  double iq[2][4];
  double avas[2];
  int grp[2];
  chain->SetBranchAddress("trgdet", trgdet);
  chain->SetBranchAddress("qsum", qsum);
  chain->SetBranchAddress("grp", grp);

  //  chain->SetBranchAddress("iq", iq);

  // Output tree
  TFile *fout = new TFile("psmd_2hit_sel.root", "RECREATE");
  TTree *outTree = new TTree("tree", "Computed hit positions");

  double x_hit[2], y_hit[2], z_hit[2], dp[2];
  outTree->Branch("trgdet", trgdet, "trgdet[2]/I");
  //  outTree->Branch("ptime", &ptime, "ptime/D");
  outTree->Branch("qsum", qsum, "qsum[2]/D");
  outTree->Branch("x_hit", x_hit, "x_hit[2]/D");
  outTree->Branch("y_hit", y_hit, "y_hit[2]/D");
  outTree->Branch("z_hit", z_hit, "z_hit[2]/D");
  outTree->Branch("grp", grp, "grp[2]/I");
    outTree->Branch("avas", avas, "avas[2]/D");
    outTree->Branch("dp", dp, "dp[2]/D");    
  
  // Dimensions
  double L = 168.0;
  double W = 30.0;   // height or width depending on orientation
  double T = 6.0;   // thickness

  TRandom3 randGen(0);

  int nEvent = chain->GetEntries();
  cout << "Total entries: " << nEvent << endl;
  
    for (int i = 0; i < nEvent; ++i) {
      if(i%10000==0){cout<<i<<" events process..."<<endl;}
      //  for (int i = 0; i < 1; ++i) {
    chain->GetEntry(i);
    if (fabs(maxA[trgdet[0]] - minA[trgdet[0]]) < 1e-6) continue;
    if (fabs(maxA[trgdet[1]] - minA[trgdet[1]]) < 1e-6) continue;

    //1hit cut
    
    if(qsum[0] < qcut[trgdet[0]]) continue;
    if(qsum[1] < qcut[trgdet[1]]) continue;
    if(grp[0]==grp[1]) continue;

    
    for(int j=0; j<2; j++){
      for(int k=0; k<4; k++){
	iq[j][k]=chain->GetLeaf("iq")->GetValue(j*4+k);
      }
      avas[j]=(((iq[j][0]-iq[j][1])/(iq[j][0]+iq[j][1]))+((iq[j][2]-iq[j][3])/(iq[j][2]+iq[j][3])))/2;
      //      cout<<trgdet[j]<<endl;
      //      cout<<avas[j]<<endl;
    
    dp[j] = (avas[j] - minA[trgdet[j]]) * L / (maxA[trgdet[j]] - minA[trgdet[j]]);	
    //    cout<<"dp "<<dp[j]<<endl;
    //    cout<<"minA "<<minA[trgdet[j]]<<endl;
    //grp 1, 4
    //    if ((trgdet[j] >= 0 && trgdet[j] <= 11) || (trgdet[j] >= 36 && trgdet[j] <= 47)) {
    if(grp[j]==1||grp[j]==4){
      x_hit[j] = (ix[trgdet[j]] + 0.5 * L) - dp[j];
      y_hit[j] = iy[trgdet[j]] + (randGen.Uniform() - 0.5) * T;
      z_hit[j] = iz[trgdet[j]] + (randGen.Uniform() - 0.5) * W;
    }

    //grp 5, 8
    //    if ((detID >= 65 && detID <= 76) || (detID >= 101 && detID <= 112)) {
    else if(grp[j]==5||grp[j]==8){
      x_hit[j] = (ix[trgdet[j]] - 0.5 * L) + dp[j];
      y_hit[j] = iy[trgdet[j]] + (randGen.Uniform() - 0.5) * T;
      z_hit[j] = iz[trgdet[j]] + (randGen.Uniform() - 0.5) * W;
    }
    //grp 2, 7
    //    else if ((detID >= 12 && detID <= 23) || (detID >= 89 && detID <= 100)) {
    else if(grp[j]==2||grp[j]==7){
      x_hit[j] = ix[trgdet[j]] + (randGen.Uniform() - 0.5) * T;
      y_hit[j] = (iy[trgdet[j]] + 0.5 * L) - dp[j];
      z_hit[j] = iz[trgdet[j]] + (randGen.Uniform() - 0.5) * W;
     }

    //grp 3 6
    //    else if ((detID >= 24 && detID <= 35) || (trgdet[j] >= 77 &&  trgdet[j]<= 88)) {
    else if(grp[j]==3||grp[j]==6){
      x_hit[j] = ix[trgdet[j]] + (randGen.Uniform() - 0.5) * T;
      y_hit[j] = (iy[trgdet[j]] - 0.5 * L) + dp[j];
      z_hit[j] = iz[trgdet[j]] + (randGen.Uniform() - 0.5) * W;
     }
    //bot grp 9
    //    else if(trgdet[j] >= 48 && trgdet[j] <= 58){
    else if(grp[j]==9){
      x_hit[j] = ix[trgdet[j]] + (randGen.Uniform() - 0.5) * W;
      y_hit[j] = (iy[trgdet[j]] + 0.5 * L) - dp[j];
      z_hit[j] = iz[trgdet[j]] + (randGen.Uniform() - 0.5) * T;
    }

    //grp 10
    else {
      x_hit[j] = ix[trgdet[j]] + (randGen.Uniform() - 0.5) * W;
      y_hit[j] = (iy[trgdet[j]] - 0.5 * L) + dp[j];
      z_hit[j] = iz[trgdet[j]] + (randGen.Uniform() - 0.5) * T;
    }//det113~123

    }

    if(z_hit[0]<z_hit[1]){
      std::swap(trgdet[0], trgdet[1]);
      std::swap(qsum[0], qsum[1]);
      std::swap(x_hit[0], x_hit[1]);
      std::swap(y_hit[0], y_hit[1]);
      std::swap(z_hit[0], z_hit[1]);
      std::swap(grp[0], grp[1]);

    }
    
    outTree->Fill();
    }

  outTree->Write();
  fout->Close();

  std::cout << "Hit position tree saved.\n";

}
