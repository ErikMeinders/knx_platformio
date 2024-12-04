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

// Web includes - must come before network includes to handle HTTP method definitions
#ifdef USE_ASYNC_WEB
    #include "knxp_web_async.h"
    #include "knxp_webs_async.h"
#else
    #ifdef FEATURE_WEB
        #include "knxp_web.h"
    #endif
    #ifdef FEATURE_WEBS
        #include "knxp_webs.h"
    #endif
#endif

// Network includes
#include "knxp_network.h"
#include "knxp_services.h"

// Console handling
extern Stream* console;

#ifndef NO_TELNET
    #include <TelnetStream.h>
    
    // Printf macros that use the console pointer
    #define Printf(...) console->printf(__VA_ARGS__)
    #define Print(...) console->print(__VA_ARGS__)
    #define Println(...) console->println(__VA_ARGS__)
#else
    // When telnet is disabled, always use Serial
    #define Printf(...) Serial.printf(__VA_ARGS__)
    #define Print(...) Serial.print(__VA_ARGS__)
    #define Println(...) Serial.println(__VA_ARGS__)
#endif

// Application layer
#include "knxp_app.h"

// Ensure KNX mask version is defined
#ifndef MASK_VERSION
#define MASK_VERSION 0x57B0
#endif

#endif // KNXP_PLATFORMIO_H
