R__LOAD_LIBRARY(libHist)
R__LOAD_LIBRARY(libRawObjs)
//R__LOAD_LIBRARY(libMuonObjs)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
R__LOAD_LIBRARY(/home/kkw/muon_3.1.2/shlib/Linux5.14-GCC_11_4/libMuonObjs.so)
//R__LOAD_LIBRARY(/home/kmseo/Works/muon_3.1.2/test/libPSHit.so)



void wave_4ch()
{
  int runnum = 476;
  int subnum = 0;
  int selid=71; //1~47
  int selmax=50;
  int selmin=-10;


  //  TTree * t = (TTree*)f.Get("AbsEvent"); 
   TChain * chain = new TChain("AbsEvent");
   
   //  chain -> Add("/data/amore2test/RAW/000402/FADC_000402.root.00000");
   // chain -> Add(Form("/data/amore2test/RAW/%06d/FADC_%06d.root.%05d",runnum, runnum, subnum));
   //   chain -> Add(Form("/data/amore2test/RAW/%06d/FADC_%06d.root.*",runnum, runnum));

   if(selid>=0&&selid<=58){
   chain -> Add(Form("/data/amore2test/RAW/%06d/IADC1_%06d.root.00000",runnum, runnum));
   }
   else{
   chain -> Add(Form("/data/amore2test/RAW/%06d/IADC2_%06d.root.00000",runnum, runnum));
   }

   
  EventInfo * info = new EventInfo();
  FChannelData * sdata = new FChannelData();
  chain->SetBranchAddress("EventInfo", &info);
  chain->SetBranchAddress("FChannelData", &sdata);

  //  TCanvas * can = new TCanvas("can", "", 800, 600);
  //  TCanvas * can = new TCanvas("can", "", 800, 600);
  //  can->Divide(7,7);

  int nent = chain->GetEntries();
  int tch;
  int fid[48], fdet[48], max[48], charge[48], qtot[48], min[48];
  int select=0; int sel_max = 100;
  int evtnum;
  TCanvas * can = new TCanvas("can", "", 1200, 800);
  can->Divide(2,2);
  int flag=0;
  
  for(int i = 0; i < nent; i++){
    //  for(int i = 0; i < 10; i++){    


    chain->GetEntry(i);
    evtnum = info -> GetTriggerNumber();

    //  cout<<"test"<<endl; 
    tch=sdata->GetN();

    //            cout<<"tch : "<<tch<<endl;
    //   cout<<"entry no. : "<<i<<endl;

    for(int k=0; k<tch; k++){
	FChannel * ch = sdata->Get(k);	
	
	fid[k] = ch->GetID()-1; 
	fdet[k] = fid[k]/4;
	//	cout<<"ch/max/charge : "<<id[k]<<"/"<<max[k]<<"/"<<charge[k]<<endl;
	//	if(max[k]>sel_max){select=select+1;}
	//	if(id[k]==1){

	if(fdet[k]==selid){
	  flag=1;
	if(fid[k]%4==0){cout<<"evt "<<evtnum<<" / hit "<<k+1<<" / det no "<<fdet[k]<<endl;}
	//	max[k]=ch->GetWaveformHist()->GetMaximum();
	//	min[k]=ch->GetWaveformHist()->GetMinimum();	
	//	charge[k]=ch->GetWaveformHist()->Integral();

	can->cd((fid[k]%4)+1);
	  //	  ch->GetWaveformHist()->Draw();	  

	ch->GetWaveformHist()->Draw();

	  
	  
	  //	  hist->SetStats(true);

      //    delete can;

		if(fid[k]%4==3){
	  gPad->Update();
	  
	  can->Update();
	  can->Modified();
	  
	  	}
	}


    }
    if(flag==1){
	  flag=0;
      cout<<"input any key...(q:exit)"<<endl;
	  
	  char a;
	  cin >> a;
	  if (a == 'q') break;

    }
    	 
    /*    	
	  gPad->Update();
	  
	  can->Update();
	  can->Modified();

     cout<<"input any key...(q:exit)"<<endl;
      char a;
      cin >> a;
      if (a == 'q') break;
      }
	  */
      //      if(select==1){
      // }
      
  

  }
  
      //      }
     

      //      }
}

