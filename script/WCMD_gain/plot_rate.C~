R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)

#include <cmath> // for std::nan()

void plot_rate()
{

  const int tfile = 16;
  const int tid = 47;
  char a; 
  int runnum[tfile] ={
    359, 367, 374, 378, 393,
    400, 402, 408, 412, 414,
    417, 425, 427, 434, 437,
    444};

  double runday[tfile] = {
    305.4, 311.5, 325.5, 326.6, 328.5,
    362.4, 369.7, 376.4, 381.4, 382.6,
    383.4, 385.4, 402.6, 403.6, 423.5,
    433.6};

  double mrunday[22] = {
    130.6, 138.6, 167.6, 179.4, 231.6,
    243.6, 244.6, 249.6, 305.5, 311.5,
    325.6, 362.5, 369.7, 372.4, 375.4,
    376.4, 378.4, 381.5, 382.6, 385.4,
    402.7, 403.7};

  double mrate[22] = {
    3.97, 2.78, 3.40, 3.30, 3.74,
    4.13, 4.76, 3.70, 1.05, 1.52,
    2.53, 1.07, 1.33, 1.28, 1.28,
    1.13, 1.14, 1.14, 1.10, 1.33,
    0.50, 0.61};
  
  
  int id[tid];
  double q[tfile][tid], q_err[tfile][tid], chi2[tfile][tid];
  //
  gStyle -> SetOptStat(0);

  TH1D * hrate[tid];
  for(int iid=0; iid<tid; iid++){
    hrate[iid] = new TH1D (Form("id%i",iid+1),"", 450, 0, 450);
  }
  TH1D * hmrate = new TH1D ("hmrate" ,"", 450, 0, 450);

  
  TCanvas * can = new TCanvas("can","can", 1000, 1000);
  can->Divide(7,7);

  for(int ifile=0; ifile<tfile; ifile++ ){
    TString rawfile = Form("/home/kkw/muon_3.1.2/analysis/WCMD_gain/hist/GainCal_%06d.root",runnum[ifile]);



    std::cout << "Raw file path: " << rawfile << std::endl;

    TChain * tree = new TChain("tree");
    tree->Add(rawfile);

    for(int iid=0; iid<tid; iid++){
      tree->GetEntry(iid);
      q[ifile][iid] = tree->GetLeaf("q")->GetValue();// 5=q6/q42
      q_err[ifile][iid] = tree->GetLeaf("q_err")->GetValue();// 5=q6/q42	
      //      cout<<"run " <<runnum[ifile]<<endl;
      //      cout<<"id "<< iid+1 <<endl;
      //      cout<<"q "<<q[ifile][iid]<<endl;

    }
  }

  for(int iid=0; iid<tid; iid++){
    hrate[iid]->SetMarkerStyle(21);
    hrate[iid]->SetMinimum();
    hrate[iid]->SetTitle(Form("id%i",iid));
    hrate[iid]->GetXaxis()->SetTitle("day");
    hrate[iid]->GetYaxis()->SetTitle("Q");	
    hrate[iid]->SetBinContent(1, 6E+6);

    for(int ifile=0; ifile<tfile; ifile++){
      hrate[iid]->SetBinContent(hrate[iid]->FindBin(runday[ifile]), q[ifile][iid]);
      hrate[iid]->SetBinError(hrate[iid]->FindBin(runday[ifile]), q_err[ifile][iid]);      
    }
    can->cd();
    can->cd(iid+1);
    hrate[iid]->Draw("P");

    //    gPad->SetGridx();  
    gPad->SetGridy();  

    
    can->Update();
    //   cin>>a;
    
  }

  for(int ref=0; ref<22; ref++){
    hmrate->SetBinContent(hmrate->FindBin(mrunday[ref]), mrate[ref]);


  }

    hmrate->SetMarkerStyle(21);
    hmrate->SetMarkerColor(kRed);    
    hmrate->SetTitle("muon rate");
    hmrate->GetXaxis()->SetTitle("day");
    hmrate->GetYaxis()->SetTitle("Hz");	
    hmrate->SetBinContent(1,9.97);

    can->cd();
    can->cd(tid+1);
    hmrate->Draw("P");

    //    gPad->SetGridx();  
    gPad->SetGridy();  

  
  TFile * outfile = new TFile("wcmd_rate.root","RECREATE");
  for(int iid=0; iid<tid;iid++){
  hrate[iid]->Write();
  }
  hmrate->Write();
  outfile->Close();
}
/*
      hmean[i]->SetMarkerStyle(21);
      //      hmean[i]->SetMarkerSize(1.2);
      //    hmean[i]->SetMarkerColor(2);
      hmean[i]->SetMinimum(0.0);
      hmean[i]->SetMaximum(1.1);
      hmean[i]->GetXaxis()->SetTitle("PMT distance");
      hmean[i]->GetYaxis()->SetTitle("Qx/Q0");	
      hmean[i]->Draw("P");
      
      
      ibin = daygap[lp]+i;
      //      cout<<"run : "<<run[lp]<<"day : "<<i<<" ibin: "<<ibin<<" mean : "<<mean[0]<<endl;

      hmean_tot->SetBinContent(ibin+1, mean[0]);
      if(mean[0]>0&&hit[0]>0){hmean_sel->SetBinContent(ibin+1, mean[0]*hit[6]/hit[0]);}
      if(hit[6]>0){hcnt_tot->SetBinContent(ibin+1, hit[0]/hit[6]);}
    }   
    
  

    
  }
    
  //  cmean->Update();

  
  


  
  hmean_tot->SetMarkerStyle(2);
  hmean_tot->SetMinimum(0.0);
    hmean_tot->SetMaximum(0.1);

  hmean_tot->GetXaxis()->SetTitle("time(day)");
  hmean_tot->GetYaxis()->SetTitle("Q6/Q42 mean");	
  cmean->cd();
  hmean_tot->Draw("P");

cmean->Update();

  hmean_sel->SetMarkerStyle(2);
  hmean_sel->SetMinimum(0.0);
  hmean_sel->SetMaximum(1.0);

  hmean_sel->GetXaxis()->SetTitle("time(day)");
  hmean_sel->GetYaxis()->SetTitle("Q6/Q42 mean (Q6>0)");	
  cmean_sel->cd();
  hmean_sel->Draw("P");

cmean->Update();

 
  hcnt_tot->SetMarkerStyle(2);



  hcnt_tot->GetXaxis()->SetTitle("time(day)");
  hcnt_tot->GetYaxis()->SetTitle("Q6/Q42 hit");	
  ccnt->cd();
  hcnt_tot->Draw("P");

 
  
  TFile * toutfile = new TFile("./plot/ratio_24hr.root","RECREATE");
  hmean_tot->Write();
  hmean_sel->Write();
  hcnt_tot->Write();
  toutfile->Close();

}
*/
