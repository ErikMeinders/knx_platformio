#ifndef KNXP_WEBS_ASYNC_H
#define KNXP_WEBS_ASYNC_H

#ifdef KNXP_FEATURE_WEBS

#include "web/knxp_websocket_server_base.h"
#include "web/knxp_web_server_base.h"
#include "web/async/knxp_web_async.h"
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

// Maximum number of concurrent WebSocket connections
#define MAX_WS_CLIENTS 4

/**
 * Async WebSocket server implementation
 */
class KNXAsyncWebSocketServer : public KNXWebSocketServerBase {
public:
    KNXAsyncWebSocketServer(KNXAsyncWebServer& webServer);
    virtual ~KNXAsyncWebSocketServer() {}

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
    KNXAsyncWebServer& webServer;  // Reference to web server must be first
    AsyncWebSocket ws;             // WebSocket instance
    WebSocketMessageHandler messageHandler;
    volatile uint8_t connectedClients;
    unsigned long lastBroadcastTime;  // For rate limiting broadcasts

    void handleEvent(AsyncWebSocket* server, 
                    AsyncWebSocketClient* client, 
                    AwsEventType type,
                    void* arg, 
                    uint8_t* data, 
                    size_t len);
    void cleanupConnections();
    void logEvent(AsyncWebSocketClient* client, AwsEventType type, const char* message = nullptr);
    void processMessage(AsyncWebSocketClient* client, uint8_t* data, size_t len);
    void sendInitialState(AsyncWebSocketClient* client);
};

#ifdef KNXP_USE_ASYNC_WEB
// Factory function declaration for async version
KNXWebSocketServerBase* createWebSocketServer(KNXWebServerBase* webServer);
#endif

#endif // KNXP_FEATURE_WEBS
#endif // KNXP_WEBS_ASYNC_H
