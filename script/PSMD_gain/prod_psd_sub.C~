R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/test/libPSHit.so)

int tbin;
double q, qa[0], pt, ttime;

void Init()
{
  tbin = 0;
  q = 0;
  pt = 0;
  for(int i = 0; i < 10; i++)
    qa[i] = 0;
}

void fQmaxIntegration(unsigned short *adc, int nbin, int max)
{
  int aaa, sbin; 
  for(int k = tbin-10; k < nbin; k++){
    if(adc[k] > max*0.1){
      sbin = k;
      break;
    }
  }
  for(int k = 0; k < 10; k++){
    if(k == 0)
      qa[k] = adc[tbin];
    else{
      for(int l = sbin; l < tbin+k*2; l++){
	qa[k] += adc[l];
      }
    }
  }
}

void fPulseTime(unsigned short *adc, int nbin, int max)
{
  TGraph *grp = new TGraph();
  TF1 *fitfunc = new TF1("fitfunc", "pol1", 20, 35);

  grp -> Set(0);
  double rms = 0;
  for(int k = 0; k < 25; k++)
    rms += TMath::Power(adc[k], 2);
  
  rms /= 25;
  rms = TMath::Sqrt(rms);
  //cout << "RMS: " << rms << endl;
  int np = 0;
  int maxx = tbin;
  int binc;
  double val, p0, p1;
  while(1){
    maxx--;
    
    binc = adc[maxx];
    if(binc < rms || maxx < 10){
      //cout << binc << " " << rms << " " << maxx << endl;
      break;
    }
    val = TMath::Sqrt(-TMath::Log((double)binc/max));
    grp -> SetPoint(np, maxx, val);
    // cout << np << " " << maxx << " " << val
    // 	 << " " << binc << " " << max << endl;
    np++;
  }
  
  if(grp -> GetN() > 3){
    grp -> Fit(fitfunc, "0QG");
    
    p0 = fitfunc -> GetParameter(0);
    p1 = fitfunc -> GetParameter(1);
    
    pt = (TMath::Sqrt(-TMath::Log(0.5))-p0)/p1;
    //cout << pt << " " << p0 << " " << p1 << endl;
  }
  else
    pt = -1;

  delete grp;
  delete fitfunc;
}

//void prod_psd(int subrun = 0, int daqtype = 1)
void SET01()
{	
  int run = SET02;
  int subrun = SET03;
  int daqtype = SET04;
  TStopwatch t;
  t.Start();
 
  // const int nthreads = 20;
  // ROOT::EnableImplicitMT(nthreads);

  int nent;
  
  TChain * chain = new TChain("AbsEvent");
  chain->Add(Form("/data/amore2test/RAW/%06d/IADC%d_%06d.root.%05d", run, daqtype, run, subrun));
  if(chain == NULL){
    cout << "!" << endl;
    return 0;
  }
 
  EventInfo * info = new EventInfo();
  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("EventInfo", &info);
  chain->SetBranchAddress("FChannelData", &data);

  PSEvent * pevent = new PSEvent();
//  TFile * out = new TFile(Form("/home/kkw/muon_3.1.2/test/data/PSMD/%06d/prd_psd_daq%d_%06d_%05d.root", run, daqtype, run, subrun), "recreate");
  TFile * out = new TFile(Form("/home/kkw/PROD/PSMD/%06d/prd_psd_daq%d_%06d_%05d.root", run, daqtype, run, subrun), "recreate");
  TTree * otree = new TTree("prd_psd", "prd_psd");
  otree->Branch("PSEvent", &pevent);

  int tevt = 0;
  int tch = 0;
  int id, did, max;
  int nbinsx;
  unsigned short *adc = new unsigned short[nbinsx];
  nent = nent > 0 ? nent : chain->GetEntries();
  cout << nent << endl;
  for(int i = 0; i < nent; i++){
//  for(int i = 0; i < 10; i++){
    if(i > 0 && i % 50000 == 0)
      cout << Form("%6d event processed ...", i) << endl;

    Init();
    chain->GetEntry(i);

    if(info->GetTriggerType() == 1) continue;

    pevent->Clear();
    pevent->SetEventNumber(info->GetTriggerNumber());
    pevent->SetTriggerTime(info->GetTriggerTime());

    int nch = data->GetN();
    //cout << nch << endl;
    tch = 0;
    for (int j = 0; j < nch; j++) {
      FChannel * ch = data->Get(j);
      id = ch->GetID()-1;

      if(ch -> GetBit() != 1) continue;
      if(tch == 0){
	tevt++;
	tch++;
      }
      //cout << i << " " << id << " " << id/4 << " " << ch -> GetBit() << endl;
      
      nbinsx = ch -> GetNdp();
      adc = (unsigned short*)ch -> GetWaveform();
      
      q = 0;
      max = 0;
      int ped = ch -> GetPedestal();
      for(int k = 0; k < nbinsx; k++){
	//adc[k] -= ped;
	q += adc[k] - ped;
	
	if(adc[k] > max){
	  max = adc[k] - ped;
	  tbin = k;
	}
      }
      //cout << q << " " << ch -> GetWaveformHist() -> Integral() << endl;
      fQmaxIntegration(adc, nbinsx, max);
      fPulseTime(adc, nbinsx, max);
      
      did = id / 4;
      
      PSHit * hit = pevent->GetByID(did);
      if (!hit) hit = pevent->Add(did);

      //hit -> SetWaveform(adc);
      hit->SetVariables(q, tbin, &qa[0], pt);
    }
    otree->Fill();
  }

  cout << tevt << endl;

  otree->Write();
  out->Close();

  t.Stop();
  t.Print();
}
