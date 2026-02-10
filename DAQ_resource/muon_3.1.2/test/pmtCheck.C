R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void pmtCheck()
{
  TChain * chain = new TChain("AbsEvent");
  chain->Add("/x/amore2test/RAW/000272/FADC_000272.root*");

  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("FChannelData", &data);

  int nch, id, nbinsx, max, adcp;
  unsigned short *adc = new unsigned short[nbinsx];
  unsigned short ped;
  int tbit[48] = {0};
  char a;

  int nevt = chain -> GetEntries();
  cout << nevt << endl;
  TCanvas * can = new TCanvas("can", "", 1600, 800);
  can -> Divide(7, 7);
  for (int i = 0; i < nevt; i++) {
    chain->GetEntry(i);
    if(i != 0 && i%1000 == 0) cout << i << endl;

    nch = data -> GetN();
    int stp = 0;
    for(int j = 0; j < nch; j++){
      can -> cd(j+1);
      FChannel * ch = data->Get(j);
      id = ch -> GetID()-1;
      tbit[id] = ch -> GetBit();
      
      if(tbit[id] != 1) continue;
      //if(id != 18 && id != 37) continue;
      
      adc = (unsigned short*)ch -> GetWaveform();
      ped = ch -> GetPedestal();
      max = 0;
      nbinsx = ch -> GetNdp();
      for(int k = 0; k < nbinsx; k++){
	adcp = adc[k] - ped;
	if(adcp > max){
	  max = adcp;
	}
      }
      if((id == 18 && max > 1000) ||
	 (id == 37 && max > 1000))
	stp = 1;
      ch -> GetWaveformHist() -> Draw();	  
      ch -> GetWaveformHist() -> GetYaxis() -> SetLabelSize(0.08);
      ch -> GetWaveformHist() -> GetXaxis() -> SetLabelSize(0.08);
    }
    if(stp != 1) continue;
    
    can->Modified();
    can->Update();
    
    cin >> a;
    if(a == 'q') return 0;    
  }
}
