#ifndef KNXP_NETWORK_ASYNC_H
#define KNXP_NETWORK_ASYNC_H

#include <Arduino.h>
#include <ArduinoLog.h>
#include "utils/knxp_espcompat.h"

#ifdef ESP8266
    #include <ESPAsyncTCP.h>
#else
    #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>
#include <DNSServer.h>

/**
 * Initialize network services with async support
 * Returns:
 *  0 = Success
 * -1 = WiFi connection failed
 * -2 = mDNS setup failed
 */
int initializeAsyncNetworkServices(const char* hostname);

/**
 * Start mDNS service
 */
bool startAsyncMDNS(const char* hostname);

/**
 * Network loop handler for async operations
 */
void asyncNetworkLoop();

#endif // KNXP_NETWORK_ASYNC_H
