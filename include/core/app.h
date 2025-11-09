/**
 * @file app.h
 * @brief Main KNX Platform IO Application Header
 * 
 * This file contains the base application class for KNX PlatformIO projects.
 * It provides core functionality for KNX applications including setup, configuration,
 * and optional features like web server, websockets, and NeoPixel support.
 */

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

#ifdef KNXP_NEOPIXEL_PIN
#include <Adafruit_NeoPixel.h>
#endif

#ifdef KNXP_FEATURE_WEB
#include "web/knxp_web_server_base.h"
#endif

#ifdef KNXP_FEATURE_WEBS
#include "web/knxp_websocket_server_base.h"
#endif

// Forward declarations
/** @brief Console stream for input/output operations */
extern Stream* console;
/** @brief Yield function for KNX operations */
extern void knxYield();
/** @brief Get current time as string */
extern const char* timeNowString();
/** @brief Get system uptime as string */
extern const char* uptime();
/** @brief Check if network is ready */
extern bool isNetworkReady();
/** @brief Display submenu line */
extern void submenuline(char mode, int base);

/** @brief Interval for cyclic KNX timer */
extern unsigned long _cyclicKnxTimer_interval;

/**
 * @brief Base class for KNX PlatformIO applications
 * 
 * This class provides the foundation for building KNX applications on ESP8266/ESP32 platforms.
 * It handles core functionality like pin setup, configuration, and optional features such as
 * web server, websockets, and NeoPixel support.
 */
class _knxapp {
  public:
    /** @brief Constructor */
    _knxapp();
    /** @brief Virtual destructor */
    virtual ~_knxapp();

    /**
     * @brief Setup GPIO pins
     * 
     * Configures pins for LED, programming button, heartbeat, and NeoPixel if enabled.
     * Can be overridden by derived classes for custom pin configuration.
     */
    virtual void pinsetup();

    /**
     * @brief Configure application settings
     * 
     * Virtual method for application-specific configuration.
     * Should be implemented by derived classes if needed.
     */
    virtual void conf();

    /**
     * @brief General setup routine
     * 
     * Called during initialization. Can be overridden for custom setup logic.
     */
    virtual void setup();

    /**
     * @brief KNX-specific setup
     * 
     * Pure virtual method that must be implemented by derived classes
     * to setup KNX-specific functionality.
     */
    virtual void knx_setup() = 0;

    /**
     * @brief Main loop routine
     * 
     * Called repeatedly. Can be overridden for custom loop logic.
     */
    virtual void loop();

    /**
     * @brief Cyclic timer handler
     * 
     * Handles periodic tasks based on the cyclic timer interval.
     */
    void cyclic();

    /**
     * @brief Get device hostname
     * @return Pointer to hostname string
     */
    virtual char *hostname();

    /**
     * @brief Display device status
     * 
     * Can be overridden to show custom status information.
     */
    virtual void status();

    /**
     * @brief Handle incoming WebSocket messages
     * @param num Client number
     * @param payload Message payload
     * @param length Payload length
     * 
     * Pure virtual method that must be implemented by derived classes
     * to handle WebSocket communication.
     */
    virtual void handleWebSocketMessage(uint8_t num, uint8_t* payload, size_t length) = 0;

    /**
     * @brief Set cyclic timer interval
     * @param interval Timer interval in milliseconds
     */
    void setCyclicTimer(unsigned long interval);

    /**
     * @brief Set number of group objects
     * @param count Number of group objects
     */
    void setGroupObjectCount(int count) { _groupObjectCount = count; }

    /**
     * @brief Display menu interface
     */
    void menu();

  public:
#ifdef KNXP_FEATURE_WEB
    /**
     * @brief Set web server instance
     * @param server Pointer to web server object
     */
    void setWebServer(KNXWebServerBase* server) {
        webServer = server;
    }
#endif

#ifdef KNXP_FEATURE_WEBS
    /**
     * @brief Set WebSocket server instance
     * @param server Pointer to WebSocket server object
     */
    void setWebSocketServer(KNXWebSocketServerBase* server) {
        webSocketServer = server;
    }
#endif

  protected:
#ifdef KNXP_FEATURE_WEB
    /** @brief Web server instance */
    KNXWebServerBase* webServer;
#endif

#ifdef KNXP_FEATURE_WEBS
    /** @brief WebSocket server instance */
    KNXWebSocketServerBase* webSocketServer;
#endif

  private:
    /** @brief Pointer to current application instance */
    static _knxapp* currentInstance;

  public:
    /**
     * @brief Set current application instance
     * @param instance Pointer to application instance
     */
    static void setCurrentInstance(_knxapp* instance) { currentInstance = instance; }

    /**
     * @brief Set NeoPixel LED color
     * @param r Red component (0-255)
     * @param g Green component (0-255)
     * @param b Blue component (0-255)
     */
    void setNeoPixelColor(uint8_t r, uint8_t g, uint8_t b) {
#ifdef KNXP_NEOPIXEL_PIN
        pixels.setPixelColor(0, pixels.Color(r, g, b));
        pixels.show();
#endif
    }

    /**
     * @brief Turn off programming LED
     * 
     * Static callback for KNX programming mode LED off
     */
    static void progLedOff() {
#ifdef KNXP_NEOPIXEL_PIN
        if (currentInstance) {
            currentInstance->setNeoPixelColor(0, 0, 0);
        }
#endif
    }
    
    /**
     * @brief Turn on programming LED
     * 
     * Static callback for KNX programming mode LED on
     */
    static void progLedOn() {
#ifdef KNXP_NEOPIXEL_PIN
        if (currentInstance) {
            currentInstance->setNeoPixelColor(0, 0, 128);
        }
#endif
    }

  private:
    /**
     * @brief Display help menu
     */
    void help();

    /**
     * @brief Dump parameter information
     * @param i Parameter index
     */
    void dumpParameter(int i);

    /**
     * @brief Dump group object information
     * @param i Group object index
     */
    void dumpGroupObject(int i);

    /**
     * @brief Dump EEPROM contents
     */
    void dumpEEPROM();

    /**
     * @brief Display ESP status information
     */
    void esp_status();

    /** @brief Number of KNX group objects */
    int _groupObjectCount = 0;

#ifdef KNXP_NEOPIXEL_PIN
    /** @brief NeoPixel LED instance */
    Adafruit_NeoPixel pixels{1, KNXP_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800};
#endif
};

#endif
