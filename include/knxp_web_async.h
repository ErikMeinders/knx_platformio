#ifndef KNXP_WEB_ASYNC_H
#define KNXP_WEB_ASYNC_H

#ifdef FEATURE_WEB

#include "knxp_web_base.h"

#ifdef ESP32
  #include <AsyncWebServer.h>
  #include <AsyncTCP.h>
  using AsyncWebServerClass = AsyncWebServer;
#else
  #include <ESPAsyncTCP.h>
  #include <ESPAsyncWebServer.h>
  using AsyncWebServerClass = AsyncWebServer;
#endif

// HTTP method definitions
#define KNX_HTTP_GET HTTP_GET
#define KNX_HTTP_POST HTTP_POST

/**
 * Async web server implementation that serves static files from LittleFS.
 */
class KNXAsyncWebServer : public KNXWebServerBase {
public:
    KNXAsyncWebServer();
    bool begin() override;
    void loop() override;

    // Get the underlying server instance (for WebSocket attachment)
    AsyncWebServerClass* getServer() { return &server; }

private:
    AsyncWebServerClass server;
    
    void setupHandlers();
    void handleFile(AsyncWebServerRequest *request);
    void logRequest(AsyncWebServerRequest *request, const String& path, size_t size);
    const char* getContentType(const String& filename) override;
};

#ifdef USE_ASYNC_WEB
// Factory function implementation for async version
inline KNXWebServerBase* createWebServer() {
    return new KNXAsyncWebServer();
}
#endif

#endif // FEATURE_WEB
#endif // KNXP_WEB_ASYNC_H
