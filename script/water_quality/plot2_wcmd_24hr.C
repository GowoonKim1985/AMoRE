R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

#include <cmath> // for std::nan()

void plot2_wcmd_24hr()
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
  TCanvas * cmean = new TCanvas("cmean","cmean",800,600);
  TCanvas * cmean_sel = new TCanvas("cmean_sel","cmean_sel",800,600);  
  TCanvas * ccnt = new TCanvas("ccnt","ccnt",800,600);  

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
    //  for(int lp=0; lp<4; lp++){    

    //    ibin=daygap[lp]+1;// ibin start from 0

    TH1D * hmean[procnum[lp]];
    for(int ihis=0; ihis<procnum[lp]; ihis++){
      hmean[ihis]= new TH1D (Form("ratio%i",ihis),Form("Run%i day%i ratio",run[lp],ihis),8,-0.5,7.5);
    } 

    TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/water_quality/ratio/run%i_24hr.root",run[lp]);
    TChain * chain = new TChain("value");
    chain->Add(rawfile);
    int tent = chain->GetEntries();


    
    for(int i=0; i<tent; i++){
      chain->GetEntry(i);

      for(int j=0; j<7; j++){
	mean[j] = chain->GetLeaf("r_mean")->GetValue(j);// 5=q6/q42
	hit[j] = chain->GetLeaf("hit")->GetValue(j);// 5=q6/q42	
	if(j<6){
	  if(mean[j]>0){
	    int binindex=hmean[i]->FindBin(7-j-1);
	    hmean[i]->SetBinContent(binindex, mean[j]);
	  }
	  if(mean[j]==0){
	    //	  cout<<"run"<<run[lp]<<" div "<<i<<" mean["<<j<<"] = 0"<<endl;
	    //	  hmean[i]->SetBinContent(7-j, -9999);
	  }
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

      hmean_tot->SetBinContent(ibin+1, mean[0]);
      if(mean[0]>0&&hit[0]>0){hmean_sel->SetBinContent(ibin+1, mean[0]*hit[6]/hit[0]);}
      if(hit[6]>0){hcnt_tot->SetBinContent(ibin+1, hit[0]/hit[6]);}
    }   
    
  
  TFile * outfile = new TFile(Form("./plot/run%i_ratio_24hr.root",run[lp]),"RECREATE");
  for(int i=0; i<tent;i++){
  hmean[i]->Write();
  }
  outfile->Close();

    
  }
    
  //  cmean->Update();

  
  


  
  hmean_tot->SetMarkerStyle(2);
  hmean_tot->SetMinimum(0.0);
    hmean_tot->SetMaximum(0.1);

  hmean_tot->GetXaxis()->SetTitle("time(day)");
  hmean_tot->GetYaxis()->SetTitle("Q6/Q42 mean");	
  cmean->cd();
  hmean_tot->Draw("P");

cmean->Update();

  hmean_sel->SetMarkerStyle(2);
  hmean_sel->SetMinimum(0.0);
  hmean_sel->SetMaximum(1.0);

  hmean_sel->GetXaxis()->SetTitle("time(day)");
  hmean_sel->GetYaxis()->SetTitle("Q6/Q42 mean (Q6>0)");	
  cmean_sel->cd();
  hmean_sel->Draw("P");

cmean->Update();

 
  hcnt_tot->SetMarkerStyle(2);



  hcnt_tot->GetXaxis()->SetTitle("time(day)");
  hcnt_tot->GetYaxis()->SetTitle("Q6/Q42 hit");	
  ccnt->cd();
  hcnt_tot->Draw("P");

 
  
  TFile * toutfile = new TFile("./plot/ratio_24hr.root","RECREATE");
  hmean_tot->Write();
  hmean_sel->Write();
  hcnt_tot->Write();
  toutfile->Close();

}
