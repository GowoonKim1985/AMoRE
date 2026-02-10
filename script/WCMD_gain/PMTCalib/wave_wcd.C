R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)

void wave_wcd()
{
  gStyle -> SetOptStat(0);

  TChain * t = new TChain("AbsEvent");
  t->Add("/data/amore2test/RAW/000250/FADC_000250.root.00000");  

  FChannelData * data = new FChannelData();
  t->SetBranchAddress("FChannelData", &data);
  unsigned int tbit[48];
  
  TSpectrum *s;
  TCanvas * can = new TCanvas("can", "", 1500, 1000);
  can->Divide(7,7);
  
  int nevt = t->GetEntries();
  for (int i = 0; i < nevt; i++) {
    if (i % 10000 == 0 && i > 0)
      cout << Form("%6d events processed ...", i) << endl;

    t->GetEntry(i);

    int nch = data->GetN();

    for (int j = 0; j < nch; j++) {
      FChannel * ch = data->Get(j);
      // int id = ch->GetID()-1;
      // int did = id/4;

      can -> cd(j+1);
      ch -> GetWaveformHist() -> Draw();
      tbit[j] = ch -> GetBit();
      //s -> Search(ch -> GetWaveformHist());
      cout << tbit[j] << " ";
      if((j+1)%7 == 0 && j != 0)
	cout << endl;

      // if(j == 0){
      // 	TCanvas *can_dum = new TCanvas("can_du,", "", 800, 600);
      // 	ch -> GetWaveformHist() -> Draw();
      // }
    }
    cout << endl;
    cout << "--" << endl;
    
    can->Modified();
    can->Update();

    char a;
    cin >> a;

    if (a == 'q') break;
  }


}
