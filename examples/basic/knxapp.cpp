#include "knxapp.h"

DECLARE_TIMER( YourCodeShoutOut, 15);

knxapp knxApp;

void knxapp::setup()
{
    Println("Your setup");

    setGroupObjectCount(1);

    knx.getGroupObject(1).dataPointType(DPT_Value_Temp);
    knx.getGroupObject(1).value(19.8);

    setCyclicTimer(15);

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

