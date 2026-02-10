R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits

void psmd_bot(){

  int runnum=330;
  //  int fsub=64;

  int evtnum, bitsum;
  double trgtime, timegap;
  int tevtnum, trgtype;
  int trgdet[120];
  double qsum[120], iq[120][4], qmax[120], qmin[120];

  TTree * pbot = new TTree("pbot","pbot");

  pbot->Branch("evtnum", &evtnum, "evtnum/I");  
  pbot->Branch("trgtime", &trgtime, "trgtime/D");
  pbot->Branch("bitsum", &bitsum, "bitsum/I");
  pbot->Branch("trgdet", trgdet, "trgdet[bitsum]/I");  
  pbot->Branch("qsum", qsum, "qsum[bitsum]/D");
  pbot->Branch("qmax", qmax, "qmax[bitsum]/D");  
  pbot->Branch("qmin", qmin, "qmin[bitsum]/D");  
  pbot->Branch("iq", iq, "iq[bitsum][4]/D");  

  
  TChain * psmd = new TChain("psmd");
  psmd->Add(Form("./prod_2nd/psmd_%06d.root", runnum));  

  int ptent = psmd->GetEntries();

  for(int i=0; i<ptent; i++){
    //  for(int i=0; i<1000; i++){    

    if(i%10000==0){cout<<i<<" event process..."<<endl;}
    bitsum=0;

    psmd->GetEntry(i);
    evtnum = psmd->GetLeaf("evtnum")->GetValue();
    trgtime = psmd->GetLeaf("trgtime")->GetValue();    
    bitsum = psmd->GetLeaf("bitsum")->GetValue();   

    int flag=0;
    for(int j=0; j<bitsum; j++){
      trgdet[j] = psmd->GetLeaf("trgdet")->GetValue(j);
      if((trgdet[j]>=48&&trgdet[j]<=58)||(trgdet[j]>=113&&trgdet[j]<=123)){
	flag=flag+1;
      }
    }
    if(flag==bitsum){//only bot hit
      //      cout<<"evtnum "<<evtnum<<endl;
      for(int ipbit=0; ipbit<bitsum; ipbit++){

	trgdet[ipbit] = psmd->GetLeaf("trgdet")->GetValue(ipbit);
	qsum[ipbit] = psmd->GetLeaf("qsum")->GetValue(ipbit);
	qmax[ipbit] = psmd->GetLeaf("qmax")->GetValue(ipbit);
	qmin[ipbit] = psmd->GetLeaf("qmin")->GetValue(ipbit);
	//	cout<<"det "<<trgdet[ipbit]<<endl;
	for(int iiq=0; iiq<4; iiq++){
	  iq[ipbit][iiq]=psmd->GetLeaf("iq")->GetValue(4*ipbit+iiq);
	  //	  cout<<"iq"<<iiq<<" : "<<iq[ipbit][iiq]<<endl;
	}
      }      
      pbot->Fill();
    }
  }
  TString anafile = Form("./prod_2nd/psmd_%06d_bot_only.root", runnum);
  TFile *out = new TFile(anafile.Data(),"RECREATE");
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
        

   
  
