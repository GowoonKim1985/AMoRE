//R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)


R__LOAD_LIBRARY(/home/cupsoft/muon_3.1.2/shlib/Linux5.14-GCC_11_3/libRawObjs.so)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits

void psmd_2hit(){

  int runnum=368;
  //  int fsub=64;

  int evtnum, bitsum, grp[2];
  double trgtime, timegap;
  int tevtnum, trgtype;
  int sdet, bdet;
  int trgdet[2];
  double qsum[2], iq[2][4];

  TTree * phit = new TTree("phit","phit");

  phit->Branch("evtnum", &evtnum, "evtnum/I");  
  phit->Branch("trgtime", &trgtime, "trgtime/D");
  phit->Branch("trgdet", trgdet, "trgdet[2]/I");
  phit->Branch("grp", grp, "grp[2]/I");  
  phit->Branch("qsum", qsum, "qsum[2]/D");
  phit->Branch("iq", iq, "iq[2][4]/D"); 

  TChain * psmd = new TChain("psmd");
  //  psmd->Add(Form("./prod_2nd/psmd_%06d.root", runnum));  
    psmd->Add("/home/kkw/Works/paper/prod_2nd/psmd_all.root");  

  int ptent = psmd->GetEntries();
  int flag;
   for(int i=0; i<ptent; i++){
     //    for(int i=0; i<50000; i++){    

    if(i%100000==0){cout<<i<<" event process..."<<endl;}
    flag=0;
    psmd->GetEntry(i);
    evtnum = psmd->GetLeaf("evtnum")->GetValue();
    trgtime = psmd->GetLeaf("trgtime")->GetValue();
    bitsum = psmd->GetLeaf("bitsum")->GetValue();   

    if(bitsum ==2){
      //      cout<<"evt : bitsum = "<<evtnum<<" : "<<bitsum<<endl;
      
      trgdet[0]=psmd->GetLeaf("trgdet")->GetValue(0);
      trgdet[1]=psmd->GetLeaf("trgdet")->GetValue(1);
      qsum[0]=psmd->GetLeaf("qsum")->GetValue(0);
      qsum[1]=psmd->GetLeaf("qsum")->GetValue(1);

      for(int j=0; j<2; j++){
	for(int k=0; k<4; k++){
	  iq[j][k]=psmd->GetLeaf("iq")->GetValue(j*4+k);
	}

	//side
	if(trgdet[j]>=0&&trgdet[j]<=11){grp[j]=1;}
	if(trgdet[j]>=12&&trgdet[j]<=23){grp[j]=2;}
	if(trgdet[j]>=24&&trgdet[j]<=35){grp[j]=3;}
	if(trgdet[j]>=36&&trgdet[j]<=47){grp[j]=4;}
	if(trgdet[j]>=65&&trgdet[j]<=76){grp[j]=5;}
	if(trgdet[j]>=77&&trgdet[j]<=88){grp[j]=6;}
	if(trgdet[j]>=89&&trgdet[j]<=100){grp[j]=7;}
	if(trgdet[j]>=101&&trgdet[j]<=112){grp[j]=8;}      
	//bot
	if(trgdet[j]>=48&&trgdet[j]<=58){grp[j]=9;}
	if(trgdet[j]>=113&&trgdet[j]<=123){grp[j]=10;}	
      }
    phit->Fill();	
    }

  }
	
   TString anafile = "./psmd_2hit.root";
      // TString anafile = Form("./prod_2nd/psmd_%06d_2hit.root",runnum);
  TFile *out = new TFile(anafile.Data(),"RECREATE");

  phit->Write();
 
  
}


   
  
