R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void wcmd_plot()
{
  //
  gStyle -> SetOptStat(0);


  int run = 330;
  int finsub = 695;
 


  TTree * value = new TTree("value","value");
  //  TTree * charge = new TTree("charge","charge");

  double trgtime, qsum;
  double qtot[48], fmax[48], qmax[48];
  int ch[48];
  int counts, sub, evtnum, tbit;
  double hit[48], fid[48];

  const int tbitmax=48;
  //  value->Branch("sub",&sub,"sub/I");
  value->Branch("evtnum",&evtnum,"evtnum/I");
  value->Branch("trgtime",&trgtime,"trgtime/D");
  value->Branch("tbit",&tbit,"tbit/I");
  value->Branch("qsum",&qsum,"qsum/D");
  value->Branch("ch",ch, "ch[tbit]/I");
  value->Branch("qtot",qtot,"qtot[tbit]/D");
  value->Branch("qmax",qmax,"qmax[tbit]/D");    
  value->Branch("fmax",fmax,"fmax[tbit]/D");  
 
  TChain *chain = new TChain("prd_wcd");
  
  for(int sub=0; sub<=finsub; sub++){
    //  for(int sub=0; sub<=0; sub++){
    chain -> Add(Form("/home/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,sub));
    cout<<"sub file "<<sub<<" processing..."<<endl;
   
  int tent = chain->GetEntries();
  
   for(int i=0; i<tent; i++){
     //  for(int i=0; i<10; i++){    

    chain->GetEntry(i);
    evtnum = chain->GetLeaf("fEventNumber")->GetValue();
    trgtime = chain->GetLeaf("fTriggerTime")->GetValue();
    tbit = chain->GetLeaf("fColl.fID")->GetNdata();

    qsum=0;
    for(int j=0; j<tbit; j++){
      ch[j] = chain->GetLeaf("fColl.fID")->GetValue(j);
      qtot[j] = chain->GetLeaf("fColl.fQtot")->GetValue(j);     
      qmax[j] = chain->GetLeaf("fColl.fQmax")->GetValue(j);
      fmax[j] = chain->GetLeaf("fColl.fFmax")->GetValue(j);      
      qsum = qsum+qtot[j];
      //      cout<<ch[j]<<" "<< qtot[j] <<" "<<qmax[j] <<" "<<fmax[j]<<endl;
      }
    //    cout<<qsum<<endl;
    value->Fill();
  }

  TString outfile = Form("./prod_2nd/%06d/wcmd_%06d_%05d.root",run,run,sub); 
   TFile *of = new TFile(outfile.Data(), "RECREATE");

    //    rhis1->Write();

   value->Write();

    of->Close();
  }

  /*
      
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

	for(int k=0; k<48; k++){q[k]=0; ratio[k]=0;}
	//	q[0]=0;	q[1]=0;	q[2]=0;	q[3]=0;	q[4]=0;	q[5]=0; q[6]=0;//q6, 12, 18, 24, 30, 36, 42
	
	for(int j=0; j<nch; j++){
	  fid[j]=chain->GetLeaf("fColl.fID")->GetValue(j);
	  q[fid[j]]=chain->GetLeaf("fColl.fQtot")->GetValue(j);
	}
	charge->Fill();
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
  

    


  }
  
*/
    //    hmean->SetBinContent(proc+1,mean_r1);
  
  }
    
