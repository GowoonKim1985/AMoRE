R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits
//void SETN00()
void pwcoin_bot()
{

  //  double dnull = std::numeric_limits<double>::quiet_NaN(); // Null value for double
  
  int runnum=330;
  //  int fsub=64;

  int wevtnum, pevtnum, wbitsum, pbitsum, bitsum;
  double wtrgtime, ptrgtime, timegap, wqmax, wqsum;
  int wtevtnum, ptevtnum, trgtype;
  int wtrgdet[48], ptrgdet[120];
  double pqsum[120], piq[120][4], wqtot[48], pqmax[120], pqmin[120];

  TTree * coin = new TTree("coin","coin");

  coin->Branch("wevtnum", &wevtnum, "wevtnum/I");
  coin->Branch("pevtnum", &pevtnum, "pevtnum/I");  
  coin->Branch("wtrgtime", &wtrgtime, "wtrgtime/D");
  coin->Branch("ptrgtime", &ptrgtime, "ptrgtime/D");
  coin->Branch("timegap", &timegap,"timegap/D");
  
  coin->Branch("bitsum", &bitsum, "bitsum/I");
  coin->Branch("wbitsum", &wbitsum, "wbitsum/I");
  coin->Branch("pbitsum", &pbitsum, "pbitsum/I");

  coin->Branch("wtrgdet", wtrgdet, "wtrgdet[wbitsum]/I");
  coin->Branch("ptrgdet", ptrgdet, "ptrgdet[pbitsum]/I");  
  coin->Branch("pqsum", pqsum, "pqsum[pbitsum]/D");
  coin->Branch("pqmax", pqmax, "pqmax[pbitsum]/D");  
  coin->Branch("pqmin", pqmin, "pqmin[pbitsum]/D");  
  coin->Branch("piq", piq, "piq[pbitsum][4]/D");  
  coin->Branch("wqtot", wqtot, "wqtot[wbitsum]/D");
  coin->Branch("wqsum", &wqsum, "wqsum/D");  
  coin->Branch("wqmax", &wqmax, "wqmax/D");
  


  TChain * psmd = new TChain("pbot");
  psmd->Add(Form("./prod_2nd/psmd_%06d_bot_only.root", runnum));
  //  psmd->Add("./prod_2nd/psmd_all_bot_only.root");    

  TChain * wcmd = new TChain("value");
  wcmd->Add(Form("./prod_2nd/wcmd_%06d.root", runnum));

  int ptent = psmd->GetEntries();
  int wtent = wcmd->GetEntries();
  cout<<"wcmd tot entries : "<<wtent<<endl;
    int ipent_temp=0;
    // int ipent_temp=98327705;
  double timedff;
  double dtime=1000;//200ns

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
      /*
      cout<<"======="<<endl;
      cout<<"w entry "<<i<<endl;
      cout<<"w time "<<wtrgtime<<endl;
      cout<<"======="<<endl;
      cout<<" p entry "<<endl;
      */
      
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
	    pqmin[ipbit] = psmd->GetLeaf("qmin")->GetValue(ipbit);
	    for(int ipiq=0; ipiq<4; ipiq++){
	      piq[ipbit][ipiq]=psmd->GetLeaf("iq")->GetValue(4*ipbit+ipiq);
	    }
	  }
	  /*
	  cout<<"timegap "<<timegap<<endl;
	  cout<<"wtrgtime "<<wtrgtime<<endl;
	  cout<<"ptrgtime "<<ptrgtime<<endl;	  
	  */
	  coin->Fill();
	}
	if(timedff>dtime){
	  ipent_temp=ipent-1;
	  //	  	  cout<<"no coin"<<endl;
	  break;}      
      }

  }
    
    //         cout<<"count "<<count<<endl;  

      
  TString anafile = Form("./prod_2nd/pwcoin_%06d_pbot_only.root", runnum);
  TFile *out = new TFile(anafile.Data(),"RECREATE");

  coin->Write();
    
}
    
    //    cout<<""<<endl;
    //   cout<<"wcmd evnt num / trgtime : "<<fevtnum<<" / "<<ftrgtime<<endl;
        

   
  
