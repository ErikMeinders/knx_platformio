#include "knxp_webs_async.h"

#ifdef FEATURE_WEBS

KNXAsyncWebSocketServer::KNXAsyncWebSocketServer(KNXAsyncWebServer& webServer)
    : ws("/ws"), connectedClients(0), webServer(webServer) {
    
    // Set up WebSocket event handler
    ws.onEvent([this](AsyncWebSocket* server, AsyncWebSocketClient* client, 
                     AwsEventType type, void* arg, uint8_t* data, size_t len) {
        this->handleEvent(server, client, type, arg, data, len);
    });

    // Add WebSocket handler to web server
    auto server = webServer.getServer();
    server->addHandler(&ws);
}

bool KNXAsyncWebSocketServer::begin() {
    Log.notice("WebSocket server started on port 81\n");
    return true;
}

void KNXAsyncWebSocketServer::loop() {
    static unsigned long lastCleanup = 0;
    const unsigned long CLEANUP_INTERVAL = 30000; // 30 seconds

    // Periodic cleanup
    unsigned long now = millis();
    if (now - lastCleanup >= CLEANUP_INTERVAL) {
        cleanupConnections();
        lastCleanup = now;
    }
}

void KNXAsyncWebSocketServer::broadcast(const char* payload) {
    if (connectedClients > 0) {
        ws.textAll(payload);
    }
}

void KNXAsyncWebSocketServer::handleEvent(AsyncWebSocket* server, 
                                        AsyncWebSocketClient* client, 
                                        AwsEventType type, 
                                        void* arg, 
                                        uint8_t* data, 
                                        size_t len) {
    switch (type) {
        case WS_EVT_CONNECT:
            if (connectedClients < MAX_WS_CLIENTS) {
                connectedClients++;
                logEvent(client, type, "Client connected");
            } else {
                // Too many clients, close connection
                client->close();
                Log.warning("Max clients reached, connection refused\n");
            }
            break;

        case WS_EVT_DISCONNECT:
            connectedClients--;
            logEvent(client, type, "Client disconnected");
            break;

        case WS_EVT_ERROR:
            logEvent(client, type, "Error");
            break;

        case WS_EVT_PONG:
            logEvent(client, type, "Pong");
            break;

        case WS_EVT_DATA:
            AwsFrameInfo* info = (AwsFrameInfo*)arg;
            if (info->final && info->index == 0 && info->len == len) {
                // Complete message received
                if (info->opcode == WS_TEXT) {
                    // Null terminate the data for string handling
                    data[len] = 0;
                    handleJsonMessage(client, (char*)data, len);
                }
            }
            break;
    }
}

void KNXAsyncWebSocketServer::cleanupConnections() {
    ws.cleanupClients();
    
    // Force garbage collection
    ESP.getFreeHeap();
    Log.notice("WebSocket cleanup - Free heap: %d\n", ESP.getFreeHeap());
}

void KNXAsyncWebSocketServer::logEvent(AsyncWebSocketClient* client, 
                                     AwsEventType type, 
                                     const char* message) {
    if (message) {
        Log.notice("WebSocket client #%u - %s\n", client->id(), message);
    }
}

void KNXAsyncWebSocketServer::handleJsonMessage(AsyncWebSocketClient* client, 
                                              const char* data, 
                                              size_t len) {
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, data, len);

    if (error) {
        Log.error("WebSocket JSON parse failed: %s\n", error.c_str());
        return;
    }

    // Handle ping messages for response time measurement
    if (doc["type"] == "ping") {
        client->text("{\"type\":\"pong\"}");
        return;
    }

    // Forward message to application handler if registered
    if (messageHandler) {
        messageHandler(client->id(), (uint8_t*)data, len);
    }
}

#endif // FEATURE_WEBS
