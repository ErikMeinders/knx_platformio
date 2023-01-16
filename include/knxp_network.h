#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <knxp_espcompat.h>
#include <WiFiManager.h>

#include <TelnetStream.h>  
     
#include <knxp_debug.h>

// #include "ESP32ModUpdateServer.h"

void startWiFi(const char *Hostname);
void startTelnet();
void startMDNS(const char *Hostname);

extern WebServer        httpServer;

// extern ESP32HTTPUpdateServer httpUpdater;

#endif
