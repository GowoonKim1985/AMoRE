R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void ratio1_wcmd_24hr()
{
  //
  gStyle -> SetOptStat(0);

  int run=315; 
  int finsub=191;
 
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
  
  int subdiv=24; // how many sub files
  int procnum=finsub/subdiv; // loop num
  TFile *of[procnum];

  TCanvas * cmean = new TCanvas("cmean","cmean",800,600);
  TH1D * hmean = new TH1D ("hmean",Form("Run%i ratio1",run),procnum,0,subdiv*procnum);
  
  for(int proc=0; proc<procnum; proc++){
    int sub1=subdiv*proc;
    int sub2=sub1+subdiv-1;
    double mean_r1;
    int tent, hit42, hit06;
    TTree * value = new TTree("value","ratio mean of Q6/Q42 @ Qmax=Q42");
    value->Branch("mean_r1",&mean_r1,"mean_r1/D");
    value->Branch("tent",&tent,"tent/I");
    value->Branch("hit42",&hit42,"hit42/I");
    value->Branch("hit06",&hit06,"hit06/I");

  
    TChain *chain = new TChain("prd_wcd");
    for(int sub=sub1; sub<=sub2; sub++){
      //      chain -> Add(Form("/home/kkw/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,sub));
      chain -> Add(Form("/home/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,sub));
    }

  tent = chain->GetEntries();
  cout<<"tent "<<tent<<endl;
  int q1ent = chain->GetEntries("fID==42");
  int fid[48]; int flag;
  //  cout<<q1ent<<endl;
  double q[48];
  double qmax, r, r_mean;
  int qmax_fid;
  double q42, q6; //42=0, 6=1
  int cnt42, cnt6;
  
  TH1D * rhis1 = new TH1D("rhis1","q6/q42",100,0,1);

  cnt42=0; cnt6=0; r_mean=0;
  
    for(int i=0; i<tent; i++){
      if(i%100000==0){cout<<i<<" ent process..."<<endl;}
      //     for(int i=0; i<1000; i++){
	 //	 cout<<""<<endl;
	 //	 cout<<"ent "<<i<<endl;
    flag=0; qmax=0, qmax_fid=999;
    
    chain->GetEntry(i);
    int nch = chain->GetLeaf("fColl.fID")->GetNdata();

    //max Q check
    for(int j=0; j<nch; j++){
      fid[j]=chain->GetLeaf("fColl.fID")->GetValue(j);
      q[j]=chain->GetLeaf("fColl.fQtot")->GetValue(j);
      //      cout<<"id "<<fid[j]<<" q "<<q[j]<<endl;
      if(qmax<q[j]){qmax=q[j]; qmax_fid=fid[j];}      
    }
    
    //if Q42 = max, get Q6/Q42 ratio
    
    if(qmax_fid==42){

      q42=0; q6=0; 
      cnt42=cnt42+1;

      for(int j=0; j<nch; j++){
	fid[j]=chain->GetLeaf("fColl.fID")->GetValue(j);
	if(fid[j]==42){
	  q42=chain->GetLeaf("fColl.fQtot")->GetValue(j);
	  //  cout<<"q42 "<<q42<<endl;
	}
	if(fid[j]==6){
	  q6=chain->GetLeaf("fColl.fQtot")->GetValue(j);
	  //  cout<<"q6 "<<q6<<endl;
	}
      }

      if(q6>0){
	cnt6=cnt6+1; r=q6/q42;
	rhis1->Fill(r);
	r_mean=r_mean+r;
      }
    }
  }

  cout<<"--Qmax fid 42--"<<endl;
  cout<<"fid 42 hit : "<<cnt42<<endl;
  cout<<"fid 6 hit : "<<cnt6<<endl;
  cout<<"sum of Q6/Q42 : "<<r_mean<<endl;
  r_mean=r_mean/cnt6;
  cout<<"Ratio mean : "<<r_mean<<endl;

  TString outfile = Form("./ratio1_24hr/run%i_%04d_to_%04d.root",run, sub1, sub2);
  //  TFile *of = new TFile(outfile.Data(), "RECREATE");
  of[proc] = new TFile(outfile.Data(), "RECREATE");
  // TFile of(Form("./run%i_%i_to_%i.root",run, sub1, sub2),"recreate");
  rhis1->Write();

  mean_r1=r_mean;
  hit42=cnt42;
  hit06=cnt6;
  value->Fill();
  value->Write();

  
  of[proc]->Close();
  cout<<"process "<<proc<<" finished"<<endl;;

  hmean->SetBinContent(proc+1,mean_r1);
  
  }
  
  hmean->SetMarkerStyle(20);
  hmean->SetMinimum(0.0);
  hmean->SetMaximum(1.0);
  hmean->Draw("PL");
  cmean->Update();

  TFile * plotfile = new TFile(Form("./plot/run%i_div%i.root",run,subdiv),"RECREATE");
  hmean->Write();
  plotfile->Close();

}
