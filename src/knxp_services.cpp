#include <knxp_services.h>

/**
 * @brief Initialize network-dependent services
 * @param step Progress step counter
 * @return Updated step counter
 */
int initializeNetworkServices(int step) {
    if (!isNetworkReady()) return step;

    #ifndef NO_NTP
        progress(step, "Starting NTP");
        timeInit();
        step++;
    #endif

    #ifndef NO_TELNET
        progress(step, "Starting Telnet");
        startTelnet();
        step++;
    #endif

    #ifndef NO_MDNS
        progress(step, "Starting MDNS");
        startMDNS(HOSTNAME);
        step++;
    #endif

    #ifndef NO_OTA
        progress(step, "Starting OTA");
        otaInit();
        step++;
    #endif

    #ifdef FEATURE_WEB
        progress(step, "Starting Web Server");
        if (!LittleFS.begin()) {
            Log.error("Failed to mount LittleFS\n");
            return step;
        }
        Log.notice("Web server started\n");
        step++;
    #endif

    return step;
}

/**
 * @brief Process network-dependent services in the main loop
 */
void processNetworkServices() {
    if (!isNetworkReady()) return;

    #ifndef NO_OTA
        otaLoop();
    #endif
}
