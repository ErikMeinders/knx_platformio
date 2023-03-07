#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <knxp_espcompat.h>
#include <WiFiManager.h>

#include <TelnetStream.h>  
     
void startWiFi(const char *Hostname);
#ifndef NO_TELNET
void startTelnet();
#endif
void startMDNS(const char *Hostname);
#ifndef NO_HTTP
extern WebServer        httpServer;
#endif

#endif
