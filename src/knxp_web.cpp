#include "knxp_web.h"

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
    printf("Started web server\n");
    server.begin();
    return true;
}

void KNXWebServer::loop() {
    server.handleClient();
}

void KNXWebServer::handleFile(String path) {
    if (!path.startsWith("/")) {
        path = "/" + path;
    }
    printf("Request for %s\n", path.c_str());

    if (!LittleFS.exists(path)) {
        server.send(404, "text/plain", "File Not Found");
        return;
    }

    File file = LittleFS.open(path, "r");
    if (!file) {
        server.send(500, "text/plain", "Failed to open file");
        return;
    }

    const char* contentType = getContentType(path);
    WiFiClient client = server.client();
    
    // Send minimal HTTP headers
    client.print(F("HTTP/1.1 200 OK\r\n"));
    client.printf("Content-Type: %s\r\n", contentType);
    client.print(F("Transfer-Encoding: chunked\r\n"));
    client.print(F("Connection: close\r\n"));
    client.print(F("\r\n"));
    
    // Send file in chunks
    uint8_t buffer[CHUNK_SIZE];
    size_t totalBytesRead = 0;
    size_t fileSize = file.size();
    
    while (totalBytesRead < fileSize) {
        yield(); // Allow background tasks between chunks
        
        size_t bytesToRead = min(CHUNK_SIZE, (int)(fileSize - totalBytesRead));
        size_t bytesRead = file.read(buffer, bytesToRead);
        
        if (bytesRead == 0) {
            printf("Error reading file at position %d\n", totalBytesRead);
            break;
        }
        
        // Send chunk size
        client.printf("%x\r\n", bytesRead);  // Lowercase hex, no leading zeros
        
        // Send chunk data
        client.write(buffer, bytesRead);
        client.print(F("\r\n"));
        
        totalBytesRead += bytesRead;
        printf("Sent chunk: %d bytes\n", bytesRead);
        
        yield(); // Allow background tasks after sending chunk
    }
    
    // Send final empty chunk
    client.print(F("0\r\n\r\n"));
    client.flush();
    
    file.close();
    printf("File transfer complete: %d bytes\n", totalBytesRead);
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
