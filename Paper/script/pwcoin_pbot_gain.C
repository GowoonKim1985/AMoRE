R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits

void pwcoin_pbot_gain(){

  //  int runnum=330;
  //  int fsub=64;

  int pevtnum, pbitsum, wevtnum, wbitsum;
  double ptrgtime, wtrgtime, timegap;
  int tevtnum, trgtype;
  int ptrgdet[120], wtrgdet[48], wqmax_id;
  double pqsum[120], piq[120][4], pqmax[120], pqcal[120];
  double wqsum, wqmax, wqtot[48];

  
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


  //  cout<<"gain matching file 1 : psmd_all_bot_only_cut.root"<<endl;
  
  TTree * pbot = new TTree("pbot","pbot");

  pbot->Branch("pevtnum", &pevtnum, "pevtnum/I");  
  pbot->Branch("ptrgtime", &ptrgtime, "ptrgtime/D");
  pbot->Branch("pbitsum", &pbitsum, "pbitsum/I");
  pbot->Branch("ptrgdet", ptrgdet, "ptrgdet[pbitsum]/I");  
  pbot->Branch("pqsum", pqsum, "pqsum[pbitsum]/D");
  pbot->Branch("pqcal", pqcal, "pqcal[pbitsum]/D");  
  pbot->Branch("pqmax", pqmax, "pqmax[pbitsum]/D");  
  pbot->Branch("piq", piq, "piq[pbitsum][4]/D");  

  pbot->Branch("wevtnum", &wevtnum, "wevtnum/I");  
  pbot->Branch("wtrgtime", &wtrgtime, "wtrgtime/D");
  pbot->Branch("wbitsum", &wbitsum, "wbitsum/I");
  pbot->Branch("wtrgdet", wtrgdet, "wtrgdet[wbitsum]/I");  
  pbot->Branch("wqsum", &wqsum, "wqsum/D");
  pbot->Branch("wqmax", &wqmax, "wqmax/D");
  pbot->Branch("wqmax_id", &wqmax_id, "wqmax_id/I");   
  pbot->Branch("wqtot", wqtot, "wqtot[wbitsum]/D");  




  
  TChain * psmd = new TChain("coin");
  psmd->Add("./prod_2nd/pwcoin_all_pbot_only.root");
  //  psmd->Add("./prod_2nd/pwcoin_all_bot_only.root");    

  int ptent = psmd->GetEntries();

    for(int i=0; i<ptent; i++){
  //            for(int i=0; i<1000; i++){    

    if(i%10000==0){cout<<i<<" event process..."<<endl;}
    pbitsum=0; wbitsum=0;

    
    psmd->GetEntry(i);
    timegap = psmd->GetLeaf("timegap")->GetValue();

    if(timegap>=200&&timegap<=400){
      pevtnum = psmd->GetLeaf("pevtnum")->GetValue();
      ptrgtime = psmd->GetLeaf("ptrgtime")->GetValue();    
      pbitsum = psmd->GetLeaf("pbitsum")->GetValue();   
      wevtnum = psmd->GetLeaf("wevtnum")->GetValue();
      wtrgtime = psmd->GetLeaf("wtrgtime")->GetValue();    
      wbitsum = psmd->GetLeaf("wbitsum")->GetValue();   

    for(int ipbit=0; ipbit<pbitsum; ipbit++){

      ptrgdet[ipbit] = psmd->GetLeaf("ptrgdet")->GetValue(ipbit);
      pqsum[ipbit] = psmd->GetLeaf("pqsum")->GetValue(ipbit);
      pqmax[ipbit] = psmd->GetLeaf("pqmax")->GetValue(ipbit);
      pqcal[ipbit] = pqsum[ipbit] * (mpv_ref/mpv[ptrgdet[ipbit]]);
      
      for(int iiq=0; iiq<4; iiq++){
	piq[ipbit][iiq]=psmd->GetLeaf("piq")->GetValue(4*ipbit+iiq);
	//	  cout<<"iq"<<iiq<<" : "<<iq[ipbit][iiq]<<endl;
      }
      
    }    

    wqsum = psmd->GetLeaf("wqsum")->GetValue();
    wqmax = psmd->GetLeaf("wqmax")->GetValue();

    for(int iwbit=0; iwbit<wbitsum; iwbit++){

      wtrgdet[iwbit] = psmd->GetLeaf("wtrgdet")->GetValue(iwbit);
      wqtot[iwbit] = psmd->GetLeaf("wqtot")->GetValue(iwbit);      
      if(wqtot[iwbit]==wqmax){wqmax_id=wtrgdet[iwbit];}
    
      
    }


    pbot->Fill();
    }
    }
  TString anafile1 = "./prod_2nd/cpwcoin_all_pbot_only.root";
  TFile *out = new TFile(anafile1.Data(),"RECREATE");
  pbot->Write();



    
}

