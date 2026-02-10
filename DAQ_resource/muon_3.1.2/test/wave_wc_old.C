R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void wave_wc(int run = 0)
{
  TChain * chain = new TChain("AbsEvent");
  chain->Add("/data/amore2test/RAW/000303/FADC_000303.root.00000");

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
  TCanvas * can = new TCanvas("can", "", 1600, 800);
  can -> Divide(7, 7);
  for (int i = 0; i < nevt; i++) {
    chain->GetEntry(i);
    if(i != 0 && i%1000 == 0) cout << i << endl;
    nbit = 0;

    nch = data -> GetN();
    //cout << "nch: " << nch << " in event " << i << endl;
    for(int j = 0; j < nch; j++){
      rms = 0;

      can -> cd(j+1);
      FChannel * ch = data->Get(j);
      id = ch -> GetID()-1;
      npeaks[id] = 0;
      tbit[id] = ch -> GetBit();

      nbit += (int)tbit[id];
      //if(tbit[id] != 1) continue;
      
      ch = data->Get(j);
      ch -> GetWaveformHist() -> Draw();
      
      int nbinsx = ch -> GetWaveformHist() -> GetNbinsX();
      maxxd = ch -> GetWaveformHist() -> GetMaximumBin();
      for(int k = 0; k < nbinsx/4; k++){
	if(k > maxxd*0.9 && k < maxxd*1.2){
	  //cout << k << endl;
	  continue;
	}
	int a = ch -> GetWaveformHist() -> GetBinContent(k+1);
	rms += a*a;
      }
      rms /= nbinsx/4.;

      for(int k = 0; k < nbinsx; k++){
	if(ch -> GetWaveformHist() -> GetBinContent(k+1) > rms*5){
	  qsum[id][npeaks[id]] = ch -> GetWaveformHist() -> GetBinContent(k+1);
	  maxx[id][npeaks[id]] = ch -> GetWaveformHist() -> GetBinCenter(k+1);

	  npeaks[id]++;
	  k += 50;
	}
      }
      if(npeaks[id] != 0){
	grp[id] = new TGraph();
	for(int k = 0; k < npeaks[id]; k++){
	  grp[id] -> SetPoint(k, maxx[id][k], qsum[id][k]);
	}
	grp[id] -> SetMarkerStyle(23);
	grp[id] -> SetMarkerColor(kRed);
	grp[id] -> SetMarkerSize(1);
	grp[id] -> Draw("P");
      }

      // cout << Form("|%2d, %3.0f @ %3.0f%%, %.3f, %2d, tbit %d",
      // 		   id, qsum[id], 100*(double)maxx[id]/nbinsx,
      // 		   rms, npeaks, tbit) << "| ";
      // if(j == 3 || (j != 0 && (j+1)%4 == 0)) cout << endl;
    }
    //if(nbit < 46) continue;

    can->Modified();
    can->Update();

    cin >> a;
    if(a == 'q') break;
  }
}
