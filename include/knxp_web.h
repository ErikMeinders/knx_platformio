#ifndef KNXP_WEB_H
#define KNXP_WEB_H

#ifdef FEATURE_WEB

#ifdef ESP32
  #include <WebServer.h>
  using WebServerClass = WebServer;
#else
  #include <ESP8266WebServer.h>
  using WebServerClass = ESP8266WebServer;
#endif

#include <LittleFS.h>

/**
 * Simple web server that serves static files from LittleFS.
 * 
 * Note: This uses the same LittleFS instance as your application.
 * If your application also uses LittleFS for config files:
 * - Both uses share the same filesystem space
 * - Multiple LittleFS.begin() calls are safe (it's a singleton)
 * - Just ensure web files don't have same names as your config files
 */
class KNXWebServer {
public:
    KNXWebServer();
    bool begin();
    void loop();

private:
    WebServerClass server;
    static const size_t CHUNKSIZE = 1024;
    
    void handleFile(String path);
    const char* getContentType(String filename);
};

#define CHUNK_SIZE 1024
#endif // FEATURE_WEB
#endif // KNXP_WEB_H
