#include "knxp_web.h"

#ifndef NOWEB

// MIME type definitions
const KNXWebServer::MimeType KNXWebServer::mimeTypes[] = {
    {".html", "text/html"},
    {".css", "text/css"},
    {".js", "application/javascript"},
    {".json", "application/json"},
    {".ico", "image/x-icon"},
    {".png", "image/png"},
    {".jpg", "image/jpeg"},
    {".ini", "text/plain"},
    {nullptr, "text/plain"}
};

KNXWebServer::KNXWebServer() : server(80) {}

bool KNXWebServer::begin() {
    if (!LittleFS.begin()) {
        Log.error("Failed to mount LittleFS\n");
        return false;
    }

    setupDefaultEndpoints();
    server.begin();
    Log.notice("Web server started\n");
    return true;
}

void KNXWebServer::loop() {
    server.handleClient();
}

void KNXWebServer::addAPIHandler(const char* path, HTTPMethod method, APIHandler handler) {
    server.on(path, method, [this, handler]() {
        // Add CORS headers for API endpoints
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        
        if (server.method() == HTTP_OPTIONS) {
            server.send(200);
            return;
        }
        
        handler(server);
    });
}

void KNXWebServer::setNotFoundHandler(APIHandler handler) {
    notFoundHandler = handler;
}

void KNXWebServer::setupDefaultEndpoints() {
    // Handle all requests through our file handler
    server.onNotFound([this]() {
        // Add CORS headers for all endpoints
        server.sendHeader("Access-Control-Allow-Origin", "*");
        server.sendHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        server.sendHeader("Access-Control-Allow-Headers", "Content-Type");
        
        if (server.method() == HTTP_OPTIONS) {
            server.send(200);
            return;
        }
        
        // Check if it's an API path first
        if (isAPIPath(server.uri())) {
            if (notFoundHandler) {
                notFoundHandler(server);
            } else {
                server.send(404, "text/plain", "API endpoint not found");
            }
            return;
        }
        
        // Otherwise try to serve file
        handleFileRequest();
    });
}

bool KNXWebServer::isAPIPath(const String& path) {
    return path.startsWith("/api/");
}

void KNXWebServer::handleFileRequest() {
    String path = server.uri();
    
    // Default to index.html for root
    if (path.equals("/") || path.endsWith("/")) {
        path = "/index.html";
    }
    // Ensure path starts with /
    else if (!path.startsWith("/")) {
        path = "/" + path;
    }

    // Remove query parameters
    int queryIndex = path.indexOf('?');
    if (queryIndex != -1) {
        path = path.substring(0, queryIndex);
    }

    Log.trace("Serving file: %s\n", path.c_str());

    if (!LittleFS.exists(path)) {
        Log.error("File not found: %s\n", path.c_str());
        server.send(404, "text/plain", "File not found");
        return;
    }

    const char* contentType = getMimeType(path);
    serveFile(path, contentType);
}

void KNXWebServer::serveFile(const String& path, const char* contentType) {
    File file = LittleFS.open(path, "r");
    if (!file) {
        Log.error("Failed to open file: %s\n", path.c_str());
        server.send(500, "text/plain", "Failed to open file");
        return;
    }

    const size_t fileSize = file.size();
    Log.trace("Serving %s (%d bytes)\n", path.c_str(), fileSize);

    // Set cache headers
    setCacheHeaders();
    
    // Check if client's cached version is still valid
    if (server.hasHeader("If-None-Match")) {
        String ifNoneMatch = server.header("If-None-Match");
        String etag = String(PIO_BUILD_TIME);
        if (ifNoneMatch == etag) {
            file.close();
            server.send(304, "text/plain", ""); // Not Modified
            return;
        }
    }

    // Send file in chunks
    server.setContentLength(fileSize);
    server.send(200, contentType, "");

    uint8_t buf[WEB_CHUNK_SIZE];
    size_t totalSent = 0;

    while (totalSent < fileSize) {
        yield();  // Allow other tasks to run
        
        size_t toRead = min(WEB_CHUNK_SIZE, (int)(fileSize - totalSent));
        size_t bytesRead = file.read(buf, toRead);
        
        if (bytesRead == 0) break;

        server.sendContent((char*)buf, bytesRead);
        totalSent += bytesRead;
        
        delay(1);  // Small delay to prevent watchdog triggers
    }

    file.close();
    Log.trace("Finished serving %s\n", path.c_str());
}

const char* KNXWebServer::getMimeType(const String& path) {
    for (const auto& mime : mimeTypes) {
        if (mime.extension && path.endsWith(mime.extension)) {
            return mime.type;
        }
    }
    return "text/plain";
}

void KNXWebServer::setCacheHeaders() {
    // Use build timestamp as ETag
    server.sendHeader("ETag", String(PIO_BUILD_TIME));
    
    // Cache for 1 hour
    server.sendHeader("Cache-Control", "public, max-age=3600");
    
    // Set Last-Modified to build time
    server.sendHeader("Last-Modified", String(PIO_BUILD_TIME));
}

#endif // NOWEB
