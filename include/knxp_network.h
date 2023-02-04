#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <knxp_espcompat.h>
#include <WiFiManager.h>

#include <TelnetStream.h>  
     
void startWiFi(const char *Hostname);
void startTelnet();
void startMDNS(const char *Hostname);

extern WebServer        httpServer;

#endif
