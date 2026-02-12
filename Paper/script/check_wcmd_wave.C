R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libRawObjs.so)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void check_wcmd_wave()
{
  int runnum = 330;
  int subnum = 0;
  TChain * chain = new TChain("AbsEvent");
  //chain->Add("/x/kkw/data/20230810_MTH10RL16TH5.root.00001");
  chain->Add(Form("/amore2/amore2test/RAW/%06d/FADC_%06d.root.%05d",runnum, runnum, subnum));

  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("FChannelData", &data);

  int nch, id;
  double rms;
  double qsum[48][10] = {0};
  int tbit[48] = {0};
  int npeaks[48] = {0};
  int maxx[48][10] = {0};
  int nbit, maxxd;
  TGraph *grp[48];
  char a;

  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  TCanvas * can = new TCanvas("can", "", 1200, 1000);
  can -> Divide(7, 7);
  for (int i = 0; i < nevt; i++) {
    chain->GetEntry(i);
    if(i != 0 && i%1000 == 0) cout << i << endl;
    nbit = 0;

    nch = data -> GetN();
    //cout << "nch: " << nch << " in event " << i << endl;
    for(int j = 0; j < nch; j++){
      rms = 0;

      FChannel * ch = data->Get(j);
      id = ch -> GetID(); // from 1~48
      can -> cd(id);

      
      ch = data->Get(j);
      ch -> GetWaveformHist() -> Draw();
    }
    //if(nbit < 46) continue;

    can->Modified();
    can->Update();

    cin >> a;
    if(a == 'q') break;
  }
}
