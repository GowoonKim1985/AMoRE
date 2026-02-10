R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void ttt(int run = 0)
{
  TChain * chain = new TChain("AbsEvent");
  //chain->Add("/x/kkw/data/20230810_MTH10RL16TH5.root.00001");
  chain->Add("/x/amore2test/RAW/000283/FADC_000283.root.00000");

  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("FChannelData", &data);

  int nch, id;

  TH1D *his[48];
  for(int i = 0; i < 48; i++)
    his[i] = new TH1D(Form("his_%d", i), "", 100, 0, 100);
  
  int nevt = 10000;//chain -> GetEntries();
  cout << nevt << endl;
  for (int i = 0; i < nevt; i++) {
    chain->GetEntry(i);
    if(i != 0 && i%1000 == 0) cout << i << endl;

    nch = data -> GetN();
    //cout << "nch: " << nch << " in event " << i << endl;
    for(int j = 0; j < nch; j++){
      FChannel * ch = data->Get(j);
      id = ch -> GetID()-1;
      
      ch = data->Get(j);
      his[id] -> Fill(ch -> GetWaveformHist() -> GetMaximum());
      int nbinsx = ch -> GetWaveformHist() -> GetNbinsX();
    }
  }

  TCanvas * can = new TCanvas("can", "", 1600, 800);
  can -> Divide(7, 7);
  for(int i = 0; i < 48; i++){
    can -> cd(i+1);
    his[i] -> Draw();
    gPad->SetLogy();
  }
}
