R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits
//void SETN00()
void wcmd_mul_cut()
{

  //  double dnull = std::numeric_limits<double>::quiet_NaN(); // Null value for double
  
  int runnum=327;
  //  int fsub=64;

  int evtnum, bitsum, bitsum_raw;
  double trgtime,qsum;
  int tevtnum, trgtype, qmax_ch;
  int trgdet[48], fmax[48];
  double qtot[48];

  TTree * mul = new TTree("mul","mul");

  mul->Branch("evtnum", &evtnum, "evtnum/I");
  mul->Branch("trgtime", &trgtime, "trgtime/D");
  mul->Branch("bitsum_raw", &bitsum_raw, "bitsum_raw/I");
  mul->Branch("bitsum", &bitsum, "bitsum/I");

  TChain * wcmd = new TChain("value");
  wcmd->Add("./prod_2nd/wcmd_all.root");

  int wtent = wcmd->GetEntries();
  cout<<"wcmd tot entries : "<<wtent<<endl;

  for(int i=0; i<wtent; i++){
    //  for(int i=0; i<10; i++){

    bitsum=0; bitsum_raw=0;
    //     for(int i=0; i<10000; i++){
      if(i%10000==0){cout<<i<<" event process..."<<endl;}
      wcmd->GetEntry(i);
      evtnum = wcmd->GetLeaf("evtnum")->GetValue();
      trgtime = wcmd->GetLeaf("trgtime")->GetValue();    
      bitsum_raw = wcmd->GetLeaf("tbit")->GetValue();   
      bitsum=bitsum_raw;
      /*
      cout<<"======="<<endl;
      cout<<"w entry "<<i<<endl;
      cout<<"w time "<<wtrgtime<<endl;
      cout<<"======="<<endl;
      cout<<" p entry "<<endl;
      */
      
      for(int iwbit=0; iwbit<bitsum; iwbit++){
	fmax[iwbit] = wcmd->GetLeaf("fmax")->GetValue(iwbit);

	if(fmax[iwbit]<10){bitsum=bitsum-1;}
      }
      mul->Fill();
  }   
  TString anafile = Form("./prod_2nd/wcmd_mul_cut.root");
  TFile *out = new TFile(anafile.Data(),"RECREATE");

  mul->Write();
    
  }

    
    //    cout<<""<<endl;
    //   cout<<"wcmd evnt num / trgtime : "<<fevtnum<<" / "<<ftrgtime<<endl;
        

   
  
