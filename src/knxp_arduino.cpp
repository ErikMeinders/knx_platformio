#include <knxp_platformio.h>
#include <knxp_progress.h>
#include <knxp_timeinfo.h>
#include <knxp_heartbeat.h>

// Initialize console to Serial by default
Stream* console = &Serial;

#ifdef FEATURE_WEB
KNXWebServerBase* webServer = nullptr;
#endif

#ifdef FEATURE_WEBS
KNXWebSocketServerBase* webSocketServer = nullptr;  // Global instance
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

    // Initialize networking first and wait for connection
    #ifndef NO_WIFI
        progress(step, "Starting WiFi");
        if (!startWiFi(hostname())) {
            Log.error("Failed to connect to WiFi\n");
            return;
        }
        
        // Wait for network to be fully ready
        int retries = 0;
        while (!isNetworkReady() && retries < 20) {
            delay(500);
            retries++;
        }
        
        if (!isNetworkReady()) {
            Log.error("Network not ready after timeout\n");
            return;
        }
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

    // Initialize KNX
    #ifndef NO_KNX
        progress(step, "Starting KNX configuration");
        conf();
        step++;
    #endif

    // Initialize web and WebSocket servers after network is ready
    #ifdef FEATURE_WEB
        if (isNetworkReady()) {
            progress(step, "[setup] Starting Web Server");
            ::webServer = createWebServer(); // Set global instance
            if (!::webServer) {
                Log.error("Failed to create web server\n");
                return;
            }
            this->webServer = ::webServer;  // Set class member
            if (!webServer->begin()) {
                Log.error("Failed to start web server\n");
                return;
            }
            Log.notice("[setup] Web server started\n");
            step++;

            #ifdef FEATURE_WEBS
                progress(step, "[setup] Starting WebSocket Server");
                ::webSocketServer = createWebSocketServer();  // Set global instance
                if (!::webSocketServer) {
                    Log.error("Failed to create WebSocket server\n");
                    return;
                }
                this->webSocketServer = ::webSocketServer;  // Set class member
                if (this->webSocketServer) {
                this->webSocketServer->onMessage(
                    [this](uint8_t num, uint8_t* payload, size_t length) {
                        this->handleWebSocketMessage(num, payload, length);
                    });
                this->webSocketServer->begin();
                Log.notice("[setup] WebSocket server started\n");
            }
                step++;
            #endif
        } else {
            Log.error("Network not ready, skipping web server initialization\n");
        }
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
    //Log.trace("Loop start at %s\n", timeNowString());
    #ifndef NO_KNX
        knx.loop();
    #endif

    if (console->available() > 0) {
        menu();
    }

    //Log.trace("Heartbeat at %s\n", timeNowString());
    #ifndef NO_HEARTBEAT
        handleHeartbeat();
    #endif   

    //Log.trace("Cyclic at %s\n", timeNowString());
    #ifndef NO_KNX
        if (knx.progMode()) return; // shortest possible loop
        cyclic();
    #endif

    //Log.trace("processNetwork at %s\n", timeNowString());
    #ifndef NO_NETWORK
        processNetwork();  // Regular network connectivity check
    #endif

    #ifdef FEATURE_WEB
        if (webServer) {
            webServer->loop();
        }
        #ifdef FEATURE_WEBS
            if (this->webSocketServer) {  // Use class member
                this->webSocketServer->loop();
            }
        #endif
    #endif
}
