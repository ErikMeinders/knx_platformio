#include <knxp_platformio.h>

DECLARE_TIMER( YourCodeShoutOut, 5 );

knxapp knxApp;

void knxapp::setup()
{
    stdOut->println("Your setup");
}

void knxapp::loop()
{
    if (DUE( YourCodeShoutOut ))
        stdOut->println("stdOut Your loop");
        Debugln("dbgOut Your loop")
}
