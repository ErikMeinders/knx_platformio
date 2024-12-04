#if defined(FEATURE_WEBS) && !defined(USE_ASYNC_WEB)

#include "knxp_webs.h"
#include <Arduino.h>
#include <ArduinoLog.h>

KNXWebSocketServer::KNXWebSocketServer(KNXWebServer& webServer)
    : ws(81)  // Dedicated WebSocket port
    , connectedClients(0)
    , webServer(webServer)
{
    ws.begin();  // Start WebSocket server immediately
    ws.onEvent([this](uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
        handleEvent(num, type, payload, length);
    });

    // Enable ping/pong for connection monitoring
    ws.enableHeartbeat(15000, 3000, 2);
}

bool KNXWebSocketServer::begin() {
    Log.notice("WebSocket server started on port 81\n");
    return true;
}

void KNXWebSocketServer::loop() {
    ws.loop();
    
    static uint32_t lastCleanup = 0;
    uint32_t now = millis();
    
    if (now - lastCleanup > 30000) {
        cleanupConnections();
        lastCleanup = now;
    }
}

void KNXWebSocketServer::broadcast(const char* payload) {
    if (connectedClients > 0) {
        Log.notice("Broadcasting to %d clients: %s\n", connectedClients, payload);
        ws.broadcastTXT(payload);
    }
}

void KNXWebSocketServer::handleEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
    switch (type) {
        case WStype_CONNECTED:
            if (connectedClients >= MAX_WS_CLIENTS) {
                Log.warning("Max clients reached, connection refused\n");
                ws.disconnect(num);
                return;
            }
            connectedClients++;
            logEvent(num, type, "Connected");
            break;
            
        case WStype_DISCONNECTED:
            if (connectedClients > 0) {  // Guard against underflow
                connectedClients--;
            }
            logEvent(num, type, "Disconnected");
            break;
            
        case WStype_TEXT:
            if (messageHandler && length > 0) {
                Log.notice("Received message from client #%u: %.*s\n", num, length, payload);
                messageHandler(num, payload, length);
            }
            break;
            
        case WStype_ERROR:
            logEvent(num, type, "Error");
            break;
            
        case WStype_PING:
            logEvent(num, type, "Ping");
            break;
            
        case WStype_PONG:
            logEvent(num, type, "Pong");
            break;

        case WStype_BIN:
            Log.warning("Binary data not supported\n");
            break;

        default:
            Log.warning("Unknown WebSocket event type: %d\n", type);
            break;
    }
}

void KNXWebSocketServer::cleanupConnections() {
    Log.notice("WebSocket cleanup - Connected clients: %d, Free heap: %d\n", 
              connectedClients, ESP.getFreeHeap());
}

void KNXWebSocketServer::logEvent(uint8_t num, WStype_t type, const char* message) {
    if (message) {
        Log.notice("WebSocket client #%u - %s\n", num, message);
    }
}

#endif // FEATURE_WEBS && !USE_ASYNC_WEB
