#include "knxp_platformio.h"
#include "network/knxp_network.h"
#include "utils/knxp_timeinfo.h"
#include <ArduinoLog.h>

#ifndef KNXP_NO_WIFI

#ifdef KNXP_USE_ASYNC_WEB
#include <ESPAsyncWiFiManager.h>
#include <ESPAsyncWebServer.h>
#include <DNSServer.h>
#else
#include <WiFiManager.h>
#endif

#ifdef ESP8266
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <ESPmDNS.h>
#endif

#ifdef KNXP_USE_ASYNC_WEB
static AsyncWiFiManager* wifiManager = nullptr;
static AsyncWebServer* wifiConfigServer = nullptr;
static DNSServer* asyncDNSServer = nullptr;
#else
static WiFiManager wifiManager;
#endif

static unsigned long lastConnectionTime = 0;
static bool waitingForStabilization = false;

bool startWiFi(const char* hostname) {
    Log.notice("Starting WiFi setup for hostname: \"%s\"\n", hostname);
    
    // Set hostname before WiFi connection
    #ifdef ESP8266
    WiFi.hostname(hostname);
    #elif defined(ESP32)
    WiFi.setHostname(hostname);
    #endif

    #ifdef KNXP_USE_ASYNC_WEB
    // Create instances only when starting WiFi
    if (!wifiConfigServer) {
        wifiConfigServer = new AsyncWebServer(80);
    }
    if (!asyncDNSServer) {
        asyncDNSServer = new DNSServer();
    }
    if (!wifiManager) {
        wifiManager = new AsyncWiFiManager(wifiConfigServer, asyncDNSServer);
    }
    
    // Configure WiFi manager
    wifiManager->setDebugOutput(true);
    wifiManager->setConfigPortalTimeout(180); //hjm
    wifiManager->setConnectTimeout(30); //hjm
    wifiManager->setBreakAfterConfig(true);  // Exit after saving settings
    
    // Try to connect using stored credentials
    Log.notice("Using AsyncWiFiManager\n");
    const char* autoConnectSSID = hostname;  // Use hostname as AP name
    if (wifiManager->autoConnect(autoConnectSSID)) {
    #else
    Log.notice("Using WiFiManager\n");
    wifiManager.setDebugOutput(true);
    wifiManager.setConfigPortalTimeout(180);
    if (wifiManager.autoConnect()) {
    #endif
        // Start connection stabilization period
        lastConnectionTime = millis();
        waitingForStabilization = true;
        return true;
    }

    Log.error("Failed to connect to WiFi\n");
    return false;
}

bool isNetworkReady() {
    if (waitingForStabilization) {
        if (millis() - lastConnectionTime >= 500) {  // 500ms stabilization period
            waitingForStabilization = false;
            if (WiFi.status() == WL_CONNECTED) {
                Log.notice("WiFi connected - SSID: %s, IP: %s, Channel: %d, RSSI: %d\n",
                    WiFi.SSID().c_str(),
                    WiFi.localIP().toString().c_str(),
                    WiFi.channel(),
                    WiFi.RSSI()
                );

                Log.notice("Network interface details:\n");
                Log.notice("  MAC Address: %s\n", WiFi.macAddress().c_str());
                Log.notice("  Subnet Mask: %s\n", WiFi.subnetMask().toString().c_str());
                Log.notice("  Gateway IP: %s\n", WiFi.gatewayIP().toString().c_str());
                Log.notice("  DNS Server: %s\n", WiFi.dnsIP().toString().c_str());

                #ifndef KNXP_NO_TELNET
                // Start telnet after network is stable
                startTelnet();
                #endif
            }
        }
        return false;  // Not ready during stabilization
    }
    return WiFi.status() == WL_CONNECTED;

}

void processNetwork() {
    static uint32_t lastCheck = 0;
    uint32_t now = millis();

    // Check connection every 30 seconds
    if (now - lastCheck > 30000) {
        lastCheck = now;
        if (WiFi.status() != WL_CONNECTED) {
            Log.warning("WiFi connection lost, attempting to reconnect...\n");
            WiFi.reconnect();
            #ifndef KNXP_NO_TELNET
            console = &Serial;  // Switch back to Serial during reconnection
            Log.begin(LOG_LEVEL, console);
            #endif
        }
    }
}

#ifndef KNXP_NO_TELNET
#include <TelnetStream.h>

void startTelnet() {
    TelnetStream.begin();
    Log.notice("Console about to switch to telnet\n");
    console = &TelnetStream;  // Switch console to TelnetStream
    Log.begin(LOG_LEVEL, console);  // Update logger to use TelnetStream
    Log.notice("Telnet server started\n");
}
#endif

#ifndef KNXP_NO_MDNS
void startMDNS(const char* hostname) {
    Log.notice("Starting mDNS with hostname: \"%s\"\n", hostname);
    if (MDNS.begin(hostname)) {
        MDNS.addService("http", "tcp", 80);
        Log.notice("mDNS responder started at \"%s\".local\n", hostname);
    } else {
        Log.error("Error setting up MDNS responder\n");
    }
}
#endif

#else
bool isNetworkReady() {

    return true;
}

#endif // KNXP_NO_WIFI
