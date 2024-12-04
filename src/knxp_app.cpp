#include <knxp_platformio.h>
#include "knxp_yield.h"
#define xstr(s) str(s)
#define str(s) #s

DECLARE_TIMER(BaseCodeShoutOut, 5);

_knxapp::_knxapp()
#if defined(FEATURE_WEB) || defined(FEATURE_WEBS)
    : webServer(nullptr), webSocketServer(nullptr)
#endif
{
}

_knxapp::~_knxapp() {
#ifdef FEATURE_WEB
    if (webServer) {
        delete webServer;
    }
#endif
#ifdef FEATURE_WEBS
    if (webSocketServer) {
        delete webSocketServer;
    }
#endif
}

void _knxapp::pinsetup()
{
    // put your pin setup code here, to run once before platform setup:

    Log.info("[platform] pinsetup\n");

    // pin or GPIO the programming led is connected to. Default is LED_BUILTIN
    knx.ledPin(LED_BUILTIN);

    // is the led active on HIGH or low? Default is LOW
    knx.ledPinActiveOn(LED_BUILTIN_ON);

    // pin or GPIO programming button is connected to. Default is 0
    knx.buttonPin(PROGMODE_PIN);
    pinMode(PROGMODE_PIN, INPUT_PULLUP);

    Log.info("[platform] KNX Program Button pin: %d\n", knx.buttonPin());
#ifndef NO_HEARTBEAT
    pinMode(PIN_HEARTBEAT, OUTPUT);
    Log.info("[platform] Heartbeat pin: %d\n", PIN_HEARTBEAT);
#endif
}

char *_knxapp::hostname()
{
    return (char *) xstr(HOSTNAME);
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
