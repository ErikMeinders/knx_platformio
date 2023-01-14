#ifndef NETWORK_H
#define NETWORK_H

#include <Arduino.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>   
#include <TelnetStream.h>       
#include <Debug.h>

// #include "ESP32ModUpdateServer.h"

void startWiFi(const char *Hostname);
void startTelnet();
void startMDNS(const char *Hostname);

extern WebServer        httpServer;

// extern ESP32HTTPUpdateServer httpUpdater;

#endif
