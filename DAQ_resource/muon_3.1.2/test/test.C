void test()
{
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_psd");
  chain -> Add("data/prd_psd_000109_wQx.root");

  PSEvent *pevt = new PSEvent();
  chain -> SetBranchAddress("PSEvent", &pevt);

  double q[4], qa[4][10];
  int nch, t[4], id;
  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);

    nch = pevt -> GetN();
    for(int j = 0; j < nch; j++){
      PSHit *hit = pevt -> Get(j);
      // id = hit -> GetID();
      
      for(int k = 0; k < 4; k++){
	q[k] = hit -> GetQ(k);
	t[k] = hit -> GetQx(k);
	//cout << j << " " << q[k] << " " << t[k] << endl;

	for(int l = 0; l < 10; l++){
	  qa[k][l] = hit -> GetQa(k, l);
	  cout << l << " " << k << " " << qa[k][l] << endl;
	}
      }
    }

    
  }

}
