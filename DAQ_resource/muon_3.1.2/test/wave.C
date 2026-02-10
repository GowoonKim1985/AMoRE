R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void wave(int run = 327)
{
  TChain * chain = new TChain("AbsEvent");
  chain->Add(Form("/data/amore2test/RAW/%06d/IADC1_%06d.root.00000", run, run));

  EventInfo * info = new EventInfo();
  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("EventInfo", &info);
  chain->SetBranchAddress("FChannelData", &data);

  int nch, id;
  char a;
  TCanvas * can = new TCanvas("can", "", 1000, 500);
  can -> Divide(2, 2);
  for (int i = 0; i < 100000; i++) {
    chain->GetEntry(i);
    cout << i << endl;
    
    nch = data -> GetN();
    //cout << "nch: " << nch << " " << i << endl;
    for(int j = 210; j < nch; j = j+4){
      cout << j << endl;
      FChannel * ch = data->Get(j);
      id = ch -> GetID()-1;
      
      can -> cd(1);
      ch -> GetWaveformHist() -> Draw();
      cout << ch -> GetBit() << " ";
      can -> cd(2);
      ch = data -> Get(j+1);
      ch -> GetWaveformHist() -> Draw();
      cout << ch -> GetBit() << " ";
      can -> cd(3);
      ch = data -> Get(j+2);
      ch -> GetWaveformHist() -> Draw();
      cout << ch -> GetBit() << " ";
      can -> cd(4);
      ch = data -> Get(j+2);
      ch -> GetWaveformHist() -> Draw();
      cout << ch -> GetBit() << " " << endl;;
      
      can->Modified();
      can->Update();
      ch -> GetWaveformHist() -> Clear();
      
      cin >> a;
      if(a == 'q') break;
    }
  }
}
