void get_angle_step2() {

    TChain *chain = new TChain("tree");
    chain->Add("psmd_2hit_sel.root");

    // Variables from input tree
    int trgdet[2], grp[2];
    //    double ptime;
    double x_hit[2], y_hit[2], z_hit[2], qsum[2];

    // Set branch addresses
    chain->SetBranchAddress("trgdet", trgdet);
    chain->SetBranchAddress("grp", grp);
    chain->SetBranchAddress("qsum", qsum);
    
    chain->SetBranchAddress("x_hit", x_hit);
    chain->SetBranchAddress("y_hit", y_hit);
    chain->SetBranchAddress("z_hit", z_hit);

    // Output file and tree
    TFile *outFile = new TFile("psmd_angle.root", "RECREATE");
    TTree *outTree = new TTree("tree", "Muon direction from two side hits");

    double theta_zenith, phi;
    int id1, id2, g1, g2;
    double x1, y1, z1, x2, y2, z2, qsum1, qsum2;

    outTree->Branch("theta", &theta_zenith);
    outTree->Branch("phi", &phi);
    outTree->Branch("id1", &id1);
    outTree->Branch("id2", &id2);
    outTree->Branch("x1", &x1); outTree->Branch("y1", &y1); outTree->Branch("z1", &z1);
    outTree->Branch("x2", &x2); outTree->Branch("y2", &y2); outTree->Branch("z2", &z2);
    outTree->Branch("g1", &g1); outTree->Branch("g2", &g2); 
    outTree->Branch("qsum1", &qsum1); outTree->Branch("qsum2", &qsum2); 

    int ntot = chain->GetEntries();
    std::cout << "Total entries: " << ntot << std::endl;

    for (int i = 0; i < ntot; i++) {      
      //    for (int i = 0; i < 10; i++) {      
      //      cout<<i<<endl;
        chain->GetEntry(i);
	id1=trgdet[0]; id2=trgdet[1];
	x1=x_hit[0];	x2=x_hit[1];
	y1=y_hit[0];	y2=y_hit[1];
	z1=z_hit[0];	z2=z_hit[1];
	g1=grp[0]; g2=grp[1];
		qsum1=qsum[0]; qsum2=qsum[1];
	
	//	if(grp[0]<=8&&grp[1]<=8){

        // Compute direction
         // Direction vector: from side (entry) to side (exit)
	  //	  TVector3 v(x2 - x1, y2 - y1, z2 - z1);
	  TVector3 v(x_hit[1] - x_hit[0], y_hit[1]-y_hit[0], z_hit[1]-z_hit[0]);

	  
	  // Zenith definition: 0° = vertical down, 90° = horizontal
	  theta_zenith = std::acos(-v.Z() / v.Mag()) * 180.0 / TMath::Pi();

	  // Azimuth
	  phi = v.Phi() * 180.0 / TMath::Pi();
	  if (phi < 0) phi += 360;
        outTree->Fill();

	//	}
	
    }

    outFile->cd();
    outTree->Write();
    outFile->Close();

//    std::cout << "Saved side-side muon directions to side_side.root\n";
}
