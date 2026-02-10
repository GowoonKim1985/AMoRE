R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(../lib/libRawObjs)



void ghis_bot_327()
{
  const int tent = 22;
  const int seldet = 14;
  int detnum[seldet]={48, 49, 51, 53, 54, 58, 114, 116, 117, 118, 119, 120, 121, 122};
    
  // good ped det 48, 49, 51,53,
  //gStyle->SetOptFit(1);
  //gStyle->SetOptStat(0);
  
  int run = 327;
  int evt, det;
  int fid[124];
  int tch = 124*4;
  int tdet = 124;
  //  int tch = 88;//bot
  //  int tdet = 22;//bot
  double q[tdet][4], q_cal[tdet][4], gain_cal[tdet][4];
  int trgdet[120]; double iq[120][4];
  double gnom = 100.;
  double gain[4];
  int idet[tent];


  TChain * tree = new TChain("gain");
  //  tree->Add(Form("./gain/gain_%06d.root", run));
  tree->Add("./gain/gain_000330.root");    

  tree->SetBranchAddress("det", &det);
  tree->SetBranchAddress("gain", gain);  

  //  int icnt = 0;
  for(int i=0; i<22; i++){
       
    tree->GetEntry(i);
    idet[i] = tree->GetLeaf("det")->GetValue();
    //    cout<<"det "<<idet[i]<<endl;
    for(int j=0; j<4; j++){
      gain_cal[idet[i]][j]=tree->GetLeaf("gain")->GetValue(j);
      //      cout<<"ch "<<j<<" gain ref " <<gain_cal[idet[i]][j]<<endl;
      }
  }
  

  TChain * psmd = new TChain("psmd");
  psmd->Add(Form("./data/%06d/psmd_%06d.root", run, run)); 
  //  chain->SetBranchAddress("trgdet", trgdet);
  //  chain->SetBranchAddress("iq", iq);  


  TFile * out = new TFile(Form("./gain/ghis_%06d_bot.root",run), "recreate");


  TH1D *ghis[tdet][4];

  //  for(int i = 48; i <= 58; i++) {//48-58, 113-123
  for(int i = 0; i <= 123; i++) {//48-58, 113-123    
    for(int j=0; j<4; j++){
      ghis[i][j] = new TH1D(Form("ghis_d%i_c%i", i, j+1), "", 1100, -100, 1000);
    }
  }

  
  //  TH1D *gainhis = new TH1D("gainhis", "", 50, 20, 180);
  //  TH1D *gainhis2 = new TH1D("gainhis2", "", 50, 20, 180);

  int tevt = psmd->GetEntries();
  cout << "Total number of entries: " << tevt << endl;
  
  for(int i = 0; i < tevt; i++){
    //  for(int i = 393; i < 394; i++){
    psmd->GetEntry(i);

    if (i > 0 && i % 100000 == 0)
      cout << Form("%i/%i events processed ......", i, tevt) << endl;

    //bot det 48 - 58

    //    q_ref = chain->GetLeaf("iq")->GetValue(48*4+0);//det 48 ch0
    //    cout<<"q ref(d48 c1) "<<q_ref<<endl;
      
    for(int j=48; j<=58; j++){
      if((j==48)||(j==49)||(j==51)||(j==53)||(j==54)||(j==58)){
	//	cout<<"j "<<j<<endl;
	fid[j] = psmd->GetLeaf("trgdet")->GetValue(j);
	for(int k=0; k<4; k++){
	  q[j][k] = psmd->GetLeaf("iq")->GetValue(j*4+k);
	  q_cal[j][k] = q[j][k]*(gnom/gain_cal[j][k]);//normalize 100
	  ghis[j][k]->Fill(q_cal[j][k]);
	  //	  cout<<"det "<<fid[j]<<" ch "<<k<<" q "<<q[j][k]<<" -> cal "<<q_cal[j][k]<<endl;
	}
      }
    }

    for(int j=113; j<=123; j++){
      if((j==114)||(j>=116&&j<=122)){
	//		cout<<"j "<<j<<endl;
	fid[j] = psmd->GetLeaf("trgdet")->GetValue(j-5);
	for(int k=0; k<4; k++){
	  q[j][k] = psmd->GetLeaf("iq")->GetValue((j-5)*4+k);
	  q_cal[j][k] = q[j][k]*(gnom/gain_cal[j][k]);//normalize 100
	  ghis[j][k]->Fill(q_cal[j][k]);
	  //	  cout<<"det "<<fid[j]<<" ch "<<k<<" q "<<q[j][k]<<" -> cal "<<q_cal[j][k]<<endl;
	}
      }
    }
    
    
    
  }
  //    tree->Write();

  for(int i = 0; i < tdet; i++) {
    if((i==48)||(i==49)||(i==51)||(i==53)||(i==54)||(i==58)||(i==114)||(i>=116&&i<=122)){
      for(int j=0; j<4; j++){
	ghis[i][j]->Write();
      }
      //    }
    }
  }
    out->Close();


}
