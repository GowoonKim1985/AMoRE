R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)
#include <algorithm>
#include <functional>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits

void psmd_iq_align(){


  int evtnum, bitsum;
  double trgtime;
  int tevtnum, trgtype;
  int trgdet[120];
  double qsum[120], iq[120][4], iqmax[120][4];

  TTree * psmd = new TTree("psmd","psmd");

  psmd->Branch("evtnum", &evtnum, "evtnum/I");  
  //  psmd->Branch("trgtime", &trgtime, "trgtime/D");
  psmd->Branch("bitsum", &bitsum, "bitsum/I");
  psmd->Branch("trgdet", trgdet, "trgdet[bitsum]/I");  
  psmd->Branch("qsum", qsum, "qsum[bitsum]/D");
  psmd->Branch("iqmax", iqmax, "iqmax[bitsum][4]/D");  
  psmd->Branch("iq", iq, "iq[bitsum][4]/D");  

  /*  
  TChain * raw = new TChain("psmd");
  raw->Add("./prod_2nd/psmd_all.root");  
  raw->SetBranchAddress("evtnum", &evtnum);
  raw->SetBranchAddress("bitsum", &bitsum);
  raw->SetBranchAddress("trgdet", trgdet);
  raw->SetBranchAddress("qsum", qsum);
  raw->SetBranchAddress("iq", iq);
  */
  TChain * raw = new TChain("coin");
  raw->Add("./prod_2nd/pwcoin_all.root");  
  raw->SetBranchAddress("pevtnum", &evtnum);
  raw->SetBranchAddress("pbitsum", &bitsum);
  raw->SetBranchAddress("ptrgdet", trgdet);
  raw->SetBranchAddress("pqsum", qsum);
  raw->SetBranchAddress("piq", iq);

  
  
  int tent = raw->GetEntries();

    for(int i=0; i<tent; i++){
      //    for(int i=0; i<1000; i++){    

    if(i%10000==0){cout<<i<<" event process..."<<endl;}
    bitsum=0;

    raw->GetEntry(i);

    for(int j=0; j<bitsum; j++){
      double iq_tmp[4];
      for(int k=0; k<4; k++){iq_tmp[k] = iq[j][k];}
      std::sort(iq_tmp, iq_tmp+4, std::greater<double>());
      for(int k=0; k<4; k++){iqmax[j][k] = iq_tmp[k];}
    }
    
    
    psmd->Fill();    
      }

    //  TString anafile = ("./prod_2nd/psmd_iq_align.root");
  TString anafile = ("./prod_2nd/pwcoin_iq_align.root");  
  TFile *out = new TFile(anafile.Data(),"RECREATE");
  psmd->Write();
out->Close(); 
}


  
