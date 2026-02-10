R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void check_tbit_psd(int subrun = 0, int daqtype = 1)
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
  TH1D *hwave[4];
  TCanvas *can = new TCanvas("can", "", 1200, 600);
  can -> Divide(2, 2);
  nent = nent > 0 ? nent : chain->GetEntries();
  cout << nent << endl;
  for(int i = 0; i < nent; i++){
    if(i > 0 && i % 50000 == 0)
      cout << Form("%6d event processed ...", i) << endl;
    
    chain->GetEntry(i);
    
    //if(info->GetTriggerType() == 1) continue;
    
    int nch = data->GetN();
    nch /= 4;
    for (int j = 0; j < nch; j++) {
      int tbit;
      int tbit_check[4];
      int q[4] = {0};
      for(int m = 0; m < 4; m++){
	FChannel * ch = data->Get(4*j+m);
	id = ch->GetID()-1;
	tbit = ch -> GetBit();
	tbit_check[m] = 0;
	nbinsx = ch -> GetNdp();

	adc = (unsigned short*)ch -> GetWaveform();
	
	int ped = ch -> GetPedestal();
	for(int k = 0; k < nbinsx-16; k++){
	  q[m] = 0;
	  for(int l = 0; l < 16; l++){
	    q[m] += (int)adc[k+l] - ped;
	  }
	  if(q[m] > thr[id]){
	    tbit_check[m] = 1;
	    break;
	  }
	}
	hwave[m] = (TH1D*)ch -> GetWaveformHist();
      }
      
      if(tbit_check[0] == 1 && tbit_check[1] == 1 &&
	 tbit_check[2] == 1 && tbit_check[3] == 1 && j > 40 && j < 50){
	if(tbit_check[0] - tbit != 0){
	  cout << i << endl;
	  cout << Form("Tbit by DAQ: %d, By self: %d", tbit, tbit_check[0]) << endl;
	  cout << Form("Det: %3d (1st ch: %3d), %3d(%3d), %3d(%3d), %3d(%3d), %3d(%3d)",
		       j+1, j*4+1, q[0], thr[4*j+0], q[1], thr[4*j+1],
		       q[2], thr[4*j+2], q[3], thr[4*j+3]) << endl;
	  cout << endl;

	  can -> cd(1);
	  hwave[0] -> Draw();
	  can -> cd(2);
	  hwave[1] -> Draw();
	  can -> cd(3);
	  hwave[2] -> Draw();
	  can -> cd(4);
	  hwave[3] -> Draw();

	  can -> Modified();
	  can -> Update();
	  
	  cin >> a;
	  if(a == 'q') return 0;
	}
      }
      else{
	hwave[0] -> Clear();
	hwave[1] -> Clear();
	hwave[2] -> Clear();
	hwave[3] -> Clear();
      }
    }
  }

  t.Stop();
  t.Print();
}
