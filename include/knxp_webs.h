#ifndef KNXP_WEBS_H
#define KNXP_WEBS_H

#if defined(FEATURE_WEBS) && !defined(USE_ASYNC_WEB)

#include <Arduino.h>
#include "knxp_webs_base.h"
#include "knxp_web.h"
#include <WebSocketsServer.h>

// Maximum number of concurrent WebSocket connections
#define MAX_WS_CLIENTS 4

/**
 * Synchronous WebSocket server implementation
 */
class KNXWebSocketServer : public KNXWebSocketServerBase {
public:
    KNXWebSocketServer(KNXWebServer& webServer);
    bool begin() override;
    void loop() override;
    void broadcast(const char* payload) override;
    void onMessage(WebSocketMessageHandler handler) override {
        messageHandler = handler;
    }
    uint8_t getConnectedClients() const override {
        return connectedClients;
    }

private:
    WebSocketsServer ws;
    WebSocketMessageHandler messageHandler;
    uint8_t connectedClients;
    KNXWebServer& webServer;

    void handleEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
    void cleanupConnections();
    void logEvent(uint8_t num, WStype_t type, const char* message = nullptr);
};

// Factory function implementation for sync version
inline KNXWebSocketServerBase* createWebSocketServer() {
    auto webServer = static_cast<KNXWebServer*>(createWebServer());
    return new KNXWebSocketServer(*webServer);
}

#endif // FEATURE_WEBS && !USE_ASYNC_WEB
#endif // KNXP_WEBS_H
