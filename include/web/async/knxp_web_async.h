#ifndef KNXP_WEB_ASYNC_H
#define KNXP_WEB_ASYNC_H

#if defined(KNXP_FEATURE_WEB) && defined(KNXP_USE_ASYNC_WEB)

#include <Arduino.h>
#include "web/knxp_web_server_base.h"
#include "utils/knxp_async_wrapper.h"
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

// Factory function declaration for async version
KNXWebServerBase* createWebServer();

#endif // KNXP_FEATURE_WEB && KNXP_USE_ASYNC_WEB
#endif // KNXP_WEB_ASYNC_H
