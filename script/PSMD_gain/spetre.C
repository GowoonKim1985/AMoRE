R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(../lib/libRawObjs)

void spetre()
{
  
  // int run = 367;//6Sep2024
  //int run = 393;//26Sep2024 ok
  //int run = 402;//6Nov2024 ok
  //int run = 427;//9Dec2024
  int run = 330;//-651Jan2024 ok

  TChain *pchain[2];
  int pnevt[2], idp;
  int pevt[2], nchp[2];
  EventInfo * info[2];
  FChannelData * data[2];
  int id;
  char a;
  int evt, nevt;
  int nch = 500;
  int nbinsx;
  double ped, hmax, hmaxx;
  double q;
  int head = 5;
  int tail = 10;

  TH1D *qhis[500];

  TFile * out = new TFile(Form("./psdspe_%06d.root", run), "recreate");
  TTree * tree = new TTree("prod", "");
 
  tree->Branch("evt", &evt, "evt/I");
  tree->Branch("id", &id, "id/I");
  tree->Branch("q", &q, "q/D");

  for(int i = 0; i < 500; i++) {
    qhis[i] = new TH1D(Form("qhis_%d", i+1), "", 1100, -100, 1000);
  }
  
  for(int i = 0; i < 2; i++){
    pchain[i] = new TChain("AbsEvent"); 
    pchain[i] -> Add(Form("/data/amore2test/RAW/%06d/IADC%d_%06d.root.*", run, i+1, run));

    info[i] = new EventInfo();
    data[i] = new FChannelData();
    
    pchain[i]->SetBranchAddress("EventInfo", &info[i]);
    pchain[i] -> SetBranchAddress("FChannelData", &data[i]);

    pnevt[i] = pchain[i] -> GetEntries();
    
    cout << "number of entries: " << pnevt[i] << endl;
  }
  evt = 0;
  for(int j = 0; j < 2; j++){   
//    for(int k = 0; k < pnevt[j]; k++){ // event start
  for(int k = 0; k < 1000; k++){ // event start 
      pchain[j] -> GetEntry(k);
      if(k != 0 && k%10000 == 0) cout << k << endl;
      evt++;
      nchp[j] = data[j]->GetN();
      //if(j==1 && k>=0) cout << j << ": " << k << ": " << nchp[j] << endl;

      for(int d = 0; d < nchp[j]; d++) {
	FChannel * ch = data[j]->Get(d);
	id = ch -> GetID()-1;
	//id = ch -> GetID();
	//idp = id/4;
	nbinsx = ch -> GetNdp();
	if(j== 0 && k==0 && d==0) cout << "Number of data points: " << nbinsx << endl;
	TH1D *hist = ch->GetWaveformHist();
	hmax = hist->GetMaximum();
	hmaxx = hist->GetMaximumBin();
	q = 0;
	for(int w = hmaxx-5; w < hmaxx+10; w++) {
	  double Q = hist->GetBinContent(w);
	  q += Q;
	}
	
	tree->Fill();
      }      
    }
  }

  tree->Write();
  out->Close();
 
  cout << "total number of events: " << evt << endl;
 
}




