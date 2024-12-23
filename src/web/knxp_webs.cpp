#if defined(KNXP_FEATURE_WEB) && !defined(KNXP_USE_ASYNC_WEB)

#include "web/knxp_webs.h"
#include <ArduinoLog.h>

KNXWebSocketServer::KNXWebSocketServer(KNXWebServer& webServer)
    : webServer(webServer)
    , ws(81)  // WebSocket server port
    , messageHandler(nullptr)
    , connectedClients(0)
{
    Log.notice("Creating WebSocket server instance\n");
}

bool KNXWebSocketServer::begin() {
    Log.notice("Starting WebSocket server\n");
    
    ws.begin();
    ws.onEvent([this](uint8_t num, WStype_t type, uint8_t* payload, size_t length) {
        switch (type) {
            case WStype_CONNECTED:
                connectedClients++;
                Log.notice("Client #%u connected\n", num);
                break;
            case WStype_DISCONNECTED:
                if (connectedClients > 0) connectedClients--;
                Log.notice("Client #%u disconnected\n", num);
                break;
            case WStype_TEXT:
                if (messageHandler) {
                    messageHandler(num, payload, length);
                }
                break;
            case WStype_ERROR:
                Log.error("WebSocket error on client #%u\n", num);
                break;
            default:
                break;
        }
    });

    Log.notice("WebSocket server started\n");
    return true;
}

void KNXWebSocketServer::loop() {
    ws.loop();
}

void KNXWebSocketServer::broadcast(const char* payload) {
    if (!payload) {
        Log.error("Broadcast data is null\n");
        return;
    }

    ws.broadcastTXT(payload);
}

// Factory function implementation for sync version
KNXWebSocketServerBase* createWebSocketServer(KNXWebServerBase* webServer) {
    auto syncWebServer = static_cast<KNXWebServer*>(webServer);
    if (!syncWebServer) {
        Log.error("Web server not properly initialized\n");
        return nullptr;
    }

    // Create and return WebSocket server instance
    static KNXWebSocketServer* instance = nullptr;
    if (!instance) {
        instance = new KNXWebSocketServer(*syncWebServer);
        Log.notice("Created new WebSocket server instance\n");
    } else {
        Log.notice("Returning existing WebSocket server instance\n");
    }
    return instance;
}

#endif // KNXP_FEATURE_WEB && !KNXP_USE_ASYNC_WEB
