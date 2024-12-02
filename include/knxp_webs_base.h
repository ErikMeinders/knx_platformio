#ifndef KNXP_WEBS_BASE_H
#define KNXP_WEBS_BASE_H

#ifdef FEATURE_WEBS

#include <functional>
#include <ArduinoLog.h>

// Message handler function type
using WebSocketMessageHandler = std::function<void(uint8_t num, uint8_t * payload, size_t length)>;

/**
 * Base interface for WebSocket server functionality.
 * This allows switching between sync and async implementations
 * based on compile flags.
 */
class KNXWebSocketServerBase {
public:
    virtual ~KNXWebSocketServerBase() = default;
    virtual bool begin() = 0;
    virtual void loop() = 0;
    virtual void broadcast(const char* payload) = 0;
    virtual void onMessage(WebSocketMessageHandler handler) = 0;
    virtual uint8_t getConnectedClients() const = 0;
};

// Forward declare the factory function
KNXWebSocketServerBase* createWebSocketServer();

#endif // FEATURE_WEBS
#endif // KNXP_WEBS_BASE_H
