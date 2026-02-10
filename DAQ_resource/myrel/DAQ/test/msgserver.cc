//
// Created by cupsoft on 12/2/20.
//
#include <iostream>
#include "TServerSocket.h"
#include "TSocket.h"
#include "TMonitor.h"
#include "TInetAddress.h"

#include "CupConstants/onlconsts.hh"
#include "DAQUtils/onlutils.hh"

using namespace std;

int main(int argc, char ** argv)
{
  auto * server = new TServerSocket(9090, true);
  auto * monitor = new TMonitor();
  monitor->Add(server);

  char message[kMESSLEN];
  unsigned short from, to;
  unsigned long mess1, mess2;
  uintptr_t foo = 0xffffffffffffffff;

  while (true) {
    TSocket * socket = monitor->Select(10000);
    if ((uintptr_t)socket == foo) {
      cout << "timeout" << endl;
      continue;
    }

    if (socket->IsValid()) {
      if (socket->IsA() == TServerSocket::Class()) {
        TSocket * s = ((TServerSocket *)socket)->Accept();
        TInetAddress addr = s->GetInetAddress();
        cout << Form("new client connected [ip=%s, port=%d]", addr.GetHostAddress(), addr.GetPort()) << endl;
        monitor->Add(s);
      }
      else {
        int stat = socket->RecvRaw(message, kMESSLEN);
        cout << stat << endl;
        if (stat < 0) {
          TInetAddress addr = socket->GetInetAddress();

          cout << Form("received error (%d) [ip=%s, port=%d]",
                       stat, addr.GetHostAddress(), addr.GetPort()) << endl;

          socket->Close();
          delete socket;
          monitor->Remove(socket);
        }
        if (stat == 0) {
          TInetAddress addr = socket->GetInetAddress();
          cout << Form("client disconnected [ip=%s, port=%d]", addr.GetHostAddress(), addr.GetPort()) << endl;
          socket->Close();
          delete socket;
          monitor->Remove(socket);
        }
        else {
          decode_message(message, from, to, mess1, mess2);
          cout << "mess1: " << mess1 << endl;
        }
      }
    }
  }








  return 0;
}