R__LOAD_LIBRARY(libHist)
//R__LOAD_LIBRARY(libRawObjs)
R__LOAD_LIBRARY(/home/cupsoft/muon_3.1.2/shlib/Linux5.14-GCC_11_3/libMuonObjs.so)

//#include <stdio.h>
//#include "/home/kkw/muon_3.1.2/MuonObjs/MuonObjs/WCPmt.hh"
//#include <TText.h>

void draw_occup_wcmd()
{
  gStyle -> SetOptStat(0);

  // get current run number, subrun number
  //327 sub 282, 330 sub 696


  //trg rate per hour
  int div = 24;
  int subrun[2] = {282, 672};
  const int n1 = subrun[0]/div;
  const int n2 = subrun[1]/div;
  double tent1, tent2, itime1, itime2, ftime1, ftime2;
  double rate1[n1], rate2[n2];
  double nevt1[n1], nevt2[n2];
  
  TChain * pchain1 = new TChain("wcmd");
  TChain * pchain2 = new TChain("wcmd");
 
  pchain1 -> Add("/home/kkw/Works/paper/prod_2nd/wcmd_000327.root");
  pchain2 -> Add("/home/kkw/Works/paper/prod_2nd/wcmd_000330.root");
  
  tent1 = pchain1->GetEntries();
  tent2 = pchain2->GetEntries();
 
  //run327 daq 1,2
  pchain1->GetEntry(0);
  itime1= (pchain1->GetLeaf("trgtime")->GetValue())/1000000000.;
  pchain1->GetEntry(tent1-1);
  ftime1= (pchain1->GetLeaf("trgtime")->GetValue())/1000000000.;
 
  //run330 daq 1,2
  pchain2->GetEntry(0);
  itime2= (pchain2->GetLeaf("trgtime")->GetValue())/1000000000.;
  pchain2->GetEntry(tent2-1);
  ftime2= (pchain2->GetLeaf("trgtime")->GetValue())/1000000000.;

  //get 1 day entry

  double timesec;
  double time_hr1[n1], time_hr2[n2], time_div1[n1], time_div2[n2], irate1[n1], irate2[n2], crate1[n1], crate2[n2], ent_hr1[n1][5], ent_hr2[n2][5], iocc1[n1][5], iocc2[n2][5], cocc1[n1][5], cocc2[n2][5];
  int hr_cnt;
  int ent_div1[n1][5], ent_div2[n2][5], ent_cdiv1[n1][5], ent_cdiv2[n2][5];
  int pmtnum[5]={25,4,22,28,46};
  //run327

  cout<<"run 327"<<endl;
  
  hr_cnt=1; 
  for(int i=0; i<tent1; i++){
    pchain1->GetEntry(i);
    timesec=(pchain1->GetLeaf("trgtime")->GetValue())/1000000000.;
    
    if(timesec>(hr_cnt*3600*div)){
      //      ent_day1[hr_cnt-1]=i;
      ent_div1[hr_cnt-1][0]=pchain1->GetEntries(Form("trgtime<%f&&ch==%i",timesec*1000000000.,pmtnum[0]));
      ent_cdiv1[hr_cnt-1][0]=pchain1->GetEntries(Form("trgtime<%f&&ch==%i&&(0.8*qsum-2100>qmax)&&qsum>4500",timesec*1000000000.,pmtnum[0]));

      time_div1[hr_cnt-1]=timesec/((double)div*3600); //1 day

      if(hr_cnt==1){
	iocc1[hr_cnt-1][0]=ent_div1[hr_cnt-1][0];
	cocc1[hr_cnt-1][0]=ent_cdiv1[hr_cnt-1][0];
      }
      else{
	iocc1[hr_cnt-1][0]=(ent_div1[hr_cnt-1][0]-ent_div1[hr_cnt-2][0]);
      	cocc1[hr_cnt-1][0]=(ent_cdiv1[hr_cnt-1][0]-ent_cdiv1[hr_cnt-2][0]);
      }

      cout<<time_div1[hr_cnt-1]<< " day : pmt no."<<pmtnum[0]<<" "<<ent_div1[hr_cnt-1][0]<<" evts"<<endl;
      cout<<"occ : "<<iocc1[hr_cnt-1][0]<<"(w/ cut "<<cocc1[hr_cnt-1][0]<<")"<<endl;

      hr_cnt++;
      if(hr_cnt > n1) break;
    }
  }
  
  //run330

  hr_cnt=1; 
  for(int i=0; i<tent2; i++){
    pchain2->GetEntry(i);
    timesec=(pchain2->GetLeaf("trgtime")->GetValue())/1000000000.;
    
    if(timesec>(hr_cnt*3600*div)){
     //      ent_day1[hr_cnt-1]=i;
      ent_div2[hr_cnt-1][0]=pchain2->GetEntries(Form("trgtime<%f&&ch==%i",timesec*1000000000.,pmtnum[0]));
      ent_cdiv2[hr_cnt-1][0]=pchain2->GetEntries(Form("trgtime<%f&&ch==%i&&(0.8*qsum-2100>qmax)&&qsum>4500",timesec*1000000000.,pmtnum[0]));

      time_div2[hr_cnt-1]=timesec/((double)div*3600); //1 day
      
      if(hr_cnt==1){
	iocc2[hr_cnt-1][0]=ent_div2[hr_cnt-1][0];
      	cocc2[hr_cnt-1][0]=ent_cdiv2[hr_cnt-1][0];
      }
      else{
	iocc2[hr_cnt-1][0]=(ent_div2[hr_cnt-1][0]-ent_div2[hr_cnt-2][0]);
      	cocc2[hr_cnt-1][0]=(ent_cdiv2[hr_cnt-1][0]-ent_cdiv2[hr_cnt-2][0]);
      }

      cout<<time_div2[hr_cnt-1]<< " day : pmt no."<<pmtnum[0]<<" / "<<ent_div2[hr_cnt-1][0]<<" evts"<<endl;
      cout<<"occ : "<<iocc2[hr_cnt-1][0]<<"(w/ cut "<<cocc2[hr_cnt-1][0]<<")"<<endl;

      hr_cnt++;
      if(hr_cnt > n2) break; 
	    
    }
  }
  
    // ===== graph arrays =====
  //  const int n1 = 12;
  //  const int n2 = 30;
  const int ntot = n1 + n2;

  double x[ntot], ex[ntot];
  double y_ir[ntot], ey_ir[ntot];
  double y_cr[ntot], ey_cr[ntot];

  double day_offset = 0.0;

  // run327
  for(int i=0; i<n1; i++){
    x[i] = time_div1[i];
    ex[i] = 0.0;

    y_ir[i] = iocc1[i][0];
    y_cr[i] = cocc1[i][0];

    double Ni, dt, cNi;

    if(i==0){
      Ni   = ent_div1[0][0];
      dt   = time_div1[0];
      cNi = ent_cdiv1[0][0];
    }
    else{
      Ni   = ent_div1[i][0] - ent_div1[i-1][0];
      cNi = ent_cdiv1[i][0] - ent_cdiv1[i-1][0];
      dt   = time_div1[i] - time_div1[i-1];
    }

    //    ey_ir[i] = sqrt(Ni)/dt;
    //    ey_cr[i] = sqrt(Ntot)/ttot;
    ey_ir[i] = sqrt(Ni);
    ey_cr[i] = sqrt(cNi);
  }

  day_offset = time_div1[n1-1];

  // run330
  for(int i=0; i<n2; i++){
    int k = n1 + i;

    x[k] = day_offset + time_div2[i];
    ex[k] = 0.0;

    y_ir[k] = iocc2[i][0];
    //  y_cr[k] = cocc2[i][0];

    double Ni, dt, cNi;

    if(i==0){
      Ni   = ent_div2[0][0];
      cNi   = ent_cdiv2[0][0];
      dt   = time_div2[0];
    }
    else{
      Ni   = ent_div2[i][0] - ent_div2[i-1][0];
      cNi   = ent_cdiv2[i][0] - ent_cdiv2[i-1][0];
      dt   = time_div2[i] - time_div2[i-1];
    }

    //    ey_ir[k] = sqrt(Ni)/dt;
    //    ey_cr[k] = sqrt(Ntot)/ttot;
    ey_ir[k] = sqrt(Ni);
    ey_cr[k] = sqrt(cNi);
  }

  // ===== draw =====
  TCanvas *c1 = new TCanvas("c1","rate",900,600);

  TGraphErrors *g_ir = new TGraphErrors(ntot, x, y_ir, ex, ey_ir);
  TGraphErrors *g_cr = new TGraphErrors(ntot, x, y_cr, ex, ey_cr);

  g_ir->SetTitle(";Day;PMT response(counts/day)");
  g_ir->SetMarkerStyle(20);
  g_ir->SetMarkerSize(0.7);
  g_ir->SetMarkerColor(kBlue);
  g_ir->SetLineColor(kBlue);
  g_ir->SetLineWidth(2);
  
  g_cr->SetMarkerStyle(20);
  g_cr->SetMarkerSize(0.7);
  g_cr->SetMarkerColor(kRed);
  g_cr->SetLineColor(kRed);
  g_cr->SetLineWidth(2);
  
  gStyle->SetEndErrorSize(4);
  g_ir->Draw("APE");
  g_cr->Draw("PE SAME");

  g_ir->GetYaxis()->SetRangeUser(0.0, 1.2*TMath::MaxElement(ntot, y_ir) > 1.2*TMath::MaxElement(ntot, y_cr) ? 1.2*TMath::MaxElement(ntot, y_ir) : 1.2*TMath::MaxElement(ntot, y_cr));
  //g_ir->GetYaxis()->SetRangeUser(0.0, 1.2*TMath::MaxElement(ntot, y_ir));
  TLegend *leg = new TLegend(0.65,0.75,0.88,0.88);
  leg->AddEntry(g_ir,"all trigger events","lp");
  leg->AddEntry(g_cr,"Muon","lp");
  leg->Draw();

  c1->Update();
  

}
