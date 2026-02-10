R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits
//void SETN00()
void psmd_align2nd_bot()
{

  //  double dnull = std::numeric_limits<double>::quiet_NaN(); // Null value for double
  
  int runnum=327;
  //  int fsub=64;

  int evtnum, tbit, bbit, sbit;
  double trgtime, qmax;
  int trgdet[120];
  double qsum[120];

  TTree * t = new TTree("psmd","psmd");

  t->Branch("evtnum", &evtnum, "evtnum/I");
  t->Branch("trgtime", &trgtime, "trgtime/D");
  t->Branch("tbit", &tbit, "tbit/I");
  t->Branch("bbit", &bbit, "bbit/I");
  t->Branch("sbit", &sbit, "sbit/I");
  t->Branch("trgdet", trgdet, "trgdet[tbit]/I");
  t->Branch("qsum", qsum, "qsum[tbit]/D");
  t->Branch("qmax", &qmax, "qmax/D");

  TChain * psmd = new TChain("psmd");
  psmd->Add(Form("./prod_2nd/psmd_all.root", runnum));  

  int tent = psmd->GetEntries();
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

      
  TString anafile = Form("./prod_2nd/pwcoin_%06d_1000ns.root", runnum);
  TFile *out = new TFile(anafile.Data(),"RECREATE");

  coin->Write();
    
}
    
    //    cout<<""<<endl;
    //   cout<<"wcmd evnt num / trgtime : "<<fevtnum<<" / "<<ftrgtime<<endl;
        

   
  
