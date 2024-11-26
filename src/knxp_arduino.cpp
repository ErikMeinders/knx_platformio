#include "knxp_platformio.h"

WebServer httpServer;

Stream *stdIn = &Serial;
Stream *stdOut = &Serial;

/**
 * @brief Initialize network-dependent services
 * @param step Progress step counter
 * @return Updated step counter
 */
int initializeNetworkServices(int step) {
    if (!isNetworkReady()) return step;

    #ifndef NO_NTP
        _knxApp.progress(step++, "Starting NTP");
        timeInit();
    #endif

    #ifndef NO_TELNET
        _knxApp.progress(step++, "Starting Telnet");
        startTelnet();
    #endif

    #ifndef NO_MDNS
        _knxApp.progress(step++, "Starting MDNS");
        startMDNS(_knxApp.hostname());
    #endif

    #ifndef NO_OTA
        _knxApp.progress(step++, "Starting OTA");
        otaInit();
    #endif

    #ifndef NO_HTTP
        _knxApp.progress(step++, "Starting HTTP");
        httpServer.begin();
    #endif

    return step;
}

/**
 * @brief HiJack the Arduino setup() function
 * Do not continue with application setup until KNX is ready
 */
void setup() {
    int step = 0;
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    // Initialize basic hardware
    _knxApp.progress(step++, "Starting pin setup");
    _knxApp.pinsetup();

    // Initialize networking
    #ifndef NO_WIFI
        _knxApp.progress(step++, "Starting WiFi");
        startWiFi(_knxApp.hostname());
    #endif

    // Initialize network-dependent services
    step = initializeNetworkServices(step);

    // Initialize KNX
    #ifndef NO_KNX
        _knxApp.progress(step++, "Starting KNX configuration");
        _knxApp.conf();
    #endif

    // Initialize application
    _knxApp.progress(step++, "Starting KNX Application Setup");
    _knxApp.setup();

    #ifndef NO_KNX   
        _knxApp.progress(step++, "Starting KNX");
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

    #ifndef NO_HTTP
        timeThis(httpServer.handleClient());
    #endif
}

/**
 * @brief HiJack the Arduino loop() function
 * Do not continue with application loop until KNX is ready and not in programming mode
 */
void loop() {
    #ifndef NO_KNX
        knx.loop();
    #endif

    if (stdIn->available() > 0) {
        _knxApp.menu();
    }

    #ifndef NO_HEARTBEAT
        timeThis(handleHeartbeat());
    #endif   

    #ifndef NO_KNX
        if (knx.progMode()) return;
        timeThis(_knxApp.loop());
        timeThis(_knxApp.cyclic());
    #endif

    #ifndef NO_NETWORK
        processNetwork();  // Regular network connectivity check
        processNetworkServices();
    #endif

}
