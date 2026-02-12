R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_3/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/Muon/muon_3.1.2/shlib/Linux5.14-GCC_11_3/libRawObjs.so)


void wcmd_align()
{
  //  gStyle -> SetOptStat(0);


  int run = 330;
  int insub = 0;
  int finsub = 695;
  /*
   int run = 327;
   int insub = 0;
    int finsub = 282;
  */
 
  
  for(int sub=insub; sub<=finsub; sub++){

    //  for(int sub=0; sub<=0; sub++){
    TChain *chain = new TChain("prd_wcd");    
    chain -> Add(Form("/home/kkw/Muon/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,sub));
    TString outfile = Form("./prod_2nd/%06d/wcmd_%06d_%05d.root",run,run,sub); 
    TFile *of = new TFile(outfile.Data(), "RECREATE");

    cout<<"sub file "<<sub<<" processing..."<<endl;
  
    double trgtime, qsum, qmax;
    double qtot[48], fmax[48];
    int ch[48];
    int counts, evtnum, tbit, qmax_ch;
    double hit[48], fid[48];
    TTree * wcmd = new TTree("wcmd","wcmd");
    //  TTree * charge = new TTree("charge","charge");

    const int tbitmax=48;
    //  value->Branch("sub",&sub,"sub/I");
    wcmd->Branch("evtnum",&evtnum,"evtnum/I");
    wcmd->Branch("trgtime",&trgtime,"trgtime/D");
    wcmd->Branch("tbit",&tbit,"tbit/I");
    wcmd->Branch("qsum",&qsum,"qsum/D");
    wcmd->Branch("ch",ch, "ch[tbit]/I");
    wcmd->Branch("qtot",qtot,"qtot[tbit]/D");
    wcmd->Branch("qmax",&qmax,"qmax/D");    
    wcmd->Branch("qmax_ch",&qmax_ch,"qmax_ch/I");    
    wcmd->Branch("fmax",fmax,"fmax[tbit]/D");  
    
  
    int tent = chain->GetEntries();
  
    for(int i=0; i<tent; i++){
     //  for(int i=0; i<10; i++){    

      chain->GetEntry(i);
      evtnum = chain->GetLeaf("fEventNumber")->GetValue();
      trgtime = chain->GetLeaf("fTriggerTime")->GetValue();
      tbit = chain->GetLeaf("fColl.fID")->GetNdata();

      qsum=0; qmax=0; 
      for(int j=0; j<tbit; j++){
	ch[j] = chain->GetLeaf("fColl.fID")->GetValue(j);
	qtot[j] = chain->GetLeaf("fColl.fQtot")->GetValue(j);     
	//	qmax[j] = chain->GetLeaf("fColl.fQmax")->GetValue(j);
	fmax[j] = chain->GetLeaf("fColl.fFmax")->GetValue(j);      
	qsum = qsum+qtot[j];

	if(qmax<qtot[j]){qmax_ch=ch[j]; qmax=qtot[j];}
	//      cout<<ch[j]<<" "<< qtot[j] <<" "<<qmax[j] <<" "<<fmax[j]<<endl;
      }
      //    cout<<qsum<<endl;
      wcmd->Fill();
    }


    //    rhis1->Write();

    wcmd->Write();

    of->Close();

    delete chain;
    delete of;
  }

  
}
    
