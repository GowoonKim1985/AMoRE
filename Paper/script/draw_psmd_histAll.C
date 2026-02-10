R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void draw_psmd_histAll(){
  int runnum = 330;
  
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("psmd");
  //  chain -> Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq1_%06d_*", runnum, runnum));
  //  chain -> Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq2_%06d_*", runnum, runnum));  
  chain -> Add("./prod_2nd/psmd_all.root");
  //chain -> Add("./test_psd.root");
  
  //psmd fid grp 1: 0~11 / 2: 12~23 / 3: 23~35 / 4:36~47 / 5(bot):48~58 / 6: 65~76 / 7:77~88 / 8: 89~100 / 9: 101~112 / 10 : 113~123(bot) 

  TH1D * Qahist[124][4];
  TH2D * QTBhist[124];
  TH1D * Qsumhist[124];
  TString hisname;
  double qsum, qa[4], qp1, qp2;
  
  for(int i = 0; i < 124; i++){
    Qsumhist[i] = new TH1D(Form("hqsum_%d", i), "", 400, 0, 100000);
    QTBhist[i] = new TH2D(Form("hqtb_%d", i), "", 200, 0, 50000, 200, 0, 50000);    
    for(int j = 0; j < 4; j++){
      Qahist[i][j] = new TH1D(Form("hqa%d_ch%d", i,j), "", 500, 0, 50000);
    }
  }
  
  
  int nevt = chain -> GetEntries();
  //  cout << nevt << endl;

    for(int i = 0; i < nevt; i++){
  //          for(int i = 0; i < 1000000; i++){

    chain -> GetEntry(i);
    if(i != 0 && i%1000000 == 0) {cout << i <<"/"<<nevt << endl;}

    int ndata = chain->GetLeaf("bitsum")->GetNdata();

    //    cout<<ndata<<endl;
    for(int j=0; j<ndata; j++){
      int fid = chain->GetLeaf("trgdet")->GetValue(j);
      qsum = chain->GetLeaf("qsum")->GetValue(j);
      //      cout<<"qsum "<<qsum<<endl;
      Qsumhist[fid] ->Fill(qsum);
      for(int k=0; k<4; k++){
	qa[k]= chain->GetLeaf("iq")->GetValue(j*4 + k);
	Qahist[fid][k]->Fill(qa[k]);
	//	cout<<"q"<<k+1<<" "<<qa[k]<<endl;
      }
      qp1=qa[0]+qa[1];
      qp2=qa[2]+qa[3];
      QTBhist[fid] ->Fill(qp1, qp2);
    }
  }

  

  
    //psmd fid grp 1: 0~11 / 2: 12~23 / 3: 24~35 / 4:36~47 / 5(bot):48~58 / 6: 65~76 / 7:77~88 / 8: 89~100 / 9: 101~112 / 10 : 113~123(bot) 

  /*
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
  */

  //v2 : 327 330 339 368

    /*
  //test
    TFile of1("./hist/qhist_psmd_test.root", "recreate");

      Qsumhist[0]->Write();
      QTBhist[0]->Write();
      for(int j=0; j<4; j++){
	Qahist[0][j]->Write();
      }
    
    of1.Close();
    */
    
    /*
  
    TFile of1("./hist/qhist_psmd_grp1.root", "recreate");
    for(int i=0; i<=11; i++){
      Qsumhist[i]->Write();
      QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of1.Close();

    TFile of2("./hist/qhist_psmd_grp2.root", "recreate");
    for(int i=12; i<=23; i++){
      Qsumhist[i]->Write();
      QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of2.Close();

    TFile of3("./hist/qhist_psmd_grp3.root", "recreate");
    for(int i=24; i<=35; i++){
      Qsumhist[i]->Write();
      QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of3.Close();
    
    TFile of4("./hist/qhist_psmd_grp4.root", "recreate");
    for(int i=36; i<=47; i++){
      Qsumhist[i]->Write();
            QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of4.Close();

    TFile of5("./hist/qhist_psmd_grp5.root", "recreate");
    for(int i=65; i<=76; i++){
      Qsumhist[i]->Write();
            QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of5.Close();
      
    TFile of6("./hist/qhist_psmd_grp6.root", "recreate");
    for(int i=77; i<=88; i++){
      Qsumhist[i]->Write();
            QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of6.Close();

    TFile of7("./hist/qhist_psmd_grp7.root", "recreate");
    for(int i=89; i<=100; i++){
      Qsumhist[i]->Write();
            QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of7.Close();

    TFile of8("./hist/qhist_psmd_grp8.root", "recreate");
    for(int i=101; i<=112; i++){
      Qsumhist[i]->Write();
            QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of8.Close();
*/
    TFile of9("./hist/qhist_psmd_grp9.root", "recreate");
    for(int i=48; i<=58; i++){
      Qsumhist[i]->Write();
            QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of9.Close();

    TFile of10("./hist/qhist_psmd_grp10.root", "recreate");
    for(int i=113; i<=123; i++){
      Qsumhist[i]->Write();
            QTBhist[i]->Write();
      for(int j=0; j<4; j++){
	Qahist[i][j]->Write();
      }
    }
    of10.Close();
    
    
    
  
 
    //psmd fid grp 1: 0~11 / 2: 12~23 / 3: 24~35 / 4:36~47 / 5(bot):48~58 / 6: 65~76 / 7:77~88 / 8: 89~100 / 9: 101~112 / 10 : 113~123(bot) 
   
  }

