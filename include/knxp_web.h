#ifndef KNXP_WEB_H
#define KNXP_WEB_H

#ifndef NOWEB

#ifdef ESP32
  #include <WebServer.h>
  using WebServerClass = WebServer;
#else
  #include <ESP8266WebServer.h>
  using WebServerClass = ESP8266WebServer;
#endif

#include <LittleFS.h>
#include <functional>
#include <vector>
#include <ArduinoLog.h>

#define WEB_CHUNK_SIZE 1024

class KNXWebServer {
public:
    using APIHandler = std::function<void(WebServerClass&)>;

    KNXWebServer();
    
    // Initialize web server and filesystem
    bool begin();
    
    // Main processing loop
    void loop();
    
    // Add custom API endpoint
    void addAPIHandler(const char* path, HTTPMethod method, APIHandler handler);
    
    // Set custom 404 handler
    void setNotFoundHandler(APIHandler handler);

    // Get reference to underlying server
    WebServerClass& getServer() { return server; }

private:
    WebServerClass server;
    std::vector<std::pair<String, APIHandler>> apiHandlers;
    APIHandler notFoundHandler;

    struct MimeType {
        const char* extension;
        const char* type;
    };
    static const MimeType mimeTypes[];

    // Internal handlers
    void handleFileRequest();
    void serveFile(const String& path, const char* contentType);
    const char* getMimeType(const String& path);
    void setCacheHeaders();
    bool isAPIPath(const String& path);
    void setupDefaultEndpoints();
};

#endif // NOWEB
#endif // KNXP_WEB_H
