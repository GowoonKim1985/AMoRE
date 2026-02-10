R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)

void prodt46_1500()
{
  TStopwatch sw;
  sw.Start();

  gStyle->SetOptFit(1);  

  const int head = 6;
  const int tail = 8;

  /*
  TString rawname = "KA_29_32_1500V.root.00000";
  //TString rawfile = "/data/HPGe/RAW/Muon/pmtdark_2022Nov/" + rawname + ".root.*";
  TString rawfile = "rootfile" + rawname + ".root.*";
//  TString rawfile = "../pmtcalib/rawdata/" + rawname + ".root.*";
  TString anafile = "../treefile/tree_" + rawname + ".root";
  */
 
  TChain * chain = new TChain("AbsEvent");
  chain->Add("data/KA_29_32_1500.root.00000");
  //chain->Add(rawfile.Data());

  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("FChannelData", &data);

  double qmax[4];

 // TFile f("out.root", "recreate");
  TTree * tree = new TTree("prod", "");
  tree->Branch("qmax", qmax, "qmax[4]/D");

  int nent = chain->GetEntries();
  for(int i = 0; i < nent; i++){
    chain->GetEntry(i);

    if (i > 0 && i % 1000 == 0)
      cout << Form("%6d events processed ......", i) << endl;

    for (int j = 0; j < 4; j++ ) {
      FChannel * ch = data->Get(j);
      TH1D * his = ch->GetWaveformHist();

      int maxx = his->GetMaximumBin();
      int bini = maxx-head;
      int binf = maxx+tail;

      if (bini < 1 || binf > his->GetNbinsX()) continue;

      qmax[j] = 0;
      for (int k = bini; k < binf; k++) {
        qmax[j] += his->GetBinContent(k);
      }
    }

    tree->Fill();
  }

  TFile out("rootfile/KA_29_32_1500V.root","recreate");
  tree->Write();
  out.Close();

  sw.Stop();
 
  cout << sw.RealTime() << endl;
}
                         
