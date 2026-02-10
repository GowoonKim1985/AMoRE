R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void outplot()
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
  //run 368 sub 0 - 239

  const int fn = 10;
  /*
  //16
  int run[16] = {272,287,288,294,303,304,311,315,318,327,330,339,343,356,361,368};
  int finsub[16] = {64,93,525,76,1154,791,190,191,689,282,695,286,94,667,143,239};
  int subdiv=24; // how many sub files
  */
  /*
  //12
  int run[fn] = {288,303,304,311,315,318,327,330,339,356,361,368};
  int finsub[fn] = {525,1154,791,190,191,689,282,695,286,667,143,239};
  int subdiv=168; // how many sub files
  */
  //10
  int run[fn] = {304,311,315,318,327,330,339,356,361,368};
  int finsub[fn] = {791,190,191,689,282,695,286,667,143,239};
  int subdiv=168; // how many sub files


  
  cout<<"run : isub : mean"<<endl;
  
  for(int i=0; i<fn; i++){
    int procnum = finsub[i]/subdiv;

    for(int proc=0; proc<procnum; proc++){
      int sub1=subdiv*proc;
      int sub2=sub1+subdiv-1;
      //      TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/ratio1_24hr/run%i_%04d_to_%04d.root",run[i],sub1,sub2);
      TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/ratio1_1wk/run%i_%04d_to_%04d.root",run[i],sub1,sub2);
      //    cout<<rawfile<<endl;
      TChain * chain = new TChain("value");
      chain->Add(rawfile);
      chain->GetEntry(0);
      double mean = chain->GetLeaf("mean_r1")->GetValue();
      cout<<run[i]<<" : "<<sub1<<" : "<<mean<<endl;
    
    }
  }
}
  

