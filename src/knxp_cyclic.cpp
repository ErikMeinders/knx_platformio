#include <knxp_platformio.h>

DECLARE_TIMER(CyclicTimer,10);

void _knxapp::setCyclicTimer(unsigned long interval)
{
    SET_TIMER(CyclicTimer, interval);
}

void _knxapp::cyclic()
{
    if(!DUE(CyclicTimer))
        return;
    
    if(!knx.configured())
    {
        Log.error("KNX not configured\n");
        return;
    }
    Log.trace("Entering cyclic\n");

    KnxYieldGuard yieldGuard;
    for (int16_t g = 1; g <= _groupObjectCount; g++)
    {
        yieldGuard.check();
        
        if (knx.getGroupObject(g).asap() != g)
        {
            Log.error("ASAP mismatch %d %d\n", g, knx.getGroupObject(g).asap());
            break;
        }
        Log.info("GO %d FLGS %c%c%c%c\n", g, knx.getGroupObject(g).communicationEnable() ? 'C' : '-',
            knx.getGroupObject(g).readEnable() ? 'R' : '-', 
            knx.getGroupObject(g).writeEnable() ? 'W' : '-',
            knx.getGroupObject(g).transmitEnable() ? 'T' : '-');

        if (knx.getGroupObject(g).readEnable() && knx.getGroupObject(g).communicationEnable() && 
            knx.getGroupObject(g).transmitEnable() && !knx.getGroupObject(g).writeEnable())
        {                        
            knx.getGroupObject(g).objectWritten();
            delay(50);  // Need to slow down to avoid bus overload
        }
    }
}