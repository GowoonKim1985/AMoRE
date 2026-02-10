void ana_psmd_eff()
{

	//////const. check//////
	const int boxn = 2;
	const int chn = 4;	// ch number for each box.
	int bw = 100; // bin width
	int cutx = 500; // threshold
	double hv = 54;
	///////////////////////

	int tch = boxn * chn;
	int DetN = boxn;
	
	double corr[boxn][chn], corr_h[boxn][chn], corr_mean[boxn];
	double calFitH[boxn][chn], calFitX[boxn][chn];
	double totMC[boxn], totMinX[boxn], totMaxX[boxn], totFitX[boxn], totFitH[boxn];
	double sectime;

	//	TString rawname = "210810_eff_hv54.3_th250_10hr";
	//	TString rawname = "210823_effset2_box6box2_hv54.5_th300_10hr";

//		TString rawname = "210813_effcom_box3_hv54.3_th250_10hr";
		//	TString rawname = "210818_effset1_hv54.5_th350_10hr";
		//	TString rawname = "210819_effset1_box3_hv54.5_th300_10hr";
	
	TString rawfile = "./data/eff/prd_" + rawname + ".root";
	TString anafile = "./hist/eff/his_" + rawname + ".root";

	//set parameter
//	int cutx; 	// cut front part (x axis), threshold
	int fitrange[2];	// x value(rough cut for gamma),  get maxh of muon his.
	int maxrange[2];	// x value(rough cut for gamma),  get maxh of muon his.
	int maxcheck_range;	// +/- ? bin's y value check to find maximum height of gaussian
	int fit_width;
	int hist_range = 35000;
	double draw_range = 15000;

	
	//parameter setting for HV

	if(hv<55){
//	cutx = 200;
	maxrange[0] = 1500;
	maxrange[1] = 6000;
	maxcheck_range = 5;
	fit_width = 500;
	}
	else{
	cout<<""<<endl;
	cout<<"no parameter set for this HV"<<endl;
	cout<<""<<endl;
	return;
	}


	int histbin = hist_range/bw;	//bin numbers
//	int histbin = (hist_range-cutx)/bw;	//bin numbers
	//get numbers for calculation
	int maxh, maxb, minb, getmax, x1, x2, pmaxh, pmaxb, fminh, fminb;
	double maxx[boxn][4], minx[boxn][4], pmaxx, fminx, totN[boxn][4], minh[boxn][4];
	int maxcheck[2], mincheck[2];
	double fitpa[3];


	TF1 * fitfunc = new TF1("fitfunc", "gaus");
	TChain * t = new TChain("psmd");
	t->Add(rawfile.Data());
	/*
	TLeaf * evtnumleaf = chain->GetLeaf("evtnum");
	TLeaf * timeleaf = chain->GetLeaf("trgtime");
	TLeaf * chleaf = chain->GetLeaf("id");
	TLeaf * qleaf = chain->GetLeaf("iq");	
	*/
	TCanvas * can1[boxn];
	TCanvas * can2[boxn];

	TH1D * consthist[boxn][chn];
	TH1D * calhist[boxn][chn];
	TH1D * S1hist[boxn];	//bottom ch1,2
	TH1D * S2hist[boxn];	//top, ch1,2
	TH1D * MChist[boxn];
	TH2D * MC2d[boxn];

	for(int i1=0; i1<boxn; i1++){

	can1[i1] = new TCanvas(Form("D%ican1", i1), Form("D%i Scint channel(raw & cal)", i1), 1400, 800);
	can1[i1]->Divide(4,2);
	can2[i1] = new TCanvas(Form("D%ican2", i1), Form("D%i Charge histogram", i1), 1000, 800);
	can2[i1]->Divide(2,2);

		for (int i2 = 0; i2 < chn; i2++) {
			consthist[i1][i2] = new TH1D(Form("D%ihis%i", i1, i2), "", histbin, 0, hist_range);
			calhist[i1][i2] = new TH1D(Form("D%ical%d", i1, i2), "", histbin, 0, hist_range);
		}
		S1hist[i1] = new TH1D(Form("D%iBhist", i1),"", histbin*2, 0, hist_range*2);
		S2hist[i1] = new TH1D(Form("D%iThist", i1),"", histbin*2, 0, hist_range*2);
		MChist[i1] = new TH1D(Form("D%iMChist", i1),"", histbin*4, 0, hist_range*4);
		MC2d[i1] = new TH2D(Form("D%iMC2d", i1), "", histbin*2, 0, hist_range*2, histbin*2, 0, hist_range*2);

		MC2d[i1]->SetXTitle("BottomScint(1&2)");
		MC2d[i1]->SetYTitle("TopScint(3&4)");
	}
	
	TTree * result = new TTree("result","analysis result");
	result->Branch("DetN",&DetN,"TotDetN/I");
	result->Branch("time",&sectime,"TriggerTime/D");
	result->Branch("tmuon",totMC,Form("MuonCounts[%i]/D",boxn));
	result->Branch("tminx",totMinX,Form("MinXvalue[%i]/D",boxn));
	result->Branch("tmaxx",totMaxX,Form("MaxXvalue[%i]/D",boxn));
	result->Branch("tfitx",totFitX,Form("FitXvalue[%i]/D",boxn));
	result->Branch("tfith",totFitH,Form("FitHvalue[%i]/D",boxn));
	for(int i=0; i<boxn; i++){
		result->Branch(Form("D%iminx",i),minx[i],Form("Det%iminx[4]/D",i));
		result->Branch(Form("D%iminh",i),minh[i],Form("Det%iminh[4]/D",i));
		result->Branch(Form("D%imaxx",i),maxx[i],Form("Det%imaxx[4]/D",i));
		result->Branch(Form("D%ifitx",i),corr[i],Form("Det%ifitH[4]/D",i));
		result->Branch(Form("D%ifith",i),corr_h[i],Form("Det%ifitX[4]/D",i));
		result->Branch(Form("D%imuon",i),totN[i],Form("Det%imuon[4]/D",i));
	}

	double count[boxn][chn];
	int divbox = -1;
	int divch;
//before calibration. 4ch hist making (raw hist)

	int tent = t->GetEntries();
	
	for (int i = 0; i < tent; i++) {
	  //	for (int i = 0; i < 10; i++) {
		t->GetEntry(i);
		for (int j = 0; j < tch; j++) {
		  //			AChannel * ch = data->Get(j);
			divch = j%chn;
			if(divch==0){divbox=divbox+1;};	
			//			count[divbox][divch] = ch->GetCount();
			count[divbox][divch] = t->GetLeaf("iq")->GetValue(j);			
			//	cout<<"ch / q "<<j<<"/"<<count[divbox][divch]<<endl;
			consthist[divbox][divch]->Fill(count[divbox][divch]);
//			count[j] = ch->GetCount();
//			consthist[j]->Fill(count[j]);
			if(j==tch-1){divbox = -1;};
		}
	}

	//fitting muon part. @ raw hist
	for(int i1=0; i1<boxn; i1++){
		for (int i2 = 0; i2 < chn; i2++) {

			cout<<""<<endl;
			cout<<"[box "<<i1<<" ch "<<i2<<"]"<<endl;

			can1[i1]->cd(i2+1);
			consthist[i1][i2]->Draw();

			//get external part max h&x
			x1 = cutx;
			x2 = maxrange[0];
			consthist[i1][i2]->GetXaxis()->SetRangeUser(x1, x2);
			pmaxh=consthist[i1][i2]->GetMaximum();
			pmaxb=consthist[i1][i2]->GetMaximumBin();
			pmaxx=consthist[i1][i2]->GetBinCenter(pmaxb);


			//get maximum h & x
			x1 = maxrange[0];
			x2 = maxrange[1];

			while(1){

				consthist[i1][i2]->GetXaxis()->SetRangeUser(x1, x2);
				maxh=consthist[i1][i2]->GetMaximum();
				maxb=consthist[i1][i2]->GetMaximumBin();
				maxx[i1][i2]=consthist[i1][i2]->GetBinCenter(maxb);
//			cout<<maxx<<endl;
				maxcheck[0]=consthist[i1][i2]->GetBinContent(maxb-maxcheck_range);
				maxcheck[1]=consthist[i1][i2]->GetBinContent(maxb+maxcheck_range);

				if(maxcheck[0]!=0&&maxcheck[0]<maxh&&maxcheck[1]<maxh){break;}
				else if(x1>=x2){break;}
				else{x1=consthist[i1][i2]->GetXaxis()->GetBinCenter(maxb+maxcheck_range);
//								cout<<"x1 checked to "<<x1<<endl;
				}
			}
		maxx[i1][i2] = consthist[i1][i2]->GetXaxis()->GetBinCenter(maxb);
		

		//get minimum x
		x1 = pmaxx;
		x2 = maxx[i1][i2];
//cout<<pmaxx<<" " <<maxx<<endl;
		consthist[i1][i2]->GetXaxis()->SetRangeUser(x1, x2);
		minh[i1][i2]=consthist[i1][i2]->GetMinimum();
		minb=consthist[i1][i2]->GetMinimumBin();
		minx[i1][i2]=consthist[i1][i2]->GetXaxis()->GetBinCenter(minb);

		cout<<"minx in range: "<<minx[i1][i2]<<endl;
		cout<<"minh in range: "<<minh[i1][i2]<<endl;


		//fitting 1st, for finding max height
		fitrange[0]=maxx[i1][i2]-fit_width;
		fitrange[1]=maxx[i1][i2]+fit_width;

		consthist[i1][i2]->GetXaxis()->SetRangeUser(cutx, hist_range);
		fitfunc->SetRange(fitrange[0], fitrange[1]);

		consthist[i1][i2]->Fit("fitfunc","RQ0");
		fitpa[1]=fitfunc->GetParameter(1);

		//fitting 2nd
		fitrange[0]=fitpa[1]-fit_width;
		fitrange[1]=fitpa[1]+fit_width;
		fitfunc->SetRange(fitrange[0], fitrange[1]);
		consthist[i1][i2]->Fit("fitfunc","RQ+");
		fitpa[0]=fitfunc->GetParameter(0);
		fitpa[1]=fitfunc->GetParameter(1);
		fitpa[2]=fitfunc->GetParameter(2);
		
		cout<<"fit max h : "<<fitpa[0]<<endl;
		cout<<"fit max x : "<<fitpa[1]<<endl;
	
		corr[i1][i2]=fitpa[1];
		corr_h[i1][i2]=fitpa[0];	
		totN[i1][i2] = consthist[i1][i2]->Integral(minb,histbin);
		cout<<"Tot N (Muon part, for ch) : "<<totN[i1][i2]<<endl;
		}
	}


// calibrated hist & muon counter

	double recount[boxn][chn];
	double S1count[boxn], S2count[boxn], MCcount[boxn];
	divbox = -1;
	

	for (int i = 0; i < tent; i++) {
	  //	for (int i = 0; i < 10; i++) {	  
		t->GetEntry(i);

		for (int j = 0; j < tch; j++) {
			divch = j%chn;
			if(divch==0){divbox=divbox+1;};			

			//			AChannel * ch = data->Get(j);
			//			recount[divbox][divch] = ch->GetCount()*corr[0][0]/corr[divbox][divch];
			recount[divbox][divch] = (t->GetLeaf("iq")->GetValue(j))*corr[0][0]/corr[divbox][divch];			
//			recount[divbox][divch] = ch->GetCount()*corr[divbox][0]/corr[divbox][divch];
			calhist[divbox][divch]->Fill(recount[divbox][divch]);
			if(j==tch-1){divbox = -1;}
		}
		
		for(int k=0; k<boxn; k++){
		S1count[k] = recount[k][0] + recount[k][1];
		S2count[k] = recount[k][2] + recount[k][3];
		MCcount[k] = S1count[k]+S2count[k];
		//	cout<<"cnt : b "<<S1count[k]<<" / t "<<S2count[k]<<" / Tot "<<MCcount[k]<<endl;
		S1hist[k]->Fill(S1count[k]);
		S2hist[k]->Fill(S2count[k]);
		MChist[k]->Fill(MCcount[k]);
		MC2d[k]->Fill(S1count[k], S2count[k]);
		}
	}


	// get calibrated x values @ cal histogram
	cout<<""<<endl;
	for(int i1=0; i1<boxn; i1++){
		cout<<"<<Det. box "<<i1<<" calibration check (x @ muon maxH)>>"<<endl;
		for(int i2=0; i2<chn; i2++){

//			fitfunc->SetRange(corr[i1][0]-fit_width, corr[i1][0]+fit_width);
			fitfunc->SetRange(corr[0][0]-fit_width, corr[0][0]+fit_width);
			calhist[i1][i2]->Fit("fitfunc","RQ+");
			//calFitH[i]=fitfunc->GetParameter(0);
			calFitX[i1][i2]=fitfunc->GetParameter(1);
			cout<<"ch "<<i2<<" "<<calFitX[i1][i2]<<endl;
		}
	}

	
	// get tot MC counts(for each Det.) from final hist. 


	cout<<""<<endl;
	cout<<"<<Muon Counts result>>"<<endl;

	for(int i=0; i<boxn; i++){

		cout<<"Det. box "<<i<<endl;
		MChist[i]->GetXaxis()->SetRangeUser(maxrange[0]*4,corr[0][0]*4); // set range for finding minh		
//		MChist[i]->GetXaxis()->SetRangeUser(maxrange[0]*4,corr[i][0]*4); // set range for finding minh		

		fminh=MChist[i]->GetMinimum();
		fminb=MChist[i]->GetMinimumBin();
		fminx=MChist[i]->GetBinCenter(fminb);
		totMinX[i]=fminx;
		MChist[i]->GetXaxis()->SetRangeUser(corr[0][0]*4-fit_width*4, corr[0][0]*4+fit_width*4); // set range for finding maxh
		totMaxX[i]=MChist[i]->GetBinCenter(MChist[i]->GetMaximumBin());

		fitfunc->SetRange(totMaxX[i]-(fit_width*3), totMaxX[i]+(fit_width*3));
		MChist[i]->Fit("fitfunc","RQ0+");
		totFitH[i]=fitfunc->GetParameter(0);
		totFitX[i]=fitfunc->GetParameter(1);

		cout<<"fit min : "<<fminb<<endl;
		totMC[i] = MChist[i]->Integral(fminb,histbin*4);

		cout<<"muon tot N : "<<totMC[i]<<endl;

		cout<<""<<endl;
		MChist[i]->GetXaxis()->SetRangeUser(0, hist_range*4); // set range for finding minh		
	}



	int TotEvent = t->GetEntries();
	cout<<"tot Event : "<<TotEvent<<endl; 
	t->GetEntry(TotEvent-1);
	//	sectime = (info->GetTriggerTime())/1000000000.;
	sectime = (t->GetLeaf("trgtime")->GetValue())/1000000000.;	
	cout<<"tot time : "<<sectime<<" s ("<<sectime/60.<<" min)"<<endl;	

	printf("%.0f * %.0f * %.0f * %.0f * %.0f * %.0f * %.0f * %.0f * %.0f	\n",minh[0][0], minh[0][1], minh[0][2], minh[0][3], totN[0][0], totN[0][1], totN[0][2], totN[0][3], totMC[0]);


//////

	result->Fill();
        TFile *out = new TFile(anafile.Data(),"RECREATE");
	result->Write();
	for(int i1=0; i1<boxn; i1++){
		for(int i2=0; i2<chn; i2++){
		consthist[i1][i2]->GetXaxis()->SetRangeUser(0, hist_range);
		calhist[i1][i2]->GetXaxis()->SetRangeUser(0, hist_range);
		consthist[i1][i2]->Write();
		calhist[i1][i2]->Write();
		}

	S1hist[i1]->Write();
	S2hist[i1]->Write();
	MChist[i1]->Write();
	MC2d[i1]->Write();

	}

	for(int i1=0; i1<boxn; i1++){
		for(int i2=0; i2<chn; i2++){

			consthist[i1][i2]->GetXaxis()->SetRangeUser(cutx,draw_range);
			calhist[i1][i2]->GetXaxis()->SetRangeUser(cutx, draw_range);
			can1[i1]->cd(i2+1);
			consthist[i1][i2]->Draw();
			can1[i1]->cd(i2+4+1);
			calhist[i1][i2]->Draw();
		}

	S1hist[i1]->GetXaxis()->SetRangeUser(cutx*2, draw_range*2);
	S2hist[i1]->GetXaxis()->SetRangeUser(cutx*2, draw_range*2);
	MChist[i1]->GetXaxis()->SetRangeUser(cutx*4, draw_range*4);
	MC2d[i1]->GetXaxis()->SetRangeUser(cutx*2, draw_range*2);
	MC2d[i1]->GetYaxis()->SetRangeUser(cutx*2, draw_range*2);

	can2[i1]->cd(1);
	S1hist[i1]->Draw();
	can2[i1]->cd(2);
	S2hist[i1]->Draw();
	can2[i1]->cd(3);
	MChist[i1]->Draw();
	can2[i1]->cd(4);
	MC2d[i1]->Draw("colz");
	can1[i1]->Write();
	can2[i1]->Write();

	}


	

}
