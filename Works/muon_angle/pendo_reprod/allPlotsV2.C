void allPlotsV2() {
    // 1. SETTINGS
    double efficiencyFactor = 10511.0 / 34833.0; 
    double liveTimeSeconds = 3522441.6; // 6 weeks

    // 2. LOAD DATA
    TChain *tData = new TChain("tree");
    //tData->Add("../gamma2/side_sidePairUpdate.root");
    tData->Add("./side_sidePair.root");
    tData->Add("./side_bottomPair.root");
    // tData->Add("../gamma3_perTrgTime/side_sidePairNew.root");
    //tData->Add("../gamma3_perTrgTime/side_bottomPair.root");
    /*
    // 3. LOAD SIMULATION
    TFile *fSim = new TFile("coins.root", "READ");
    TTree *tSim = (TTree*)fSim->Get("tree"); // Ensure this matches your simTree name
    */
    
    // 4. HISTOGRAMS
    //    TH1D *hPhiData = new TH1D("hPhiData", "Azimuth Comparison; #phi [deg]; Rate [Hz/deg]", 72, 0, 360);
    TH1D *hPhiData = new TH1D("hPhiData", "Azimuth Comparison; #phi [deg]; Rate [Hz/deg]", 90, 0, 360);
    TH1D *hPhiSim  = new TH1D("hPhiSim",  "Sim Azimuth", 72, 0, 360);
    TH1D *hThetaData = new TH1D("hThetaData", "Zenith Comparison; #theta [deg]; Rate [Hz/deg]", 45, 0, 90);
    TH1D *hThetaSim  = new TH1D("hThetaSim",  "Sim Zenith", 45, 0, 90);
    TH1D *hCosData = new TH1D("hCosData", "cos(#theta) Comparison; cos(#theta); Rate [Hz/unit]", 45, 0, 1);
    TH1D *hCosSim  = new TH1D("hCosSim",  "Sim cos(#theta)", 50, 0, 1);
    
    TH2D *h2D_PhiCos = new TH2D("h2D_PhiCos", "Data: #phi vs cos(#theta); cos(#theta); #phi [deg]", 45, 0, 1, 72, 0, 360);
    TH2D *h2D_ThetaPhi = new TH2D("h2D_ThetaPhi", "Data: #theta vs #phi; #phi [deg]; #theta [deg]", 72, 0, 360, 45, 0, 90);

    // 5. FILL DATA (Smooth Gaussian/Sigmoid Warp)
    double phiD, thetaD;
    tData->SetBranchAddress("phi", &phiD);
    tData->SetBranchAddress("theta", &thetaD);

    int evt = tData->GetEntries();
    cout << "number of entries: " << evt << endl;

    for(int i=0; i<tData->GetEntries(); i++) {
        tData->GetEntry(i);
	/*
        // Step A: Baseline shift
        double corrPhiD = phiD - 8.0; 

        // Step B: Smooth Warp logic
        // We use a Gaussian-style "push" to shift the middle section
        // Center at 160 means the maximum shift happens around the valley
        double shiftAmplitude = 42.0; 
        double shiftCenter = 165.0;
        double shiftWidth = 55.0; // Controls how "spread out" the stretch is
        
        double smoothBoost = shiftAmplitude * TMath::Gaus(corrPhiD, shiftCenter, shiftWidth);
        corrPhiD += smoothBoost;

        // Step C: Wrap-around
        while(corrPhiD >= 360.0) corrPhiD -= 360.0;
        while(corrPhiD < 0)      corrPhiD += 360.0;
	*/

	double corrPhiD = phiD;
	
        hPhiData->Fill(corrPhiD);
        hThetaData->Fill(thetaD);
        hCosData->Fill(TMath::Cos(thetaD * TMath::DegToRad()));
        
        // 2D Plots now fill without vertical gaps
        h2D_PhiCos->Fill(TMath::Cos(thetaD * TMath::DegToRad()), corrPhiD);
        h2D_ThetaPhi->Fill(corrPhiD, thetaD);
    }
    /*
    // 6. FILL SIMULATION
    double phiS, thetaS, rateS;
    tSim->SetBranchAddress("phi", &phiS);
    tSim->SetBranchAddress("theta", &thetaS);
    tSim->SetBranchAddress("rate", &rateS);

    for(int i=0; i<tSim->GetEntries(); i++) {
        tSim->GetEntry(i);
        double cosS = TMath::Cos(thetaS * TMath::DegToRad());
        double weight = rateS * efficiencyFactor;

        hPhiSim->Fill(phiS, weight);
        hThetaSim->Fill(thetaS, weight);
        hCosSim->Fill(cosS, weight);
    }
    */
    // 7. NORMALIZATION & STYLING
    //    auto StyleSim = [](TH1D* h) { h->SetLineColor(kRed); h->SetLineWidth(3); h->Scale(1.0, "width"); };
    auto StyleData = [](TH1D* h, double time) { 
        h->Scale(1.0 / time); 
        h->Scale(1.0, "width"); 
        h->SetMarkerStyle(20); h->SetMarkerColor(kBlue); h->SetLineColor(kBlue); 
    };

    //  StyleSim(hPhiSim); StyleSim(hThetaSim); StyleSim(hCosSim);
    StyleData(hPhiData, liveTimeSeconds); StyleData(hThetaData, liveTimeSeconds); StyleData(hCosData, liveTimeSeconds);

    // 8. DRAWING (Omitted for brevity, keep your existing Drawing code)
     TCanvas *c1 = new TCanvas("c1", "Azimuth Overlay", 900, 700);
     hPhiData->Draw("E1"); hPhiSim->Draw("HIST SAME");

    TCanvas *c2 = new TCanvas("c2", "Zenith Overlay", 800, 700);
    hThetaData->GetYaxis()->SetTitleOffset(1.3); 
    hThetaData->Draw("E1");
    //    hThetaSim->Draw("HIST SAME");

    TCanvas *c3 = new TCanvas("c3", "CosTheta Overlay", 800, 700); 
    hCosData->Draw("E1");
    //   hCosSim->Draw("HIST SAME");

    // 2D Plots
    gStyle->SetOptStat(0);
    TCanvas *c4 = new TCanvas("c4", "2D: Phi vs CosTheta", 800, 700);
    h2D_PhiCos->Draw("COLZ");

    TCanvas *c5 = new TCanvas("c5", "2D: Theta vs Phi", 800, 700);
    h2D_ThetaPhi->Draw("COLZ");

    //  TFile *outf = TFile::Open("plots1.root", "RECREATE");
 
    // outf->cd();

    // hThetaData->Write();
    // hPhiData->Write();
    // hCosData->Write();
    // h2D_ThetaPhi->Write();

    // outf->Close();
}
