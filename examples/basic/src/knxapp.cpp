#include "knxapp.h"

DECLARE_TIMER(YourCodeShoutOut, 15);

// Define the global instance
knxapp knxAppInstance;


void knxapp::setup()
{

    _knxapp::setup();  // Call base class implementation first
    Println("Your setup completed");
}

void knxapp::loop()
{
    _knxapp::loop();  // Call base class implementation first
    
    if (DUE(YourCodeShoutOut))
    {
        Log.verbose("Log Your loop %u\n", xx);
        xx = 0;
    }
    xx++;   
}

void knxapp::knx_setup()
{
    Println("Your KNX setup");
return;
    setGroupObjectCount(1);
    knx.getGroupObject(1).dataPointType(DPT_Value_Temp);
    knx.getGroupObject(1).value(19.8);

    setCyclicTimer(15);
}

void knxapp::status()
{
    _knxapp::status();  // Call base class implementation first
    Println("Your status");
}
