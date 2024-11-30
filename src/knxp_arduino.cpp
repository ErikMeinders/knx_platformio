#include <knxp_platformio.h>

Stream *stdIn = &Serial;
Stream *stdOut = &Serial;

#ifdef FEATURE_WEB
KNXWebServer webServer;
#endif

/**
 * @brief Initialize network-dependent services
 * @param step Progress step counter
 * @return Updated step counter
 */
int initializeNetworkServices(int step) {
    if (!isNetworkReady()) return step;

    #ifndef NO_NTP
        progress(step++, "Starting NTP");
        timeInit();
    #endif

    #ifndef NO_TELNET
        progress(step++, "Starting Telnet");
        startTelnet();
    #endif

    #ifndef NO_MDNS
        progress(step++, "Starting MDNS");
        startMDNS(HOSTNAME);
    #endif

    #ifndef NO_OTA
        progress(step++, "Starting OTA");
        otaInit();
    #endif

    #ifdef FEATURE_WEB
        progress(step++, "Starting Web Server");
        if (!LittleFS.begin()) {
            Log.error("Failed to mount LittleFS\n");
            return step;
        }
        if (!webServer.begin()) {
            Log.error("Failed to start web server\n");
            return step;
        }
        Log.notice("Web server started\n");
    #else
        #ifndef NO_HTTP
            progress(step++, "Starting HTTP");
            httpServer.begin();
        #endif
    #endif

    return step;
}

/**
 * @brief HiJack the Arduino setup() function
 * Do not continue with application setup until KNX is ready
 */
void _knxapp::setup() {
    int step = 0;
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    // Initialize basic hardware
    progress(step++, "Starting pin setup");
    pinsetup();

    // Initialize networking
    #ifndef NO_WIFI
        progress(step++, "Starting WiFi");
        startWiFi(hostname());
    #endif

    // Initialize network-dependent services
    step = initializeNetworkServices(step);

    #ifdef FEATURE_WEBS
        progress(step++, "Starting WebSocket Server");
        if (!webSocketServer.begin()) {
            Log.error("Failed to start WebSocket server\n");
            return;
        }
        Log.notice("WebSocket server started\n");
    #endif

    // Initialize KNX
    #ifndef NO_KNX
        progress(step++, "Starting KNX configuration");
        conf();
    #endif

    // Initialize application
    progress(step++, "Starting KNX Application Setup");
    knx_setup();

    #ifndef NO_KNX   
        progress(step++, "Starting KNX");
        knx.start();
    #endif

    resetUptime();
    Log.trace("Platform Startup Completed at %s in %u ms.\n\n", timeNowString(), millis());
}

/**
 * @brief Process network-dependent services in the main loop
 */
void processNetworkServices() {
    if (!isNetworkReady()) return;

    #ifndef NO_OTA
        timeThis(otaLoop());
    #endif

    #ifdef FEATURE_WEB
        timeThis(webServer.loop());
    #endif
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
        timeThis(handleHeartbeat());
    #endif   

    #ifndef NO_KNX
        if (knx.progMode()) return; // shorteset possible loop
        timeThis(cyclic());
    #endif

    #ifndef NO_NETWORK
        processNetwork();  // Regular network connectivity check
        processNetworkServices();
    #endif

    #ifdef FEATURE_WEBS
        timeThis(webSocketServer.loop());
    #endif
}
