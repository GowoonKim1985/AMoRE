
double twogaus(double *v, double *p)
{
  double x = v[0];

  double a = p[0];
  double m1 = p[1];
  double s1 = p[2];
  double b = p[3];
  double m2 = p[4];
  double s2 = p[5];

  double y1 = TMath::Gaus(x, m1, s1, true);
  double y2 = TMath::Gaus(x, m2, s2, true);

  return a*y1 + b*y2;
}


void f32_1500(const char *sourcename = "KA_29_32_1500V", const char *name2 = "KA_29_32_1500V")
{
 
  gStyle->SetOptFit(1);

  //TFile f(Form("./rootfile/tree_%s.root", sourcename));
  TFile f(Form("./rootfile/%s.root", sourcename));
  TTree * tree = (TTree*)f.Get("prod");

  double qmax[4];
  tree->SetBranchAddress("qmax", qmax);
  int range = 200;
  int bn = 200;

  TH1D * qmaxhis[4];
  for (int i = 0; i < 4; i++) {
    qmaxhis[i]  = new TH1D("qmaxhis", "", bn, 0, range);
    qmaxhis[i]->SetDirectory(0);
  }

  int nent = tree->GetEntries();
  for(int i = 0; i < nent; i++){
    tree->GetEntry(i);

       qmaxhis[0]->Fill(qmax[0]);

  }
 // double pm = 1.1;
  
  TF1 * ff = new TF1("ff", twogaus, 0, range, 6);
  ff->SetParameter(0, 2600);
  ff->SetParameter(1, -3.38);
  ff->SetParameter(2, 9);
  ff->SetParameter(3, 34);
  ff->SetParameter(4, 31);
  ff->SetParameter(5, 8.8);

  TCanvas * can = new TCanvas("can", "", 700, 600);
  gPad->SetLogy();
  qmaxhis[0]->Fit(ff, "R");
  //qmaxhis[0]->Draw();
  
  qmaxhis[0]->SetTitle("KA-32_1500V");
  qmaxhis[0]->GetXaxis()->SetTitle("Spe charge (ADC counts)");
  qmaxhis[0]->GetXaxis()->CenterTitle();
  qmaxhis[0]->GetYaxis()->SetTitle("Entries");
  qmaxhis[0]->GetYaxis()->CenterTitle("");
  qmaxhis[0]->GetYaxis()->SetTitleOffset(1.3);

  double ADC = ff->GetParameter(4);
  double Err = ff->GetParError(4);
  double pC;

  cout << ":::::::::::: Results ::::::::::::::::::::" << endl;
  cout << "" << endl;

  cout << " ADC counts = " << ADC << " Error " << Err << endl;

  pC = ADC*2.5*2*pow(10, 3)/(4096*50);

  cout << " spe charge = " << pC << "pC" << endl;
  cout << "" << endl;
  cout << ":::::::::::::End:::::::::::::::::" << endl;
   
  TFile out(Form("rootfile/his_%s.root", name2),"recreate");    
    qmaxhis[0]->Write();
    can->Write();
    out.Close();
  
}

