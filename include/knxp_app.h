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

#ifdef FEATURE_WEB
#include "knxp_web_base.h"
#endif

#ifdef FEATURE_WEBS
#include "knxp_webs_base.h"
#endif

// Forward declarations
extern Stream* stdIn;
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

    void setCyclicTimer(unsigned long interval);
    void setGroupObjectCount(int count) { _groupObjectCount = count; }
    void menu();

  protected:
#ifdef FEATURE_WEB
    KNXWebServerBase* webServer;
#endif

#ifdef FEATURE_WEBS
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
