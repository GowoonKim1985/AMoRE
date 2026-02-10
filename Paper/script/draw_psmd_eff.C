void draw_psmd_eff()
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

	//D0 trg D1 det
	//	TString rawname = "210810_eff_hv54.3_th250_10hr"; // 0~3 det, 4~7 pan

	//		TString rawname = "210813_effcom_box3_hv54.3_th250_10hr";
	TString rawname = "210818_effset1_hv54.5_th350_10hr";//0~3 pan, 4~7 det
	//	TString rawname = "210819_effset1_box3_hv54.5_th300_10hr";
	
	TString rawfile = "./data/eff/prd_" + rawname + ".root";
	TString anafile = "./hist/eff/his2_" + rawname + ".root";

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

	//	TCanvas * can1 = new TCanvas("can1","trg pannel 2d plot",1100, 900);
	//	TCanvas * can2 = new TCanvas("can2","coincidence cut",1400, 900);
	//	TCanvas * can1 = new TCanvas("can1","trg pannel 2d plot",900, 900);
	//	TCanvas * can2 = new TCanvas("can2","coincidence cut",1400, 900);

	
	TH2D * pan_tb = new TH2D("pan_tb","",histbin*2, 0, hist_range*2, histbin*2, 0, hist_range*2);
	TH2D * det_tb = new TH2D("det_tb","",histbin*2, 0, hist_range*2, histbin*2, 0, hist_range*2);

	TH2D * pan_tbcal = new TH2D("pan_tbcal","",histbin*2, 0, hist_range*2, histbin*2, 0, hist_range*2);
	TH2D * det_tbcal = new TH2D("det_tbcal","",histbin*2, 0, hist_range*2, histbin*2, 0, hist_range*2);
	
	TH1D * det_qsum = new TH1D("det_qsum","",histbin*4,0,hist_range*4);
	TH1D * det_qsum_cut = new TH1D("det_qsum_cut","",histbin*4,0,hist_range*4);
	TH1D * det_qsum_sel = new TH1D("det_qsum_sel","",histbin*4,0,hist_range*4);		
	TH1D * det_qsum_cutsel = new TH1D("det_qsum_cutsel","",histbin*4,0,hist_range*4);		

	TH1D * pan_qsum = new TH1D("pan_qsum","",histbin*4,0,hist_range*4);
	TH1D * pan_qsum_cut = new TH1D("pan_qsum_cut","",histbin*4,0,hist_range*4);	
	TH1D * pan_t = new TH1D("pan_t","",histbin*2,0,hist_range*2);
	TH1D * pan_b = new TH1D("pan_b","",histbin*2,0,hist_range*2);	
	
	
	double count[8];
	double count_cal[8];
	int divbox = -1;
	int divch;

	double sel_t = 4780;
	double sel_b = 3922;
	
	int tent = t->GetEntries();
	int flag_p, flag_d;
	double qsum_p, qtop_p, qbot_p, qsum_d, qtop_d, qbot_d ;
	double qcal_p, qctop_p, qcbot_p,qcal_d, qctop_d, qcbot_d ;

	double tsel = 400;
	double bsel = 400;
	
	//trg pannel hist, 2d plot (all iq over 250) 210810 data ch4~7

	// det 0,1,2,3 / pan 4,5,6,7
	for(int i=0; i< tent; i++){
	  //	for(int i=0; i< 5; i++){
	  t->GetEntry(i);
	  
	  flag_p=0; flag_d=0;
	  qsum_p=0; qcal_p=0; qbot_p=0; qtop_p=0; qcbot_p=0; qctop_p=0;
	  qsum_d=0; qcal_d=0; qbot_d=0; qtop_d=0; qcbot_d=0; qctop_d=0;
	  
	  for(int j=0; j<=7; j++){
	    count[j]=t->GetLeaf("iq")->GetValue(j);
	    //	    count_cal[j]=count[j]*(mpv_ref/mpv[j]);
	  }
	  /* 250810 set
	  qsum_d = count[0]+count[1]+count[2]+count[3];
	  qbot_d = count[0]+count[1];
	  qtop_d = count[2]+count[3];	  
	  qcbot_d = count_cal[0]+count_cal[1];
	  qctop_d = count_cal[2]+count_cal[3];	  
	  
	  qsum_p = count[4]+count[5]+count[6]+count[7];
	  qbot_p = count[4]+count[5];
	  qtop_p = count[6]+count[7];	  
	  qcbot_p = count_cal[4]+count_cal[5];
	  qctop_p = count_cal[6]+count_cal[7];
	  */
	  qsum_p = count[0]+count[1]+count[2]+count[3];
	  qbot_p = count[0]+count[1];
	  qtop_p = count[2]+count[3];	  
	  qcbot_p = count_cal[0]+count_cal[1];
	  qctop_p = count_cal[2]+count_cal[3];	  
	  
	  qsum_d = count[4]+count[5]+count[6]+count[7];
	  qbot_d = count[4]+count[5];
	  qtop_d = count[6]+count[7];	  
	  qcbot_d = count_cal[4]+count_cal[5];
	  qctop_d = count_cal[6]+count_cal[7];

	  pan_t->Fill(qtop_p);
	  pan_b->Fill(qbot_p);	  
	  det_qsum->Fill(qsum_d);
	  pan_qsum->Fill(qsum_p);
	  pan_tb->Fill(qbot_p, qtop_p);	  
	  det_tb->Fill(qbot_d, qtop_d);

	  if(qbot_p>sel_b && qtop_p>sel_t){
	  det_qsum_sel->Fill(qsum_d);
	  }
	}

			   
	TFile *out = new TFile(anafile.Data(),"RECREATE");
	pan_tb->Write();
	//	pan_tbcal->Write();
	pan_t->Write();
	pan_b->Write();	
	pan_qsum->Write();
	//	pan_qsum_cut->Write();
	
	det_tb->Write();
	//	det_tbcal->Write();
	det_qsum->Write();
	det_qsum_sel->Write();
	//	det_qsum_cut->Write();
	//	det_qsum_cutsel->Write();

	

	
			   /*
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


	*/	

}
