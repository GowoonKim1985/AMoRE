R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void ratio1_wcmd()
{
  //
  gStyle -> SetOptStat(0);


  //run 330 sub 0 - 695
  //run 339 sub 0 - 286
  //run 368 sub 0 - 239
  
  int run=303;
  int finsub=1154;
  int subdiv=168; // how many sub files
  int procnum=finsub/subdiv; // loop num
  TFile *of[procnum];
  //  int sub1=0;
  // int sub2=49;

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

 // TString outfile = Form("./ratio1/run%i_%i_to_%i.root",run, sub1, sub2);
  TString outfile = Form("./ratio1_1wk/run%i_%04d_to_%04d.root",run, sub1, sub2);
  of[proc] = new TFile(outfile.Data(), "RECREATE");
  rhis1->Write();

  mean_r1=r_mean;
  hit42=cnt42;
  hit06=cnt6;
  value->Fill();
  value->Write();

  
  of[proc]->Close();
  cout<<"process "<<proc<<" finished"<<endl;;
  //  delete of[proc];
  //  his1_2d->Draw();
  /*    
  cout<<"id 42 cnts "<<cnt[0]<<endl;
  cout<<"id 6 cnts "<<cnt[1]<<endl;
  cout<<"id 24 cnts "<<cnt[2]<<endl;
  cout<<"id 36 cnts "<<cnt[3]<<endl;  
  TCanvas *c1 = new TCanvas("c1","",600,400);
    c1->cd();
    rhis1->Draw();  
    TCanvas *c2 = new TCanvas("c2","",600,400);
    c2->cd();
    rhis2->Draw();  
    TCanvas *c3 = new TCanvas("c3","",600,400);
    c3->cd();
    rhis3->Draw();  
  
  */
  
  }  
}
