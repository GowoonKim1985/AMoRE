R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

#include <cmath> // for std::nan()

void plot2_wcmd_25d_exp()
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

  const int fn = 2;
  
  //15
  int run[fn] = {303,  339, 343, 356, 361, 368};
  int finsub[fn] = {1154, 791, 190, 191, 689, 282, 695, 286, 94, 667, 143, 332};
  //int daygap[fn] = {6, 10, 35, 40, 89, 122, 130, 138, 167, 179, 231, 244, 250, 305, 311};
  int daygap[fn] = {0, 4, 31, 36, 85, 118, 126, 134, 163, 175, 227, 240, 246, 301, 307};

  //  const int dp = 18;
   const int dp = 28;  
  //  int pmtid[dp]={6,7,12,13,18,19,24,25,30,31,36,37,43};
  //  int pmtid[dp]={5,6,11,12,17,18,23,24,29,30,35,36,42};  
  //  double pmtdist[dp]={sqrt(5*5+6*6), 6*sqrt(2), sqrt(4*4+5*5), 5*sqrt(2), sqrt(3*3+4*4), 4*sqrt(2), sqrt(2*2+3*3), 3*sqrt(2), sqrt(1*1+2*2), 2*sqrt(2), 1, sqrt(2), 0};

  int pmtid[dp];
  double pmtdist[dp];
  /*
  pmtid[0]= 0; pmtdist[0] = 6;
  pmtid[1]= 1; pmtdist[1] = sqrt(1*1+6*6);
  pmtid[2]= 2; pmtdist[2] = sqrt(2*2+6*6);
  pmtid[3]= 3; pmtdist[3] = sqrt(3*3+6*6);
  pmtid[4]= 4; pmtdist[4] = sqrt(4*4+6*6);
  pmtid[5]= 5; pmtdist[5] = sqrt(5*5+6*6);
  pmtid[6]= 6; pmtdist[6] = sqrt(6*6+6*6);

  pmtid[7]= 7; pmtdist[7] = 5;
  pmtid[8]= 8; pmtdist[8] = sqrt(1*1+5*5);
  pmtid[9]= 9; pmtdist[9] = sqrt(2*2+5*5);
  pmtid[10]= 10; pmtdist[10] = sqrt(3*3+5*5);
  pmtid[11]= 11; pmtdist[11] = sqrt(4*4+5*5);
  pmtid[12]= 12; pmtdist[12] = sqrt(5*5+5*5);
  pmtid[13]= 13; pmtdist[13] = sqrt(6*6+5*5);

  pmtid[14]= 14; pmtdist[14] = 4;
  pmtid[15]= 15; pmtdist[15] = sqrt(1*1+4*4);
  pmtid[16]= 16; pmtdist[16] = sqrt(2*2+4*4);
  pmtid[17]= 17; pmtdist[17] = sqrt(3*3+4*4);
  pmtid[18]= 18; pmtdist[18] = sqrt(4*4+4*4);
  pmtid[19]= 19; pmtdist[19] = sqrt(5*5+4*4);
  pmtid[20]= 20; pmtdist[20] = sqrt(6*6+4*4);

  pmtid[21]= 21; pmtdist[21] = 3;
  pmtid[22]= 22; pmtdist[22] = sqrt(1*1+3*3);
  pmtid[23]= 23; pmtdist[23] = sqrt(2*2+3*3);
  pmtid[24]= 24; pmtdist[24] = sqrt(3*3+3*3);
  pmtid[25]= 25; pmtdist[25] = sqrt(4*4+3*3);
  pmtid[26]= 26; pmtdist[26] = sqrt(5*5+3*3);
  pmtid[27]= 27; pmtdist[27] = sqrt(6*6+3*3);

  pmtid[28]= 28; pmtdist[28] = 2;
  pmtid[29]= 29; pmtdist[29] = sqrt(1*1+2*2);
  pmtid[30]= 30; pmtdist[30] = sqrt(2*2+2*2);
  pmtid[31]= 31; pmtdist[31] = sqrt(3*3+2*2);
  pmtid[32]= 32; pmtdist[32] = sqrt(4*4+2*2);
  pmtid[33]= 33; pmtdist[33] = sqrt(5*5+2*2);
  pmtid[34]= 34; pmtdist[34] = sqrt(6*6+2*2);

  pmtid[35]= 35; pmtdist[35] = 1;
  pmtid[36]= 36; pmtdist[36] = sqrt(1*1+1*1);
  pmtid[37]= 37; pmtdist[37] = sqrt(2*2+1*1);
  pmtid[38]= 38; pmtdist[38] = sqrt(3*3+1*1);
  pmtid[39]= 39; pmtdist[39] = sqrt(4*4+1*1);
  pmtid[40]= 40; pmtdist[40] = sqrt(5*5+1*1);
  pmtid[41]= 41; pmtdist[41] = sqrt(6*6+1*1);

  pmtid[42]= 42; pmtdist[42] = 0;
  pmtid[43]= 43; pmtdist[43] = 1;
  pmtid[44]= 44; pmtdist[44] = 2;
  pmtid[45]= 45; pmtdist[45] = 3;
  pmtid[46]= 46; pmtdist[46] = 4;
*/  
  
  pmtid[0]= 0; pmtdist[0] = 6;
  pmtid[1]= 1; pmtdist[1] = sqrt(1*1+6*6);
  pmtid[2]= 2; pmtdist[2] = sqrt(2*2+6*6);
  pmtid[3]= 3; pmtdist[3] = sqrt(3*3+6*6);
  pmtid[4]= 4; pmtdist[4] = sqrt(4*4+6*6);
  pmtid[5]= 5; pmtdist[5] = sqrt(5*5+6*6);
  pmtid[6]= 6; pmtdist[6] = sqrt(6*6+6*6);

  pmtid[7]= 7; pmtdist[7] = 5;
  pmtid[8]= 8; pmtdist[8] = sqrt(1*1+5*5);
  pmtid[9]= 9; pmtdist[9] = sqrt(2*2+5*5);
  pmtid[10]= 10; pmtdist[10] = sqrt(3*3+5*5);
  pmtid[11]= 11; pmtdist[11] = sqrt(4*4+5*5);
  pmtid[12]= 12; pmtdist[12] = sqrt(5*5+5*5);

  pmtid[13]= 14; pmtdist[13] = 4;
  pmtid[14]= 15; pmtdist[14] = sqrt(1*1+4*4);
  pmtid[15]= 16; pmtdist[15] = sqrt(2*2+4*4);
  pmtid[16]= 17; pmtdist[16] = sqrt(3*3+4*4);
  pmtid[17]= 18; pmtdist[17] = sqrt(4*4+4*4);

  pmtid[18]= 21; pmtdist[18] = 3;
  pmtid[19]= 22; pmtdist[19] = sqrt(1*1+3*3);
  pmtid[20]= 23; pmtdist[20] = sqrt(2*2+3*3);
  pmtid[21]= 24; pmtdist[21] = sqrt(3*3+3*3);

  pmtid[22]= 28; pmtdist[22] = 2;
  pmtid[23]= 29; pmtdist[23] = sqrt(1*1+2*2);
  pmtid[24]= 30; pmtdist[24] = sqrt(2*2+2*2);

  pmtid[25]= 35; pmtdist[25] = 1;
  pmtid[26]= 36; pmtdist[26] = sqrt(1*1+1*1);

  pmtid[27]= 42; pmtdist[27] = 0;
  
  
 
  
  //  int subdiv=50; // how many sub files
  int subdiv=24; // how many sub files
  double mean[7];
  TCanvas * cmean = new TCanvas("cmean","cmean",800,600);
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
  TH1D * hmean_tot = new TH1D ("hmean_tot","",350,0,350);
  TH1D * hmean_sel = new TH1D ("hmean_sel","",350,0,350);  
  TH1D * hcnt_tot = new TH1D ("hcnt_tot","",350,0,350);  

  int ibin;
  int mul=1; //div bin
  double hit[7];
  for(int lp=0; lp<fn; lp++){
    //  for(int lp=0; lp<1; lp++){    
    //    ibin=daygap[lp]+1;// ibin start from 0

    TH1D * hmean[procnum[lp]];
    for(int ihis=0; ihis<procnum[lp]; ihis++){
      hmean[ihis]= new TH1D (Form("ratio%i",ihis),Form("Run%i day%i ratio",run[lp],ihis),80,0,8);
    } 

    TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/water_quality/ratio/run%i_24hr_exp.root",run[lp]);
    TChain * chain = new TChain("value");
    chain->Add(rawfile);
    int tent = chain->GetEntries();
    
    for(int i=0; i<tent; i++){

      chain->GetEntry(i);
      cout<<i<<endl;
      cout<<""<<endl;
      //          cout<<"pmt dist 0 "<<pmtdist[0]<<endl;

      for(int j=0; j<dp; j++){	
	//	cout<<j<<endl;
	int id = pmtid[j];
	//	cout<<id<<endl;
	mean[id] = chain->GetLeaf("r_mean")->GetValue(id);// 5=q6/q42
	//	cout<<"mean "<<mean[pmtid[j]]<<endl;
	hit[id] = chain->GetLeaf("hit")->GetValue(id);// 5=q6/q42	

	if(mean[id]>0){
	    cout<<"id "<<pmtid[j]<<" pmt dist "<<pmtdist[j]<<endl;
	    int binindex=hmean[i]->FindBin(pmtdist[j]);
	    hmean[i]->SetBinContent(binindex, mean[id]);
	  }
	  if(mean[pmtid[j]]==0){
	    //	  cout<<"run"<<run[lp]<<" div "<<i<<" mean["<<j<<"] = 0"<<endl;
	    //	  hmean[i]->SetBinContent(7-j, -9999);
	  }

      }

      hmean[i]->SetMarkerStyle(21);
      //      hmean[i]->SetMarkerSize(1.2);
      //    hmean[i]->SetMarkerColor(2);
      hmean[i]->SetMinimum(0.0);
      hmean[i]->SetMaximum(1.1);
      hmean[i]->GetXaxis()->SetTitle("PMT distance");
      hmean[i]->GetYaxis()->SetTitle("Qx/Q0");	
      hmean[i]->Draw("P");
      
      
      ibin = daygap[lp]+i;
      //      cout<<"run : "<<run[lp]<<"day : "<<i<<" ibin: "<<ibin<<" mean : "<<mean[0]<<endl;

      //     hmean_tot->SetBinContent(ibin+1, mean[0]);
      //      if(mean[0]>0&&hit[0]>0){hmean_sel->SetBinContent(ibin+1, mean[0]*hit[6]/hit[0]);}
      //      if(hit[6]>0){hcnt_tot->SetBinContent(ibin+1, hit[0]/hit[6]);}
    }   
    
  
  TFile * outfile = new TFile(Form("./plot/run%i_ratio_24hr_exp.root",run[lp]),"RECREATE");
  for(int i=0; i<tent;i++){
  hmean[i]->Write();
  }
  outfile->Close();

    
  }
    
  //  cmean->Update();

  
  

}
