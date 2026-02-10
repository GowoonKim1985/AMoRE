R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits

void psmd_bot_gain(){

  //  int runnum=330;
  //  int fsub=64;

  int evtnum, bitsum;
  double trgtime;
  int tevtnum, trgtype;
  int trgdet[120];
  double qsum[120], iq[120][4], qmax[120], qcal[120];

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


  cout<<"gain matching file 1 : psmd_all_bot_only.root"<<endl;
  
  TTree * pbot = new TTree("pbot","pbot");

  pbot->Branch("evtnum", &evtnum, "evtnum/I");  
  pbot->Branch("trgtime", &trgtime, "trgtime/D");
  pbot->Branch("bitsum", &bitsum, "bitsum/I");
  pbot->Branch("trgdet", trgdet, "trgdet[bitsum]/I");  
  pbot->Branch("qsum", qsum, "qsum[bitsum]/D");
  pbot->Branch("qcal", qcal, "qcal[bitsum]/D");  
  pbot->Branch("qmax", qmax, "qmax[bitsum]/D");  
  pbot->Branch("iq", iq, "iq[bitsum][4]/D");  
 
  TChain * psmd = new TChain("pbot");
  psmd->Add("./prod_2nd/psmd_all_bot_only.root");  

  int ptent = psmd->GetEntries();

  for(int i=0; i<ptent; i++){
    //        for(int i=0; i<1000; i++){    

    if(i%10000==0){cout<<i<<" event process..."<<endl;}
    bitsum=0;

    psmd->GetEntry(i);
    evtnum = psmd->GetLeaf("evtnum")->GetValue();
    trgtime = psmd->GetLeaf("trgtime")->GetValue();    
    bitsum = psmd->GetLeaf("bitsum")->GetValue();   

    for(int ipbit=0; ipbit<bitsum; ipbit++){

      trgdet[ipbit] = psmd->GetLeaf("trgdet")->GetValue(ipbit);
      qsum[ipbit] = psmd->GetLeaf("qsum")->GetValue(ipbit);
      qmax[ipbit] = psmd->GetLeaf("qmax")->GetValue(ipbit);
      qcal[ipbit] = qsum[ipbit] * (mpv_ref/mpv[trgdet[ipbit]]);

      for(int iiq=0; iiq<4; iiq++){
	iq[ipbit][iiq]=psmd->GetLeaf("iq")->GetValue(4*ipbit+iiq);
	//	  cout<<"iq"<<iiq<<" : "<<iq[ipbit][iiq]<<endl;
      }
    }      
    pbot->Fill();
  }
  TString anafile1 = "./prod_2nd/cpsmd_all_bot_only.root";
  TFile *out = new TFile(anafile1.Data(),"RECREATE");
  pbot->Write();


  /*
  cout<<"gain matching file 2 : pwcoin_all_bot.root"<<endl;



  int wevtnum, pevtnum, wbitsum, pbitsum, tbitsum;
  double wtrgtime, ptrgtime, timegap, wqmax, wqsum;
  int wtevtnum, ptevtnum;
  int wtrgdet[48], ptrgdet[120];
  double pqsum[120], piq[120][4], wqtot[48], pqmax[120], pqcal[120];

  TTree * coin = new TTree("coin","coin");

  coin->Branch("wevtnum", &wevtnum, "wevtnum/I");
  coin->Branch("pevtnum", &pevtnum, "pevtnum/I");
  coin->Branch("wtrgtime", &wtrgtime, "wtrgtime/D");
  coin->Branch("ptrgtime", &ptrgtime, "ptrgtime/D");
  coin->Branch("timegap", &timegap,"timegap/D");

  coin->Branch("tbitsum", &tbitsum, "tbitsum/I");
  coin->Branch("wbitsum", &wbitsum, "wbitsum/I");
  coin->Branch("pbitsum", &pbitsum, "pbitsum/I");

  coin->Branch("wtrgdet", wtrgdet, "wtrgdet[wbitsum]/I");
  coin->Branch("ptrgdet", ptrgdet, "ptrgdet[pbitsum]/I");
  coin->Branch("pqsum", pqsum, "pqsum[pbitsum]/D");
  coin->Branch("pqcal", pqcal, "pqcal[pbitsum]/D");
  coin->Branch("pqmax", pqmax, "pqmax[pbitsum]/D");

  coin->Branch("wqtot", wqtot, "wqtot[wbitsum]/D");
  coin->Branch("wqsum", &wqsum, "wqsum/D");
  coin->Branch("wqmax", &wqmax, "wqmax/D");
 
  TChain * pwcoin = new TChain("coin");
  pwcoin->Add("./prod_2nd/pwcoin_all_bot.root");  

  int tent = pwcoin->GetEntries();

  //  for(int i=0; i<tent; i++){
        for(int i=0; i<1000; i++){    

    if(i%10000==0){cout<<i<<" event process..."<<endl;}
    tbitsum=0;

    pwcoin->GetEntry(i);
    pevtnum = pwcoin->GetLeaf("pevtnum")->GetValue();
    ptrgtime = pwcoin->GetLeaf("ptrgtime")->GetValue();    
    pbitsum = pwcoin->GetLeaf("pbitsum")->GetValue();   
    wevtnum = pwcoin->GetLeaf("wevtnum")->GetValue();
    wtrgtime = pwcoin->GetLeaf("wtrgtime")->GetValue();    
    wbitsum = pwcoin->GetLeaf("wbitsum")->GetValue();   
    timegap = ptrgtime - wtrgtime;


    for(int iwbit=0; iwbit<wbitsum; iwbit++){
      wtrgdet[iwbit] = pwcoin->GetLeaf("wtrgdet")->GetValue(iwbit);
      wqtot[iwbit] = pwcoin->GetLeaf("wqtot")->GetValue(iwbit);
      wqmax = pwcoin->GetLeaf("wqmax")->GetValue();
      wqsum = pwcoin->GetLeaf("wqsum")->GetValue();
    }
    for(int ipbit=0; ipbit<pbitsum; ipbit++){
      ptrgdet[ipbit] = pwcoin->GetLeaf("ptrgdet")->GetValue(ipbit);
      pqsum[ipbit] = pwcoin->GetLeaf("pqsum")->GetValue(ipbit);
      pqmax[ipbit] = pwcoin->GetLeaf("pqmax")->GetValue(ipbit);
      pqcal[ipbit] = pqsum[ipbit] * (mpv_ref/mpv[ptrgdet[ipbit]]);
    }

    
    coin->Fill();
  }
  TString anafile2 = "./prod_2nd/cpwcoin_all_bot.root";
  TFile *out2 = new TFile(anafile2.Data(),"RECREATE");
  coin->Write();

  */

  
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
        

   
  
