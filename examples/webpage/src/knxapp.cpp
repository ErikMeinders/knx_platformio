#include "../include/knxapp.h"

DECLARE_TIMER(YourCodeShoutOut, 15);
DECLARE_TIMER(BroadcastValue, 1);

// Define the global instance
knxapp knxAppInstance;

// Define _knxApp as a reference to our instance
_knxapp& _knxApp = knxAppInstance;

void knxapp::setup()
{
    _knxapp::setup();  // Call base class implementation first
    Println("Your setup");

    // Initialize KNX group object
    setGroupObjectCount(1);
    knx.getGroupObject(1).dataPointType(DPT_Value_Temp);
    knx.getGroupObject(1).value(19.8);

    setCyclicTimer(15);

    // Initialize LittleFS
    if(!LittleFS.begin()) {
        Log.error("An error occurred while mounting LittleFS\n");
        return;
    }

    // Setup web server and WebSocket
    setupWebServer();
    
    // Initialize WebSocket server
    webSocket = new WebsocketsServer();
    webSocket->listen(81);
}

void knxapp::loop()
{
    _knxapp::loop();  // Call base class implementation first
    
    // Handle WebSocket connections
    WebsocketsClient client = webSocket->accept();
    if (client.available()) {
        Log.notice("New WebSocket client connected\n");
        client.onMessage([this](const WebsocketsMessage& message) {
            Log.notice("WebSocket received text: %s\n", message.data().c_str());
        });
        clients.push_back(std::move(client));
    }

    // Clean up disconnected clients
    clients.erase(
        std::remove_if(
            clients.begin(),
            clients.end(),
            [](WebsocketsClient& client) { return !client.available(); }
        ),
        clients.end()
    );

    // Handle remaining connections
    for (auto& client : clients) {
        client.poll();
    }
    
    server.handleClient();
    
    if (DUE(YourCodeShoutOut))
    {
        Log.verbose("Log Your loop %u\n", xx);
        xx = 0;
    }
    xx++;

    if (DUE(BroadcastValue))
    {
        broadcastKnxValue();
    }
}

void knxapp::status()
{
    _knxapp::status();  // Call base class implementation first
    Println("Your status");
}

void knxapp::setupWebServer()
{
    // Serve static files
    server.onNotFound([this]() {
        this->handleNotFound();
    });

    server.begin();
    Log.notice("HTTP server started\n");
}

String knxapp::getContentType(const String& filename)
{
    if (filename.endsWith(".html")) return "text/html";
    else if (filename.endsWith(".css")) return "text/css";
    else if (filename.endsWith(".js")) return "application/javascript";
    else if (filename.endsWith(".json")) return "application/json";
    else if (filename.endsWith(".ico")) return "image/x-icon";
    return "text/plain";
}

void knxapp::handleNotFound()
{
    String path = server.uri();
    if (path.endsWith("/")) path += "index.html";
    
    String contentType = getContentType(path);
    
    if (LittleFS.exists(path)) {
        File file = LittleFS.open(path, "r");
        server.streamFile(file, contentType);
        file.close();
        return;
    }
    
    server.send(404, "text/plain", "File not found");
}

void knxapp::handleWebSocket(WebsocketsClient& client, const WebsocketsMessage& message)
{
    Log.notice("WebSocket received text: %s\n", message.data().c_str());
}

void knxapp::broadcastKnxValue()
{
    float value = knx.getGroupObject(1).value();
    String json = "{\"temperature\":" + String(value, 1) + "}";
    for (auto& client : clients) {
        if (client.available()) {
            client.send(json);
        }
    }
}
