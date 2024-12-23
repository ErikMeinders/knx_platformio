#include "knxp_platformio.h"
#include "utils/knxp_yield.h"
#define xstr(s) str(s)
#define str(s) #s

DECLARE_TIMER(BaseCodeShoutOut, 5);

_knxapp::_knxapp()
#if defined(KNXP_FEATURE_WEB) && defined(KNXP_FEATURE_WEBS)
    : webServer(nullptr), webSocketServer(nullptr)
#elif defined(KNXP_FEATURE_WEB)
    : webServer(nullptr)
#endif
{
}

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

#ifdef KNXP_NEOPIXEL_PIN
#include <Adafruit_NeoPixel.h>
// create a pixel strand with 1 pixel on PIN_NEOPIXEL
Adafruit_NeoPixel pixels(1, KNXP_NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);    

void progLedOff()
{
  pixels.clear();
  pixels.show();
}

void progLedOn()
{
  pixels.setPixelColor(0, pixels.Color(0, 0, 128));
  pixels.show();
}
#endif
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
    // pixels.begin();
    knx.setProgLedOffCallback(progLedOff);
    knx.setProgLedOnCallback(progLedOn);

    pixels.setPixelColor(0, pixels.Color(0, 0, 128));
    pixels.show();
    delay(500);
    pixels.setPixelColor(0, pixels.Color(0, 128,0 ));
    pixels.show();
    delay(500);
    pixels.setPixelColor(0, pixels.Color(128, 0, 0));
    pixels.show();
    delay(500);
    pixels.clear();
    pixels.show();
    Log.info("[platform] Neopixel setup done\n");
#endif
    // pin or GPIO programming button is connected to. Default is 0
#if defined(KNXP_PROG_PIN) && ! defined(KNXP_NO_KNX)
    Log.info("[platform] KNX Program Button pin set: %d\n", KNXP_PROG_PIN);
    pinMode(KNXP_PROG_PIN, INPUT_PULLUP);
    knx.buttonPin(KNXP_PROG_PIN);
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

char *_knxapp::hostname()
{
    return (char *) xstr(KNXP_HOSTNAME);
}

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
