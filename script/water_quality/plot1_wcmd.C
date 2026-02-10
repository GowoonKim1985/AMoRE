R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void plot1_wcmd()
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

  int run=368;  
  int finsub=239;
  //  int subdiv=50; // how many sub files
  int subdiv=24; // how many sub files
  int procnum=finsub/subdiv; // loop num
  double mean;
  TCanvas * cmean = new TCanvas("cmean","cmean",800,600);
  TH1D * hmean = new TH1D ("hmean",Form("Run%i ratio1",run),procnum,0,subdiv*procnum);
  
  for(int proc=0; proc<procnum; proc++){
    int sub1=subdiv*proc;
    int sub2=sub1+subdiv-1;
    //    TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/ratio1/run%i_%i_to_%i.root",run,sub1,sub2);
    TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/ratio1_24hr/run%i_%04d_to_%04d.root",run,sub1,sub2);
    TChain * chain = new TChain("value");
    chain->Add(rawfile);
    chain->GetEntry(0);
    mean = chain->GetLeaf("mean_r1")->GetValue();
    cout<<mean<<endl;
    hmean->SetBinContent(proc+1,mean);
  }
  
  hmean->SetMarkerStyle(20);
  //  hmean->SetMarkerSize(1.2);
  //  hmean->SetLineWidth(2);
  hmean->SetMinimum(0.0);
  hmean->SetMaximum(1.0);
  
  hmean->Draw("PL");
  cmean->Update();

  TFile * outfile = new TFile(Form("./plot/run%i_div%i_24hr.root",run,subdiv),"RECREATE");
  hmean->Write();
  outfile->Close();
}
