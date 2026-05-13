void simfile_align()
{
  TFile *fin = new TFile("./ps_coincidence/angle_ps_coincidence_1000run.root","READ");
  TTree *tin = (TTree*)fin->Get("tree");

  const int MAX = 150;

  int ntheta, nphi, nr, npsveto_edep, npsveto_id;
  double theta[MAX], phi[MAX], r[MAX], psveto_edep[MAX];
  int psveto_id[MAX];

  tin->SetBranchAddress("ntheta",&ntheta);
  tin->SetBranchAddress("theta",theta);
  tin->SetBranchAddress("nphi",&nphi);
  tin->SetBranchAddress("phi",phi);
  tin->SetBranchAddress("nr",&nr);
  tin->SetBranchAddress("r",r);
  tin->SetBranchAddress("npsveto_edep",&npsveto_edep);
  tin->SetBranchAddress("psveto_edep",psveto_edep);
  tin->SetBranchAddress("npsveto_id",&npsveto_id);
  tin->SetBranchAddress("psveto_id",psveto_id);

  TFile *fout = new TFile("angle_ps_coincidence_1000run_top2.root","RECREATE");
  //  TFile *fout = new TFile("test.root","RECREATE");
  TTree *tout = new TTree("tree","tree");

  int ontheta, onphi, onr, onpsveto_edep, onpsveto_id;
  double otheta[MAX], ophi[MAX], orr[MAX], opsveto_edep[2];
  int opsveto_id[2];

  tout->Branch("ntheta",&ontheta,"ntheta/I");
  tout->Branch("theta",otheta,"theta[ntheta]/D");
  tout->Branch("nphi",&onphi,"nphi/I");
  tout->Branch("phi",ophi,"phi[nphi]/D");
  tout->Branch("nr",&onr,"nr/I");
  tout->Branch("r",orr,"r[nr]/D");
  tout->Branch("npsveto_edep",&onpsveto_edep,"npsveto_edep/I");
  tout->Branch("psveto_edep",opsveto_edep,"psveto_edep[npsveto_edep]/D");
  tout->Branch("npsveto_id",&onpsveto_id,"npsveto_id/I");
  tout->Branch("psveto_id",opsveto_id,"psveto_id[npsveto_id]/I");

  Long64_t nentries = tin->GetEntries();

  for(Long64_t i=0; i<nentries; i++){
    tin->GetEntry(i);

    ontheta = ntheta;
    onphi = nphi;
    onr = nr;

    for(int j=0; j<ntheta; j++) otheta[j] = theta[j];
    for(int j=0; j<nphi; j++) ophi[j] = phi[j];
    for(int j=0; j<nr; j++) orr[j] = r[j];

    int imax1 = -1;
    int imax2 = -1;

    for(int j=0; j<npsveto_edep; j++){
      if(imax1 < 0 || psveto_edep[j] > psveto_edep[imax1]){
        imax2 = imax1;
        imax1 = j;
      }
      else if(imax2 < 0 || psveto_edep[j] > psveto_edep[imax2]){
        imax2 = j;
      }
    }

    onpsveto_edep = 0;
    onpsveto_id = 0;

    if(imax1 >= 0){
      opsveto_edep[onpsveto_edep] = psveto_edep[imax1];
      opsveto_id[onpsveto_id] = psveto_id[imax1];
      onpsveto_edep++;
      onpsveto_id++;
    }

    if(imax2 >= 0){
      opsveto_edep[onpsveto_edep] = psveto_edep[imax2];
      opsveto_id[onpsveto_id] = psveto_id[imax2];
      onpsveto_edep++;
      onpsveto_id++;
    }

    tout->Fill();
  }

  fout->cd();
  tout->Write();
  fout->Write();
  fout->Close();

  fin->Close();
}
