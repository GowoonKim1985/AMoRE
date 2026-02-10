R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(libMuonObjs)

void plot_psd_all()
{
  int runnum = 327;
  
  gStyle -> SetOptStat(0);
  
  TChain *chain = new TChain("prd_psd");
  // for(int i = 0; i < 1; i++){
  //   chain -> Add(Form("/data/kmseo/PSMD/%06d/prd_psd_daq1_%06d_%05d.root", runnum, runnum, i));
  //   chain -> Add(Form("/data/kmseo/PSMD/%06d/prd_psd_daq2_%06d_%05d.root", runnum, runnum, i));
  // }
  chain -> Add(Form("/data/kmseo/PSMD/%06d/prd_psd_daq1_%06d_*.root", runnum, runnum));
  chain -> Add(Form("/data/kmseo/PSMD/%06d/prd_psd_daq2_%06d_*.root", runnum, runnum));
  
  PSEvent *pevt = new PSEvent();
  chain -> SetBranchAddress("PSEvent", &pevt);

  const int npanel = 130;
  int cut_qsum = 10000;
  double cut_qmqs = 0.4;

  TH1D *hqsum[npanel];
  TH1D *hqsum_sel[npanel];
  for(int i = 0; i < npanel; i++){
    hqsum[i] = new TH1D(Form("hqsum_%d", i), "", 1000, 0, 200000);
    hqsum[i] -> SetYTitle("Number of Entries");
    hqsum[i] -> SetXTitle(Form("Qsum of panel %d", i+1));
    hqsum[i] -> SetLineColor(kBlack);
    
    hqsum_sel[i] = new TH1D(Form("hqsum_sel_%d", i), "", 1000, 0, 200000);
    hqsum_sel[i] -> SetYTitle("Number of Entries");
    hqsum_sel[i] -> SetXTitle(Form("Qsum of panel %d", i+1));
    hqsum_sel[i] -> SetLineColor(kRed);
  }

  TH1D *hoccu  = new TH1D("hoccu", "", npanel, 0, npanel);
  hoccu -> SetXTitle("ID");
  hoccu -> SetYTitle("Number of Entries");
  TH1D *hmulti = new TH1D("hmulti", "", npanel, 0, npanel);
  hmulti -> SetXTitle("Multiplicity");
  hmulti -> SetYTitle("Number of Entries");
  TH2D *hom = new TH2D("", "", npanel, 0, npanel, npanel/3, 0, npanel/3);
  hom -> SetXTitle("ID");
  hom -> SetYTitle("Multiplicity");
  
  double nmuon[npanel] = {0};
  double qsum[npanel] = {0};
  double q[4], qm, qmqs, time;
  int nch, id, multi;
  int tpanel[npanel] = {0};
  
  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);
    if(i != 0 && i%1000000 == 0) cout << i << endl;

    nch = pevt -> GetN();
    //cout << nch << endl;
    time = pevt -> GetTriggerTime();

    multi = 0;
    for(int j = 0; j < nch; j++){
      qm = 0;
      PSHit *hit = pevt -> Get(j);

      id = hit -> GetID();
      tpanel[id] = 0;
      
      for(int k = 0; k < 4; k++){
	q[k] = hit -> GetQ(k);
	  
	if(q[k] > qm){
	  qm = q[k];
	}
      }
      qsum[id] = hit -> GetQsum();
      if(qsum[id] < 0 ||
	 q[0] < 0 || q[1] < 0 ||
	 q[2] < 0 || q[3] < 0){
	continue;
      }
      qmqs = qm/qsum[id];

      hqsum[id] -> Fill(qsum[id]);
      if(/*qsum[id] > cut_qsum &&*/ qmqs < cut_qmqs){
	nmuon[id]++;
	multi++;
	tpanel[id] = 1;
	
	hqsum_sel[id] -> Fill(qsum[id]);
	hoccu -> Fill(id);
      }
    }
    if(multi != 0){
      hmulti -> Fill(multi);

      if(multi != 1){
	for(int j = 0; j < npanel; j++){
	  if(tpanel[j] != 0)
	    hom -> Fill(j, multi);
	}
      }
    }
  }

  TGaxis::SetMaxDigits(4);
  int npad[10] = {12, 12, 12, 12, 11,
                  12, 12, 12, 12, 11};
  int nn = 0;
  TCanvas *can[10];
  TPad *pad[npanel];
  for(int i = 0; i < 10; i++){
    can[i] = new TCanvas(Form("can_%d", i), Form("%d", i+1), 1600, 900);
    can[i] -> Divide(4, 3);

    if(i == 5) nn += 6;
    for(int j = 0; j < npad[i]; j++){
      can[i] -> cd(j+1);
      pad[nn] = (TPad*)can[i] -> cd(j+1);
      
      hqsum[nn] -> Draw();

      pad[nn] -> SetLogy();

      nn++;
    }
  }

  TCanvas *can_om = new TCanvas("can_om", "", 800, 600);
  hom -> Draw();
  
  TFile of("./hist/muonHist_PSMD_all.root", "recreate");
  hoccu -> Write();
  hmulti -> Write();
  hom -> Write();
  for(int i = 0; i < npanel; i++){
    hqsum[i] -> Write();
    hqsum_sel[i] -> Write();
  }
  of.Close();
}
