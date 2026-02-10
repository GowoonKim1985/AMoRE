R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits

void pbot_wcoin(){

  int runnum=327;
  //  int fsub=64;

  int wevtnum, pevtnum, wbitsum, pbitsum, bitsum;
  double wtrgtime, ptrgtime, timegap, wqmax, wqsum;
  int wtevtnum, ptevtnum, trgtype;
  int wtrgdet[48], ptrgdet[120];
  double pqsum[120], piq[120][4], wqtot[48], pqmax[120];

  TTree * coin = new TTree("coin","coin");

  coin->Branch("pevtnum", &pevtnum, "pevtnum/I");  
  coin->Branch("ptrgtime", &ptrgtime, "ptrgtime/D");
  coin->Branch("pbitsum", &pbitsum, "pbitsum/I");
  coin->Branch("ptrgdet", ptrgdet, "ptrgdet[pbitsum]/I");  
  coin->Branch("pqsum", pqsum, "pqsum[pbitsum]/D");
  coin->Branch("pqmax", pqmax, "pqmax[pbitsum]/D");  

  coin->Branch("wevtnum", &wevtnum, "wevtnum/I");
  coin->Branch("wtrgtime", &wtrgtime, "wtrgtime/D");
  coin->Branch("wbitsum", &wbitsum, "wbitsum/I");
  coin->Branch("wtrgdet", wtrgdet, "wtrgdet[wbitsum]/I");
  coin->Branch("wqsum", &wqsum, "wqsum/D");  
  coin->Branch("wqtot", wqtot, "wqtot[wbitsum]/D");
  coin->Branch("wqmax", &wqmax, "wqmax/D");
  
  TChain * psmd = new TChain("psmd");
  psmd->Add(Form("./prod_2nd/psmd_%06d.root", runnum));  

  TChain * wcmd = new TChain("value");
  wcmd->Add(Form("./prod_2nd/wcmd_%06d.root", runnum));

  int ptent = psmd->GetEntries();
  int wtent = wcmd->GetEntries();
  cout<<"wcmd tot entries : "<<wtent<<endl;
  int iwent_temp=0;//run327
  double timedff;
  double dtime=1000;//200ns

  //for(int i=0; i<ptent; i++){
  for(int i=0; i<10 ; i++){
    cout<<""<<endl;
    cout<<"p ent "<<i<<endl;
    if(i%10000==0){cout<<i<<" event process..."<<endl;}
    wbitsum=0;
    pbitsum=0;

    psmd->GetEntry(i);
    pevtnum = psmd->GetLeaf("evtnum")->GetValue();
    ptrgtime = psmd->GetLeaf("trgtime")->GetValue();    
    pbitsum = psmd->GetLeaf("bitsum")->GetValue();   

    int flag=0;
    for(int j=0; j<pbitsum; j++){
      ptrgdet[j] = psmd->GetLeaf("trgdet")->GetValue(j);
      if((ptrgdet[j]>=48&&ptrgdet[j]<=58)||(ptrgdet[j]>=113&&ptrgdet[j]<=123)){
	flag=flag+1;
      }
    }
    cout<<"pt "<<ptrgtime<<endl;
    if(flag==pbitsum){//only bot hit
      cout<<"hit bot only"<<endl;
      //get psmd parameters
      for(int ipbit=0; ipbit<pbitsum; ipbit++){
	ptrgdet[ipbit] = psmd->GetLeaf("trgdet")->GetValue(ipbit);
	pqsum[ipbit] = psmd->GetLeaf("qsum")->GetValue(ipbit);
	pqmax[ipbit] = psmd->GetLeaf("qmax")->GetValue(ipbit);
      }

      //wcmd coin work
      for(int j=iwent_temp; j<wtent; j++){
	cout<<"w ent "<<j<<endl;
	wcmd->GetEntry(j);
	wtrgtime = wcmd->GetLeaf("trgtime")->GetValue();
	cout<<"wt "<<wtrgtime<<endl;
	wevtnum = wcmd->GetLeaf("evtnum")->GetValue();    
	timedff = ptrgtime - wtrgtime;

	if(abs(timedff)<=dtime){//coincidence
	  cout<<"coin found"<<endl;
	  cout<<"p evtnum "<<pevtnum<<endl;
	  iwent_temp=iwent_temp+1;
	  break;
	}
	if(timedff<-1000){cout<<"wt>pt"<<endl;break;}
      }
      
    }
  }
  /*
  TString anafile = Form("./prod_2nd/pbot_%06d.root", runnum);
  TFile *out = new TFile(anafile.Data(),"RECREATE");
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
        

   
  
