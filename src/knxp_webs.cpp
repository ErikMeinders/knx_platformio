#include "knxp_webs.h"
#include "knxp_yield.h"
#include <ArduinoLog.h>

#ifdef FEATURE_WEBS

KNXWebSocketServer::KNXWebSocketServer() : server(81), connectedClients(0) {}

bool KNXWebSocketServer::begin() {
    server.onEvent([this](uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
        this->handleEvent(num, type, payload, length);
    });
    server.begin();
    Log.notice("Started WebSocket server on port 81\n");
    return true;
}

void KNXWebSocketServer::loop() {
    static unsigned long lastYield = 0;
    static unsigned long lastCleanup = 0;
    const unsigned long CLEANUP_INTERVAL = 30000; // 30 seconds

    knxLoopYield(lastYield);

    // Periodic cleanup
    unsigned long now = millis();
    if (now - lastCleanup >= CLEANUP_INTERVAL) {
        cleanupConnections();
        lastCleanup = now;
    }

    server.loop();
}

void KNXWebSocketServer::cleanupConnections() {
    // Force memory cleanup
    ESP.getFreeHeap(); // This call can help trigger garbage collection
    
    // Log memory status
    Log.notice("Heap: %d, Clients: %d\n", ESP.getFreeHeap(), connectedClients);
}

void KNXWebSocketServer::broadcast(const char* payload) {
    if (connectedClients == 0) return; // Skip if no clients

    Log.notice("Broadcasting to %u clients: %s\n", connectedClients, payload);
    
    // Use yield guard for potentially large broadcasts
    KnxYieldGuard yieldGuard;
    yieldGuard.check();
    
    server.broadcastTXT(payload);
    
    yieldGuard.check();
}

void KNXWebSocketServer::handleEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
    // Use yield guard for event handling
    KnxYieldGuard yieldGuard;
    
    switch(type) {
        case WStype_DISCONNECTED:
            Log.notice("WebSocket client [%u] disconnected\n", num);
            if (connectedClients > 0) connectedClients--;
            break;
            
        case WStype_CONNECTED:
            {
                // Check if we're at the connection limit
                if (connectedClients >= MAX_WS_CLIENTS) {
                    Log.warning("Connection limit reached, disconnecting client [%u]\n", num);
                    server.disconnect(num);
                    return;
                }
                
                Log.notice("WebSocket client [%u] connected (total: %u)\n", num, connectedClients + 1);
                connectedClients++;

                // Force cleanup if we're getting close to the limit
                if (connectedClients >= (MAX_WS_CLIENTS - 1)) {
                    cleanupConnections();
                }
            }
            break;
            
        case WStype_TEXT:
            if (length > 0) {
                Log.notice("WebSocket received text from [%u]: %s\n", num, (char*)payload);
                yieldGuard.check();
                
                // Forward message to application handler if registered
                if (messageHandler) {
                    messageHandler(num, payload, length);
                }
            }
            break;
            
        case WStype_ERROR:
            Log.error("WebSocket error on client [%u]\n", num);
            // Disconnect client on error to free resources
            server.disconnect(num);
            if (connectedClients > 0) connectedClients--;
            break;
            
        default:
            break;
    }
    
    yieldGuard.check();
}

#endif // FEATURE_WEBS
