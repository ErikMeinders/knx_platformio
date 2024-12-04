#ifdef FEATURE_WEB

#include "knxp_web.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoLog.h>

bool KNXWebServer::begin() {
    Log.notice("Starting web server initialization\n");
    
    if (!LittleFS.begin()) {
        Log.error("Failed to mount LittleFS\n");
        return false;
    }
    Log.notice("LittleFS mounted successfully\n");

    // Simple test handler
    server.on("/test", HTTP_GET, [this]() {
        Log.notice("Test endpoint called\n");
        String response = "Server is working!";
        server.sendHeader("Connection", "close");
        server.send(200, "text/plain", response);
    });

    // Root handler
    server.on("/", HTTP_GET, [this]() {
        Log.notice("Root handler called\n");
        String response = "<html><body><h1>KNX Web Server</h1><p>Server is running!</p></body></html>";
        server.sendHeader("Connection", "close");
        server.send(200, "text/html", response);
    });

    server.begin();
    Log.notice("Web server started on port 80\n");
    return true;
}

void KNXWebServer::loop() {
    static uint32_t lastLog = 0;
    uint32_t now = millis();
    
    // Log every 5 seconds
    if (now - lastLog > 5000) {
        Log.notice("Web server loop - Free heap: %d\n", ESP.getFreeHeap());
        lastLog = now;
    }

    server.handleClient();
}

const char* KNXWebServer::getContentType(const String& filename) {
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    else if (filename.endsWith(".gz")) {
        String baseFilename = filename.substring(0, filename.length() - 3);
        return getContentType(baseFilename);
    }
    return "text/plain";
}

#endif // FEATURE_WEB
