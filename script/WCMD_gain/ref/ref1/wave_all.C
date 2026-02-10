R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void wave_all(int run = 0)
{
  int run = 359;
  TChain * chain = new TChain("AbsEvent");
  //chain->Add("/x/kkw/data/20230810_MTH10RL16TH5.root.00001");
  chain->Add(Form("/data/amore2test/RAW/%06d/FADC_%06d.root.*",run,run));

  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("FChannelData", &data);
  EventInfo *info = new EventInfo();
  chain -> SetBranchAddress("EventInfo", &info);

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
  TCanvas * can = new TCanvas("can", "", 800, 600);
  //  can -> Divide(7, 7);
  for (int i = 1025; i < nevt; i++) {
    chain->GetEntry(i);
    if(i != 0 && i%1000 == 0) cout << i << endl;
    nbit = 0; 
    cout<<"test"<<endl;
    //   int evtnum = chain->GetLeaf("fEventNumber")->GetValue();
    int evtnum = info ->GetTriggerNumber();
    cout<<"test"<<endl;

   cout<<"evtnum "<<evtnum<<endl;
    nch = data -> GetN();
    //cout << "nch: " << nch << " in event " << i << endl;
    for(int j = 0; j < nch; j++){
      rms = 0;

      can -> cd(j+1);
      FChannel * ch = data->Get(j);
      id = (ch -> GetID())-1;
      if(id==42){
      ch = data->Get(j);
      ch -> GetWaveformHist() -> Draw();
      }
      
    }
    
    can->Modified();
    can->Update();

    cin >> a;
    if(a == 'q') break;
  }
}
