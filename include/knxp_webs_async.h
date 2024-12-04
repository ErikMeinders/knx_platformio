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

#ifdef USE_ASYNC_WEB
// Factory function implementation for async version
inline KNXWebSocketServerBase* createWebSocketServer() {
    // Get the web server instance
    extern KNXWebServerBase* webServer;
    if (!webServer) {
        Log.error("Web server not initialized when creating WebSocket server\n");
        return nullptr;
    }

    // Verify web server is initialized
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
#endif

#endif // FEATURE_WEBS
#endif // KNXP_WEBS_ASYNC_H
