void evtnum_edit()
{

  int runnum=330;
  TChain * chain = new TChain("psmd");
  chain->Add(Form("./prod_2nd/psmd_%06d_org2.root", runnum));

  int evtnum, bit1, bit2, bitsum;
  double trgtime;
  int trgdet[120];
  double iq[120][4], qsum[120], qmax[120], qp1[120], qp2[120];

  
  TFile *of = new TFile(Form("psmd_%06d.root",runnum),"RECREATE");
  TTree * psmd = new TTree("psmd","psmd");

  psmd->Branch("evtnum", &evtnum, "evtnum/I");
  psmd->Branch("trgtime", &trgtime, "trgtime/D");
  psmd->Branch("bit1", &bit1, "bit1/I");
  psmd->Branch("bit2", &bit2, "bit2/I");
  psmd->Branch("bitsum", &bitsum, "bitsum/I");
  psmd->Branch("trgdet", trgdet, "trgdet[bitsum]/I");
  psmd->Branch("iq", iq, "iq[bitsum][4]/D");
  psmd->Branch("qp1", qp1, "qp1[bitsum]/D");
  psmd->Branch("qp2", qp2, "qp2[bitsum]/D");
  psmd->Branch("qsum", qsum, "qsum[bitsum]/D");
  psmd->Branch("qmax", qmax, "qmax[bitsum]/D");

  Long64_t tent = chain->GetEntries();
  int flag=0;
  int pevtnum, pbitsum;
  double ptrgtime;
  double pqsum;
  
   for(Long64_t i=0; i<tent; i++){
     //    for(int i=0; i<1000000; i++){
    if(i%100000==0){cout<<i<< "/"<<tent<<endl;}
    chain->GetEntry(i);
    
    evtnum = chain->GetLeaf("evtnum")->GetValue();
    if(i>=66983385){evtnum=evtnum+67108863;}
    trgtime = chain->GetLeaf("trgtime")->GetValue();
    bit1 = chain->GetLeaf("bit1")->GetValue();
    bit2 = chain->GetLeaf("bit2")->GetValue();
    bitsum = chain->GetLeaf("bitsum")->GetValue();
    for(int j=0; j<bitsum; j++){
      trgdet[j] = chain->GetLeaf("trgdet")->GetValue(j);
      qsum[j] = chain->GetLeaf("qsum")->GetValue(j);
      qmax[j] = chain->GetLeaf("qmax")->GetValue(j);
      qp1[j] = chain->GetLeaf("qp1")->GetValue(j);
      qp2[j] = chain->GetLeaf("qp2")->GetValue(j);
      for(int k=0; k<4; k++){
	iq[j][k] = chain->GetLeaf("iq")->GetValue(4*j+k);
      }
    }

	psmd->Fill();
   }

  of->cd();
  psmd->Write();
  of->Close();
}
