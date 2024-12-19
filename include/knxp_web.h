#ifndef KNXP_WEB_H
#define KNXP_WEB_H

#ifdef FEATURE_WEB

#include <Arduino.h>
#ifdef ESP32
#include <WebServer.h>
#else
#include <ESP8266WebServer.h>
#endif
#include "knxp_web_base.h"

/**
 * Synchronous web server implementation that serves static files from LittleFS.
 * Features:
 * - Non-blocking file operations using chunked transfer
 * - Maintains heartbeat during file operations using KnxYieldGuard
 * - Generic file serving without hardcoded paths
 * - Proper error handling and logging
 */
class KNXWebServer : public KNXWebServerBase {
public:
    KNXWebServer() : server(80), lastLog(0), lastYield(0) {}
    bool begin() override;
    void loop() override;

private:
    #ifdef ESP32
    WebServer server;
    #else
    ESP8266WebServer server;
    #endif
    
    unsigned long lastLog;    // For periodic logging
    unsigned long lastYield;  // For periodic yielding
    
    void handleRequest();
    const char* getContentType(const String& filename) override;
};

#ifndef USE_ASYNC_WEB
// Factory function implementation for sync version
inline KNXWebServerBase* createWebServer() {
    return new KNXWebServer();
}
#endif

#endif // FEATURE_WEB
#endif // KNXP_WEB_H
