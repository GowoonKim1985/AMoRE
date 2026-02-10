double gauses(double *v, double *p)
{
  double x = v[0];

  double a = p[0];
  double b = p[1];
  double c = p[2];
  double d = p[3];
  double e = p[4];
  double f = p[5];
  double g = p[6];
  double h = p[7];
  double i = p[8];
  double j = p[9];
  double k = p[10];
  
  return a*TMath::Gaus(x, b, c)
    + d*TMath::Gaus(x, e, f)
    + g*TMath::Gaus(x, h, i);
}

void pmax_psd(int idd = 0)
{
  gStyle -> SetOptStat(0);

  TChain * chain = new TChain("AbsEvent");
  chain->Add("/x/amore2test/RAW/000283/IADC1_000283.root.00000");

  EventInfo * info = new EventInfo();
  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("EventInfo", &info);
  chain->SetBranchAddress("FChannelData", &data);

  TH1D *his = new TH1D("his", "", 500, 0, 1000);
  his -> SetXTitle("Peak sum max");
  his -> SetYTitle("Number of Entries");
  int nbinsx;
  unsigned short * adc = new unsigned short[nbinsx];
  double q;
  int nevt = chain -> GetEntries();
  for(int i = 0; i < nevt; i++){
    chain -> GetEntry(i);

    int nch = data -> GetN();
    for(int j = 0; j < nch; j++){
      FChannel *ch = data -> Get(j);

      int id = ch -> GetID()-1;
      if(id != idd) continue;
      
      nbinsx = ch -> GetNdp();
      adc = (unsigned short*)ch -> GetWaveform();
	
      int ped = ch -> GetPedestal();
      int psm = 0;
      for(int k = 0; k < nbinsx-16; k++){
	q = 0;
	for(int l = 0; l < 16; l++)
	  q += (int)adc[k+l] - ped;
	if(q > psm){
	  psm = q;
	}
      }
      if(psm != 0)
	his -> Fill(psm);
    }
  }

  TF1 *func = new TF1("func", gauses, 80, 430, 9);
  func -> SetParameter(0, his -> GetMaximum());
  func -> SetParameter(1, his -> GetBinCenter(his -> GetMaximumBin()));
  func -> SetParameter(2, 10);
  func -> SetParameter(3, 480);
  func -> SetParameter(4, 226);
  func -> SetParameter(5, 22);
  func -> SetParameter(6, 75);
  func -> SetParameter(7, 377);
  func -> SetParameter(8, 30);
  his -> Fit(func, "R0");
  TCanvas *can = new TCanvas("can", "", 800, 600);
  his -> Draw();
  //func -> Draw("same");
  can -> SetLogy();
}
