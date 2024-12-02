#include "knxp_network_async.h"
#include "knxp_yield.h"
#include "knxp_espcompat.h"

// Static AsyncWebServer instance for WiFiManager
AsyncWebServer asyncServer(80);
DNSServer dns;

namespace {
    bool checkForAPMode() {
        #ifdef ESP8266
            return WiFi.getMode() & WIFI_AP;
        #else
            return WiFi.getMode() == WIFI_AP;
        #endif
    }
}

int initializeAsyncNetworkServices(const char* hostname) {
    Log.notice("Starting Async Network Services...\n");
    
    // Create AsyncWiFiManager instance
    AsyncWiFiManager wifiManager(&asyncServer, &dns);
    
    // Configure timeout
    wifiManager.setConfigPortalTimeout(180);  // 3 minutes timeout
    
    // Set custom hostname
    WiFi.hostname(hostname);
    
    // Try to connect using saved credentials
    if (!wifiManager.autoConnect()) {
        Log.error("Failed to connect to WiFi\n");
        return -1;
    }

    // Start mDNS
    if (!startAsyncMDNS(hostname)) {
        Log.error("Failed to start mDNS\n");
        return -2;
    }

    Log.notice("Connected to WiFi. IP: %s\n", WiFi.localIP().toString().c_str());
    return 0;
}

bool startAsyncMDNS(const char* hostname) {
    if (!MDNS.begin(hostname)) {
        Log.error("Error setting up mDNS responder\n");
        return false;
    }

    // Add service to mDNS
    MDNS.addService("http", "tcp", 80);
    MDNS.addService("ws", "tcp", 81);
    
    Log.notice("mDNS responder started: %s.local\n", hostname);
    return true;
}

void asyncNetworkLoop() {
    static unsigned long lastYield = 0;
    static unsigned long lastCheck = 0;
    const unsigned long CHECK_INTERVAL = 30000;  // 30 seconds
    
    // Regular yield
    knxLoopYield(lastYield);
    
    // Update mDNS - only needed for ESP8266
    #ifdef ESP8266
        MDNS.update();
    #endif
    
    // Periodic connection check
    unsigned long now = millis();
    if (now - lastCheck >= CHECK_INTERVAL) {
        lastCheck = now;
        
        if (WiFi.status() != WL_CONNECTED) {
            Log.error("WiFi connection lost!\n");
            // Let the watchdog handle reconnection
        } else {
            // Log connection status periodically
            Log.verbose("WiFi connected. IP: %s, RSSI: %d\n", 
                WiFi.localIP().toString().c_str(),
                WiFi.RSSI());
        }
    }
}
