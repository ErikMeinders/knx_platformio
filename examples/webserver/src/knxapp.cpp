#include "knxapp.h"
#include <ArduinoJson.h>

// Define the global instance
knxapp knxAppInstance;

// Define _knxApp as a reference to our instance
_knxapp& _knxApp = knxAppInstance;

#ifndef NOWEB
// Web server instance
KNXWebServer webServer;

// API handler for status
void handleStatus(WebServerClass& server) {
    JsonDocument doc;
    doc["uptime"].set(millis() / 1000);
    doc["heap"].set(ESP.getFreeHeap());
    doc["build_time"].set(PIO_BUILD_TIME);
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}

// API handler for KNX value
void handleKnxValue(WebServerClass& server) {
    JsonDocument doc;
    float value = (float)knx.getGroupObject(1).value();
    doc["value"].set(value);
    
    String response;
    serializeJson(doc, response);
    server.send(200, "application/json", response);
}
#endif

void knxapp::setup()
{
    _knxapp::setup();  // Call base class implementation first

    // Initialize KNX group object
    setGroupObjectCount(1);
    knx.getGroupObject(1).dataPointType(DPT_Value_Temp);
    knx.getGroupObject(1).value(19.8);

#ifndef NOWEB
    // Initialize web server
    if (!webServer.begin()) {
        Log.error("Failed to start web server\n");
        return;
    }

    // Add API endpoints
    webServer.addAPIHandler("/api/v1/status", HTTP_GET, handleStatus);
    webServer.addAPIHandler("/api/v1/knx/value", HTTP_GET, handleKnxValue);
    
    Log.notice("Web server initialized\n");
#endif
}

void knxapp::loop()
{
    _knxapp::loop();  // Call base class implementation first

#ifndef NOWEB
    webServer.loop();
#endif
}

void knxapp::status()
{
    _knxapp::status();  // Call base class implementation first
    Printf("Example web server status\n");
    
#ifndef NOWEB
    Printf("Web server active: Yes\n");
    Printf("Build timestamp: %s\n", PIO_BUILD_TIME);
#else
    Printf("Web server active: No (NOWEB defined)\n");
#endif
}
