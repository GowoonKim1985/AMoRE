/*
 *
 *  Module:  tcb
 *
 *  Author:  Jaison Lee
 *
 *  Purpose: tcb program (rc version)
 *
 *  Last Update:      $Author: cupsoft $
 *  Update Date:      $Date: 2022/12/21 00:02:44 $
 *  CVS/RCS Revision: $Revision: 1.12 $
 *  Status:           $State: Exp $
 *
 */
#include "TString.h"

#include "OnlConsts/adcconsts.hh"
#include "DAQ/daqopt.hh"
#include "DAQ/CupDAQManager.hh"

using namespace std;

int main(int argc, char ** argv)
{
  if (argc < 2) {
    return 0;
  }

  daqopt option;
  option.init();
  
  optparse(option, argc, argv);

  // for TCB controlled ADC


  auto * DAQ = new CupDAQManager();
  DAQ->SetRunNumber(option.runnum);
  DAQ->SetDAQType(DAQ::TCB);

  for (auto adc : option.adctype) {
    ADC::TYPE adctype = static_cast<ADC::TYPE>(static_cast<int>(adc) + 10);
    DAQ->AddDAQ(adctype);
  }
  DAQ->SetConfigFilename(option.config);
  DAQ->SetDAQTime(option.daqtime);
  DAQ->SetNEvent(option.daqevent);
  DAQ->SetOutputSplitTime(option.sptime);
  DAQ->SetVerboseLevel(option.vlevel);

  DAQ->Run();

  delete DAQ;
  return 0;
}

/**
$Log: tcb.cc,v $
Revision 1.12  2022/12/21 00:02:44  cupsoft
*** empty log message ***

Revision 1.12  2020/07/10 02:37:32  cupsoft
*** empty log message ***

Revision 1.11  2020/01/09 04:56:56  cupsoft
*** empty log message ***

Revision 1.10  2019/11/27 23:58:35  cupsoft
*** empty log message ***

Revision 1.9  2019/07/24 06:14:57  cupsoft
*** empty log message ***

Revision 1.8  2019/07/05 00:25:04  cupsoft
*** empty log message ***

**/