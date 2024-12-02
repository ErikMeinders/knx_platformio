#ifndef KNXP_WEBS_H
#define KNXP_WEBS_H

#ifdef FEATURE_WEBS

#ifdef ESP32
  #include <WebSocketsServer.h>
#else
  #include <WebSocketsServer.h>
#endif

// Maximum number of concurrent WebSocket connections
#define MAX_WS_CLIENTS 4

// Message handler function type
using WebSocketMessageHandler = std::function<void(uint8_t num, uint8_t * payload, size_t length)>;

/**
 * WebSocket server that supports application-level message handling
 */
class KNXWebSocketServer {
public:
    KNXWebSocketServer();
    bool begin();
    void loop();
    void broadcast(const char* payload);

    // Register a message handler for application-level processing
    void onMessage(WebSocketMessageHandler handler) {
        messageHandler = handler;
    }

    // Get current number of connected clients
    uint8_t getConnectedClients() const {
        return connectedClients;
    }

private:
    WebSocketsServer server;
    WebSocketMessageHandler messageHandler;  // Application message handler
    uint8_t connectedClients = 0;           // Track number of connected clients
    void handleEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
    void cleanupConnections();              // Cleanup stale connections
};

#endif // FEATURE_WEBS
#endif // KNXP_WEBS_H
