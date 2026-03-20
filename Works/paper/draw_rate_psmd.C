#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <cmath>

#include <TCanvas.h>
#include <TGraphErrors.h>
#include <TAxis.h>
#include <TLegend.h>


struct StatLine{
  int Ninst;
  int Ntot;
  double rinst;
  double rcum;
  double ttot;
  std::string stamp;
};

void read_one_log(const char* fname,
                  double inst_rate[], double inst_err[],
                  double hour_rate[], double hour_err[],
                  int &nhour,
                  int &prev_Ntot, double &prev_Ttot,
                  bool &first)
{
  std::ifstream fin(fname);
  if(!fin.is_open()){
    std::cout<<"cannot open file : "<<fname<<std::endl;
    return;
  }

  std::string line;
  std::vector<StatLine> stat_buffer;

  while(std::getline(fin,line)){

    // STAT 줄 파싱
    if(line.find("[STAT]")!=std::string::npos){

      StatLine s;
      int tmp;

      int ok = sscanf(line.c_str(),
        "%*s %*s [STAT] %d events triggered [ %d | %d / %lf(%lf) Hz / %lf s]",
        &s.Ninst,&s.Ntot,&tmp,&s.rinst,&s.rcum,&s.ttot);

      if(ok==6){
        s.stamp=line.substr(0,19);
        stat_buffer.push_back(s);
      }
    }

    // split command 발견
    if(line.find("output file split command received")!=std::string::npos){

      if(stat_buffer.size()==0) continue;

      // 뒤에서부터 순간 rate ≠ 0 찾기
      StatLine use;
      bool found=false;

      for(int i=stat_buffer.size()-1;i>=0;i--){
        if(stat_buffer[i].rinst>0){
          use=stat_buffer[i];
          found=true;
          break;
        }
      }

      if(!found) continue;

      // 순간 rate
      inst_rate[nhour]=use.rinst;
      inst_err[nhour]=use.rinst/std::sqrt((double)use.Ninst);

      // 1시간 구간 rate
      if(first){
        hour_rate[nhour]=use.Ntot/use.ttot;
        hour_err[nhour]=hour_rate[nhour]/std::sqrt((double)use.Ntot);
        first=false;
      }
      else{
        int dN=use.Ntot-prev_Ntot;
        double dT=use.ttot-prev_Ttot;

        hour_rate[nhour]=dN/dT;
        hour_err[nhour]=hour_rate[nhour]/std::sqrt((double)dN);
      }
      /*
      std::cout<<nhour<<"  "<<use.stamp
               <<"  inst="<<inst_rate[nhour]
               <<" +- "<<inst_err[nhour]
               <<"  hour="<<hour_rate[nhour]
               <<" +- "<<hour_err[nhour]
               <<std::endl;
      */
      prev_Ntot=use.Ntot;
      prev_Ttot=use.ttot;

      nhour++;
    }
  }

  fin.close();
}

void draw_rate_psmd()
{
  const int MAXHOUR = 2000;

  double inst_rate[MAXHOUR];
  double inst_err[MAXHOUR];
  double hour_rate[MAXHOUR];
  double hour_err[MAXHOUR];

  int nhour = 0;

  int prev_Ntot = 0;
  double prev_Ttot = 0;

  bool first=true;

  // run 327
  read_one_log("./prod_2nd/iadc1_000327.log",
               inst_rate, inst_err,
               hour_rate, hour_err,
               nhour,
               prev_Ntot, prev_Ttot,
               first);



  // run 330 시작 전 리셋
  prev_Ntot = 0;
  prev_Ttot = 0;
  first = true;
  
  // run 330
  read_one_log("./prod_2nd/iadc1_000330.log",
               inst_rate, inst_err,
               hour_rate, hour_err,
               nhour,
               prev_Ntot, prev_Ttot,
               first);

  std::cout<<"\nTotal hours = "<<nhour<<std::endl;

  // ===== graph =====

  double x[MAXHOUR];
  double ex[MAXHOUR];

  for(int i=0;i<nhour;i++){
    x[i] = i;
    ex[i] = 0.0;
  }

  TCanvas *c1 = new TCanvas("c1","PSMD trigger rate",1300,900);

  TGraphErrors *g_inst = new TGraphErrors(nhour,x,inst_rate,ex,inst_err);
  TGraphErrors *g_hour = new TGraphErrors(nhour,x,hour_rate,ex,hour_err);



double ymax1 = TMath::MaxElement(nhour,inst_rate);
double ymax2 = TMath::MaxElement(nhour,hour_rate);
double ymax  = (ymax1 > ymax2) ? ymax1 : ymax2;

g_inst->GetYaxis()->SetRangeUser(0,1.2*ymax);
  g_inst->SetTitle(";Runtime [H];Trigger rate [Hz]");

g_inst->GetXaxis()->SetTitleFont(62);
g_inst->GetYaxis()->SetTitleFont(62);

g_inst->GetXaxis()->SetTitleSize(0.04);
g_inst->GetYaxis()->SetTitleSize(0.04);
g_inst->GetXaxis()->SetRangeUser(0,978);
  
  g_inst->SetMarkerStyle(20);
  g_inst->SetMarkerSize(0.8);
  g_inst->SetMarkerColor(kBlue);
  g_inst->SetLineColor(kBlue);

  g_inst->Draw("AP");






  
  g_hour->SetMarkerStyle(21);
  g_hour->SetMarkerSize(0.8);
  g_hour->SetMarkerColor(kRed);
  g_hour->SetLineColor(kRed);

  g_hour->Draw("P SAME");

  TLegend *leg = new TLegend(0.63,0.75,0.88,0.87);
  leg->AddEntry(g_inst,"Instant trigger rate","lp");
  leg->AddEntry(g_hour,"1-hour averaged rate","lp");
  leg->SetTextSize(0.03);
  leg->Draw();

  c1->Update();


}

