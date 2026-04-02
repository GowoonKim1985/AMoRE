R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libRawObjs.so)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

void mrg_psmd_bundle(){

  int runnum = 330;
  int isub=0;
  int fsub=696;
  //evtnum edit (only for run 330)
  ULong64_t timeref = 1236473513000000.;
  UInt_t evt_offset = 67108864;


  for(int ifile=isub; ifile<=fsub; ifile++){
    cout<<ifile<<" file process..."<<endl;
    int subrun=ifile;
    TChain * ichain1 = new TChain("prd_psd");
    TChain * ichain2 = new TChain("prd_psd");
    ichain1->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq1_%06d_%05d.root", runnum, runnum, subrun));
    ichain2->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq2_%06d_%05d.root", runnum, runnum, subrun));
    
    //pre subrun file load
    TChain * pchain1 = new TChain("prd_psd");
    TChain * pchain2 = new TChain("prd_psd");
    if(subrun!=0){
      pchain1->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq1_%06d_%05d.root", runnum, runnum, subrun-1));
      pchain2->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq2_%06d_%05d.root", runnum, runnum, subrun-1));    
    }
    
    //pre+current sub file chain  
    TChain * chain1 = new TChain("prd_psd");
    TChain * chain2 = new TChain("prd_psd");
    if(subrun!=0){
      chain1->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq1_%06d_%05d.root", runnum, runnum, subrun-1));
      chain2->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq2_%06d_%05d.root", runnum, runnum, subrun-1));
    }
    chain1->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq1_%06d_%05d.root", runnum, runnum, subrun));
    chain2->Add(Form("/home/kkw/Muon/PROD/PSMD/%06d/prd_psd_daq2_%06d_%05d.root", runnum, runnum, subrun));
    
    //pre subrun
    PSEvent *ppsEvt1 = nullptr;  
    PSEvent *ppsEvt2 = nullptr;    
    pchain1->SetBranchAddress("PSEvent", &ppsEvt1);
    pchain2->SetBranchAddress("PSEvent", &ppsEvt2);
    // subrun
    PSEvent *ipsEvt1 = nullptr;  
    PSEvent *ipsEvt2 = nullptr;    
    ichain1->SetBranchAddress("PSEvent", &ipsEvt1);
    ichain2->SetBranchAddress("PSEvent", &ipsEvt2);
    
    //sum file
    PSEvent *psEvt1 = nullptr;   // chain1 읽는 용
    PSEvent *psEvt2 = nullptr;   // chain2 읽는 용  
    chain1->SetBranchAddress("PSEvent", &psEvt1);
    chain2->SetBranchAddress("PSEvent", &psEvt2);
    
    PSEvent *psEvt = new PSEvent();  // out에 쓸 용
    //    TFile *fout = new TFile(Form("prd_psd_%06d_%05d.root", runnum, subrun), "RECREATE");
    TFile *fout = new TFile(Form("/home/kkw/Muon/PROD/PSMD/%06d/merge/prd_psd_%06d_%05d.root", runnum, runnum, subrun), "RECREATE");
    TTree *out = new TTree("prd_psd", "prd_psd");
    out->Branch("PSEvent", &psEvt);
    // 추가 브랜치
    Int_t bit1, bit2, bitsum;
    out->Branch("bit1", &bit1, "bit1/I");
    out->Branch("bit2", &bit2, "bit2/I");
    out->Branch("bitsum", &bitsum, "bitsum/I");
    
    UInt_t evtnum1, evtnum2;
    ULong64_t trgtime1, trgtime2;
    
    //check pre entry numbers
    int ptent1, ptent2, pent1, pent2;
    pent1=0; pent2=0; ptent1=0; ptent2=0;
    if(subrun!=0){
      ptent1 = pchain1->GetEntries();
      ptent2 = pchain2->GetEntries();
      
      pchain1->GetEntry(ptent1-1);
      trgtime1=ppsEvt1->GetTriggerTime();
      pchain2->GetEntry(ptent2-1);
      trgtime2=ppsEvt2->GetTriggerTime();
      
      if(trgtime1<trgtime2){
	while(trgtime1<trgtime2){
	  pent2++;
	  pchain2->GetEntry(ptent2-1-pent2);
	  trgtime2=ppsEvt2->GetTriggerTime();
	}
      }
      else if(trgtime1>trgtime2){
	while(trgtime1>trgtime2){
	  pent1++;
	  pchain1->GetEntry(ptent1-1-pent1);
	  trgtime1=ppsEvt1->GetTriggerTime();
	}
      }
      else{}
    }
    
    //cut end entry of subfile, depend on fast ttime
    int ient1, ient2, itent1, itent2;
    ient1=0; ient2=0; itent1=0; itent2=0;
    
    itent1 = ichain1->GetEntries();
    itent2 = ichain2->GetEntries();
    
    ichain1->GetEntry(itent1-1);
    trgtime1=ipsEvt1->GetTriggerTime();
    ichain2->GetEntry(itent2-1);
    trgtime2=ipsEvt2->GetTriggerTime();
    
    if(trgtime1<trgtime2){
      while(trgtime1<trgtime2){
	ient2++;
	ichain2->GetEntry(itent2-1-ient2);
	trgtime2=ipsEvt2->GetTriggerTime();
      }
    }
    else if(trgtime1>trgtime2){
      while(trgtime1>trgtime2){
	ient1++;
	ichain1->GetEntry(itent1-1-ient1);
	trgtime1=ipsEvt1->GetTriggerTime();
      }
    }
    else{}
    
    cout<<"pre subfile daq1 entry add "<<pent1<<endl;
    cout<<"pre subfile daq2 entry add "<<pent2<<endl;
    cout<<"subfile  daq1 entry cut "<<ient1<<endl;
    cout<<"subfile  daq2 entry cut "<<ient2<<endl;
    
    Long64_t i1, i2, f1, f2;
    if(subrun==0){i1=0; i2=0;}
    else{i1 = ptent1-pent1; i2 = ptent2-pent2;}

    if(subrun==fsub){f1=ptent1+itent1; f2=ptent2+itent2;}
    else{f1 = ptent1+itent1-ient1;  f2 = ptent2+itent2-ient2; }
    //merge start  
    while(i1<f1 && i2<f2){
      if(i1%50000==0){cout<<i1<<" entry process..."<<endl;}
      
      chain1->GetEntry(i1);    
      evtnum1=psEvt1->GetEventNumber();
      trgtime1=psEvt1->GetTriggerTime();
      bit1 = psEvt1->GetN();
      
      chain2->GetEntry(i2);    
      evtnum2=psEvt2->GetEventNumber();
      trgtime2=psEvt2->GetTriggerTime();
      bit2 = psEvt2->GetN();
      
      if(trgtime1<trgtime2){
	delete psEvt;
	psEvt = new PSEvent();
	out->GetBranch("PSEvent")->SetAddress(&psEvt);      
	//     psEvt->SetEventNumber(psEvt1->GetEventNumber());
	if(runnum==330 && (trgtime1 > timeref)) {evtnum1 += evt_offset;}
	psEvt->SetEventNumber(evtnum1);    
	psEvt->SetTriggerTime(psEvt1->GetTriggerTime());
	
	for(int j=0; j<bit1; j++){
	  PSHit *hit1 = psEvt1->Get(j);
	  PSHit *newhit = psEvt->Add();
	  new(newhit) PSHit(*hit1);
	  //	*newhit = *hit1;	
	}
	
	bit2 = 0; bitsum = bit1;
	out->Fill();
	++i1;
      }
      
      else if(trgtime1>trgtime2){
	
	delete psEvt;
	psEvt = new PSEvent();
	out->GetBranch("PSEvent")->SetAddress(&psEvt);
	
	//      psEvt->SetEventNumber(psEvt2->GetEventNumber());
	if(runnum==330 && (trgtime2 > timeref)) {evtnum2 += evt_offset;}
	psEvt->SetEventNumber(evtnum2);    
	psEvt->SetTriggerTime(psEvt2->GetTriggerTime());
	
	for(int j=0; j<bit2; j++){
	  PSHit *hit2 = psEvt2->Get(j);
	  PSHit *newhit = psEvt->Add();
	  new(newhit) PSHit(*hit2);
	}
	
	bit1 = 0; bitsum = bit2;
	out->Fill();
	++i2;
      }
      
      
      else if(trgtime1==trgtime2){
	
	delete psEvt;
	psEvt = new PSEvent();
	out->GetBranch("PSEvent")->SetAddress(&psEvt);
	
	//     psEvt->SetEventNumber(psEvt1->GetEventNumber()); 
	if(runnum==330 && (trgtime1 > timeref)) {evtnum1 += evt_offset;}
	psEvt->SetEventNumber(evtnum1);    
	psEvt->SetTriggerTime(psEvt1->GetTriggerTime());
	
	for(int j=0; j<bit1; j++){
	  PSHit *hit1 = psEvt1->Get(j);
	  PSHit *newhit = psEvt->Add();
	  new(newhit) PSHit(*hit1);
	}
	
	for(int j=0; j<bit2; j++){
	  PSHit *hit2 = psEvt2->Get(j);
	  PSHit *newhit = psEvt->Add();
	  new(newhit) PSHit(*hit2);
	}
	
	
	bitsum = bit1+ bit2;
	out->Fill();
	++i1;
	++i2;
	
      }
    }
    
    //additional entries
    
    while(i1<f1){
      chain1->GetEntry(i1);
      evtnum1=psEvt1->GetEventNumber();
      trgtime1=psEvt1->GetTriggerTime();
      bit1 = psEvt1->GetN();
      delete psEvt;
      psEvt = new PSEvent();
      out->GetBranch("PSEvent")->SetAddress(&psEvt);
      //    psEvt->SetEventNumber(psEvt1->GetEventNumber());
      if(runnum==330 && (trgtime1 > timeref)) {evtnum1 += evt_offset;}
      psEvt->SetEventNumber(evtnum1);        
      psEvt->SetTriggerTime(psEvt1->GetTriggerTime());
      
      for(int j=0; j<bit1; j++){
	PSHit *hit1 = psEvt1->Get(j);
	PSHit *newhit = psEvt->Add();
	new(newhit) PSHit(*hit1);
      }
      bit2=0; bitsum=bit1;
      out->Fill();
      ++i1;    
    }
    
    while(i2 < f2){
      chain2->GetEntry(i2);
      evtnum2=psEvt2->GetEventNumber();
      trgtime2=psEvt2->GetTriggerTime();
      
      bit2 = psEvt2->GetN();
      delete psEvt;
      psEvt = new PSEvent();
      out->GetBranch("PSEvent")->SetAddress(&psEvt);
      //    psEvt->SetEventNumber(psEvt2->GetEventNumber());
      if(runnum==330 && (trgtime2 > timeref)) {evtnum2 += evt_offset;}
      psEvt->SetEventNumber(evtnum2);     
      psEvt->SetTriggerTime(psEvt2->GetTriggerTime());
      for(int j=0; j<bit2; j++){
        PSHit *hit2 = psEvt2->Get(j);
        PSHit *newhit = psEvt->Add();
        new(newhit) PSHit(*hit2);
      }
      bit1=0; bitsum=bit2;
      out->Fill();
      ++i2;
    }
    
    
    out->Write();
    fout->Close();
  }
  
}
