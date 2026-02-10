void test_tt()
{
  TChain *chain[2];
  PSEvent *pevt[2];

  for(int i = 0; i < 2; i++){
    chain[i] = new TChain("prd_psd");
    chain[i] -> Add(Form("/data/kmseo/PSMD/000327/prd_psd_daq%d_000327_*.root", i+1));

    pevt[i] = new PSEvent();
    chain[i] -> SetBranchAddress("PSEvent", &pevt[i]);
  }

  for(int i = 0; i < 50; i++){
    chain[0] -> GetEntry(i);
    chain[1] -> GetEntry(i);

    cout << i << " "
	 << pevt[0] -> GetTriggerTime()/1e9 << " "
	 << pevt[1] -> GetTriggerTime()/1e9 << endl;

  }











}
