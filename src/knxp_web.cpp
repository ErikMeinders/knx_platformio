#include "knxp_web.h"
#include "knxp_yield.h"

#ifdef FEATURE_WEB

KNXWebServer::KNXWebServer() : server(80) {}

bool KNXWebServer::begin() {
    server.onNotFound([this]() {
        String path = server.uri();
        if (path.endsWith("/")) {
            path += "index.html";
        }
        handleFile(path);
    });
    Log.notice("Started web server\n");
    server.begin();
    return true;
}

void KNXWebServer::loop() {
    static unsigned long lastYield = 0;
    static unsigned long lastCleanup = 0;
    const unsigned long CLEANUP_INTERVAL = 30000; // 30 seconds

    knxLoopYield(lastYield);

    // Periodic cleanup
    unsigned long now = millis();
    if (now - lastCleanup >= CLEANUP_INTERVAL) {
        // Force garbage collection
        ESP.getFreeHeap();
        Log.notice("Free heap: %d\n", ESP.getFreeHeap());
        lastCleanup = now;
    }

    server.handleClient();
}

void KNXWebServer::handleFile(String path) {
    if (!path.startsWith("/")) {
        path = "/" + path;
    }

    // Early return for non-existent files
    if (!LittleFS.exists(path)) {
        Log.error("File Not Found: %s\n", path.c_str());
        server.send(404, "text/plain", "File Not Found");
        return;
    }

    // Get file info before opening
    File file = LittleFS.open(path, "r");
    if (!file) {
        Log.error("Failed to open file: %s\n", path.c_str());
        server.send(500, "text/plain", "Failed to open file");
        return;
    }

    const size_t fileSize = file.size();
    const char* contentType = getContentType(path);

    // Log request details
    Log.notice("Request: %s, Size: %d, Heap: %d\n", path.c_str(), fileSize, ESP.getFreeHeap());

    // Set Cache-Control for static files
    if (path.endsWith(".css") || path.endsWith(".js") || path.endsWith(".ico")) {
        server.sendHeader("Cache-Control", "public, max-age=31536000"); // 1 year
    } else {
        server.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    }

    // Enable compression for text files
    const bool shouldCompress = server.client().localIP()[3] != 0 && // Skip for local network
        (path.endsWith(".html") || path.endsWith(".css") || path.endsWith(".js"));
    
    if (shouldCompress) {
        server.sendHeader("Content-Encoding", "gzip");
    }

    // Use ESP8266WebServer's built-in streaming
    const size_t sent = server.streamFile(file, contentType);
    file.close();

    // Log completion status
    if (sent != fileSize) {
        Log.error("Incomplete transfer: %d of %d bytes for %s\n", sent, fileSize, path.c_str());
    } else {
        Log.notice("Sent %d bytes, Heap: %d\n", sent, ESP.getFreeHeap());
    }

    // Force cleanup after large transfers
    if (fileSize > 1024) {
        ESP.getFreeHeap(); // Trigger garbage collection
    }
}

const char* KNXWebServer::getContentType(String filename) {
    if (filename.endsWith(".html")) return "text/html";
    if (filename.endsWith(".css")) return "text/css";
    if (filename.endsWith(".js")) return "application/javascript";
    if (filename.endsWith(".json")) return "application/json";
    if (filename.endsWith(".ico")) return "image/x-icon";
    if (filename.endsWith(".png")) return "image/png";
    if (filename.endsWith(".jpg")) return "image/jpeg";
    return "text/plain";
}

#endif // FEATURE_WEB
