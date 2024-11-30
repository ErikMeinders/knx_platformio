#ifndef KNXP_WEBS_H
#define KNXP_WEBS_H

#ifdef FEATURE_WEBS

#ifdef ESP8266
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif

#include <WebSocketsServer.h>

/**
 * WebSocket server implementation for KNX platform.
 * 
 * Provides real-time communication capabilities with configurable message handling.
 * Uses the links2004/WebSockets library which is compatible with both ESP8266 and ESP32.
 */
class KNXWebSocketServer {
public:
    KNXWebSocketServer();
    bool begin();
    void loop();
    void broadcast(const char* payload);

private:
    WebSocketsServer server;
    void handleEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);
};

#endif // FEATURE_WEBS
#endif // KNXP_WEBS_H
