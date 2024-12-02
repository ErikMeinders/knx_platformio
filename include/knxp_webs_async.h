#ifndef KNXP_WEBS_ASYNC_H
#define KNXP_WEBS_ASYNC_H

#ifdef FEATURE_WEBS

#include "knxp_webs_base.h"
#include "knxp_web_async.h"
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
    AsyncWebSocket ws;
    WebSocketMessageHandler messageHandler;
    uint8_t connectedClients;
    KNXAsyncWebServer& webServer;

    void handleEvent(AsyncWebSocket* server, 
                    AsyncWebSocketClient* client, 
                    AwsEventType type, 
                    void* arg, 
                    uint8_t* data, 
                    size_t len);

    void cleanupConnections();
    void logEvent(AsyncWebSocketClient* client, AwsEventType type, const char* message = nullptr);
    void handleJsonMessage(AsyncWebSocketClient* client, const char* data, size_t len);
};

#ifdef USE_ASYNC_WEB
// Factory function implementation for async version
inline KNXWebSocketServerBase* createWebSocketServer() {
    auto webServer = static_cast<KNXAsyncWebServer*>(createWebServer());
    return new KNXAsyncWebSocketServer(*webServer);
}
#endif

#endif // FEATURE_WEBS
#endif // KNXP_WEBS_ASYNC_H
