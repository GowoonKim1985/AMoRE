R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void check_tbit_psd2(int subrun = 0, int daqtype = 1, int sss = 539)
{
  gStyle -> SetOptStat(0);
  
  int run = 252;

  int thr[560] =
    {600, 600, 600, 600, 650, 650, 650, 650,
     600, 600, 600, 600, 650, 650, 650, 650,
     600, 600, 600, 600, 600, 600, 600, 600,
     600, 600, 600, 600, 650, 650, 650, 650,
     650, 650, 650, 650, 650, 650, 650, 650,

     650, 650, 650, 650, 700, 700, 700, 700,
     600, 600, 600, 600, 600, 600, 600, 600,
     600, 600, 600, 600, 600, 600, 600, 600,
     550, 550, 550, 550, 600, 600, 600, 600,
     600, 600, 600, 600, 550, 550, 550, 550,

     600, 600, 600, 600, 650, 650, 650, 650,
     600, 600, 600, 600, 550, 550, 550, 550,
     700, 700, 700, 700, 600, 600, 600, 600,
     600, 600, 600, 600, 550, 550, 550, 550,
     600, 600, 600, 600, 600, 600, 600, 600,

     550, 550, 550, 550, 600, 600, 600, 600,
     600, 600, 600, 600, 600, 600, 600, 600,
     600, 600, 600, 600, 600, 600, 600, 600,
     550, 550, 550, 550, 65535, 65535, 65535, 65535,
     600, 600, 600, 600, 600, 600, 600, 600,

     550, 550, 550, 550, 600, 600, 600, 600,
     600, 600, 600, 600, 600, 600, 600, 600,
     600, 600, 600, 600, 600, 600, 600, 600,
     600, 600, 600, 600, 650, 650, 650, 650,
     650, 650, 650, 650, 600, 600, 600, 600,

     650, 650, 650, 650, 300, 300, 300, 300,
     650, 650, 650, 650, 600, 600, 600, 600,
     700, 700, 700, 700, 650, 650, 650, 650,
     700, 700, 700, 700, 600, 600, 600, 600,
     700, 700, 700, 700, 65535, 65535, 65535, 65535,

     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,
     0,0,0,0,0,0,0,0,

     600, 600, 600, 600, 600, 600, 600, 600,
     650, 650, 650, 650, 650, 650, 650, 650,
     700, 700, 700, 700, 650, 650, 650, 650,
     65535, 65535, 65535, 65535, 650, 700, 750, 600,
     65535, 65535, 65535, 65535, 700, 700, 700, 700,

     650, 650, 650, 650, 650, 650, 650, 650,
     650, 650, 650, 650, 600, 600, 600, 600,
     650, 650, 500, 600, 550, 550, 550, 550,
     600, 600, 600, 600, 600, 600, 600, 600,
     600, 600, 600, 600, 650, 650, 650, 650,

     600, 600, 600, 600, 600, 600, 600, 600,
     550, 550, 550, 550, 650, 500, 500, 650,
     700, 700, 700, 700, 600, 600, 600, 600,
     650, 650, 650, 650, 700, 700, 700, 700,
     600, 600, 600, 600, 650, 650, 650, 650,
     
     600, 600, 600, 600, 500, 500, 500, 500,
     650, 650, 650, 650, 600, 600, 600, 600,
     550, 550, 550, 550, 600, 600, 600, 600,
     550, 550, 550, 550, 650, 650, 650, 650,
     550, 550, 550, 550, 550, 550, 550, 550,

     600, 600, 600, 600, 650, 650, 650, 650,
     600, 600, 600, 600, 600, 600, 600, 600,
     600, 600, 600, 600, 600, 600, 600, 600,
     550, 550, 550, 550, 600, 600, 600, 600,
     600, 600, 600, 600, 600, 600, 600, 600,

     600, 600, 600, 600, 600, 600, 600, 600,
     600, 600, 600, 600, 650, 650, 650, 650,
     650, 650, 650, 650, 600, 600, 600, 600,
     550, 550, 550, 550, 600, 600, 600, 600,
     650, 650, 650, 650, 65535, 65535, 65535, 65535};
  
  TStopwatch t;
  t.Start();
 
  const int nthreads = 20;
  ROOT::EnableImplicitMT(nthreads);

  int nent;
  
  TChain * chain = new TChain("AbsEvent");
  chain->Add(Form("/x/amore2test/RAW/%06d/IADC%d_%06d.root.%05d", run, daqtype, run, subrun));
  if(chain == NULL){
    cout << "!" << endl;
    return 0;
  }
 
  EventInfo * info = new EventInfo();
  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("EventInfo", &info);
  chain->SetBranchAddress("FChannelData", &data);

  char a;
  int id;
  int nbinsx = 62;
  unsigned short * adc = new unsigned short[nbinsx];
  TH1D *hwave[10][4];
  TPaveText *pave[10][4];
  TCanvas *can[2];
  for(int i = 0; i < 2; i++){
    can[i] = new TCanvas(Form("can_%d", i), Form("%d", i+1), 1500, 1000);
    can[i] -> Divide(4, 5);
  }
  nent = nent > 0 ? nent : chain->GetEntries();
  cout << nent << endl;
  //539 40412 83621 137505 161001 164103 169135 169530 205593
  for(int i = sss; i < nent; i++){
    if(i > 0 && i % 50000 == 0)
      cout << Form("%6d event processed ...", i) << endl;
    
    chain->GetEntry(i);
    
    //if(info->GetTriggerType() == 1) continue;
    
    int nch = data->GetN();
    nch /= 4;
    int tbit[10][4];
    int tbit_check[10][4];
    for (int j = 40; j < 50; j++) {
      int q[10][4] = {0};
      for(int m = 0; m < 4; m++){
	FChannel * ch = data->Get(4*j+m);
	id = ch->GetID()-1;
	tbit[j-40][m] = ch -> GetBit();
	//if(tbit[j][m] == 1) cout << i << "!" << endl;
	tbit_check[j-40][m] = 0;
	nbinsx = ch -> GetNdp();

	adc = (unsigned short*)ch -> GetWaveform();
	
	int ped = ch -> GetPedestal();
	for(int k = 0; k < nbinsx-16; k++){
	  q[j-40][m] = 0;
	  for(int l = 0; l < 16; l++){
	    q[j-40][m] += (int)adc[k+l] - ped;
	  }
	  if(q[j-40][m] > thr[id]){
	    tbit_check[j-40][m] = 1;
	    break;
	  }
	}
	hwave[j-40][m] = (TH1D*)ch -> GetWaveformHist();
      }

      if(j-40 < 5){
	for(int m = 0; m < 4; m++){
	  if(tbit[j-40][m] - tbit_check[j-40][m] != 0)
	    cout << i << " " << j << " " << m << endl;
	  can[0] -> cd(4*(j-40)+m+1);
	  pave[j-40][m] = new TPaveText(0.9, 0.9, 0.6, 0.7, "NDC");
	  pave[j-40][m] -> AddText(Form("Tbit: %d", tbit[j-40][m]));
	  pave[j-40][m] -> AddText(Form("Tbit by self: %d", tbit_check[j-40][m]));
	  
	  hwave[j-40][m] -> Draw();
	  pave[j-40][m] -> Draw("same");
	}
      }
      else{
	for(int m = 0; m < 4; m++){
	  if(tbit[j-40][m] - tbit_check[j-40][m] != 0)
	    cout << i << " " << j << " " << m<< endl;
	  can[1] -> cd(4*(j-40)+m-19);
	  pave[j-40][m] = new TPaveText(0.9, 0.9, 0.6, 0.7, "NDC");
	  pave[j-40][m] -> AddText(Form("Tbit: %d", tbit[j-40][m]));
	  pave[j-40][m] -> AddText(Form("Tbit by self: %d", tbit_check[j-40][m]));
	  
	  hwave[j-40][m] -> Draw();
	  pave[j-40][m] -> Draw("same");
	}
      }
    }

    can[0] -> Modified();
    can[0] -> Update();
    can[1] -> Modified();
    can[1] -> Update();
    
    cin >> a;
    if(a == 'q') return 0;
    else if(a == 'p'){
      can[0] -> Print(Form("./figure/20231024/AbnormalWaves_%d_1.png", i));
      can[1] -> Print(Form("./figure/20231024/AbnormalWaves_%d_2.png", i));
    }
  }

  t.Stop();
  t.Print();
}
