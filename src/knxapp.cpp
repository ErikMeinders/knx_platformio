#include <knxp_platformio.h>
#include "debug.h"

DECLARE_TIMER( YourCodeShoutOut, 5 );

knxapp knxApp;

void knxapp::setup()
{
    Println("Your setup");
    // startDebug();
    // Log.begin(LOG_LEVEL_VERBOSE, &TelnetStream);
}

unsigned long xx=0;

void knxapp::loop()
{
    if (DUE( YourCodeShoutOut ))
    {
        // Debugln("dbgOut Your loop");
        Log.verbose("Log Your loop %u\n", xx);
        xx=0;
    }
    xx++;   
}

void knxapp::status()
{
    _knxapp::status();
    Println("Your status");
}