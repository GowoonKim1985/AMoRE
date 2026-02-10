R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(../lib/libRawObjs)



void gainmatched()
{

  //gStyle->SetOptFit(1);
  gStyle->SetOptStat(0);
  
  int run = 367;
  
  int id, evt, Sid;
  double q, q1;
  int Sch = 384; // number of side channels
  double gain, gain1[384], gain2[384], gainM[384], cons, stdv;

  TChain * chain = new TChain("prod");
  chain->Add(Form("spetree_%06d.root", run));

  chain->SetBranchAddress("evt", &evt);
  chain->SetBranchAddress("id", &id);
  chain->SetBranchAddress("q", &q);

  TH1D *qhis[384], *qhis2[384];

  for(int i = 0; i < 384; i++) {
    qhis[i] = new TH1D(Form("qhis_%d", i+1), "", 1100, -100, 1000);
    qhis2[i] = new TH1D(Form("qhis2_%d", i+1), "", 1100, -100, 1000);
    qhis[i]->SetTitle(Form("Det_%d-Ch_%d", (i)/4 +1, i%4+1));
    qhis2[i]->SetTitle(Form("Det_%d-Ch_%d", (i)/4 +1, i%4+1));
  }

  //-------------------------------gain tree--------------------
  TChain * chain2 = new TChain("gain");
  chain2->Add("spegainCR.root");

  chain2->SetBranchAddress("Sid", &Sid);
  chain2->SetBranchAddress("gain", &gain);

  int nent = chain2->GetEntries();
  cout << "gain entries: " << nent << endl;

   for(int i = 0; i < 384; i++){
    chain2->GetEntry(i);

    cout << Sid << ": " << gain << endl;
    gainM[i] = gain;
   }
 
  //-----------------------------------------------------------

  evt = chain->GetEntries();
  cout << "Tatal number of entries: " << evt << endl;
  
  for(int i = 0; i < 217657921; i++){
    chain->GetEntry(i);

    if (i > 0 && i % 10000000 == 0)
      cout << Form("%6d events processed ......", i) << endl;

    //cout << i << ": " << id << ": " << q << endl;

    if((id < 192) || (id > 259 && id < 452)) {
      if(id < 192) id -= 0;
      else if(id > 259 && id < 452) id = id - 68;
      else continue;
      //cout << i << ": " << id << ": " << q << endl;
      q1 = q*gainM[295]/gainM[id];
      qhis[id]->Fill(q1);
    }
  }
  
  TCanvas * can1 = new TCanvas("can1", "Initia", 1700, 1300);
  can1->Divide(8, 6, 0.001, 0.001 );
  TPad *pad1[48];
  for(int i = 0; i < 48; i++) {
    gPad->SetLogy();
   
    int maxbin = qhis[i+288]->GetMaximumBin();
    double maxx = qhis[i+288]->GetBinCenter(maxbin);
    double std = qhis[i+288]->GetStdDev();

    double upper = maxx + std;
    double lower = maxx - std;

    TF1 *f1 = new TF1("f1", "gaus", lower, upper);
    can1->cd(i+1);
    pad1[i] = (TPad*)can1->cd(i+1);
    
    qhis[i+288]->Fit(f1, "RQ0");
    
    cons = f1->GetParameter(0);
    gain1[i+288] = f1->GetParameter(1);
    stdv = f1->GetParameter(2);
    
    cout <<"gain1: " << i+288+1 << ": " << cons << ": " << gain1[i+288] << ": " << stdv << endl;   
  }
  
  TCanvas * can2 = new TCanvas("can2", "Final1", 1700, 1300);
  can2->Divide(8, 6, 0.001, 0.001 );
  TPad *pad2[48];
  for(int i = 0; i < 48; i++) {
    gPad->SetLogy();

    int maxbin = qhis[i+288]->GetMaximumBin();
    double maxx = qhis[i+288]->GetBinCenter(maxbin);

    double upper; 
    double lower;

    upper = maxx + 1.5*stdv;
    lower = maxx - 1.5*stdv;
      
    TF1 *f2 = new TF1("f2", "gaus", lower, upper);
    f2->SetParameter(0, cons);
    f2->SetParameter(1, gain1[i+288]);
    f2->SetParameter(2, stdv);

    can2->cd(i+1);
    pad2[i] = (TPad*)can2->cd(i+1);
    
    qhis[i+288]->Fit(f2, "RQ");
    
    gain2[i+288] = f2->GetParameter(1);
    
    cout << "gain2: " << i+288+1 << ": " << gain2[i+288] << endl;
  }
  
}

 

  
    

   
