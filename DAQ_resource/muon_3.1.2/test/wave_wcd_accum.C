R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void wave_wcd_accum(int run = 0)
{
  TChain * chain = new TChain("AbsEvent");
  //chain->Add("/data/amore2test/RAW/000288/FADC_000288.root.*");
  chain->Add("/data/amore2test/RAW/000303/FADC_000303.root.*");

  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("FChannelData", &data);

  int nch, id;
  int nn[48] = {0};
  int nbinsx;
  double ped, rms, qtot;
  unsigned short *adc = new unsigned short[nbinsx];
  TH1D *his[48];
  for(int i = 0; i < 48; i++)
    his[i] = new TH1D(Form("his_%d", i), "", 1008, 0, 1008);
  int nevt = 1000000;//chain -> GetEntries();
  cout << nevt << endl;
  for(int i = 0; i < nevt; i++){
    chain->GetEntry(i);
    if(i != 0 && i%100000 == 0) cout << i << endl;

    nch = data -> GetN();
    for(int j = 0; j < nch; j++){
      FChannel * ch = data->Get(j);
      id = ch -> GetID()-1;
      if(id != 25) continue;
      
      ped = 0;
      rms = 0;
      qtot = 0;
      
      nbinsx = ch -> GetNdp();
      adc = (unsigned short*)ch -> GetWaveform();
      for(int k = 0; k < 250; k++){
	ped += adc[k];
	rms += adc[k]*adc[k];
      }
      ped /= 250;
      rms /= 250;
      rms = TMath::Sqrt(rms);
      rms -= ped;

      for(int k = 280; k < 430; k++)
	qtot += adc[k]-ped;
      if(ch -> GetBit() != 1 || qtot < 20000) continue;
      
      his[id] -> Add(ch -> GetWaveformHist());
      nn[id]++;
    }
  }

  TCanvas *can = new TCanvas("can", "", 1600, 800);
  his[25] -> Draw();
  
  // TCanvas * can = new TCanvas("can", "", 1600, 800);  
  // can -> Divide(7, 7);
  // for(int i = 0; i < 47; i++){
  //   can -> cd(i+1);
  //   his[i] -> Scale(1./nn[i]);
  //   his[i] -> Draw("hist");
  // }
  // TFile of("./hist/AccWaves_000288.root", "recreate");
  // for(int i = 0; i < 48; i++)
  //   his[i] -> Write();
  // of.Close();
}
