#include <knxp_platformio.h>

void _knxapp::conf()
{
    knx.readMemory();
    uint16_t ia = knx.individualAddress();

    Log.info("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    Log.info("Configured: %s\n", knx.configured() ? "true" : "false");

    if (!knx.configured())
    {
        Log.trace("Waiting for KNX to be configured \n");

        knx.start();
        KnxYieldGuard yieldGuard;
        while (!knx.configured())
        {
            yieldGuard.check();
            if (stdIn->available())
                this->menu();

            knx.loop();
        }
        // knx.stop();
        ESP.restart();
    }
}

