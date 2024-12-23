#if defined(KNXP_FEATURE_WEB) && defined(KNXP_USE_ASYNC_WEB)

#include "web/async/knxp_webs_async.h"
#include <ArduinoLog.h>
#include <ArduinoJson.h>

KNXAsyncWebSocketServer::KNXAsyncWebSocketServer(KNXAsyncWebServer& webServer)
    : webServer(webServer)
    , ws("/ws")
    , messageHandler(nullptr)
    , connectedClients(0)
    , lastBroadcastTime(0)
{
    Log.notice("Creating WebSocket server instance\n");
    
    // Set up WebSocket event handler
    ws.onEvent([this](AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
        handleEvent(server, client, type, arg, data, len);
    });

    // Clean start
    ws.cleanupClients();
    
    Log.notice("WebSocket event handler registered\n");
}

bool KNXAsyncWebSocketServer::begin() {
    Log.notice("Starting async WebSocket server\n");
    
    AsyncWebServer* server = webServer.getServer();
    if (!server) {
        Log.error("Web server not initialized\n");
        return false;
    }
    
    // Register WebSocket handler
    server->addHandler(&ws);
    
    Log.notice("WebSocket handler registered at /ws\n");
    return true;
}

void KNXAsyncWebSocketServer::loop() {
    // No active processing needed for async server
}

void KNXAsyncWebSocketServer::broadcast(const char* data) {
    if (!data) {
        Log.error("Broadcast data is null\n");
        return;
    }

    size_t len = strlen(data);
    if (len == 0 || len > 2048) {  // Sanity check message size
        Log.error("Invalid broadcast data length: %u\n", len);
        return;
    }

    // Rate limit broadcasts
    unsigned long now = millis();
    if (now - lastBroadcastTime < 100) { // Minimum 100ms between broadcasts
        return;
    }
    lastBroadcastTime = now;

    connectedClients = ws.count();
    if (connectedClients > 0) {
        // For large messages, use async message queue
        if (len > 512) {
            // Create a copy of the data that will persist until sent
            char* asyncData = new char[len + 1];
            memcpy(asyncData, data, len);
            asyncData[len] = '\0';
            
            // Send data and clean up after sending
            ws.textAll(asyncData, len);
            delete[] asyncData;
        } else {
            // Small messages can be sent directly
            ws.textAll(data, len);
        }
    }
}

void KNXAsyncWebSocketServer::handleEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, 
                                        AwsEventType type, void *arg, uint8_t *data, size_t len) {
    if (!client) {
        return;
    }

    switch (type) {
        case WS_EVT_CONNECT: {
            connectedClients++;
            Log.notice("Client #%u connected from %s\n", 
                      client->id(), client->remoteIP().toString().c_str());
            
            // Send initial state
            static const char* initialState = "{\"type\":\"initial\",\"timestamp\":0}";
            client->text(initialState);
            break;
        }
        case WS_EVT_DISCONNECT: {
            if (connectedClients > 0) {
                connectedClients--;
            }
            Log.notice("Client #%u disconnected\n", client->id());
            break;
        }
        case WS_EVT_DATA: {
            AwsFrameInfo *info = (AwsFrameInfo*)arg;
            if (messageHandler && len > 0 && len <= 2048 &&  // Size limit check
                info->final && info->index == 0 && info->len == len) {
                // Complete message received
                messageHandler(client->id(), data, len);
            }
            break;
        }
        case WS_EVT_ERROR: {
            Log.error("WebSocket error on client #%u\n", client->id());
            break;
        }
        default:
            break;
    }
}

// Factory function implementation for async version
KNXWebSocketServerBase* createWebSocketServer(KNXWebServerBase* webServer) {
    auto asyncWebServer = static_cast<KNXAsyncWebServer*>(webServer);
    if (!asyncWebServer || !asyncWebServer->getServer()) {
        Log.error("Web server not properly initialized\n");
        return nullptr;
    }

    // Create and return WebSocket server instance
    static KNXAsyncWebSocketServer* instance = nullptr;
    if (!instance) {
        instance = new KNXAsyncWebSocketServer(*asyncWebServer);
        Log.notice("Created new WebSocket server instance\n");
    } else {
        Log.notice("Returning existing WebSocket server instance\n");
    }
    return instance;
}

#endif // KNXP_FEATURE_WEB && KNXP_USE_ASYNC_WEB
