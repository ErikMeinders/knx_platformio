#ifndef KNXP_WEB_BASE_H
#define KNXP_WEB_BASE_H

#ifdef FEATURE_WEB

#include <LittleFS.h>
#include <ArduinoLog.h>

/**
 * Base interface class for web server functionality.
 * This allows switching between sync and async implementations
 * based on compile flags.
 */
class KNXWebServerBase {
public:
    virtual ~KNXWebServerBase() = default;
    virtual bool begin() = 0;
    virtual void loop() = 0;

protected:
    virtual const char* getContentType(const String& filename) {
        if (filename.endsWith(".html")) return "text/html";
        if (filename.endsWith(".css")) return "text/css";
        if (filename.endsWith(".js")) return "application/javascript";
        if (filename.endsWith(".json")) return "application/json";
        if (filename.endsWith(".ico")) return "image/x-icon";
        if (filename.endsWith(".png")) return "image/png";
        if (filename.endsWith(".jpg")) return "image/jpeg";
        return "text/plain";
    }
};

// Forward declare the factory function
KNXWebServerBase* createWebServer();

#endif // FEATURE_WEB
#endif // KNXP_WEB_BASE_H