/*
  
    int count=0;
    for(int i=0; i<wtent; i++){
    // for(int i=5196578; i<5196579; i++){
      if(i%10000==0){cout<<i<<" event process..."<<endl;}
      wbitsum=0;
      pbitsum=0;
      //      timedff=0;
    
      wcmd->GetEntry(i);
      wevtnum = wcmd->GetLeaf("evtnum")->GetValue();
      wtrgtime = wcmd->GetLeaf("trgtime")->GetValue();    
      wbitsum = wcmd->GetLeaf("tbit")->GetValue();   
      for(int ipent=ipent_temp; ipent<ptent; ipent++){
	//	cout<<ipent<<endl;
	psmd->GetEntry(ipent);
	ptrgtime = psmd->GetLeaf("trgtime")->GetValue();
	pbitsum = psmd->GetLeaf("bitsum")->GetValue();
	timedff = ptrgtime - wtrgtime;
	//	cout<<"p time "<<ptrgtime<<endl;
	//	cout<<"time dff "<<timedff<<endl;
	
	if(abs(timedff)<=dtime){	
	  
	  timegap = timedff;
	  
	  //cout<<"w+p coin / timegap "<<timedff<<endl;
	  //	  cout<<"wevt : "<<wevtnum<<endl;
		  count=count+1;
		  //	  cout<<count<<endl;
	  	  
	  pevtnum = psmd->GetLeaf("evtnum")->GetValue();
	  bitsum = wbitsum+pbitsum;
	  for(int iwbit=0; iwbit<wbitsum; iwbit++){
	    wtrgdet[iwbit] = wcmd->GetLeaf("ch")->GetValue(iwbit);
	    wqtot[iwbit] = wcmd->GetLeaf("qtot")->GetValue(iwbit);
	    wqmax = wcmd->GetLeaf("qmax")->GetValue();
	    wqsum = wcmd->GetLeaf("qsum")->GetValue();	    
	  }
	  for(int ipbit=0; ipbit<pbitsum; ipbit++){
	    ptrgdet[ipbit] = psmd->GetLeaf("trgdet")->GetValue(ipbit);
	    pqsum[ipbit] = psmd->GetLeaf("qsum")->GetValue(ipbit);
	    pqmax[ipbit] = psmd->GetLeaf("qmax")->GetValue(ipbit);
	  }
	  coin->Fill();
	}
	if(timedff>dtime){
	  ipent_temp=ipent-1;
	  //	  	  cout<<"no coin"<<endl;
	  break;}      
      }

    }
    
    //         cout<<"count "<<count<<endl;  

       
  TString anafile = Form("./prod_2nd/pbot_%06d.root", runnum);
  TFile *out = new TFile(anafile.Data(),"RECREATE");

  coin->Write();
}
*/   

    
    //    cout<<""<<endl;
    //   cout<<"wcmd evnt num / trgtime : "<<fevtnum<<" / "<<ftrgtime<<endl;
        

   
  
