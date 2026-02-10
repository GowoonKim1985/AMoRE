R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void ratio2_wcmd_24hr_exp()
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

  /*
  int run[fn] = {272};
  int finsub[fn] = {64};
  int daygap[fn] = {0};
  */

  //16
  int run[fn] = {287, 288, 294, 303, 304, 311, 315, 318, 327, 330, 339, 343, 356, 361, 368};
  int finsub[fn] = {93, 525, 76, 1154, 791, 190, 191, 689, 282, 695, 286, 94, 667, 143, 332};
  //int daygap[fn] = {6, 10, 35, 40, 89, 122, 130, 138, 167, 179, 231, 244, 250, 305, 311};
  int daygap[fn] = {0, 4, 31, 36, 85, 118, 126, 134, 163, 175, 227, 240, 246, 301, 307};
  
  int subdiv=24; // how many sub files

  TFile *of[fn];

      for(int lp=1; lp<fn; lp++){
  //    for(int lp=0; lp<1; lp++){    

      
  int procnum=finsub[lp]/subdiv; // loop num

   
    TH1D * r_his[procnum];
  
  //  TCanvas * cmean = new TCanvas("cmean","cmean",800,600);
  //  TH1D * hmean = new TH1D ("hmean",Form("Run%i ratio1",run[lp]),procnum,0,subdiv*procnum);

  TTree * value = new TTree("value","value");
  TTree * charge = new TTree("charge","charge");

  const int tid=7;
  double r_mean[48], q[48];// sigma: standard deviation
  double sigma[48]={0};
  int tent, day, nch, evt, tbit; 
  int hit[48];
    int fid[48];


  value->Branch("day",&day,"day/I");
  value->Branch("r_mean",r_mean,"r_mean[48]/D");
  
  value->Branch("hit",hit,"hit[48]/I");
  
    charge->Branch("tbit",&tbit,"tbit/I");  
  charge->Branch("q",q,Form("q[%i]/D",nch));


  
  // 24hr file process
  
  for(int proc=0; proc<procnum; proc++){

    //  TH1D * hmean = new TH1D ("hmean",Form("Run%i ratio1",run[lp]),procnum,0,subdiv*procnum);

    r_his[proc] = new TH1D (Form("r_his%i",proc),Form("Run%i Q7/Q43 24hr_div%i",run[lp],proc),100,0,1);

    cout<<"run "<<run[lp]<<endl;
    cout<<"day "<<proc+1<<endl;
    int sub1=subdiv*proc;
    int sub2=sub1+subdiv-1;
    day = proc;

    for(int del = 0; del<48; del++){r_mean[del]=0; hit[del]=0;}
    //    cout<<"r_mean[0]"<<r_mean[0]<<endl;
    TChain *chain = new TChain("prd_wcd");

    for(int sub=sub1; sub<=sub2; sub++){
      chain -> Add(Form("/home/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run[lp],run[lp],sub));
    }
   
        tent = chain->GetEntries();
	//    tent = 10000;
    
    double qmax;
    int qmax_fid;
    double ratio[48];
    
    for(int i=0; i<tent; i++){
      //          for(int i=0; i<1000; i++){      
      if(i%100000==0){cout<<i<<" ent process..."<<endl;}
      //             cout<<i<<endl;
      qmax=0, qmax_fid=999;
      chain->GetEntry(i);
      //      evt=i;
      //      evt = chain->GetLeaf("fEventNumber")->GetValue();
      nch = chain->GetLeaf("fColl.fID")->GetNdata();
      //      cout<<"nch "<<nch<<endl;
    //max Q check
      for(int j=0; j<nch; j++){
	fid[j]=chain->GetLeaf("fColl.fID")->GetValue(j);
	q[j]=chain->GetLeaf("fColl.fQtot")->GetValue(j);
	if(qmax<q[j]){qmax=q[j]; qmax_fid=fid[j];}      
      }
    
    //if Q42 = max, get Q6/Q42 ratio
      //       cout<<"r_mean[0]"<<r_mean[0]<<endl;
      if(qmax_fid==42){
	//	cout<<"qmax pmt 42"<<endl;
	for(int k=0; k<48; k++){q[k]=0; ratio[k]=0;}

	for(int j=0; j<nch; j++){
	  fid[j]=chain->GetLeaf("fColl.fID")->GetValue(j);
	  //	  cout<<fid[j]<<endl;
	  q[fid[j]]=chain->GetLeaf("fColl.fQtot")->GetValue(j);
	  //	  cout<<q[fid[j]]<<endl;
	}
	tbit=nch;
	//			cout<<"test"<<endl;
	charge->Fill();
	//		cout<<"test"<<endl;
	//	cout<<"qmax "<<qmax<<endl;
	//	    cout<<"r_mean[0]"<<r_mean[0]<<endl;
	for(int k=0; k<48; k++){
	  if(q[k]>0){
	    //	    	    cout<<"r_mean(sum):"<<r_mean[k]<<endl;
	    hit[k]=hit[k]+1;
	    ratio[k]=q[k]/qmax;
	    //	    cout<<"q"<<k<<" "<<q[k]<<endl;

	    r_mean[k]=r_mean[k]+ratio[k];
	    //	    	    cout<<"ratio["<<k<<"] : "<<ratio[k]<<endl;

	  }
	}
      }
	  }

    //get mean
    for(int cal=0; cal<48; cal++){
      cout<<"tot hit @fid "<<cal<<" : "<<hit[cal]<<endl;
      r_mean[cal]=r_mean[cal]/hit[42];
      cout<<"Ratio q"<<cal<<"/q42 mean : "<<r_mean[cal]<<endl;
    }
  

    
    value->Fill();

  }
  
    TString outfile = Form("./ratio/run%i_24hr_exp.root",run[lp]); 
    of[lp] = new TFile(outfile.Data(), "RECREATE");

    //    rhis1->Write();

    value->Write();
    charge->Write();
    /*    for(int ihis=0; ihis<procnum; ihis++){
      r_his[ihis]->GetXaxis()->SetTitle("Q7/Q43");
      r_his[ihis]->Write();
      }*/
    
    of[lp]->Close();

    //    hmean->SetBinContent(proc+1,mean_r1);
  
  }
    }

/* 
  hmean->SetMarkerStyle(20);
  hmean->SetMinimum(0.0);
  hmean->SetMaximum(1.0);
  hmean->Draw("PL");
  cmean->Update();

  TFile * plotfile = new TFile(Form("./plot/run%i_div%i.root",run[lp],subdiv),"RECREATE");
  hmean->Write();
  plotfile->Close();

  }
}
*/
