#ifndef KNXP_WEBSOCKET_SERVER_BASE_H
#define KNXP_WEBSOCKET_SERVER_BASE_H

#include <Arduino.h>
#include <functional>

// Forward declarations
class KNXWebServerBase;

// Message handler type for WebSocket messages
using WebSocketMessageHandler = std::function<void(uint8_t num, uint8_t* payload, size_t length)>;

/**
 * Base class for WebSocket server implementations.
 * Provides common interface for both synchronous and asynchronous servers.
 */
class KNXWebSocketServerBase {
public:
    virtual ~KNXWebSocketServerBase() = default;
    
    /**
     * Initialize and start the WebSocket server
     * @return true if server started successfully
     */
    virtual bool begin() = 0;
    
    /**
     * Process server tasks in main loop
     */
    virtual void loop() = 0;
    
    /**
     * Broadcast message to all connected clients
     * @param payload Message to broadcast
     */
    virtual void broadcast(const char* payload) = 0;
    
    /**
     * Set message handler callback
     * @param handler Function to handle incoming messages
     */
    virtual void onMessage(WebSocketMessageHandler handler) = 0;
    
    /**
     * Get number of connected clients
     * @return Number of clients
     */
    virtual uint8_t getConnectedClients() const = 0;
};

#ifdef KNXP_FEATURE_WEBS
// Factory function declaration
KNXWebSocketServerBase* createWebSocketServer(KNXWebServerBase* webServer);
#endif

#endif // KNXP_WEBSOCKET_SERVER_BASE_H
