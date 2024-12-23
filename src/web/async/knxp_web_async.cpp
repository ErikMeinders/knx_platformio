#if defined(KNXP_FEATURE_WEB) && defined(KNXP_USE_ASYNC_WEB)

#include "web/async/knxp_web_async.h"
#include "web/knxp_web_base.h"
#include <Arduino.h>
#include <LittleFS.h>
#include <ArduinoLog.h>

bool KNXAsyncWebServer::begin() {
    Log.notice("Starting async web server initialization\n");
    
    if (!WiFi.isConnected()) {
        Log.error("Cannot start web server before WiFi is connected\n");
        return false;
    }

    if (!LittleFS.begin()) {
        Log.error("Failed to mount LittleFS\n");
        return false;
    }
    Log.notice("LittleFS mounted successfully\n");

    setupHandlers();
    server.begin();
    serverInitialized = true;  // Set flag after successful initialization
    Log.notice("Async web server started on port 80\n");
    return true;
}

void KNXAsyncWebServer::loop() {
    // No active processing needed for async server
}

void KNXAsyncWebServer::setupHandlers() {
    // Add CORS headers to all responses
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");

    // Handle root and index.html explicitly
    server.on("/", HTTP_GET, [this](AsyncWebServerRequest *request) {
        handleFile(request);
    });

    server.on("/index.html", HTTP_GET, [this](AsyncWebServerRequest *request) {
        handleFile(request);
    });

    // Let the library handle all other files
    server.serveStatic("/", LittleFS, "/")
        .setDefaultFile("index.html")
        .setCacheControl("no-cache");

    // Handle 404s
    server.onNotFound([this](AsyncWebServerRequest *request) {
        request->send(404, "text/plain", "File Not Found");
    });
}

void KNXAsyncWebServer::handleFile(AsyncWebServerRequest *request) {
    String path = request->url();
    if (path == "/" || path.isEmpty()) {
        path = "/index.html";
    }

    if (!LittleFS.exists(path)) {
        request->send(404, "text/plain", "File Not Found");
        return;
    }

    File file = LittleFS.open(path, "r");
    if (!file) {
        request->send(500, "text/plain", "Failed to open file");
        return;
    }

    const size_t fileSize = file.size();
    file.close();

    AsyncWebServerResponse *response = request->beginResponse(LittleFS, path, getContentType(path));
    response->addHeader("Cache-Control", "no-cache");
    response->addHeader("Connection", "close");
    response->setContentLength(fileSize);

    request->send(response);
    logRequest(request, path, fileSize);
}

void KNXAsyncWebServer::logRequest(AsyncWebServerRequest *request, const String& path, size_t size) {
    String method = "UNKNOWN";
    switch (request->method()) {
        case HTTP_GET: method = "GET"; break;
        case HTTP_POST: method = "POST"; break;
        case HTTP_DELETE: method = "DELETE"; break;
        case HTTP_PUT: method = "PUT"; break;
        case HTTP_PATCH: method = "PATCH"; break;
        case HTTP_HEAD: method = "HEAD"; break;
        case HTTP_OPTIONS: method = "OPTIONS"; break;
    }
    Log.notice("HTTP %s %s (%d bytes)\n", method.c_str(), path.c_str(), size);
}

const char* KNXAsyncWebServer::getContentType(const String& filename) {
    if (filename.endsWith(".html")) return "text/html";
    if (filename.endsWith(".css")) return "text/css";
    if (filename.endsWith(".js")) return "application/javascript";
    if (filename.endsWith(".ico")) return "image/x-icon";
    if (filename.endsWith(".gz")) {
        const int index = filename.lastIndexOf('.');
        if (index > 0) {
            return getContentType(filename.substring(0, index));
        }
    }
    return "text/plain";
}

// Factory function implementation for async version
KNXWebServerBase* createWebServer() {
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
    
    return instance;
}

#endif // KNXP_FEATURE_WEB && KNXP_USE_ASYNC_WEB
