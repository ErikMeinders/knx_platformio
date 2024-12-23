#include "network/knxp_services.h"
#include "core/app.h"

// Forward declaration of the app instance
extern _knxapp* app;

/**
 * @brief Initialize network-dependent services
 * @param step Progress step counter
 * @return Updated step counter
 */
int initializeNetworkServices(_knxapp* app, int step) {
#ifndef KNXP_NO_WIFI
    progress(step, "Starting WiFi");
    if (!startWiFi(KNXP_HOSTNAME)) {
        Log.error("Failed to connect to WiFi\n");
        return step;
    }
    
    // Wait for network to be fully ready
    int retries = 0;
    while (!isNetworkReady() && retries < 20) {
        delay(500);
        retries++;
    }
    
    if (!isNetworkReady()) {
        Log.error("Network not ready after timeout\n");
        return step;
    }
    step++;

    #ifndef KNXP_NO_NTP
        progress(step, "Starting NTP");
        timeInit();
        step++;
    #endif

    #ifndef KNXP_NO_TELNET
        progress(step, "Starting Telnet");
        startTelnet();
        step++;
    #endif

    #ifndef KNXP_NO_MDNS
        progress(step, "Starting MDNS");
        startMDNS(KNXP_HOSTNAME);
        step++;
    #endif

    #ifndef KNXP_NO_OTA
        progress(step, "Starting OTA");
        otaInit();
        step++;
    #endif

    #ifdef KNXP_FEATURE_WEB
        if (isNetworkReady()) {
            progress(step, "[setup] Starting Web Server");
            auto server = createWebServer();
            if (!server) {
                Log.error("Failed to create web server\n");
                return step;
            }
            if (!server->begin()) {
                Log.error("Failed to start web server\n");
                return step;
            }
            app->setWebServer(server);
            Log.notice("[setup] Web server started\n");
            step++;

            #ifdef KNXP_FEATURE_WEBS
                progress(step, "[setup] Starting WebSocket Server");
                auto wsServer = createWebSocketServer(server);
                if (!wsServer) {
                    Log.error("Failed to create WebSocket server\n");
                    return step;
                }
                app->setWebSocketServer(wsServer);
                if (wsServer) {
                    // Use raw pointer to avoid capturing global variable
                    auto handler = [app](uint8_t num, uint8_t* payload, size_t length) {
                        app->handleWebSocketMessage(num, payload, length);
                    };
                    wsServer->onMessage(handler);
                    wsServer->begin();
                    Log.notice("[setup] WebSocket server started\n");
                }
                step++;
            #endif
        } else {
            Log.error("Network not ready, skipping web server initialization\n");
        }
    #endif
#else
    Log.notice("Network disabled with -DKNXP_NO_WIFI\n");
#endif
    return step;
}

/**
 * @brief Process network-dependent services in the main loop
 */
void processNetworkServices() {
    if (!isNetworkReady()) return;

    #ifndef KNXP_NO_OTA
        otaLoop();
    #endif
}
