//
// Created by cupsoft on 12/2/20.
//
#include <iostream>

#include "TSocket.h"
#include "TString.h"

#include "CupConstants/onlconsts.hh"
#include "DAQUtils/onlutils.hh"

using namespace std;

int main(int argc, char ** argv)
{
  auto * socket = new TSocket("localhost", 9090);

  char message[kMESSLEN];

  TString a(argv[1]);
  unsigned long cmd = a.Atoi();

  encode_message(message, 0, 0, cmd, 0);
  socket->SendRaw(message, kMESSLEN);

  socket->Close();
  delete socket;

  return 0;
}