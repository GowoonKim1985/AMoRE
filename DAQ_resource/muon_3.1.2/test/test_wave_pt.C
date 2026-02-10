R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)

void test_wave_pt(int run = 109, int dch = 48, int spch = 0)
{
  int md = (dch)*4;
  
  TChain * chain = new TChain("AbsEvent");
  //chain->Add(Form("/x/amore2test/RAW/%06d/IADC_%06d.root.*", run, run));
  chain->Add(Form("/x/amore2test/RAW/%06d/IADC_%06d.root.*", run, run));

  FChannelData * data = new FChannelData();
  chain->SetBranchAddress("FChannelData", &data);

  int nch, id;
  double qsum, qmax, pt;
  char a;
  TGraph *grp = new TGraph();
  TF1 *fitfunc = new TF1("fitfunc", "pol1", 25, 32);
  TCanvas * can = new TCanvas("can", "", 800, 400);
  for (int i = 490924; i < 490925; i++) {
    chain->GetEntry(i);
    
    qsum = 0;
    
    nch = data -> GetN();
    //cout << "nch: " << nch << " " << i << endl;
    for(int j = 0; j < nch; j++){
      FChannel * ch = data->Get(j);
      id = ch -> GetID()-1;
      //id /= 4;
      
      if(id == md){
	//cout << "0 " << id << endl;
	ch -> GetWaveformHist() -> Draw();
	//cout << ch -> GetWaveformHist() -> Integral() << endl;
	
	qmax = ch -> GetWaveformHist() -> GetMaximum();
	qsum += qmax;
	grp -> Set(0);
	
	int maxx = ch -> GetWaveformHist() -> GetMaximumBin();
	double rms = 0;
	for(int k = 0; k < 25; k++)
	  rms += TMath::Power(ch -> GetWaveformHist() -> GetBinContent(k+1), 2);
	
	rms /= 24;
	rms = TMath::Sqrt(rms);
	//cout << "RMS: " << rms << endl;
	int np = 0;
	while(1){
	  maxx--;
	  
	  double binc = ch -> GetWaveformHist() -> GetBinContent(maxx);
	  if(binc < rms){
	    //cout << binc << " " << rms << endl;
	    break;
	  }
	  double val = TMath::Sqrt(-TMath::Log(binc/qmax));
	  grp -> SetPoint(np, maxx, val);
	  //cout << np << " " << maxx << " " << val << endl;
	  np++;
	}
	
	if(grp -> GetN() > 3){
	  grp -> Fit(fitfunc, "0QG");

	  double p0 = fitfunc -> GetParameter(0);
	  double p1 = fitfunc -> GetParameter(1);

	  pt = (TMath::Sqrt(-TMath::Log(0.5))-p0)/p1;
	}
	//fitfunc -> Draw("same");
	//grp -> Draw("PL, same");
	cout << pt << endl;
      }
      else continue;
      
      
      unsigned int tbit = ch -> GetBit();
      //if(qsum < 7000) continue;

      if(qsum < 2000) continue;
      cout << i << " " << md << " " << qsum << " " << tbit << endl;
      
      can->Modified();
      can->Update();

      // cin >> a;
      // ch -> GetWaveformHist() -> Clear();
    }
    //if(a == 'q') break;
    //break;
  }
}
