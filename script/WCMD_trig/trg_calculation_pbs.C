R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void trg_calculation_pbs(int p=0)
{

  //
  gStyle -> SetOptStat(0);

  //run 272 sub 0 - 64 : 231105 17:16:11
  //run 287 sub 0 - 93
  //run 288 sub 0 -525
  //run 294 sub 0 - 76
  //run 303 sub 0-1154
  //run 304 sub 0 -791
  //run 311 sub 0 - 190
  //run 315  sub 0 - 191
  //run 318 sub 0 - 689
  //run 327 sub 0 - 282
  //run 330 sub 0 - 695
  //run 339 sub 0 - 286
  //run 343 sub 0 - 94
  //run 356 sub 0 - 667
  //run 361 sub 0 - 143  
  //run 368 sub 0 - 332
  
    const int fn = 27;

    /* //16
  int run[fn] = {287, 288, 294, 303, 304, 311, 315, 318, 327, 330, 339, 343, 356, 361, 368};
  int finsub[fn] = {93, 525, 76, 1154, 791, 190, 191, 689, 282, 695, 286, 94, 667, 143, 332};
    */
  //int daygap[fn] = {6, 10, 35, 40, 89, 122, 130, 138, 167, 179, 231, 244, 250, 305, 311};  


    int run[fn] = {287, 288, 294, 303, 304, 311, 315, 318, 327, 330, 339, 343, 356, 361, 368, 376, 401, 404, 405, 407, 409, 411, 413, 416, 426, 429, 435};
    int finsub[fn] = {93, 525, 76, 1154, 791, 190, 191, 689, 282, 695, 286, 94, 667, 143, 332, 18, 169, 63, 68, 23, 44, 72, 20, 16, 338, 15, 474};
    //  int finsub[fn] = {1, 1, 76, 1154, 791, 190, 191, 689, 282, 695, 286, 94, 667, 143, 332, 18, 169, 63, 68, 23, 44, 72, 20, 16, 338, 15, 474};


    int irun = run[p];
    int ifinsub = finsub[p];
 
   TFile *of;

   //   for(int ifile=0; ifile<fn; ifile++){
   //   for(int ifile=0; ifile<2; ifile++){
   
  TTree * value = new TTree("value","value");
  //  TTree * charge = new TTree("charge","charge");

  double trgtime, rate, m_rate;
  int counts, sub, sum;
  
  // value->Branch("sub",&sub,"sub/I");
  value->Branch("trgtime",&trgtime,"trgtime/D");
  //  value->Branch("counts",hit,"hit[48]/I"); 
  value->Branch("counts",&counts,"counts/I"); 
  value->Branch("rate",&rate,"rate/D");  
  value->Branch("m_rate", &m_rate, "m_rate/D");

  
  double itime, ftime;
  double divtime = 10000000000; //10s
  TString text;

  TChain *chain = new TChain("prd_wcd");

  for(int sub=0; sub<=ifinsub; sub++){
  //  for(int sub=0; sub<=0; sub++){
    chain -> Add(Form("/home/PROD/WCMD/%06d/prd_wcd_%06d_%05d.root",irun,irun,sub));
  }
  
  int tent = chain->GetEntries();
  chain->GetEntry(tent-1);
  
  ftime = chain->GetLeaf("fTriggerTime")->GetValue();
  int lp = (int) (ftime/divtime);
  sum=0;
    
  for(int i=0; i<lp; i++){
    //   cout<<i<<endl;
    itime = i * divtime;
    //          for(int i=0; i<1000; i++){      
    if(i%100==0){cout<<i<<" ent process..."<<endl;}
    //          cout<<i<<endl;

    text = Form("fTriggerTime>%f&&fTriggerTime<%f",itime, itime+divtime);
    //   cout<<text<<endl;
    counts = chain->GetEntries(text);
    //   cout<<counts<<endl;
     sum = sum+counts;
    //  cout<<sum<<endl;
    rate = ((double) counts)/(divtime/1000000000);
    m_rate = ((double) sum)/((itime+divtime)/1000000000);
    //   cout<<rate<<endl;
    //  cout<<m_rate<<endl;
    trgtime = itime/1000000000.;
    value->Fill();

    
  }
    //    hmean->SetBinContent(proc+1,mean_r1);

  TString outfile = Form("./div/run%i_rate_10s.root",irun); 
    of = new TFile(outfile.Data(), "RECREATE");

    //    rhis1->Write();

    value->Write();
    of->Close(); 
    //  }
}
