#include "knxp_platformio.h"
#include "utils/knxp_progress.h"
#include "utils/knxp_timeinfo.h"
#include "utils/knxp_heartbeat.h"
#include "network/knxp_services.h"

// Initialize console to Serial by default
Stream* console = &Serial;

/**
 * @brief HiJack the Arduino setup() function
 * Do not continue with application setup until KNX is ready
 */
void _knxapp::setup() {
    int step = 0;
#ifdef SERIAL_SPEED
    Serial.begin(SERIAL_SPEED);
#else
    Serial.begin(115200);
#endif
    Log.begin(LOG_LEVEL, &Serial);

    // Initialize basic hardware
    progress(step, "Starting pin setup");
    pinsetup();
    Log.notice("[setup] Pin setup completed\n");
    step++;

    // Initialize network and network-dependent services
    step = initializeNetworkServices(this, step);

    // Initialize KNX
    #ifndef KNXP_NO_KNX
        progress(step, "Starting KNX configuration");
        conf();
        step++;
    #endif

    // Initialize application
    progress(step, "Starting KNX Application Setup");
    knx_setup();
    step++;

    #ifndef KNXP_NO_KNX   
        progress(step, "Starting KNX");
        knx.start();
        step++;
    #endif

    Log.trace("Platform Startup Completed at %s in %u ms.\n\n", timeNowString(), millis());
}

/**
 * @brief HiJack the Arduino loop() function
 * Do not continue with application loop until KNX is ready and not in programming mode
 */
void _knxapp::loop() {
    //Log.trace("Loop start at %s\n", timeNowString());
    #ifndef KNXP_NO_KNX
        knx.loop();
    #endif

    if (console->available() > 0) {
        menu();
    }

    //Log.trace("Heartbeat at %s\n", timeNowString());
    #ifndef KNXP_NO_HEARTBEAT
        handleHeartbeat();
    #endif   

    //Log.trace("Cyclic at %s\n", timeNowString());
    #ifndef KNXP_NO_KNX
        if (knx.progMode()) return; // shortest possible loop
        cyclic();
    #endif

    //Log.trace("processNetwork at %s\n", timeNowString());
    #ifndef KNXP_NO_WIFI
        processNetwork();  // Regular network connectivity check
    #endif

    #ifdef KNXP_FEATURE_WEB
        if (webServer) {
            webServer->loop();
        }
        #ifdef KNXP_FEATURE_WEBS
            if (webSocketServer) {
                webSocketServer->loop();
            }
        #endif
    #endif
}
