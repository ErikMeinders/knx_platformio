#ifndef KNXP_WEB_ASYNC_H
#define KNXP_WEB_ASYNC_H

#if defined(FEATURE_WEB) && defined(USE_ASYNC_WEB)

#include <Arduino.h>
#include "knxp_web_base.h"
#include "knxp_async_wrapper.h"
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoLog.h>

using AsyncWebServerClass = AsyncWebServer;

/**
 * Async web server implementation that serves static files from LittleFS.
 */
class KNXAsyncWebServer : public KNXWebServerBase {
public:
    KNXAsyncWebServer() : server(80), serverInitialized(false) {
        Log.notice("Created async web server instance\n");
    }

    bool begin() override;
    void loop() override;

    // Get the underlying server instance (for WebSocket attachment)
    AsyncWebServerClass* getServer() { 
        if (!serverInitialized) {
            Log.error("Attempting to get server before initialization\n");
            return nullptr;
        }
        return &server; 
    }

private:
    AsyncWebServerClass server;
    bool serverInitialized;
    
    void setupHandlers();
    void handleFile(AsyncWebServerRequest *request);
    void logRequest(AsyncWebServerRequest *request, const String& path, size_t size);
    const char* getContentType(const String& filename) override;
};

// Factory function implementation for async version
inline KNXWebServerBase* createWebServer() {
    if (!WiFi.isConnected()) {
        Log.error("Cannot create web server before WiFi is connected\n");
        return nullptr;
    }

    static KNXAsyncWebServer* instance = nullptr;
    if (!instance) {
        instance = new KNXAsyncWebServer();
        Log.notice("Created new web server instance\n");
    } else {
        Log.notice("Returning existing web server instance\n");
    }

    // Store in global variable for other components to access
    extern KNXWebServerBase* webServer;
    webServer = instance;
    
    return instance;
}

#endif // FEATURE_WEB && USE_ASYNC_WEB
#endif // KNXP_WEB_ASYNC_H
