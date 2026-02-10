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
void pwcoin_det53_cal()
{


  int wevtnum, pevtnum, wbitsum, pbitsum, bitsum;
  double wtrgtime, ptrgtime, timegap, wqmax, wqsum;
  int wtevtnum, ptevtnum, trgtype, wqmax_ch;
  int wtrgdet[48], ptrgdet;
  double pqsum, piq[4], wqtot[48], pqmax, piqmax[4], pqmin, pqp1, pqp2;
  double pcsum, pcq[4], pcmax, pcp1, pcp2, pcqmax[4];

  double mpv[4]= {104.25420, 104.61394, 98.310605, 84.633667};
  double cal[4]= {mpv[0]/mpv[0], mpv[0]/mpv[1], mpv[0]/mpv[1], mpv[0]/mpv[1]};
  
  TTree * t = new TTree("coin","coin");

  t->Branch("wevtnum", &wevtnum, "wevtnum/I");
  t->Branch("pevtnum", &pevtnum, "pevtnum/I");
  t->Branch("wtrgtime", &wtrgtime, "wtrgtime/D");
  t->Branch("ptrgtime", &ptrgtime, "ptrgtime/D");
  t->Branch("timegap", &timegap,"timegap/D");

  t->Branch("bitsum", &bitsum, "bitsum/I");
  t->Branch("wbitsum", &wbitsum, "wbitsum/I");
  t->Branch("pbitsum", &pbitsum, "pbitsum/I");

  t->Branch("wtrgdet", wtrgdet, "wtrgdet[wbitsum]/I");
  t->Branch("ptrgdet", ptrgdet, "ptrgdet[pbitsum]/I");

  t->Branch("pqsum", &pqsum, "pqsum/D");
  t->Branch("pqmax", &pqmax, "pqmax/D");
  t->Branch("pqp1", &pqp1, "pqp1/D");
  t->Branch("pqp2", &pqp2, "pqp2/D");
  t->Branch("piq", piq, "piq[4]/D");
  t->Branch("piqmax", piqmax, "piqmax[4]/D");

  t->Branch("pcsum", &pcsum, "pcsum/D");
  t->Branch("pcmax", &pcmax, "pcmax/D");
  t->Branch("pcp1", &pcp1, "pcp1/D");
  t->Branch("pcp2", &pcp2, "pcp2/D");
  t->Branch("pcq", pcq, "pcq[4]/D");
  t->Branch("pcqmax", pcqmax, "pcqmax[4]/D");

  
  t->Branch("wqtot", wqtot, "wqtot[wbitsum]/D");
  t->Branch("wqsum", &wqsum, "wqsum/D");
  t->Branch("wqmax", &wqmax, "wqmax/D");
  t->Branch("wqmax_ch", &wqmax_ch, "wqmax_ch/I");

  
  
  TChain * coin = new TChain("coin");
  coin->Add("./prod_2nd/pwcoin_all.root");  

  int tent = coin->GetEntries();

  for(int i=0; i<tent; i++){
      //    for(int i=0; i<100000; i++){
    // for(int i=5196578; i<5196579; i++){
    if(i%10000==0){cout<<i<<" event process..."<<endl;}
      //      timedff=0;
    
    coin->GetEntry(i);
    pbitsum = coin->GetLeaf("pbitsum")->GetValue();
    wbitsum = coin->GetLeaf("wbitsum")->GetValue();
    
    for(int ibit=0; ibit<pbitsum; ibit++){
      ptrgdet = coin->GetLeaf("ptrgdet")->GetValue(ibit);

      if(ptrgdet==53){     	
	wevtnum = coin->GetLeaf("wevtnum")->GetValue();
	pevtnum = coin->GetLeaf("pevtnum")->GetValue();
	wtrgtime = coin->GetLeaf("wtrgtime")->GetValue();    
	ptrgtime = coin->GetLeaf("ptrgtime")->GetValue();    
	timegap = coin->GetLeaf("timegap")->GetValue();


	wqsum = coin->GetLeaf("wqsum")->GetValue();
	wqmax = coin->GetLeaf("wqmax")->GetValue();    	
	wqmax_ch = coin->GetLeaf("wqmax_ch")->GetValue();    	
	
	for(int wbit=0; wbit<wbitsum; wbit++){
	  wqtot[wbit] = coin->GetLeaf("wqtot")->GetValue(wbit);    
	  wtrgdet[wbit] = coin->GetLeaf("wtrgdet")->GetValue(wbit);    
	}
	
	pqsum = coin->GetLeaf("pqsum")->GetValue(ibit);
	pqmax = coin->GetLeaf("pqmax")->GetValue(ibit);
	pcmax=0;

	for(int ich=0; ich<4; ich++){
	  piq[ich]=coin->GetLeaf("piq")->GetValue(4*ibit+ich);
	  pcq[ich]=piq[ich]*cal[ich];
	  if(pcq[ich]>pcmax){pcmax=pcq[ich];}
	}
	  pqp1=piq[0]+piq[1];
	  pqp2=piq[2]+piq[3];
	  pcp1=pcq[0]+pcq[1];
	  pcp2=pcq[2]+pcq[3];
	  pcsum=pcp1+ pcp2;

	  double iq_tmp[4], ic_tmp[4];
	  for(int k=0; k<4; k++){iq_tmp[k] = piq[k]; ic_tmp[k] = pcq[k];}
	  std::sort(iq_tmp, iq_tmp+4, std::greater<double>());
	  std::sort(ic_tmp, ic_tmp+4, std::greater<double>());

	  for(int k=0; k<4; k++){piqmax[k] = iq_tmp[k]; pcqmax[k] = ic_tmp[k];}


	  
      t->Fill();
	}
      }

    }
      
  TString anafile = "./prod_2nd/pwcoin_det53_2nd.root";
  TFile *out = new TFile(anafile.Data(),"RECREATE");

  t->Write();
    
}
    
    //    cout<<""<<endl;
    //   cout<<"wcmd evnt num / trgtime : "<<fevtnum<<" / "<<ftrgtime<<endl;
        

   
  
