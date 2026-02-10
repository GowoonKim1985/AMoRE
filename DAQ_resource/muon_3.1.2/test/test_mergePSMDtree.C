void test_mergePSMDtree()
{
  int runnum = 327;
  
  TChain *pchain[2];
  PSEvent *pevt[2];
  int nevt_d1[2] = {0, 0};
  int pnevt[2];
  for(int i = 0; i < 2; i++){
    pchain[i] = new TChain("prd_psd");
    pchain[i] -> Add(Form("/data/kmseo/PSMD/%06d/prd_psd_daq%d_%06d_00000.root", runnum, i+1, runnum));
    pevt[i] = new PSEvent();
    pchain[i] -> SetBranchAddress("PSEvent", &pevt[i]);
    
    pnevt[i] = pchain[i] -> GetEntries();
  }

  int nevt[2] = {static_cast<int>(pchain[0] -> GetEntries())/1,
                 static_cast<int>(pchain[1] -> GetEntries())/1};
  const int tevt = nevt[0]+nevt[1];
  int devt[2] = {0};
  double time_dum[2];
  int nn = 0;

  double qsum[130] = {0};
  int id[130] = {0};
  double time[130] = {0};
  int nch = 0;
  TTree *tree = new TTree("tree_psd", "tree_psd");
  tree -> Branch("id", id, "id[130]/I");
  tree -> Branch("nch", &nch);
  tree -> Branch("qsum", qsum, "qsum[130]/D");
  tree -> Branch("time", time, "time[130]/D");
  while(nn < tevt){
    for(int i = devt[0]; i < nevt[0]; i++){
      pchain[0] -> GetEntry(i);
      time_dum[0] = pevt[0] -> GetTriggerTime();

      //nch = pevt[0] -> GetN();
      int nch1 = pevt[0] -> GetN();
      nch += nch1;

      for(int a = devt[1]; a < nevt[1]; a++){
	pchain[1] -> GetEntry(a);
	time_dum[1] = pevt[1] -> GetTriggerTime();
	//time[1] = time_dum[1];
	
	if(time_dum[1] > time_dum[0]){
	  devt[1] = a;
	  break;
	}
	else{
	  //nch = pevt[1] -> GetN();
	  int nch2 = pevt[1] -> GetN();
	  nch += nch2;
	  
	  //cout << "daq2 " << nn << " " << time_dum[1]/1e9 << " " << endl;
	  // cout << nch << endl;
	  for(int b = 0; b < nch2; b++){
	    PSHit *hit = pevt[1] -> Get(b);
	    if(!hit) continue;
	    
	    id[b+nch1] = hit -> GetID();
	    qsum[b+nch1] = hit -> GetQsum();
	    time[b+nch1] = time_dum[1];
	  }
	  tree -> Fill();

	  nch = 0;
	  nn++;
	  if(nn == tevt){
	    TFile of("./test_tree.root", "recreate");
	    tree -> Write();
	    of.Close();
	    
	    return 0;
	  }
	}
      }
      //time[0] = time_dum[0];

      //cout << "daq1 " << nn << " " << time_dum[0]/1e9 << " " << endl;
      // cout << nch << endl;
      for(int j = 0; j < nch1; j++){
	PSHit *hit = pevt[0] -> Get(j);
	if(!hit) continue;

	id[j] = hit -> GetID();
	qsum[j] = hit -> GetQsum();
	time[0] = time_dum[0];
      }
      
      tree -> Fill();

      nch = 0;
      nn++;
      if(nn == tevt) break;
    }

    if(nn == tevt) break;
  }

  TFile of("./test_tree.root", "recreate");
  tree -> Write();
  of.Close();
}
