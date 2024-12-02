#include <knxp_platformio.h>
#include <knxp_progress.h>
#include <knxp_timeinfo.h>
#include <knxp_heartbeat.h>

Stream *stdIn = &Serial;
Stream *stdOut = &Serial;

#ifdef FEATURE_WEB
KNXWebServer webServer;
#endif

/**
 * @brief HiJack the Arduino setup() function
 * Do not continue with application setup until KNX is ready
 */
void _knxapp::setup() {
    int step = 0;
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    // Initialize basic hardware
    progress(step, "Starting pin setup");
    pinsetup();
    step++;

    // Initialize networking
    #ifndef NO_WIFI
        progress(step, "Starting WiFi");
        startWiFi(hostname());
        step++;
    #endif

    // Initialize network-dependent services
    #ifndef NO_TELNET
        if (isNetworkReady()) {
            progress(step, "Starting Telnet");
            startTelnet();
            step++;
            progress(step, "Starting MDNS");
            startMDNS(hostname());
            step++;
        }
    #endif

    #ifdef FEATURE_WEBS
        progress(step, "Starting WebSocket Server");
        if (!webSocketServer.begin()) {
            Log.error("Failed to start WebSocket server\n");
            return;
        }
        Log.notice("WebSocket server started\n");
        step++;
    #endif

    // Initialize KNX
    #ifndef NO_KNX
        progress(step, "Starting KNX configuration");
        conf();
        step++;
    #endif

    // Initialize application
    progress(step, "Starting KNX Application Setup");
    knx_setup();
    step++;

    #ifndef NO_KNX   
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
    #ifndef NO_KNX
        knx.loop();
    #endif

    if (stdIn->available() > 0) {
        menu();
    }

    #ifndef NO_HEARTBEAT
        handleHeartbeat();
    #endif   

    #ifndef NO_KNX
        if (knx.progMode()) return; // shortest possible loop
        cyclic();
    #endif

    #ifndef NO_NETWORK
        processNetwork();  // Regular network connectivity check
    #endif

    #ifdef FEATURE_WEB
        if (webServer) {
            webServer->loop();
        }
    #endif

    #ifdef FEATURE_WEBS
        if (webSocketServer) {
            webSocketServer->loop();
        }
    #endif
}
