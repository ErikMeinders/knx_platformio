#ifdef FEATURE_WEB

#include "knxp_web.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoLog.h>
#include "knxp_yield.h"

bool KNXWebServer::begin() {
    if (!LittleFS.begin()) return false;
    server.onNotFound([this]() { handleRequest(); });
    server.begin();
    return true;
}

void KNXWebServer::handleRequest() {
    KnxYieldGuard yieldGuard;
    String path = server.uri();
    if (path == "/") path = "/index.html";

    if (!LittleFS.exists(path)) {
        server.send(404, "text/plain", "404");
        return;
    }

    File file = LittleFS.open(path, "r");
    if (!file) {
        server.send(500, "text/plain", "500");
        return;
    }

    const char* contentType = getContentType(path);
    server.sendHeader("Cache-Control", path.endsWith(".html") ? "no-cache" : "max-age=3600");
    
    // Stream file in chunks
    const size_t CHUNK_SIZE = 512; // Reduced from 1KB to 512B
    uint8_t buffer[CHUNK_SIZE];
    size_t totalSent = 0;
    size_t fileSize = file.size();
    
    if (fileSize > 0) server.setContentLength(fileSize);
    server.send(200, contentType, "");
    
    while (file.available()) {
        yieldGuard.check();
        size_t bytesRead = file.read(buffer, CHUNK_SIZE);
        if (bytesRead && server.client().write(buffer, bytesRead) != bytesRead) break;
        totalSent += bytesRead;
    }
    
    file.close();
}

void KNXWebServer::loop() {
    server.handleClient();
    knxLoopYield(lastYield);
}

const char* KNXWebServer::getContentType(const String& filename) {
    if (filename.endsWith(".html")) return "text/html";
    if (filename.endsWith(".css")) return "text/css";
    if (filename.endsWith(".js")) return "application/javascript";
    if (filename.endsWith(".ico")) return "image/x-icon";
    if (filename.endsWith(".png")) return "image/png";
    if (filename.endsWith(".gz")) {
        String baseFilename = filename.substring(0, filename.length() - 3);
        return getContentType(baseFilename);
    }
    return "text/plain";
}

#endif // FEATURE_WEB
