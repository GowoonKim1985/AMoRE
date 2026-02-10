R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)

//R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_3/libMuonObjs.so)
//R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_3/libRawObjs.so)


#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>

//void SETN00()
void psmd_align_327()
{
 
  int runnum=327  ;//wcmd evt<psmd, tag==1, iadc1<iadc2
  
  int evtnum, bitsum, evtnum1, evtnum2, bitsum1, bitsum2;
  double trgtime1, trgtime2, trgtime;
  int trgdet[120];
  int daqnum[120];
  double qsum[120], qmax[120], qmin[120];
  double iq[120][4];
  
  TTree * psmd = new TTree("psmd","psmd");
  psmd->Branch("evtnum", &evtnum, "evtnum/I");
  psmd->Branch("trgtime", &trgtime, "trgtime/D");
  psmd->Branch("bitsum", &bitsum, "bitsum/I");
  psmd->Branch("bitsum1", &bitsum1, "bitsum1/I");
  psmd->Branch("bitsum2", &bitsum2, "bitsum2/I");
  psmd->Branch("daqnum", daqnum, "daqnum[bitsum]/I");  
  psmd->Branch("trgdet", trgdet, "trgdet[bitsum]/I");
  psmd->Branch("qsum", qsum, "qsum[bitsum]/D");
  psmd->Branch("iq", iq, "iq[bitsum][4]/D");
  psmd->Branch("qmax", qmax, "qmax[bitsum]/D");
  psmd->Branch("qmin", qmin, "qmin[bitsum]/D");      

  TChain * psmd1 = new TChain("prd_psd");
  psmd1->Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq1_%06d_*.root", runnum, runnum));
  //  psmd1->Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq1_%06d_00000.root", runnum, runnum));

  
  TChain * psmd2 = new TChain("prd_psd");
  psmd2->Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq2_%06d_*.root", runnum, runnum));
  //   psmd2->Add(Form("/home/PROD/PSMD/%06d/prd_psd_daq2_%06d_00000.root", runnum, runnum));    

  int tent1 = psmd1->GetEntries();
  int tent2 = psmd2->GetEntries();

  psmd1->GetEntry(tent1-1);
  int tevtnum1 = psmd1->GetLeaf("fEventNumber")->GetValue();
  double ttrgtime1 = psmd1->GetLeaf("fTriggerTime")->GetValue();  
  psmd2->GetEntry(tent2-1);
  int tevtnum2 = psmd2->GetLeaf("fEventNumber")->GetValue();
  double ttrgtime2 = psmd2->GetLeaf("fTriggerTime")->GetValue();  
  int tevtnum; double ttrgtime;
  if(tevtnum1>tevtnum2){tevtnum=tevtnum1;}
  else{tevtnum=tevtnum2;}
  if(ttrgtime1>ttrgtime2){ttrgtime=ttrgtime1;}
  else{ttrgtime=ttrgtime2;}


  cout<<"tevtnum daq1 : "<<tevtnum1<<endl;
  cout<<"tevtnum daq2 : "<<tevtnum2<<endl;
  cout<<"tevtnum : "<<tevtnum<<endl;
  cout<<"daq1 time : "<<ttrgtime1<<endl;
  cout<<"daq2 time : "<<ttrgtime2<<endl;
  cout<<"daq time : "<<ttrgtime<<endl;
  printf("daq1 time : %.f\n", ttrgtime1);
  printf("daq2 time : %.f\n", ttrgtime2);
  printf("daq  time : %.f\n", ttrgtime);


  int ient2_temp=0;
  int coin=0;
  
    for(int ient1=0; ient1<tent1; ient1++){
      //          for(int ient1=0; ient1<10000; ient1++){
    coin=0;
    bitsum1=0; bitsum2=0; bitsum=0;
    if(ient1%100000==0){cout<<ient1<<"/"<<tent1<<" evts process..."<<endl;}
    psmd1->GetEntry(ient1);
    evtnum1=psmd1->GetLeaf("fEventNumber")->GetValue();
    trgtime1=psmd1->GetLeaf("fTriggerTime")->GetValue();
    //    cout<<"trgtime1 "<<trgtime1<<endl;
    
    //daq2 loop in daq1
    for(int ient2=ient2_temp; ient2<tent2; ient2++){
      //      cout<<"ient2 "<<ient2<<endl; 
      psmd2->GetEntry(ient2);

      trgtime2=psmd2->GetLeaf("fTriggerTime")->GetValue();    
    evtnum2=psmd2->GetLeaf("fEventNumber")->GetValue();
      bitsum2 = psmd2->GetLeaf("fColl.fID")->GetNdata();
      //      cout<<"trgtime2 "<<trgtime2<<endl;
     
      if(trgtime1>trgtime2){//only daq2 hit
	bitsum=bitsum2;
	for(int ibit2=0; ibit2<bitsum2; ibit2++){
	daqnum[ibit2]=2;
	trgdet[ibit2]=psmd2->GetLeaf("fColl.fID")->GetValue(ibit2);
	qsum[ibit2]=psmd2->GetLeaf("fColl.fQsum")->GetValue(ibit2);	
	qmax[ibit2]=0; qmin[ibit2]=99999999;
	for(int iq2=0; iq2<4; iq2++){
	  iq[ibit2][iq2]=psmd2->GetLeaf("fColl.fQ")->GetValue(ibit2*4+iq2);
	  if(qmax[ibit2]<iq[ibit2][iq2]){qmax[ibit2]=iq[ibit2][iq2];}
	  if(qmin[ibit2]>iq[ibit2][iq2]){qmin[ibit2]=iq[ibit2][iq2];}	  
	}
      }
	trgtime=trgtime2; evtnum=evtnum2;
	//	cout<<"trg1<trg2... trg2 fill"<<endl;
	psmd->Fill();
		ient2_temp=ient2+1;
      }
      else if(trgtime1==trgtime2){coin=1; ient2_temp=ient2+1;break;}
      else if(trgtime1<trgtime2){ient2_temp=ient2; break;}
    }

    //only 1daq hit
    bitsum1 = psmd1->GetLeaf("fColl.fID")->GetNdata();
    if(coin==0){
      bitsum2=0;
      bitsum=bitsum1;
      for(int ibit1=0; ibit1<bitsum1; ibit1++){
	daqnum[ibit1]=1;
	trgdet[ibit1]=psmd1->GetLeaf("fColl.fID")->GetValue(ibit1);
	qsum[ibit1]=psmd1->GetLeaf("fColl.fQsum")->GetValue(ibit1);
	qmax[ibit1]=0; qmin[ibit1]=99999999;
	for(int iq1=0; iq1<4; iq1++){
	  iq[ibit1][iq1]=psmd1->GetLeaf("fColl.fQ")->GetValue((ibit1*4)+iq1);
	  if(qmax[ibit1]<iq[ibit1][iq1]){qmax[ibit1]=iq[ibit1][iq1];}
	  if(qmin[ibit1]>iq[ibit1][iq1]){qmin[ibit1]=iq[ibit1][iq1];}	  
	}
      }
      trgtime=trgtime1; evtnum=evtnum1;
      //      	cout<<"trg1>trg2... trg1 fill"<<endl;
      psmd->Fill();    
	}

	
    else if(coin==1){ //1and2 coin
      bitsum=bitsum1+bitsum2;

      for(int ibit1=0; ibit1<bitsum1; ibit1++){
	daqnum[ibit1]=1;
	trgdet[ibit1]=psmd1->GetLeaf("fColl.fID")->GetValue(ibit1);
	qsum[ibit1]=psmd1->GetLeaf("fColl.fQsum")->GetValue(ibit1);
	qmax[ibit1]=0; qmin[ibit1]=99999999;
	for(int iq1=0; iq1<4; iq1++){
	  iq[ibit1][iq1]=psmd1->GetLeaf("fColl.fQ")->GetValue((ibit1*4)+iq1);
	  if(qmax[ibit1]<iq[ibit1][iq1]){qmax[ibit1]=iq[ibit1][iq1];}
	  if(qmin[ibit1]>iq[ibit1][iq1]){qmin[ibit1]=iq[ibit1][iq1];}	  
	}
      }

      for(int ibit2=0; ibit2<bitsum2; ibit2++){
	daqnum[bitsum1+ibit2]=2;
	trgdet[bitsum1+ibit2]=psmd2->GetLeaf("fColl.fID")->GetValue(ibit2);
	qsum[bitsum1+ibit2]=psmd2->GetLeaf("fColl.fQsum")->GetValue(ibit2);	
	qmax[bitsum1+ibit2]=0; qmin[bitsum1+ibit2]=99999999;
	for(int iq2=0; iq2<4; iq2++){
	  iq[bitsum1+ibit2][iq2]=psmd2->GetLeaf("fColl.fQ")->GetValue((ibit2*4)+iq2);
	  if(qmax[bitsum1+ibit2]<iq[bitsum1+ibit2][iq2]){qmax[bitsum1+ibit2]=iq[bitsum1+ibit2][iq2];}
	  if(qmin[bitsum1+ibit2]>iq[bitsum1+ibit2][iq2]){qmin[bitsum1+ibit2]=iq[bitsum1+ibit2][iq2];}
	}
      }
      /*
      cout<<"daq 1 & 2 coincidence"<<endl;
            cout<<"trgtime1 "<<trgtime1<<endl;
            cout<<"trgtime2 "<<trgtime2<<endl;      
            cout<<"evtnum1 "<<evtnum1<<endl;
            cout<<"evtnum2 "<<evtnum2<<endl;      

	    trgtime=trgtime1; evtnum=evtnum1;
      */
      //      	cout<<"trg1=trg2... all fill"<<endl;
      psmd->Fill();    

      
    }
  }
  
  //  TString anafile = Form("./prod_2nd/psmd_%06d.root", runnum);
  TString anafile = Form("./prod_2nd/psmd_%06d_test.root", runnum);  
    //  TString anafile = "/home/kkw/muon_3.1.2/sync_check/test.root";

  TFile *out = new TFile(anafile.Data(),"RECREATE");

  psmd->Write();
      
}
/*

  
  int ient1;
  int ient2;
  int ient1_temp=0;
  int ient2_temp=0;
  //  int ient1_temp= psmd1->GetLeaf("fEventNumber")->GetValue();
  //  int ient2_temp= psmd2->GetLeaf("fEventNumber")->GetValue();

  
    
    for (int ievtnum=1; ievtnum<=tevtnum; ievtnum++){
      //      for (int ievtnum=1; ievtnum<=10; ievtnum++){    
    bitsum1=0;
    bitsum2=0;
    bitsum=0;
    if(ievtnum%100000==0){cout<<ievtnum<<"/"<<tevtnum<<" evts process..."<<endl;}
    for(ient1 = ient1_temp; ient1<tent1; ient1++){
      psmd1->GetEntry(ient1);
      evtnum1 = psmd1->GetLeaf("fEventNumber")->GetValue();
      //     cout<<"evtnum1 : "<<evtnum1<<endl;
      if(evtnum1<ievtnum){}
      else if(evtnum1==ievtnum){
	evtnum=evtnum1;
	trgtime= psmd1->GetLeaf("fTriggerTime")->GetValue();
	bitsum1 = psmd1->GetLeaf("fColl.fID")->GetNdata();
	for(int ibit1=0; ibit1<bitsum1; ibit1++){
	  daqnum[ibit1]=1;
	  trgdet[ibit1]=psmd1->GetLeaf("fColl.fID")->GetValue(ibit1);
	  qsum[ibit1]=psmd1->GetLeaf("fColl.fQsum")->GetValue(ibit1);
	  qmax[ibit1]=0;
	  for(int iq1=0; iq1<4; iq1++){
	    iq[ibit1][iq1]=psmd1->GetLeaf("fColl.fQ")->GetValue((ibit1*4)+iq1);
	    if(qmax[ibit1]<iq[ibit1][iq1]){qmax[ibit1]=iq[ibit1][iq1];}
	  }
	}
	//	psmd->Fill();
      }
      else if(evtnum1>ievtnum){ient1_temp=ient1; break;}
    }  
    for(ient2=ient2_temp; ient2<tent2; ient2++){
      //            cout<<"psmd2 loop"<<endl;
      //      cout<<"daq2 ent "<<ient1<<endl;
      psmd2->GetEntry(ient2);
      evtnum2 = psmd2->GetLeaf("fEventNumber")->GetValue();
      //            cout<<"daq2 evtnum "<<evtnum2<<endl;      
      if(evtnum2<ievtnum){}
      else if(evtnum2==ievtnum){
	//		cout<<"find"<<endl;
	evtnum=evtnum2;
	//	daqnum=2;
	trgtime= psmd2->GetLeaf("fTriggerTime")->GetValue();
	bitsum2= psmd2->GetLeaf("fColl.fID")->GetNdata();
	for(int ibit2=0; ibit2<bitsum2; ibit2++){
	  daqnum[bitsum1+ibit2]=2;
	  trgdet[bitsum1+ibit2]=psmd2->GetLeaf("fColl.fID")->GetValue(ibit2);
	  qsum[bitsum1+ibit2]=psmd2->GetLeaf("fColl.fQsum")->GetValue(ibit2);	
	  qmax[bitsum1+ibit2]=0;
	  for(int iq2=0; iq2<4; iq2++){
	    iq[bitsum1+ibit2][iq2]=psmd2->GetLeaf("fColl.fQ")->GetValue((ibit2*4)+iq2);
	    if(qmax[bitsum1+ibit2]<iq[bitsum1+ibit2][iq2]){qmax[bitsum1+ibit2]=iq[bitsum1+ibit2][iq2];}
	  }

	}
	//	cout<<"psmd fill"<<endl;
      }
      else if(evtnum2>ievtnum){ient2_temp=ient2; break;}
    }
    bitsum=bitsum1+bitsum2;
    psmd->Fill();    
  }

  TString anafile = Form("./prod_2nd/psmd_%06d.root", runnum);
    //  TString anafile = "/home/kkw/muon_3.1.2/sync_check/test.root";

  TFile *out = new TFile(anafile.Data(),"RECREATE");

  psmd->Write();

}  
  */  



