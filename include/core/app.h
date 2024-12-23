#ifndef _KNXP_APP_H
#define _KNXP_APP_H

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoOTA.h>
#endif

#ifdef ESP32
#include <WiFi.h>
#include <ESPmDNS.h>
#include <HTTPClient.h>
#include <ArduinoOTA.h>
#endif

#include <EEPROM.h>
#include <Stream.h>

#ifdef KNXP_FEATURE_WEB
#include "web/knxp_web_server_base.h"
#endif

#ifdef KNXP_FEATURE_WEBS
#include "web/knxp_websocket_server_base.h"
#endif

// Forward declarations
extern Stream* console;  // Single stream for both input and output
extern void knxYield();
extern const char* timeNowString();
extern const char* uptime();
extern bool isNetworkReady();
extern void submenuline(char mode, int base);

extern unsigned long _cyclicKnxTimer_interval;

class _knxapp {
  public:
    _knxapp();
    virtual ~_knxapp();

    virtual void pinsetup();
    virtual void conf();
    virtual void setup();
    virtual void knx_setup() = 0;
    virtual void loop();
    void cyclic();
    virtual char *hostname();
    virtual void status();
    virtual void handleWebSocketMessage(uint8_t num, uint8_t* payload, size_t length) = 0;

    void setCyclicTimer(unsigned long interval);
    void setGroupObjectCount(int count) { _groupObjectCount = count; }
    void menu();

  public:
    void setWebServer(KNXWebServerBase* server) {
#ifdef KNXP_FEATURE_WEB
        webServer = server;
#endif
    }

    void setWebSocketServer(KNXWebSocketServerBase* server) {
#ifdef KNXP_FEATURE_WEBS
        webSocketServer = server;
#endif
    }

  protected:
#ifdef KNXP_FEATURE_WEB
    KNXWebServerBase* webServer;
#endif

#ifdef KNXP_FEATURE_WEBS
    KNXWebSocketServerBase* webSocketServer;
#endif

  private:
    void help();
    void dumpParameter(int i);
    void dumpGroupObject(int i);
    void dumpEEPROM();
    void esp_status();

    int _groupObjectCount = 0;
};

#endif
