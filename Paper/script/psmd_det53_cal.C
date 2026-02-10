R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libGui)
R__LOAD_LIBRARY(libTree)

#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits> // for std::numeric_limits
#include <algorithm>
#include <functional>

//void SETN00()
void psmd_det53_cal()
{

  //  double dnull = std::numeric_limits<double>::quiet_NaN(); // Null value for double
  
  int runnum=327;
  //  int fsub=64;

  int evtnum, bitsum, bitsum1, bitsum2;
  double trgtime;
  int trgdet, daqnum[120];
  double qsum,iq[4],qmax, qp1, qp2, iqmax[4];
  double csum,cq[4],cmax, cp1, cp2, cqmax[4];  

  double mpv[4]= {104.25420, 104.61394, 98.310605, 84.633667};
  double cal[4]= {mpv[0]/mpv[0], mpv[0]/mpv[1], mpv[0]/mpv[2], mpv[0]/mpv[3]};
  
  TTree * t = new TTree("psmd","psmd");

  t->Branch("evtnum", &evtnum, "evtnum/I");
  t->Branch("trgtime", &trgtime, "trgtime/D");
  t->Branch("trgdet", &trgdet, "trgdet/I");
  //  t->Branch("bitsum", &bitsum, "bitsum/I");
  //  t->Branch("bitsum1", &bitsum1, "bitsum1/I");
  //  t->Branch("bitsum2", &bitsum2, "bitsum2/I");
  t->Branch("qsum", &qsum, "qsum/D");
  t->Branch("qp1", &qp1, "qp1/D");
  t->Branch("qp2", &qp2, "qp2/D");
  t->Branch("iq", iq, "iq[4]/D");
  t->Branch("qmax", &qmax, "qmax/D");
  t->Branch("iqmax", iqmax, "iqmax[4]/D");

  t->Branch("csum", &csum, "csum/D");
  t->Branch("cp1", &cp1, "cp1/D");
  t->Branch("cp2", &cp2, "cp2/D");
  t->Branch("cq", cq, "cq[4]/D");
  t->Branch("cmax", &cmax, "cmax/D");
  t->Branch("cqmax", cqmax, "cqmax[4]/D");

  
  
  TChain * psmd = new TChain("psmd");
  psmd->Add("./prod_2nd/psmd_all_2nd.root");  

  int tent = psmd->GetEntries();

    int count=0;
    for(int i=0; i<tent; i++){
      //    for(int i=0; i<100000; i++){
    // for(int i=5196578; i<5196579; i++){
      if(i%10000==0){cout<<i<<" event process..."<<endl;}
      //      timedff=0;
    
      psmd->GetEntry(i);
      evtnum = psmd->GetLeaf("evtnum")->GetValue();
      trgtime = psmd->GetLeaf("trgtime")->GetValue();    
      bitsum = psmd->GetLeaf("bitsum")->GetValue();

      for(int ibit=0; ibit<bitsum; ibit++){
	trgdet = psmd->GetLeaf("trgdet")->GetValue(ibit);

	if(trgdet==53){     	
	  qsum = psmd->GetLeaf("qsum")->GetValue(ibit);
	  qmax = psmd->GetLeaf("qmax")->GetValue(ibit);	
	  cmax=0;

	  for(int ich=0; ich<4; ich++){
	    iq[ich]=psmd->GetLeaf("iq")->GetValue(4*ibit+ich);
	    cq[ich]=iq[ich]*cal[ich];
	    if(cq[ich]>cmax){cmax=cq[ich];}
	  }
	  qp1=iq[0]+iq[1];
	  qp2=iq[2]+iq[3];
	  cp1=cq[0]+cq[1];
	  cp2=cq[2]+cq[3];
	  csum=cp1+ cp2;

	  double iq_tmp[4], ic_tmp[4];
	  for(int k=0; k<4; k++){iq_tmp[k] = iq[k]; ic_tmp[k] = cq[k];}
	  std::sort(iq_tmp, iq_tmp+4, std::greater<double>());
	  std::sort(ic_tmp, ic_tmp+4, std::greater<double>());
	  for(int k=0; k<4; k++){iqmax[k] = iq_tmp[k]; cqmax[k] = ic_tmp[k];}


	  
      t->Fill();
	}
      }

    }
      
  TString anafile = "./prod_2nd/psmd_det53_2nd.root";
  TFile *out = new TFile(anafile.Data(),"RECREATE");

  t->Write();
    
}
    
    //    cout<<""<<endl;
    //   cout<<"wcmd evnt num / trgtime : "<<fevtnum<<" / "<<ftrgtime<<endl;
        

   
  
