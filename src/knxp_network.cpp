#include "knxp_platformio.h"

// Network status - file scope
static bool networkReady = false;

namespace {
    // Network configuration constants
    constexpr int DEFAULT_WIFI_TIMEOUT = 30;      // seconds
    constexpr int AP_MODE_WIFI_TIMEOUT = 120;     // seconds
    constexpr int PROG_BUTTON_LONG_PRESS = 100;   // 10 seconds (100 * 100ms)
    constexpr int WIFI_RECONNECT_DELAY = 2000;    // milliseconds
    constexpr int HTTP_PORT = 80;

    // WiFi status monitoring
    unsigned long lastWiFiCheck = 0;
    constexpr unsigned long WIFI_CHECK_INTERVAL = 30000; // 30 seconds

    /**
     * @brief Check if the program button is pressed long enough for AP mode
     * @return true if button pressed long enough for AP mode
     */
    bool checkForAPMode() {
        int pressCount = 0;
        while (digitalRead(PROGMODE_PIN) == LOW) {
            delay(100);
            pressCount++;
            if (pressCount > PROG_BUTTON_LONG_PRESS) {
                Log.info("PROG button pressed for more than 10 seconds, starting AP upon release\n");
                return true;
            }
        }
        return false;
    }

    /**
     * @brief Monitor WiFi connection status and attempt reconnection if needed
     */
    void monitorWiFiConnection() {
        unsigned long now = millis();
        if (now - lastWiFiCheck >= WIFI_CHECK_INTERVAL) {
            lastWiFiCheck = now;
            if (WiFi.status() != WL_CONNECTED) {
                Log.warning("WiFi connection lost, attempting reconnection...\n");
                WiFi.reconnect();
            }
        }
    }
}

/**
 * @brief Get current network ready status
 * @return true if network is ready, false otherwise
 */
bool isNetworkReady() {
    return networkReady;
}

/**
 * @brief Initialize and start WiFi connection
 * @param hostname Device hostname for network identification
 * @details Handles both station and AP modes, with automatic fallback
 *          In NETWORK_ONDEMAND mode, only starts when PROG button is pressed
 */
void startWiFi(const char *hostname) {
    if (!hostname) {
        Log.error("Invalid hostname provided\n");
        return;
    }

    WiFiManager manageWiFi;
    wifi_mode_t wifiMode = WIFI_STA;
    int wifiTimeout = DEFAULT_WIFI_TIMEOUT;

    #ifdef NETWORK_ONDEMAND
        if (digitalRead(PROGMODE_PIN) != LOW) {
            Log.info("PROG button not pressed, not starting WiFi\n");
            return;
        }
        
        Log.info("PROG button pressed, starting WiFi\n");
        if (checkForAPMode()) {
            wifiMode = WIFI_AP_STA;
            wifiTimeout = AP_MODE_WIFI_TIMEOUT;
        }
    #endif

    // Configure WiFi settings
    String apName = String(hostname) + "-" + WiFi.macAddress();
    WiFi.hostname(hostname);
    WiFi.mode(wifiMode);

    // Configure WiFiManager
    manageWiFi.setDebugOutput(true);
    manageWiFi.setHostname(hostname);
    manageWiFi.setTimeout(wifiTimeout);

    // Attempt connection
    Log.info("Attempting WiFi connection...\n");
    if (!manageWiFi.autoConnect(apName.c_str())) {
        Log.fatal("Failed to connect and hit timeout\n");
        delay(WIFI_RECONNECT_DELAY);
        ESP.restart();
        return;
    }

    Log.info("Connected with IP-address [%s]\n", WiFi.localIP().toString().c_str());
    networkReady = true;
}

#ifndef NO_TELNET
/**
 * @brief Initialize and start Telnet server
 * @details Configures Telnet for remote debugging and monitoring
 *          When STDIO_TELNET is defined, redirects standard I/O to Telnet
 */
void startTelnet() {
    if (!isNetworkReady()) {
        Log.error("Cannot start Telnet: Network not ready\n");
        return;
    }

    TelnetStream.begin();
    Log.trace("Telnet server started\n");

    #ifdef STDIO_TELNET
        stdIn = &TelnetStream;
        stdOut = &TelnetStream;
        Log.begin(LOG_LEVEL_VERBOSE, &TelnetStream);
    #endif
}
#endif

/**
 * @brief Initialize and start mDNS responder
 * @param hostname Device hostname for mDNS resolution
 * @details Sets up mDNS for local network device discovery
 *          Advertises HTTP service on port 80
 */
void startMDNS(const char *hostname) {
    if (!isNetworkReady()) {
        Log.error("Cannot start mDNS: Network not ready\n");
        return;
    }

    if (!hostname) {
        Log.error("Invalid hostname provided for mDNS\n");
        return;
    }

    Log.info("Setting up mDNS as [%s.local]\n", hostname);
    
    if (!MDNS.begin(hostname)) {
        Log.error("Failed to start mDNS responder!\n");
        return;
    }

    MDNS.addService("http", "tcp", HTTP_PORT);
    Log.trace("mDNS responder started successfully\n");
}

/**
 * @brief Process network-related tasks
 * @details Should be called regularly from the main loop
 *          Handles WiFi connection monitoring and recovery
 */
void processNetwork() {
    if (isNetworkReady()) {
        monitorWiFiConnection();
    }
}

/***************************************************************************
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/
