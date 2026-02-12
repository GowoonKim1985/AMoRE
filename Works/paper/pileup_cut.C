void pileup_cut()
{

  int runnum=327;
  TChain * chain = new TChain("psmd");
  chain->Add(Form("./prod_2nd/psmd_%06d_org.root", runnum));

  int evtnum, bit1, bit2, bitsum;
  double trgtime;
  int trgdet[120];
  double iq[120][4], qsum[120], qmax[120], qp1[120], qp2[120];

  
  TFile *of = new TFile(Form("psmd_%06d_clean.root",runnum),"RECREATE");
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

  int tent = chain->GetEntries();
  int flag=0;
  int pevtnum, pbitsum;
  double ptrgtime;
  double pqsum;
  
   for(int i=0; i<tent; i++){
     //    for(int i=0; i<1000000; i++){
    if(i%100000==0){cout<<i<< "/"<<tent<<endl;}
    chain->GetEntry(i);
    
    evtnum = chain->GetLeaf("evtnum")->GetValue();
    trgtime = chain->GetLeaf("trgtime")->GetValue();
    bitsum = chain->GetLeaf("bitsum")->GetValue();
    for(int j=0; j<bitsum; j++){
      qsum[j] = chain->GetLeaf("qsum")->GetValue(j);
    }

    if(i==0){flag=0;}
    if(i!=0){
      chain->GetEntry(i-1);
      pevtnum = chain->GetLeaf("evtnum")->GetValue();
      ptrgtime = chain->GetLeaf("trgtime")->GetValue();
      pbitsum = chain->GetLeaf("bitsum")->GetValue();
      pqsum = chain->GetLeaf("qsum")->GetValue(0);
      if((evtnum==pevtnum)&&(ptrgtime==trgtime)&&(pbitsum==bitsum)&&(qsum[0]==pqsum)){flag=1;} // overlap
      else{flag=0;}
    }    

    if(flag==0){
      chain->GetEntry(i);
      for(int j=0; j<bitsum; j++){
	bit1 = chain->GetLeaf("bit1")->GetValue();
	bit2 = chain->GetLeaf("bit2")->GetValue();
	qmax[j]=0;
	
	trgdet[j] = chain->GetLeaf("trgdet")->GetValue(j);
	for(int k=0; k<4; k++){
	  iq[j][k] = chain->GetLeaf("iq")->GetValue(4*j+k);
	  if(qmax[j]<iq[j][k]){qmax[j]=iq[j][k];}
	}
	qp1[j]=iq[j][0]+iq[j][1];
	qp2[j]=iq[j][2]+iq[j][3];
      }
      
	psmd->Fill();
    }
    if(flag==1){
      //	cout<<"entry "<<i<<" evt "<<evtnum<<" trgtime "<<trgtime<<" qsum1 "<<qsum[0]<<endl;
    }
    
    
  }
  
 

  of->cd();
  psmd->Write();
  of->Close();
}
