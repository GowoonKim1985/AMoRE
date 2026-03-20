//R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(libGui)
//R__LOAD_LIBRARY(libTree)
//R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(/home/cupsoft/muon_3.1.2/shlib/Linux5.14-GCC_11_3/libMuonObjs.so)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits
//void SETN00()
void psmd_align_2nd()
{

  //  double dnull = std::numeric_limits<double>::quiet_NaN(); // Null value for double
  
  int runnum=330;
  //  int fsub=64;

  int evtnum, bitsum, bit1, bit2;
  double trgtime;
  int trgdet[120], daqnum[120];
  double qsum[120],iq[120][4],qmax[120], qp1[120], qp2[120];

  TTree * t = new TTree("psmd","psmd");

  t->Branch("evtnum", &evtnum, "evtnum/I");
  t->Branch("trgtime", &trgtime, "trgtime/D");
  t->Branch("bitsum", &bitsum, "bitsum/I");
  t->Branch("bit1", &bit1, "bit1/I");
  t->Branch("bit2", &bit2, "bit2/I");
  t->Branch("trgdet", trgdet, "trgdet[bitsum]/I");
  t->Branch("qsum", qsum, "qsum[bitsum]/D");
  t->Branch("qp1", qp1, "qp1[bitsum]/D");
  t->Branch("qp2", qp2, "qp2[bitsum]/D");
  t->Branch("iq", iq, "iq[bitsum][4]/D");
  t->Branch("qmax", qmax, "qmax[bitsum]/D");

  
  TChain * psmd = new TChain("psmd");
  //  psmd->Add("./prod_2nd/psmd_all.root");  
  psmd->Add(Form("./prod_2nd/psmd_%06d_org.root", runnum));  

  int tent = psmd->GetEntries();

    int count=0;
    for(int i=0; i<tent; i++){
      //    for(int i=0; i<100; i++){
    // for(int i=5196578; i<5196579; i++){
      if(i%10000==0){cout<<i<<" event process..."<<endl;}
      //      timedff=0;
    
      psmd->GetEntry(i);
      evtnum = psmd->GetLeaf("evtnum")->GetValue();
      trgtime = psmd->GetLeaf("trgtime")->GetValue();    
      bitsum = psmd->GetLeaf("bitsum")->GetValue();
      bit1 = psmd->GetLeaf("bit1")->GetValue();
      bit2 = psmd->GetLeaf("bit2")->GetValue();         

      for(int ibit=0; ibit<bitsum; ibit++){
	trgdet[ibit] = psmd->GetLeaf("trgdet")->GetValue(ibit);
	qsum[ibit] = psmd->GetLeaf("qsum")->GetValue(ibit);
	qmax[ibit] = psmd->GetLeaf("qmax")->GetValue(ibit);	

	for(int ich=0; ich<4; ich++){
	  iq[ibit][ich]=psmd->GetLeaf("iq")->GetValue(4*ibit+ich);
	}
	qp1[ibit]=iq[ibit][0]+iq[ibit][1];
	qp2[ibit]=iq[ibit][2]+iq[ibit][3];

      }
      t->Fill();
    }
      
    //  TString anafile = "./prod_2nd/psmd_all_2nd.root";
    TString anafile = Form("./prod_2nd/psmd_%06d.root",runnum);
  TFile *out = new TFile(anafile.Data(),"RECREATE");

  t->Write();
    
}
    
    //    cout<<""<<endl;
    //   cout<<"wcmd evnt num / trgtime : "<<fevtnum<<" / "<<ftrgtime<<endl;
        

   
  
