R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)


void wave_wc(int run = 0)
{
  gStyle -> SetOptStat(0);
  
  TChain * chain = new TChain("AbsEvent");
  //chain->Add("/data/amore2test/RAW/000303/FADC_000303.root.00000");
  chain->Add("/data/amore2test/RAW/000339/FADC_000339.root.00000");

  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("FChannelData", &data);

  int nch, id;
  double rms, ped;
  double qsum[48] = {0};
  int tbit[48] = {0};
  int nbit;
  char a;

  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  TCanvas * can = new TCanvas("can", "", 1600, 800);
  can -> Divide(7, 7);
  for (int i = 0; i < nevt; i++) {
    chain->GetEntry(i);
    cout << i << endl;
    if(i != 0 && i%1000 == 0) cout << i << endl;
    nbit = 0;

    nch = data -> GetN();
    for(int j = 0; j < nch; j++){
      rms = 0;
      ped = 0;

      FChannel * ch = data->Get(j);
      id = ch -> GetID()-1;
      can -> cd(id+1);
      tbit[id] = ch -> GetBit();
      qsum[id] = 0;
      
      nbit += (int)tbit[id];
      //if(tbit[id] != 1) continue;
      
      ch = data->Get(j);
      ch -> GetWaveformHist() -> Draw();

      // if(j == 0){
      // 	TCanvas *can_dum = new TCanvas("can_dum", "", 1200, 600);
      // 	ch -> GetWaveformHist() -> Draw();
      // 	ch -> GetWaveformHist() -> SetXTitle("Time [bin]");
      // 	ch -> GetWaveformHist() -> SetYTitle("Height [ADC]");
      // }
      
      // int nbinsx = ch -> GetWaveformHist() -> GetNbinsX();
      // for(int k = 0; k < 250; k++){
      // 	int a = ch -> GetWaveformHist() -> GetBinContent(k+1);
      // 	ped += a;
      // 	rms += a*a;
      // }
      // ped /= 250;
      // rms /= 250.;
      // rms = TMath::Sqrt(rms);
      // rms -= ped;

      // for(int k = 280; k < 430; k++){
      // 	qsum[id] += ch -> GetWaveformHist() -> GetBinContent(k+1);
      // }
    }

    // if(qsum[25] > 20000){
    //   cout << qsum[25] << endl;
    //   can->Modified();
    //   can->Update();
    // }
    // else continue;

    can->Modified();
    can->Update();
    
    cin >> a;
    if(a == 'q') break;
  }
}
