void psmd_align(){

  int runnum = 330;
  int subrun = 696;
  // int subrun = 1;
  
  //  TChain * chain1 = new TChain("prd_psd");
  //  chain1->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq1_%06d_%05d.root", runnum, runnum, subrun));
  //  TChain * chain2 = new TChain("prd_psd");
  //  chain2->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq2_%06d_%05d.root", runnum, runnum, subrun));

  TChain * chain1 = new TChain("prd_psd");
  //  chain1->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq1_%06d_*", runnum, runnum));
  TChain * chain2 = new TChain("prd_psd");
  //  chain2->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq2_%06d_*", runnum, runnum));

  for(int i=0; i<=subrun; i++){
    // for(int i=400; i<=400; i++){
    chain1->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq1_%06d_%05d.root", runnum, runnum, i));
    chain2->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq2_%06d_%05d.root", runnum, runnum, i));
  }
  
  int evtnum1, evtnum2, evtnum, bit1, bit2, bitsum;
  double trgtime1, trgtime2, trgtime;  
  int trgdet1[120], trgdet2[120], trgdet[120];
  double iq1[120][4], iq2[120][4], iq[120][4], qsum1[120], qsum2[120], qsum[120], qmax1[120], qmax2[120], qmax[120];
  double qp1[120], qp2[120], qp1d1[120], qp2d1[120], qp1d2[120], qp2d2[120];
  
  TFile * of = new TFile(Form("./prod_2nd/psmd_%06d.root",runnum),"RECREATE"); 
  // TFile * of = new TFile(Form("./prod_2nd/psmd_%06d_test.root",runnum),"RECREATE"); 
  TTree * psmd = new TTree("psmd","psmd");

  psmd->Branch("evtnum", &evtnum, "evtnum/I");
  psmd->Branch("trgtime", &trgtime, "trgtime/D");
  psmd->Branch("bit1", &bit1, "bit1/I");
  psmd->Branch("bit2", &bit2, "bit2/I");
  psmd->Branch("bitsum", &bitsum, "bitsum/I");
  psmd->Branch("trgdet", trgdet, "trgdet[bitsum]/I");
  psmd->Branch("iq", iq, "iq[bitsum][4]/D");
  psmd->Branch("qsum", qsum, "qsum[bitsum]/D");
  psmd->Branch("qp1", qp1, "qp1[bitsum]/D");
  psmd->Branch("qp2", qp2, "qp2[bitsum]/D");
  psmd->Branch("qmax", qmax, "qmax[bitsum]/D");

  
    int totN1 = chain1->GetEntries();
    int totN2 = chain2->GetEntries();

    cout<<"totN1 "<<totN1<<endl;
    cout<<"totN2 "<<totN2<<endl;

    
    int i1=0; int i2=0;
    int evt_offset = 67108864;
    double timeref = 1236473513000000.;
  while(i1< totN1 && i2<totN2){
    if(i1%50000==0){cout<<i1<<" entry process..."<<endl;}
    
    chain1->GetEntry(i1);    
    evtnum1=chain1->GetLeaf("fEventNumber")->GetValue();
    trgtime1=chain1->GetLeaf("fTriggerTime")->GetValue();

    if(trgtime1>timeref){evtnum1=evtnum1+evt_offset;}
    
    bit1=chain1->GetLeaf("fColl.fID")->GetNdata();
    for(int j1=0; j1<bit1; j1++){
      trgdet1[j1]=chain1->GetLeaf("fColl.fID")->GetValue(j1);
      qsum1[j1]=chain1->GetLeaf("fColl.fQsum")->GetValue(j1);
      qmax1[j1]=0;
      for(int k1=0; k1<4; k1++){
	iq1[j1][k1]=chain1->GetLeaf("fColl.fQ")->GetValue(4*j1+k1);
	if(qmax1[j1]<iq1[j1][k1]){qmax1[j1]=iq1[j1][k1];}
      }
      qp1d1[j1]=iq1[j1][0]+iq1[j1][1];
      qp2d1[j1]=iq1[j1][2]+iq1[j1][3];
    }

    chain2->GetEntry(i2);    
    evtnum2=chain2->GetLeaf("fEventNumber")->GetValue();
    trgtime2=chain2->GetLeaf("fTriggerTime")->GetValue();

    if(trgtime2>timeref){evtnum2=evtnum2+evt_offset;}
    bit2=chain2->GetLeaf("fColl.fID")->GetNdata();
    for(int j2=0; j2<bit2; j2++){
      trgdet2[j2]=chain2->GetLeaf("fColl.fID")->GetValue(j2);
      qsum2[j2]=chain2->GetLeaf("fColl.fQsum")->GetValue(j2);
      qmax2[j2]=0;
      for(int k2=0; k2<4; k2++){
	iq2[j2][k2]=chain2->GetLeaf("fColl.fQ")->GetValue(4*j2+k2);
	if(qmax2[j2]<iq2[j2][k2]){qmax2[j2]=iq2[j2][k2];}
      }
      qp1d2[j2]=iq2[j2][0]+iq2[j2][1];
      qp2d2[j2]=iq2[j2][2]+iq2[j2][3];
      
    }

    if(evtnum1<evtnum2){
      ++i1;
      evtnum=evtnum1; trgtime = trgtime1; bit2=0; bitsum=bit1;
      for(int w1=0; w1<bit1; w1++){
	qsum[w1]=qsum1[w1];trgdet[w1]=trgdet1[w1]; qmax[w1]=qmax1[w1]; qp1[w1]=qp1d1[w1]; qp2[w1]=qp2d1[w1];
	for(int w1sub=0; w1sub<4; w1sub++){
	  iq[w1][w1sub]=iq1[w1][w1sub];
	}
      }
      psmd->Fill();
    }


    else if(evtnum1>evtnum2){
        ++i2;
      // cout<<"evt2 "<<evtnum2<<endl;      
	evtnum=evtnum2; trgtime = trgtime2; bit1=0; bitsum=bit2;
      for(int w2=0; w2<bit2; w2++){
	qsum[w2]=qsum2[w2]; trgdet[w2]=trgdet2[w2];  qmax[w2]=qmax2[w2]; qp1[w2]=qp1d2[w2]; qp2[w2]=qp2d2[w2];
	for(int w2sub=0; w2sub<4; w2sub++){
	  iq[w2][w2sub]=iq2[w2][w2sub];
	}
      }
      psmd->Fill();

  }


    else if(evtnum1==evtnum2){
      ++i1;
      ++i2;

      evtnum=evtnum1; trgtime = trgtime1; bitsum=bit1+bit2;
      
      for(int we1=0; we1<bit1; we1++){
	qsum[we1]=qsum1[we1]; trgdet[we1]=trgdet1[we1]; qmax[we1]=qmax1[we1]; qp1[we1]=qp1d1[we1]; qp2[we1]=qp2d1[we1];
	for(int we1sub=0; we1sub<4; we1sub++){
	  iq[we1][we1sub]=iq1[we1][we1sub];
	}
      }
      for(int we2=0; we2<bit2; we2++){
	qsum[bit1+we2]=qsum2[we2]; trgdet[bit1+we2]=trgdet2[we2]; qmax[bit1+we2]=qmax2[we2];
	qp1[bit1+we2]=qp1d2[we2]; qp2[bit1+we2]=qp2d2[we2];
	for(int we2sub=0; we2sub<4; we2sub++){
	  iq[bit1+we2][we2sub]=iq2[we2][we2sub];
	}
      }
      psmd->Fill();

    }
  
  }

// chain1 남은 것 처리
while(i1 < totN1){
  chain1->GetEntry(i1);

  evtnum1 = chain1->GetLeaf("fEventNumber")->GetValue();
  trgtime1 = chain1->GetLeaf("fTriggerTime")->GetValue();

  if(trgtime1 > timeref){ evtnum1 += evt_offset; }

  bit1 = chain1->GetLeaf("fColl.fID")->GetNdata();

  evtnum = evtnum1;
  trgtime = trgtime1;
  bit2 = 0;
  bitsum = bit1;

  for(int j1=0; j1<bit1; j1++){
    trgdet[j1] = chain1->GetLeaf("fColl.fID")->GetValue(j1);
    qsum[j1]   = chain1->GetLeaf("fColl.fQsum")->GetValue(j1);
    qmax[j1]=0;
    for(int k1=0; k1<4; k1++){
      iq[j1][k1] = chain1->GetLeaf("fColl.fQ")->GetValue(4*j1+k1);
      if(qmax[j1]<iq[j1][k1]){qmax[j1]=iq[j1][k1];}
    }
    qp1[j1]=iq[j1][0]+iq[j1][1];
    qp2[j1]=iq[j1][2]+iq[j1][3];
  }

  psmd->Fill();
  ++i1;
}


// chain2 남은 것 처리
while(i2 < totN2){
  chain2->GetEntry(i2);

  evtnum2 = chain2->GetLeaf("fEventNumber")->GetValue();
  trgtime2 = chain2->GetLeaf("fTriggerTime")->GetValue();

  if(trgtime2 > timeref){ evtnum2 += evt_offset; }

  bit2 = chain2->GetLeaf("fColl.fID")->GetNdata();

  evtnum = evtnum2;
  trgtime = trgtime2;
  bit1 = 0;
  bitsum = bit2;

  for(int j2=0; j2<bit2; j2++){
    trgdet[j2] = chain2->GetLeaf("fColl.fID")->GetValue(j2);
    qsum[j2]   = chain2->GetLeaf("fColl.fQsum")->GetValue(j2);
    qmax[j2] = 0;
    for(int k2=0; k2<4; k2++){
      iq[j2][k2] = chain2->GetLeaf("fColl.fQ")->GetValue(4*j2+k2);
      if(qmax[j2]<iq[j2][k2]){qmax[j2]=iq[j2][k2];}
      
    }
    qp1[j2]=iq[j2][0]+iq[j2][1];
    qp2[j2]=iq[j2][2]+iq[j2][3];
  }

  psmd->Fill();
  ++i2;
}
  
  of->cd();
  psmd->Write();
  of->Close();
    
}
