#ifndef KNXAPP_H
#define KNXAPP_H

#include <knxp_platformio.h>
#include <ArduinoWebsockets.h>
#include <LittleFS.h>

#ifdef ESP8266
#include <ESP8266WebServer.h>
using WebServer = ESP8266WebServer;
#else
#include <WebServer.h>
#endif

using namespace websockets;

// Application class that inherits from _knxapp
class knxapp : public _knxapp
{
  public:
    // Override methods to provide app-specific implementations
    void setup() override;
    void loop() override;
    void status() override;

  private:
    unsigned long xx = 0;
    WebServer server{80};
    WebsocketsServer* webSocket;
    std::vector<WebsocketsClient> clients;
    
    void setupWebServer();
    void handleWebSocket(WebsocketsClient& client, const WebsocketsMessage& message);
    void broadcastKnxValue();
    
    static String getContentType(const String& filename);
    void handleNotFound();
};

// Declare the global instance
extern knxapp knxAppInstance;

#endif
