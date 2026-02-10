#include "TString.h"
#include "TSocket.h"

#include "CupConstants/onlconsts.hh"
#include "DAQUtils/onlutils.hh"

int main(int argc, char ** argv)
{
  TString hostname = kFADCDAQIPADDR;
  int port = kFADCDAQPORT;

  TSocket * socket = new TSocket(hostname.Data(), port);

  char buffer[kMESSLEN];
  encode_message(buffer, 0, 0, kENDRUN);
  socket->SendRaw(buffer, kMESSLEN);

  socket->Close();
  delete socket;

  return 0;
}