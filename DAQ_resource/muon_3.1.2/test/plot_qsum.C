R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libMuonObjs)

void plot_qsum(int runnum = 252, int daqtype = 1)
{
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_psd");
  chain -> Add(Form("/y/kmseo/data/PSMD/%06d/prd_psd_daq%d_%06d*.root", runnum, daqtype, runnum));
  //chain -> Add("./test_psd.root");
  
  PSEvent *pevt = new PSEvent();
  chain -> SetBranchAddress("PSEvent", &pevt);

  const int col[5] = {1, 2, 4, 6, 8};

  int cut_qsum[22] =
    {13000, 18000, 17000, 22000,
     17000, 16000, 13000, 14000,
     19000, 16000, 16000,
     16000, 16000, 18000, 17000,
     10000, 13000, 13000, 14000,
     19000, 17000, 10500};
  double cut_qmqs[22] =
    {0.4, 0.4, 0.4, 0.4,
     0.4, 0.4, 0.4, 0.4,
     0.4, 0.4, 0.4,
     0.4, 0.4, 0.4, 0.4,
     0.4, 0.4, 0.4, 0.4,
     0.4, 0.4, 0.4};

  TH1D *hqsum[22];
  TH1D *hqsum_sel[22];
  for(int i = 0; i < 22; i++){
    hqsum[i] = new TH1D(Form("hqsum_%d", i), "", 200, 0, 50000);
    hqsum[i] -> SetXTitle("Qsum [ADC]");
    hqsum[i] -> SetYTitle("Number fo Entries");
    hqsum_sel[i] = new TH1D(Form("hqsum_sel_%d", i), "", 200, 0, 50000);
    hqsum_sel[i] -> SetXTitle("Qsum [ADC]");
    hqsum_sel[i] -> SetYTitle("Number fo Entries");
  }
  
  double qsum[22] = {0};
  double q[4], qm, qmqs, time;
  int nch, id, did, chpsd, multi;
  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);
    if(i != 0 && i%1000000 == 0) cout << i << endl;

    nch = pevt -> GetN();
    time = pevt -> GetTriggerTime();

    multi = 0;
    for(int j = 0; j < nch; j++){
      qm = 0;
      PSHit *hit = pevt -> Get(j);

      id = hit -> GetID();

      if((id > 47  && id < 59) ||
	 (id > 112 && id < 124)){
	chpsd = 0;
	if(id < 60)
	  id -= 48;
	else
	  id = id-113+11;

	for(int k = 0; k < 4; k++)
	  q[k] = hit -> GetQ(k);
	qsum[id] = hit -> GetQsum();

	if(qsum[id] > 0)
	  hqsum[id] -> Fill(qsum[id]);
	if(q[0] > 1500 && q[1] > 1500 &&
	   q[2] > 1500 && q[3] > 1500){
	  hqsum_sel[id] -> Fill(qsum[id]);
	}
	
	// cout << qsum[id] << " "
	//      << q[0] << " " << q[1] << " "
	//      << q[2] << " " << q[3] << " "
	//      << qm << endl;
      }
    }
  }

  int n[22] = {0};
  int n_sel[22] = {0};
  int ntot = 0;
  int ntot_sel = 0;
  TGaxis::SetMaxDigits(4);
  TCanvas *can = new TCanvas("can", Form("DAQ %d", daqtype), 1600, 900);
  can -> Divide(4, 3);
  TPad *pad[22];
  for(int i = 11*(daqtype-1); i < 11+11*(daqtype-1); i++){
    can -> cd(i-11*(daqtype-1) +1);
    pad[i] = (TPad*)can -> cd(i+11*(daqtype-1) +1);

    hqsum[i] -> Draw();
    hqsum_sel[i] -> SetLineColor(kRed);
    hqsum_sel[i] -> Draw("same");

    pad[i] -> SetLogy();

    n[i] = hqsum[i] -> Integral();
    ntot += n[i];
    n_sel[i] = hqsum_sel[i] -> Integral();
    ntot_sel += n_sel[i];
  }
  cout << ntot << " " << ntot_sel << endl;
}
