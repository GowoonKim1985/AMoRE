R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_pwcoin_qhis_bot_only(){
  int runnum = 330;
  
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("pbot");
  //  chain -> Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq1_%06d_*", runnum, runnum));
  //  chain -> Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq2_%06d_*", runnum, runnum));  
  chain -> Add("./prod_2nd/cpwcoin_all_bot_only.root");  
  //chain -> Add("./test_psd.root");

  //psmd fid grp 1: 0~11 / 2: 12~23 / 3: 23~35 / 4:36~47 / 5(bot):48~58 / 6: 65~76 / 7:77~88 / 8: 89~100 / 9: 101~112 / 10 : 113~123(bot) 

  TH1D * Qahist[124][4];
  TH1D * Qsumhist[124];
  TString hisname;
  double qsum, qcal, qa[4];


  for(int i = 0; i < 124; i++){
    Qsumhist[i] = new TH1D(Form("hqcal_%d", i), "", 200, 0, 50000);
  }
  
    TH1D * hqcal_all = new TH1D("hqcal_all", "", 200, 0, 50000);
  
  int nevt = chain -> GetEntries();

    for(int i = 0; i < nevt; i++){
  //      for(int i = 0; i < 10; i++){

    chain -> GetEntry(i);



      

    if(i != 0 && i%1000000 == 0) {cout << i <<"/"<<nevt << endl;}

    //    int ndata = chain->GetLeaf("bitsum")->GetVaule();
    int ndata = chain->GetLeaf("bitsum")->GetValue();

    //    cout<<ndata<<endl;
    for(int j=0; j<ndata; j++){
      int fid = chain->GetLeaf("trgdet")->GetValue(j);
      qcal = chain->GetLeaf("qcal")->GetValue(j);
      Qsumhist[fid]->Fill(qcal);
      hqcal_all->Fill(qcal);
    }
  }

  
  TFile of5("./hist/chist_pwcoin_bot_only.root", "recreate");
    for(int i=48; i<=58; i++){
      Qsumhist[i]->SetName(Form("hqcal_%d",i-48));
      Qsumhist[i]->Write();
    }
    for(int i=113; i<=123; i++){
      Qsumhist[i]->SetName(Form("hqcal_%d",i-113+11));
      Qsumhist[i]->Write();
    }
    hqcal_all->Write();   
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

