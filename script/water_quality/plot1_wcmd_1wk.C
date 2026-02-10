R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void plot1_wcmd_1wk()
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
  //run 368 sub 0 - 239

  const int fn = 16;
  
  int run[fn] = {272,287,288,294,303,304,311,315,318,327,330,339,343,356,361,368};  
  int finsub[fn] = {64,93,525,76,1154,791,190,191,689,282,695,286,94,667,143,239};
  
  //  int subdiv=50; // how many sub files
  int subdiv=168; // how many sub files
  double mean;
  TCanvas * cmean = new TCanvas("cmean","cmean",800,600);

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

  TH1D * hmean_tot = new TH1D ("hmean_tot","",binnum,0,finbin);
  int ibin = -1;
  
  for(int lp=0; lp<fn; lp++){
    ibin=ibin+1;// ibin start from 0
    
    int procnum=finsub[lp]/subdiv; // loop num
    TH1D * hmean = new TH1D ("hmean",Form("Run%i ratio1",run[lp]),procnum,0,subdiv*procnum);
  
    for(int proc=0; proc<procnum; proc++){
      ibin=ibin+1;
      int sub1=subdiv*proc;
      int sub2=sub1+subdiv-1;
      //    TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/ratio1/run%i_%i_to_%i.root",run,sub1,sub2);
      TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/ratio1_1wk/run%i_%04d_to_%04d.root",run[lp],sub1,sub2);
      TChain * chain = new TChain("value");
      chain->Add(rawfile);
      chain->GetEntry(0);
      mean = chain->GetLeaf("mean_r1")->GetValue();
      cout<<mean<<endl;
      hmean->SetBinContent(proc+1,mean);
      hmean_tot->SetBinContent(ibin,mean);
  }
  
  hmean->SetMarkerStyle(2);
  //  hmean->SetMarkerSize(1.2);
  //  hmean->SetLineWidth(2);
  hmean->SetMinimum(0.0);
  hmean->SetMaximum(1.0);
  
  hmean->Draw("PL");
  cmean->Update();


  
  TFile * outfile = new TFile(Form("./plot/run%i_div%i_24hr.root",run[lp],subdiv),"RECREATE");
  hmean->Write();
  outfile->Close();
  }

  
  hmean_tot->SetMarkerStyle(2);
  //  hmean->SetMarkerSize(1.2);
  //  hmean->SetLineWidth(2);
  hmean_tot->SetMinimum(0.0);
  hmean_tot->SetMaximum(1.0);
  
  hmean_tot->Draw("PL");
  cmean->Update();

  
  TFile * toutfile = new TFile("./plot/ratio1_1wk.root","RECREATE");
  hmean_tot->Write();
  toutfile->Close();

}
