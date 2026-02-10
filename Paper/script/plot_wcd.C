R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)



void plot_wcd()
{
  gStyle -> SetOptStat(0);

  int runnum = 330;
  //  TChain *chain = new TChain("tree");
  TChain *chain = new TChain("value");  
  //  chain -> Add("/y/kmseo/data/WCMD/000216/oldtree/tree_WCMD_216_*.root");
  //  chain -> Add("./prod/tree_WCMD_216_00000.root");
  //  chain -> Add(Form("./prod/tree_WCMD_%03d_00000.root",runnum));

  //  500 1500
  TH2D *htbQ = new TH2D("htbQ", "", 48, 0, 48, 35000, 0, 350000);
  htbQ -> SetXTitle("Number of TRG bit");
  htbQ -> SetYTitle("Qsum");

  TH2D *htbQ_cut1 = new TH2D("htbQ_cut1", "", 48, 0, 48, 35000, 0, 350000);
  htbQ_cut1 -> SetXTitle("Number of TRG bit");
  htbQ_cut1 -> SetYTitle("Qsum");

  TH2D *htbQ_cut2 = new TH2D("htbQ_cut2", "", 48, 0, 48, 35000, 0, 350000);
  htbQ_cut2 -> SetXTitle("Number of TRG bit");
  htbQ_cut2 -> SetYTitle("Qsum");

  
  
  for(int i=0;i<=695;i++){
    chain -> Add(Form("./prod_2nd/000330/wcmd_000330_%05d.root",i));
  }
  for(int i=0;i<=28;i++){
    chain -> Add(Form("./prod_2nd/000327/wcmd_000327_%05d.root",i));      
  }

    int tent = chain->GetEntries();
  double bitsum;
  double iqsum, iqmax, qmax[48], ch[48];
  //  for(int i=0; i<10; i++){
  for(int i=0; i<tent; i++){    
    
    chain->GetEntry(i);
    //    cout<<i<<endl;
    bitsum = chain->GetLeaf("tbit")->GetValue();
    iqsum = chain->GetLeaf("qsum")->GetValue();
    iqmax = chain->GetLeaf("qmax")->GetValue();
    if(iqsum>0&&iqmax>0){
      htbQ -> Fill(bitsum, iqsum);
      if((iqmax/iqsum)<=1){
      htbQ_cut1 -> Fill(bitsum, iqsum);

          if(iqmax<(0.94*iqsum-3750.)){
	htbQ_cut2 -> Fill(bitsum, iqsum);
      }

      }

    }


    
    //    cout<<"tbit "<<bitsum<<" / qsum "<<iqsum<<endl;
  }
  htbQ->Draw();
    TFile of("./hist/plot_wcmd_bot.root", "recreate");
    htbQ->Write();
    htbQ_cut1->Write();
    htbQ_cut2->Write();
    of.Close();

  
}
  
