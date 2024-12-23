#ifndef KNXP_WEBS_H
#define KNXP_WEBS_H

#if defined(KNXP_FEATURE_WEB) && !defined(KNXP_USE_ASYNC_WEB)

#include "web/knxp_websocket_server_base.h"
#include "web/knxp_web.h"

#include <WebSocketsServer.h>

/**
 * Synchronous WebSocket server implementation
 */
class KNXWebSocketServer : public KNXWebSocketServerBase {
public:
    KNXWebSocketServer(KNXWebServer& webServer);
    virtual ~KNXWebSocketServer() {}

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
    KNXWebServer& webServer;  // Reference to web server must be first
    WebSocketsServer ws;      // WebSocket server instance
    WebSocketMessageHandler messageHandler;
    volatile uint8_t connectedClients;
};

#endif // KNXP_FEATURE_WEB && !KNXP_USE_ASYNC_WEB
#endif // KNXP_WEBS_H
