void test_mergeData()
{
  TChain *cw = new TChain("prd_wcd");
  cw -> Add("/data/kmseo/WCMD/000327/prd_wcd_000327_00000.root");
  WCEvent *wevt = new WCEvent();
  cw -> SetBranchAddress("WCEvent", &wevt);

  TChain *cp[2];
  PSEvent *pevt[2];
  for(int i = 0; i < 2; i++){
    cp[i] = new TChain("prd_psd");
    cp[i] -> Add(Form("/data/kmseo/PSMD/000327/prd_psd_daq%d_000327_00000.root", i+1));

    pevt[i] = new PSEvent();
    cp[i] -> SetBranchAddress("PSEvent", &pevt[i]);    
  }

  int nevtw = cw -> GetEntries()/1;
  int nevtp[2] = {static_cast<int>(cp[0] -> GetEntries())/1,
                  static_cast<int>(cp[1] -> GetEntries())/1};
  int tevt = nevtw+nevtp[0]+nevtp[1];
  double dum[3];
  double etime = 0;
  for(int i = 0; i < 3; i++){
    if(i == 0){
      cw -> GetEntry(nevtw-1);
      dum[i] = wevt -> GetTriggerTime();
    }
    else{
      cp[i-1] -> GetEntry(nevtp[i-1]-1);
      dum[i] = pevt[i-1] -> GetTriggerTime();
    }

    if(dum[i] > etime)
      etime = dum[i];
  }

  double qsum[130], id[130], time[3];
  int flag[3];
  int devtp[2] = {0, 0};
  int nn = 0;
  TTree *tree = new TTree("tree", "tree_muon");
  while(nn == tevt){
    for(int i = 0; i < nevtw; i++){
      cw -> GetEntry(i);
      if(i != 0 && i%10000 == 0) cout << i << endl;
      
      time[0] = wevt -> GetTriggerTime();

      





      
    }

  }



}
