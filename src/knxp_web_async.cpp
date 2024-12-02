#include "knxp_web_async.h"
#include "knxp_yield.h"

#ifdef FEATURE_WEB

KNXAsyncWebServer::KNXAsyncWebServer() : server(80) {}

bool KNXAsyncWebServer::begin() {
    setupHandlers();
    server.begin();
    Log.notice("Started async web server\n");
    return true;
}

void KNXAsyncWebServer::loop() {
    // No active loop needed for async server
    static unsigned long lastCleanup = 0;
    const unsigned long CLEANUP_INTERVAL = 30000; // 30 seconds

    // Periodic cleanup
    unsigned long now = millis();
    if (now - lastCleanup >= CLEANUP_INTERVAL) {
        // Force garbage collection
        ESP.getFreeHeap();
        Log.notice("Free heap: %d\n", ESP.getFreeHeap());
        lastCleanup = now;
    }
}

void KNXAsyncWebServer::setupHandlers() {
    // Serve static files from LittleFS
    server.onNotFound([this](AsyncWebServerRequest *request) {
        String path = request->url();
        if (path.endsWith("/")) {
            path += "index.html";
        }
        handleFile(request);
    });

    // Add cache headers for static files using regex patterns
    server.on("\\.js$", KNX_HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(200);
        response->addHeader("Cache-Control", "public, max-age=31536000");
        request->send(response);
    });
    
    server.on("\\.css$", KNX_HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(200);
        response->addHeader("Cache-Control", "public, max-age=31536000");
        request->send(response);
    });
    
    server.on("\\.ico$", KNX_HTTP_GET, [](AsyncWebServerRequest *request) {
        AsyncWebServerResponse *response = request->beginResponse(200);
        response->addHeader("Cache-Control", "public, max-age=31536000");
        request->send(response);
    });
}

void KNXAsyncWebServer::handleFile(AsyncWebServerRequest *request) {
    String path = request->url();
    if (!path.startsWith("/")) {
        path = "/" + path;
    }

    // Early return for non-existent files
    if (!LittleFS.exists(path)) {
        Log.error("File Not Found: %s\n", path.c_str());
        request->send(404, "text/plain", "File Not Found");
        return;
    }

    File file = LittleFS.open(path, "r");
    if (!file) {
        Log.error("Failed to open file: %s\n", path.c_str());
        request->send(500, "text/plain", "Failed to open file");
        return;
    }

    const size_t fileSize = file.size();
    logRequest(request, path, fileSize);

    // Create response with proper content type
    AsyncWebServerResponse *response = request->beginResponse(LittleFS, path, getContentType(path));

    // Add caching headers for static assets
    if (path.endsWith(".css") || path.endsWith(".js") || path.endsWith(".ico")) {
        response->addHeader("Cache-Control", "public, max-age=31536000");
    } else {
        response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
    }

    // Enable compression for text files
    const bool shouldCompress = 
        request->client()->localIP()[3] != 0 && // Skip for local network
        (path.endsWith(".html") || path.endsWith(".css") || path.endsWith(".js"));

    if (shouldCompress) {
        response->addHeader("Content-Encoding", "gzip");
    }

    request->send(response);

    // Log completion and cleanup
    Log.notice("Sent %s, Size: %d, Heap: %d\n", 
        path.c_str(), fileSize, ESP.getFreeHeap());

    // Force cleanup after large transfers
    if (fileSize > 1024) {
        ESP.getFreeHeap(); // Trigger garbage collection
    }
}

const char* KNXAsyncWebServer::getContentType(const String& filename) {
    if (filename.endsWith(".html")) return "text/html";
    if (filename.endsWith(".css")) return "text/css";
    if (filename.endsWith(".js")) return "application/javascript";
    if (filename.endsWith(".json")) return "application/json";
    if (filename.endsWith(".ico")) return "image/x-icon";
    if (filename.endsWith(".png")) return "image/png";
    if (filename.endsWith(".jpg")) return "image/jpeg";
    return "text/plain";
}

void KNXAsyncWebServer::logRequest(AsyncWebServerRequest *request, const String& path, size_t size) {
    Log.notice("Request: %s %s, Size: %d, Heap: %d\n", 
        request->methodToString(),
        path.c_str(), 
        size,
        ESP.getFreeHeap());
}

#endif // FEATURE_WEB
