R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void hist_psmd_bot(){
  int runnum = 330;
  
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_psd");
  //  chain -> Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq1_%06d_*", runnum, runnum));
  //  chain -> Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq2_%06d_*", runnum, runnum));  
  chain -> Add("/home/PROD/PSMD/000327/prd_psd_daq1_000327_*");
  chain -> Add("/home/PROD/PSMD/000327/prd_psd_daq2_000327_*");  
  chain -> Add("/home/PROD/PSMD/000330/prd_psd_daq1_000330_*");
  chain -> Add("/home/PROD/PSMD/000330/prd_psd_daq2_000330_*");  
  //chain -> Add("./test_psd.root");
  
  PSEvent *pevt = new PSEvent();
  chain -> SetBranchAddress("PSEvent", &pevt);

  //psmd fid grp 1: 0~11 / 2: 12~23 / 3: 23~35 / 4:36~47 / 5(bot):48~58 / 6: 65~76 / 7:77~88 / 8: 89~100 / 9: 101~112 / 10 : 113~123(bot) 

  TH1D * Qahist[124][4];
  TH1D * Qsumhist[124];
  TString hisname;
  double qsum, qa[4];
  
  for(int i = 0; i < 124; i++){
    Qsumhist[i] = new TH1D(Form("hqsum_%d", i), "", 200, 0, 50000);
    for(int j = 0; j < 4; j++){
      Qahist[i][j] = new TH1D(Form("hqa%d_ch%d", i,j), "", 300, 0, 30000);
    }
  }
  

  
  int nevt = chain -> GetEntries();
  //  cout << nevt << endl;

  for(int i = 0; i < nevt; i++){
    //  for(int i = 0; i < 10; i++){

    chain -> GetEntry(i);
    if(i != 0 && i%1000000 == 0) {cout << i <<"/"<<nevt << endl;}

    int ndata = chain->GetLeaf("fColl.fID")->GetNdata();

    //    cout<<ndata<<endl;
    for(int j=0; j<ndata; j++){
      int fid = chain->GetLeaf("fColl.fID")->GetValue(j);
      qsum = chain->GetLeaf("fColl.fQsum")->GetValue(j);
      Qsumhist[fid] ->Fill(qsum);
      for(int k=0; k<4; k++){
	qa[k]= chain->GetLeaf("fColl.fQ")->GetValue(j*4 + k);
	Qahist[fid][k]->Fill(qa[k]);
      }
    }
  }

  

  
    //psmd fid grp 1: 0~11 / 2: 12~23 / 3: 24~35 / 4:36~47 / 5(bot):48~58 / 6: 65~76 / 7:77~88 / 8: 89~100 / 9: 101~112 / 10 : 113~123(bot) 


      TFile of5("./hist/qhist_psmd_bot.root", "recreate");
    for(int i=48; i<=58; i++){
      Qsumhist[i]->SetName(Form("hqsum_%d",i-48));
      Qsumhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->SetName(Form("hqa_%d_ch%d",i-48,j));
	Qahist[i][j]->Write();
      }
    }
    for(int i=113; i<=123; i++){
      Qsumhist[i]->SetName(Form("hqsum_%d",i-113+11));
      Qsumhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->SetName(Form("hqa_%d_ch%d",i-113+11,j));
	Qahist[i][j]->Write();
      }
    }
   
      of5.Close();

      /*  
    TFile of5(Form("./hist/%06d/qhist_psmd_b1.root",runnum), "recreate");
    for(int i=48; i<=58; i++){
      Qsumhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
      of5.Close();


    TFile of10(Form("./hist/%06d/qhist_psmd_b2.root",runnum), "recreate");
    for(int i=113; i<=123; i++){
      Qsumhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
      of10.Close();
      */
  
 
    //psmd fid grp 1: 0~11 / 2: 12~23 / 3: 24~35 / 4:36~47 / 5(bot):48~58 / 6: 65~76 / 7:77~88 / 8: 89~100 / 9: 101~112 / 10 : 113~123(bot) 

  }

