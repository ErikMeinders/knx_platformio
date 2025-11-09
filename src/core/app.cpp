/**
 * @file app.cpp
 * @brief Implementation of the KNX PlatformIO Application base class
 */

#include "knxp_platformio.h"
#include "utils/knxp_yield.h"

/** @brief Helper macro to convert macro value to string */
#define xstr(s) str(s)
/** @brief Helper macro to stringify */
#define str(s) #s

/** @brief Timer for base code periodic operations */
DECLARE_TIMER(BaseCodeShoutOut, 5);

/** @brief Initialize static instance pointer */
_knxapp* _knxapp::currentInstance = nullptr;

/**
 * @brief Constructor
 * 
 * Initializes web server and websocket server pointers to nullptr if the features
 * are enabled through KNXP_FEATURE_WEB and KNXP_FEATURE_WEBS defines.
 */
_knxapp::_knxapp()
#if defined(KNXP_FEATURE_WEB) && defined(KNXP_FEATURE_WEBS)
    : webServer(nullptr), webSocketServer(nullptr)
#elif defined(KNXP_FEATURE_WEB)
    : webServer(nullptr)
#endif
{
}

/**
 * @brief Destructor
 * 
 * Cleans up web server and websocket server instances if they were created.
 */
_knxapp::~_knxapp() {
#ifdef KNXP_FEATURE_WEB
    if (webServer) {
        delete webServer;
    }
#endif
#ifdef KNXP_FEATURE_WEBS
    if (webSocketServer) {
        delete webSocketServer;
    }
#endif
}

/**
 * @brief Initialize GPIO pins and LED configurations
 * 
 * Sets up:
 * - Programming LED (built-in or NeoPixel)
 * - Programming button
 * - Heartbeat pin (if enabled)
 * 
 * For NeoPixel setup, performs an RGB test sequence.
 */
void _knxapp::pinsetup()
{
    // put your pin setup code here, to run once before platform setup:

    Log.info("[platform] pinsetup\n");
#ifndef KNXP_NEOPIXEL_PIN
    // pin or GPIO the programming led is connected to. Default is LED_BUILTIN
    knx.ledPin(LED_BUILTIN);

    // is the led active on HIGH or low? Default is LOW
    knx.ledPinActiveOn(LED_BUILTIN_ON);
#else
    Log.info("[platform] Neopixel pin: %d\n", KNXP_NEOPIXEL_PIN);
    setCurrentInstance(this);
    pixels.begin();
    knx.setProgLedOffCallback(progLedOff);
    knx.setProgLedOnCallback(progLedOn);

    setNeoPixelColor(0, 0, 128);
    delay(500);
    setNeoPixelColor(0, 128, 0);
    delay(500);
    setNeoPixelColor(128, 0, 0);
    delay(500);
    setNeoPixelColor(0, 0, 0);
    Log.info("[platform] Neopixel setup done\n");
#endif
    // pin or GPIO programming button is connected to. Default is 0
#if defined(KNXP_PROGMODE_PIN) && ! defined(KNXP_NO_KNX)
    Log.info("[platform] KNX Program Button pin set: %d\n", KNXP_PROGMODE_PIN);
    pinMode(KNXP_PROGMODE_PIN, INPUT_PULLUP);
    knx.buttonPin(KNXP_PROGMODE_PIN);
    Log.info("[platform] KNX Program Button pin get: %d\n", knx.buttonPin());
#else
    Log.info("[platform] KNX Program Button pin: NOT SET\n");
#endif

#if defined(KNXP_HEARTBEAT_PIN) && ! defined(KNXP_NO_HEARTBEAT)
    pinMode(KNXP_HEARTBEAT_PIN, OUTPUT);
    Log.info("[platform] Heartbeat pin: %d\n", KNXP_HEARTBEAT_PIN);
#else
    Log.info("[platform] Heartbeat pin: NOT SET\n");
#endif

  Log.info("[platform] pinsetup done\n");
}

/**
 * @brief Get device hostname
 * @return Hostname string from KNXP_HOSTNAME define
 */
char *_knxapp::hostname()
{
    return (char *) xstr(KNXP_HOSTNAME);
}

/**
 * @brief Display help menu
 * 
 * Prints available commands:
 * - EEPROM dump
 * - Parameter dump mode
 * - GroupObject dump mode
 * - Verbosity level control
 * - Programming mode toggle
 * - Status display
 * - Base selection for dumps
 * - Numeric commands for parameter/object dumps
 */
void _knxapp::help()
{
    Println("[E] EEPROM: dump the EEPROM content");
    Println("[P] Switch to Parameters dump mode");
    Println("[G] Switch to GroupObject dump mode");
    Println("[V] Switch to set Verbosity level mode");
    Println("[T] ToggleProgMode: toggle the programming mode");
    Println("[S] Status: dump the status of the KNX stack");
    Println("[abcz] Base: set the 10base for the dump [z=0 a=10 b=20 c=30]");
    Println("[0..9] Dump: dump the selected Parameter or Group object or set the Verbosity level");
    Println("[?] Help: print this help");
}
