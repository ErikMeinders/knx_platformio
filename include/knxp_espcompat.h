#ifndef _KNXP_ESPCOMPAT_H
#define _KNXP_ESPCOMPAT_H

#ifdef ESP32

    #include "rom/rtc.h"
    #include <WebServer.h>
    #include <ESPmDNS.h>          
    #include <HTTPClient.h>

    #define LED_BUILTIN_ON HIGH
    #define LED_BUILTIN_OFF LOW
#ifndef PROGMODE_PIN
    #define PROGMODE_PIN 27
#endif

#endif

#ifdef ESP8266

    #include <ESP8266WiFi.h>
    #include "ESP8266mDNS.h"
    #include <ESP8266HTTPClient.h>

    #define WebServer ESP8266WebServer

    #define LED_BUILTIN_ON LOW
    #define LED_BUILTIN_OFF HIGH
#ifndef PROGMODE_PIN
    #define PROGMODE_PIN 27         // probably not!
#endif

#endif
#endif