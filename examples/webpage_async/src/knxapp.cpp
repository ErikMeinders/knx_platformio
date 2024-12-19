#include "knxapp.h"

DECLARE_TIMER(YourCodeShoutOut, 15);
DECLARE_TIMER(BroadcastValue, 1);  // 1 second for frequent updates

// Define the global instance
knxapp knxAppInstance;

void knxapp::loop()
{
    _knxapp::loop();  // Call base class implementation first
    
    if (DUE(YourCodeShoutOut))
    {
        Log.verbose("Log Your loop %u\n", xx);
        xx = 0;
    }
    xx++;

#ifdef FEATURE_WEBS
    if (DUE(BroadcastValue))
    {
        static float temp = 19.0;  // Local static for testing
        String json = "{\"temperature\":" + String(temp, 1) + "}";
        
        // Convert float to integer parts for logging (ArduinoLog doesn't support %f)
        int tempInt = (int)temp;
        int tempDec = (int)((temp - tempInt) * 10);
        Log.notice("Broadcasting temperature %d.%d as JSON: %s\n", 
                  tempInt, tempDec, json.c_str());
        
        // Simulate temperature changes
        temp += 0.1;
        if (temp > 25.0) temp = 19.0;
        
        if (this->webSocketServer) {
            this->webSocketServer->broadcast(json.c_str());
        } else {
            Log.error("WebSocket server is null\n");
        }
    }
#endif
}

void knxapp::status()
{
    _knxapp::status();  // Call base class implementation first
    Println("Your status");
}

void knxapp::knx_setup()
{
    // Initialize KNX group object
    setGroupObjectCount(1);
    knx.getGroupObject(1).dataPointType(DPT_Value_Temp);
    knx.getGroupObject(1).value(19.8);
    Log.notice("KNX setup complete\n");

    setCyclicTimer(15);
}
