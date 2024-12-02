#ifndef KNXP_PLATFORMIO_H
#define KNXP_PLATFORMIO_H

// Core includes
#include <Arduino.h>
#include <ArduinoLog.h>
#include <LittleFS.h>

// KNX includes
#include <knx.h>

// ESP compatibility layer
#include "knxp_espcompat.h"

// Utility includes
#include "knxp_timeinfo.h"
#include "knxp_progress.h"
#include "knxp_yield.h"

// Network includes
#include "knxp_network.h"
#include "knxp_services.h"

// Printf macros
#ifdef STDIO_TELNET
    #include <TelnetStream.h>
    #define Printf(...) TelnetStream.printf( __VA_ARGS__ )
    #define Print(...) TelnetStream.print( __VA_ARGS__ )
    #define Println(...) TelnetStream.println( __VA_ARGS__ )
#else
    #define Printf(...) Serial.printf( __VA_ARGS__ )
    #define Print(...) Serial.print( __VA_ARGS__ )
    #define Println(...) Serial.println( __VA_ARGS__ )
#endif

// Web includes - only when features are enabled
#ifdef FEATURE_WEB
    #ifdef USE_ASYNC_WEB
        #include "knxp_web_async.h"
    #else
        #include "knxp_web.h"
    #endif
#endif

#ifdef FEATURE_WEBS
    #ifdef USE_ASYNC_WEB
        #include "knxp_webs_async.h"
    #else
        #include "knxp_webs.h"
    #endif
#endif

// Application layer
#include "knxp_app.h"

// Ensure KNX mask version is defined
#ifndef MASK_VERSION
#define MASK_VERSION 0x57B0
#endif

// Create web server instance using factory function - only when feature is enabled
#ifdef FEATURE_WEB
    inline KNXWebServerBase* createAndInitWebServer() {
        auto server = createWebServer();
        if (server) {
            server->begin();
        }
        return server;
    }
#endif

#endif // KNXP_PLATFORMIO_H
