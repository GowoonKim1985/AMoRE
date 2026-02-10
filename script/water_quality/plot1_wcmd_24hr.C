R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

#include <cmath> // for std::nan()

void plot1_wcmd_24hr()
{
  //
  gStyle -> SetOptStat(0);
  //run 272 sub 0 - 64 : 231105 17:16:11
  //run 287 sub 0 - 93
  //run 288 sub 0 -525
  //run 294 sub 0 - 76
  //run 303 sub 0-1154
  //run 304 sub 0 -791
  //run 311 sub 0 - 190
  //run 315  sub 0 - 191
  //run 318 sub 0 - 689
  //run 327 sub 0 - 282
  //run 330 sub 0 - 695
  //run 339 sub 0 - 286
  //run 343 sub 0 - 94
  //run 356 sub 0 - 667
  //run 361 sub 0 - 143  
  //run 368 sub 0 - 332

  const int fn = 15;
  
  //15
  int run[fn] = {287, 288, 294, 303, 304, 311, 315, 318, 327, 330, 339, 343, 356, 361, 368};
  int finsub[fn] = {93, 525, 76, 1154, 791, 190, 191, 689, 282, 695, 286, 94, 667, 143, 332};
  //int daygap[fn] = {6, 10, 35, 40, 89, 122, 130, 138, 167, 179, 231, 244, 250, 305, 311};
  int daygap[fn] = {0, 4, 31, 36, 85, 118, 126, 134, 163, 175, 227, 240, 246, 301, 307};

  
  //  int subdiv=50; // how many sub files
  int subdiv=24; // how many sub files
  double mean[7];
  TCanvas * chit = new TCanvas("chit","chit",800,600);
  //  TCanvas * cmean_sel = new TCanvas("cmean_sel","cmean_sel",800,600);  
  //  TCanvas * ccnt = new TCanvas("ccnt","ccnt",800,600);  

  int procnum[fn];
  int finbin=0;
  int binnum=0;
  for(int i=0; i<fn; i++){
   procnum[i]=finsub[i]/subdiv;
    binnum = binnum + procnum[i];
    finbin = finbin+(subdiv*procnum[i]);
  }
  binnum = binnum + fn - 1;
  finbin = finbin + fn -1;
  cout<<"tot binnum : "<<binnum<<endl;
  cout<<"fin x value : "<<finbin<<endl;

  //  TH1D * hmean_tot = new TH1D ("hmean_tot","",binnum,0,finbin);
  TH1D * hhit = new TH1D ("hhit","",350,0,350);
  //  TH1D * hmean_sel = new TH1D ("hmean_sel","",350,0,350);  
  //  TH1D * hcnt_tot = new TH1D ("hcnt_tot","",350,0,350);  

  int ibin;
  int mul=1; //div bin
  double hit[7];
  for(int lp=0; lp<fn; lp++){
    //      for(int lp=0; lp<1; lp++){    

    //    ibin=daygap[lp]+1;// ibin start from 0

    TH1D * hhit[procnum[lp]];
    for(int ihis=0; ihis<procnum[lp]; ihis++){
      hhit[ihis]= new TH1D (Form("hit%i",ihis),Form("Run%i day%i trgbit_ratio",run[lp],ihis),8,-0.5,7.5);
    } 

    TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/water_quality/ratio/run%i_24hr.root",run[lp]);
    TChain * chain = new TChain("value");
    chain->Add(rawfile);
    int tent = chain->GetEntries();


    
    for(int i=0; i<tent; i++){
      chain->GetEntry(i);
      double hit0 = chain->GetLeaf("hit")->GetValue(6);//pmt42 hit
      for(int j=0; j<7; j++){

	//	mean[j] = chain->GetLeaf("r_mean")->GetValue(j);// 5=q6/q42
	hit[j] = chain->GetLeaf("hit")->GetValue(j);// 5=q6/q42
	//	cout<<"hit "<<hit[j]<<endl;
	//	cout<<"hit ratio "<<hit[j]/hit0<<endl;
	int binindex=hhit[i]->FindBin(7-j-1);
	hhit[i]->SetBinContent(binindex, hit[j]/hit0);
      }
      

     hhit[i]->SetMarkerStyle(21);
      //      hmean[i]->SetMarkerSize(1.2);
      //    hmean[i]->SetMarkerColor(2);
      hhit[i]->SetMinimum(0.0);
      hhit[i]->SetMaximum(1.1);
      hhit[i]->GetXaxis()->SetTitle("PMT distance");
      hhit[i]->GetYaxis()->SetTitle("hitCNTx/hitCNT0");	
      hhit[i]->SetMarkerColor(2);
      hhit[i]->Draw("P");
      
      
    }
    
  
  TFile * outfile = new TFile(Form("./plot/run%i_trgbit_24hr.root",run[lp]),"RECREATE");
  for(int i=0; i<tent;i++){
  hhit[i]->Write();
  }
  outfile->Close();

    
  }
    
  //  cmean->Update();

  
  


}
