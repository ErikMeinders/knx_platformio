#include "knxp_webs.h"
#include <ArduinoLog.h>

#ifdef FEATURE_WEBS

KNXWebSocketServer::KNXWebSocketServer() : server(81) {}

bool KNXWebSocketServer::begin() {
    server.onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        this->handleEvent(num, type, payload, length);
    });
    server.begin();
    Log.notice("Started WebSocket server on port 81\n");
    return true;
}

void KNXWebSocketServer::loop() {
    server.loop();
}

void KNXWebSocketServer::broadcast(const char* payload) {
    Log.notice("Broadcasting: %s\n", payload);
    server.broadcastTXT(payload);
}

void KNXWebSocketServer::handleEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    switch(type) {
        case WStype_DISCONNECTED:
            Log.notice("WebSocket client [%u] disconnected\n", num);
            break;
            
        case WStype_CONNECTED:
            {
                Log.notice("WebSocket client [%u] connected\n", num);
                // Optional: Send initial state to newly connected client
                // server.sendTXT(num, "Connected to KNX WebSocket Server");
            }
            break;
            
        case WStype_TEXT:
            if (length > 0) {
                Log.notice("WebSocket received text from [%u]: %s\n", num, (char*)payload);
                // Echo back to sender
                server.sendTXT(num, (char*)payload);
            }
            break;
            
        case WStype_ERROR:
            Log.error("WebSocket error on client [%u]\n", num);
            break;
            
        default:
            break;
    }
}

#endif // FEATURE_WEBS
