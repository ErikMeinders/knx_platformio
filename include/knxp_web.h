#ifndef KNXP_WEB_H
#define KNXP_WEB_H

#ifdef FEATURE_WEB

#include "knxp_web_base.h"

#ifdef ESP32
  #include <WebServer.h>
  using WebServerClass = WebServer;
#else
  #include <ESP8266WebServer.h>
  using WebServerClass = ESP8266WebServer;
#endif

/**
 * Synchronous web server implementation that serves static files from LittleFS.
 */
class KNXWebServer : public KNXWebServerBase {
public:
    KNXWebServer() : server(80) {}
    bool begin() override;
    void loop() override;

private:
    WebServerClass server;
    void handleFile(String path);
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
