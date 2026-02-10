R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void cal_hist_psmd_bot(){
  int runnum = 330;
  
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("psmd");
  //  chain -> Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq1_%06d_*", runnum, runnum));
  //  chain -> Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq2_%06d_*", runnum, runnum));  
  //  chain -> Add("/home/PROD/PSMD/000327/prd_psd_daq1_000327_*");
  //  chain -> Add("/home/PROD/PSMD/000327/prd_psd_daq2_000327_*");  
  //  chain -> Add("/home/PROD/PSMD/000330/prd_psd_daq1_000330_*");
  //  chain -> Add("/home/PROD/PSMD/000330/prd_psd_daq2_000330_*");  
  chain -> Add("./prod_2nd/psmd_all.root");

  //chain -> Add("./test_psd.root");
  
  //  PSEvent *pevt = new PSEvent();
  //  chain -> SetBranchAddress("PSEvent", &pevt);

  //psmd fid grp 1: 0~11 / 2: 12~23 / 3: 23~35 / 4:36~47 / 5(bot):48~58 / 6: 65~76 / 7:77~88 / 8: 89~100 / 9: 101~112 / 10 : 113~123(bot) 

  TH1D * Qahist[124][4];
  TH1D * Qsumhist[124];
  TString hisname;
  double qsum, qa[4];

  double mpv[124]={0};


  mpv[48] = 21059.719;
  mpv[49] = 21437.093;
  mpv[50] = 23148.369;  
  mpv[51] = 20193.367;  
  mpv[52] = 18838.910;  
  mpv[53] = 19965.489;  //ref
  mpv[54] = 16189.476;  
  mpv[55] = 17000.071;  
  mpv[56] = 19305.792;  
  mpv[57] = 14074.212;  
  mpv[58] = 25253.339;  

  mpv[113] = 21541.634;  
  mpv[114] = 22788.639;  
  mpv[115] = 20459.820;  
  mpv[116] = 19035.158;  
  mpv[117] = 15563.882;  
  mpv[118] = 12288.910;  
  mpv[119] = 17498.681;  
  mpv[120] = 17935.470;  
  mpv[121] = 18129.145;  
  mpv[122] = 16781.129;  
  mpv[123] = 13272.110;

  double mpv_ref = mpv[53];
  //  cout<<"ref "<<mpv_ref<<endl;
 for(int i = 0; i < 124; i++){
    Qsumhist[i] = new TH1D(Form("hqsum_%d", i), "", 2000, 0, 500000);
  }
 TH1D * Qsum_all = new TH1D("hqsum_bot", "", 2000, 0, 500000);
  

  
  int nevt = chain -> GetEntries();
  //  cout << nevt << endl;

      for(int i = 0; i < nevt; i++){
	//          for(int i = 0; i < 100; i++){

    chain -> GetEntry(i);
    if(i != 0 && i%1000000 == 0) {cout << i <<"/"<<nevt << endl;}

    //    int ndata = chain->GetLeaf("fColl.fID")->GetNdata();
    int ndata = chain->GetLeaf("bitsum")->GetValue();
    //    cout<<ndata<<endl;
    for(int j=0; j<ndata; j++){
      //    int fid = chain->GetLeaf("fColl.fID")->GetValue(j);
    int fid = chain->GetLeaf("trgdet")->GetValue(j);

    if((fid>=48&&fid<=58)||(fid>=113&&fid<=123)){
      //    qsum = chain->GetLeaf("fColl.fQsum")->GetValue(j);
    qsum = chain->GetLeaf("qsum")->GetValue(j);

    //    cout<<"qsum "<<qsum<<endl;
      qsum = qsum * (mpv_ref/mpv[fid]);
      //      cout<<"id "<<fid<<endl;
      //      cout<<"ref/mpv "<<mpv_ref<<"/"<<mpv[fid]<<endl;
      //          cout<<"cal qsum "<<qsum<<endl;
      //      cout<<"fid "<<fid<<endl;
      Qsumhist[fid] ->Fill(qsum);
    }
      /*
      for(int k=0; k<4; k++){
	qa[k]= chain->GetLeaf("fColl.fQ")->GetValue(j*4 + k);
	Qahist[fid][k]->Fill(qa[k]);
      }
      */
    }
    }


  
    //psmd fid grp 1: 0~11 / 2: 12~23 / 3: 24~35 / 4:36~47 / 5(bot):48~58 / 6: 65~76 / 7:77~88 / 8: 89~100 / 9: 101~112 / 10 : 113~123(bot) 


      TFile of5("./hist/chist_psmd_bot.root", "recreate");
    for(int i=48; i<=58; i++){
      Qsum_all->Add(Qsumhist[i]);
      Qsumhist[i]->SetName(Form("hqsum_%d",i-48));
      Qsumhist[i]->Write();
    }
    for(int i=113; i<=123; i++){
      Qsum_all->Add(Qsumhist[i]);
      Qsumhist[i]->SetName(Form("hqsum_%d",i-113+11));
      Qsumhist[i]->Write();
    }
   
      of5.Close();
      TFile of0("./hist/qhist_psmd_all.root", "recreate");
      Qsum_all->Write();
      of0.Close();
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

