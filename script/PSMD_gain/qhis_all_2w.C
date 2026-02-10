R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(../lib/libRawObjs)



void qhis_all()
{

  //gStyle->SetOptFit(1);
  //gStyle->SetOptStat(0);
  
  int run = 330;
  int evt, det;
  int fid[124];
  int tch = 124*4;
  int tdet = 124;
  //  int tch = 88;//bot
  //  int tdet = 22;//bot
  double q[tdet][4], q_cal[tdet][4];

  double gain_ref, gain[4];

  TChain * chain = new TChain("psmd");
  //  chain->Add(Form("spetree_%06d.root", run));
  //  chain->Add(Form("/home/kkw/PROD/PSMD/%06d/prd_psd_daq1_%06d_00000.root", run, run));  
  //  chain->Add(Form("/home/kkw/PROD/PSMD/%06d/prd_psd_daq2_%06d_00000.root", run, run));  
  chain->Add(Form("./data/%06d/psmd_%06d.root", run, run));  
  
  //  chain->SetBranchAddress("evt", &evt);
  //  chain->SetBranchAddress("id", &id);
  //  chain->SetBranchAddress("q", &q);

  TFile * out = new TFile(Form("./gain/qhis_%06d_all.root",run), "recreate");
  TTree * tree = new TTree("gain", "");

  //  tree->Branch("det", &det, "det/I");
  //  tree->Branch("gain", gain, "gain[4]/D");
 
  TH1D *qhis[tdet][4], *qcal[tdet][4];

  //  for(int i = 48; i <= 58; i++) {//48-58, 113-123
  for(int i = 0; i <= 123; i++) {//48-58, 113-123    
    for(int j=0; j<4; j++){
      qhis[i][j] = new TH1D(Form("qhis_d%i_c%i", i, j+1), "", 1100, -100, 1000);
      qcal[i][j] = new TH1D(Form("qcal_d%i_c%i", i, j+1), "", 1100, -100, 1000);
    }
  }
  
  TH1D *gainhis = new TH1D("gainhis", "", 50, 20, 180);
  TH1D *gainhis2 = new TH1D("gainhis2", "", 50, 20, 180);

  int tevt = chain->GetEntries();
  cout << "Tatal number of entries: " << tevt << endl;
  
  for(int i = 0; i < tevt; i++){
      //  for(int i = 0; i < 100; i++){
    chain->GetEntry(i);

    if (i > 0 && i % 100000 == 0)
      cout << Form("%i/%i events processed ......", i, tevt) << endl;

    //bot det 48 - 58
    for(int j=0; j<=123; j++){
      fid[j] = chain->GetLeaf("trgdet")->GetValue(j);
      for(int k=0; k<4; k++){
	q[j][k] = chain->GetLeaf("iq")->GetValue(j*4+k);
	qhis[j][k]->Fill(q[j][k]);
      }
    }
    /*
    //bot 113-123
    for(int j=113; j<=123; j++){
      fid[j] = chain->GetLeaf("trgdet")->GetValue(j-5);
      for(int k=0; k<4; k++){
	q[j][k] = chain->GetLeaf("iq")->GetValue((j-5)*4+k);
	qhis[j][k]->Fill(q[j][k]);
      }
    }
    */
  }
  /*  
  //--------------------------------first fit----------------------
  int maxbin[4];
  double maxx[4], upper[4], lower[4], std[4], cons[4], stdv[4];

  for(int i = 0; i < tdet; i++) {

    if((i>=48&&i<=58)||(i>=113&&i<=123)){

      for(int j=0; j<4; j++){
	maxbin[j] = qhis[i][j]->GetMaximumBin();
	maxx[j] = qhis[i][j]->GetBinCenter(maxbin[j]);
	std[j] = qhis[i][j]->GetStdDev();

	upper[j] = maxx[j] + std[j];
	lower[j] = maxx[j] - std[j];

	TF1 *f1 = new TF1("f1", "gaus", lower[j], upper[j]);
    
	qhis[i][j]->Fit(f1, "RQ0");
    
	cons[j] = f1->GetParameter(0);
	gain[j] = f1->GetParameter(1);
	stdv[j] = f1->GetParameter(2);
      

	upper[j] = maxx[j] + 1.5*stdv[j];
	lower[j] = maxx[j] - 1.5*stdv[j];
      
	TF1 *f2 = new TF1("f2", "gaus", lower[j], upper[j]);

	f2->SetParameter(0, cons[j]);
	f2->SetParameter(1, gain[j]);
	f2->SetParameter(2, stdv[j]);
    
	qhis[i][j]->Fit(f2, "RQ");
    
	gain[j] = f2->GetParameter(1);
    
	//	cout << " fid : ch : gain: " << fid[i] << ": " << j+1<<" : " <<gain[j] << endl;

	//    gainhis->Fill(gain);
      }
      det=fid[i];
      tree->Fill();


      
    //cout <<"gain1: " << i+1 << ": " << cons << ": " << gain1 << ": " << stdv << endl;   
    }
  }
  */
  //    tree->Write();

  for(int i = 0; i < tdet; i++) {
    //    if((i>=48&&i<=58)||(i>=113&&i<=123)){
      for(int j=0; j<4; j++){
	qhis[i][j]->Write();
      }
      //    }
  }
    out->Close();
    /*
   TCanvas *can1 = new TCanvas("can1", "", 800, 800);
   can1->cd();
   gainhis->Draw();
    */
}
