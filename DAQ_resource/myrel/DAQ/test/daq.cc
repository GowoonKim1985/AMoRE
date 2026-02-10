/*
 *
 *  Module:  daq
 *
 *  Author:  Jaison Lee
 *
 *  Purpose: CUP DAQ program for FADC/AMOREADC/SADC with run control
 *
 *  Last Update:      $Author: cupsoft $
 *  Update Date:      $Date: 2022/12/21 00:02:44 $
 *  CVS/RCS Revision: $Revision: 1.13 $
 *  Status:           $State: Exp $
 *
 */
#include <iostream>
#include <getopt.h>
#include <cstdio>
#include <sys/stat.h>
#include <cstdlib>
#include <unistd.h>
#include <signal.h>

#include "TString.h"

#include "CupConstants/adcconsts.hh"
#include "DAQTrigger/CupSoftTrigger.hh"
#include "DAQ/CupDAQManager.hh"

using namespace std;

TString gDIRNAME;
CupDAQManager * gDAQ = nullptr;

void (*breakCapture)(int);
void (*killCapture)(int);
void signalingHandler(int signo)
{
  gSystem->Exec(Form("%s/destroyshm", gDIRNAME.Data()));
  exit(1);
}

void PrintHelp()
{
  cout << "CUP DAQ program for FADC500/FADC125/SADC with run control" << endl;
  cout << endl;
  cout << "Usage: daq [OPTION] [ARG] ..." << endl;
  cout << "Example: daq [-f/g/s] -o output.root" << endl;
  cout << endl;
  cout << "options:" << endl;
  cout << "  -f     FADC500" << endl;
  cout << "  -g     FADC125" << endl;
  cout << "  -s     SADC" << endl;
  cout << "  -r     [run number]" << endl;
  cout << "  -o     [output file]" << endl;
  cout << "  -h     [histogramer file for monitoring]" << endl;
  cout << "  -v     [reporting time]" << endl;
  cout << "  -d     [verbose level]" << endl;
}

int main(int argc, char ** argv)
{
  if (argc < 2) {
    PrintHelp();
    return 0;
  }

  ADC::TYPE adctype = ADC::FADCT;
  TString outputfilename;
  TString histfilename;
  int runnum = 0;
  int reporttime = 10;
  int verboselevel = 0;

  bool dohist = false;

  int opt;
  while ((opt = getopt(argc, argv, "c:o:t:n:p:r:v:hfgs")) != EOF) {
    switch (opt) {
      case 'f':adctype = ADC::FADCT;
        break;
      case 'g':adctype = ADC::GADCT;
        break;
      case 's':adctype = ADC::SADCT;
        break;
      case 'r': runnum = TString(optarg).Atoi();
        break;
      case 'o': outputfilename = optarg;
        break;
      case 'h': dohist = true;
        break;
      case 'p': reporttime = TString(optarg).Atoi();
        break;
      case 'v': verboselevel = TString(optarg).Atoi();
        break;
      default: PrintHelp();
        return -1;
    }
  }

  gDIRNAME = gSystem->DirName(argv[0]);

  setsid();
  umask(0);

  breakCapture = signal(SIGINT, signalingHandler);
  killCapture = signal(SIGKILL, signalingHandler);

  gDAQ = new CupDAQManager();
  gDAQ->SetRunNumber(runnum);
  gDAQ->SetDAQType(DAQ::TCBCTRL);
  gDAQ->SetADCType(adctype);
  gDAQ->SetTriggerMode(TRIGGER::GLOBAL);
  gDAQ->SetOutputFileName(outputfilename.Data());
  gDAQ->SetStatusReportTime(reporttime);
  gDAQ->SetVerboseLevel(verboselevel);
  gDAQ->EnableHistograming(dohist);

  auto * swtrigger = new CupSoftTrigger();
  swtrigger->SetVerboseLevel(verboselevel);
  gDAQ->SetSoftTrigger(swtrigger);


  gDAQ->Run();

  delete swtrigger;
  delete gDAQ;

  gSystem->Exec(Form("%s/destroyshm", gDIRNAME.Data()));

  return 0;
}

/**
$Log: daq.cc,v $
Revision 1.13  2022/12/21 00:02:44  cupsoft
*** empty log message ***

Revision 1.13  2020/07/10 02:37:31  cupsoft
*** empty log message ***

Revision 1.12  2019/07/24 06:14:57  cupsoft
*** empty log message ***

Revision 1.11  2019/07/11 08:21:35  cupsoft
*** empty log message ***

Revision 1.10  2019/07/05 00:25:04  cupsoft
*** empty log message ***

**/
