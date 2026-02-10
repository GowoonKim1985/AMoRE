R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)

void wave(int run, int dch, int spch = 0)
{
  int md = (dch)*4;
  
  TChain * chain = new TChain("AbsEvent");
  //chain->Add(Form("/x/amore2test/RAW/%06d/IADC_%06d.root.*", run, run));
  chain->Add(Form("/x/amore2test/RAW/%06d/IADCDAQ1_%06d.root.*", run, run));

  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("FChannelData", &data);

  int nch, id;
  double qsum;
  char a;
  TCanvas * can = new TCanvas("can", "", 1000, 500);
  can -> Divide(2, 2);
  for (int i = 0; i < chain->GetEntries(); i++) {
    chain->GetEntry(i);

    qsum = 0;
    
    nch = data -> GetN();
    //cout << "nch: " << nch << " " << i << endl;
    for(int j = 0; j < nch; j++){
      FChannel * ch = data->Get(j);
      id = ch -> GetID()-1;
      //id /= 4;
      //if(id != md) continue;

      if(id != md+spch) continue;

      for(int k = 0; k < 4; k++){
	can -> cd(k+1);
	// cout << dch << " " << spch << " " << j << " " << k << endl;
	// cout << md << " " << k << endl;
	ch = data -> Get(md+k);
	cout << md+k << " " << ch -> GetID() << endl;
	ch -> GetWaveformHist() -> Draw();
	qsum += ch -> GetWaveformHist() -> Integral();
      }
      
      unsigned int tbit = ch -> GetBit();
      //if(qsum < 7000) continue;
      cout << i << " " << md << " " << qsum << " " << tbit << endl;
      
      can->Modified();
      can->Update();

      cin >> a;
      ch -> GetWaveformHist() -> Clear();
    }
    if(a == 'q') break;
  }
}
