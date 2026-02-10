R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void wcmd_plot_330_500()
{
  //
  gStyle -> SetOptStat(0);


  int run = 330;
  int intsub = 401;
  int finsub = 500;


  
  for(int sub=intsub; sub<=finsub; sub++){
  TChain *chain = new TChain("prd_wcd");
    //  for(int sub=0; sub<=0; sub++){
    chain -> Add(Form("/home/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",run,run,sub));
    cout<<"sub file "<<sub<<" processing..."<<endl;

  TTree * value = new TTree("value","value");
  //  TTree * charge = new TTree("charge","charge");

  double trgtime, qsum, qmax;
  double qtot[48], fmax[48];
  int ch[48];
  int counts, evtnum, tbit, qmax_ch;
  double hit[48], fid[48];

  const int tbitmax=48;
  //  value->Branch("sub",&sub,"sub/I");
  value->Branch("evtnum",&evtnum,"evtnum/I");
  value->Branch("trgtime",&trgtime,"trgtime/D");
  value->Branch("tbit",&tbit,"tbit/I");
  value->Branch("qsum",&qsum,"qsum/D");
  value->Branch("qmax",&qmax,"qmax/D");
  value->Branch("qmax_ch",&qmax_ch,"qmax_ch/I");      
  value->Branch("ch",ch, "ch[tbit]/I");
  value->Branch("qtot",qtot,"qtot[tbit]/D");

  value->Branch("fmax",fmax,"fmax[tbit]/D");  



    
  int tent = chain->GetEntries();
  
   for(int i=0; i<tent; i++){
     //  for(int i=0; i<10; i++){    

    chain->GetEntry(i);
    evtnum = chain->GetLeaf("fEventNumber")->GetValue();
    trgtime = chain->GetLeaf("fTriggerTime")->GetValue();
    tbit = chain->GetLeaf("fColl.fID")->GetNdata();

    qsum=0; qmax=0; qmax_ch=0;
    for(int j=0; j<tbit; j++){
      ch[j] = chain->GetLeaf("fColl.fID")->GetValue(j);
      qtot[j] = chain->GetLeaf("fColl.fQtot")->GetValue(j);     
      fmax[j] = chain->GetLeaf("fColl.fFmax")->GetValue(j);      
      qsum = qsum+qtot[j];
      if(qmax<qtot[j]){qmax=qtot[j]; qmax_ch=ch[j];}
      //      cout<<ch[j]<<" "<< qtot[j] <<" "<<qmax[j] <<" "<<fmax[j]<<endl;
      }
    //    cout<<qsum<<endl;
 
    if (tbit < 0 || tbit > 48) { continue;}
   
    value->Fill();
  }

  TString outfile = Form("../prod_2nd/%06d/wcmd_%06d_%05d.root",run,run,sub); 
   TFile *of = new TFile(outfile.Data(), "RECREATE");

    //    rhis1->Write();

   value->Write();

    of->Close();

    delete chain;
    delete value;
    
  }

  
  }
    
