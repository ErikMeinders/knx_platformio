# KNX PlatformIO Library

## Async Mode Usage Guide

### 1. Project Configuration

Add the following to your `platformio.ini`:

```ini
[env]
framework = arduino
lib_deps = 
    knx_platformio
    me-no-dev/ESPAsyncTCP @ ^1.2.2
    me-no-dev/ESP Async WebServer @ ^1.2.3
    ArduinoJson @ ^7.1.0
    thijse/ArduinoLog @ ^1.1.1
    jandrassy/TelnetStream @ 1.2.5

build_flags = 
    -DFEATURE_WEB      ; Enable web server
    -DFEATURE_WEBS     ; Enable WebSocket server
    -DUSE_ASYNC_WEB    ; Use async mode
    -DMASK_VERSION=0x57B0  ; KNX mask version
    -DHOSTNAME='"knx-esp"' ; Device hostname
```

For ESP8266, also add:
```ini
[env:esp8266]
platform = espressif8266
board = d1
build_flags = 
    ${env.build_flags}
    -DESP8266
    -DPIO_FRAMEWORK_ARDUINO_LWIP2_HIGHER_BANDWIDTH
board_build.filesystem = littlefs
```

For ESP32:
```ini
[env:esp32]
platform = espressif32
board = wemos_d1_mini32
build_flags = 
    ${env.build_flags}
    -DESP32
board_build.filesystem = littlefs
```

### 2. Application Structure

Your application should inherit from `_knxapp`:

```cpp
// knxapp.h
class knxapp : public _knxapp {
public:
    void loop() override;
    void status() override;
    void knx_setup() override;
};
```

### 3. WebSocket Server Usage

The WebSocket server is automatically initialized in the base class. To use it in your application:

```cpp
// In your loop() implementation
if (this->webSocketServer) {
    // Create your JSON payload
    String json = "{\"key\":\"value\"}";
    
    // Broadcast to all connected clients
    this->webSocketServer->broadcast(json.c_str());
}
```

### 4. Client-Side Integration

Add WebSocket client in your HTML/JavaScript:

```javascript
const ws = new WebSocket('ws://' + window.location.hostname + '/ws');

ws.onopen = () => {
    console.log('WebSocket connected');
};

ws.onmessage = (event) => {
    const data = JSON.parse(event.data);
    // Handle received data
};
```

### 5. Important Notes

1. **Initialization Order**:
   - WiFi must be connected before web server starts
   - Web server must be initialized before WebSocket server
   - Both are handled automatically by the library
   - The sequence is managed in _knxapp::setup()

2. **Instance Management**:
   - Use `this->webSocketServer` in your application code
   - Don't create your own instances of web or WebSocket servers
   - The base class manages both global and instance variables

3. **Error Handling**:
   - Always check if `this->webSocketServer` is not null before using
   - WebSocket server might be null if initialization failed
   - Check logs for initialization status

### 6. Example Implementation

```cpp
// knxapp.cpp
#include "knxapp.h"

DECLARE_TIMER(BroadcastValue, 1);  // 1 second interval

void knxapp::loop() {
    _knxapp::loop();  // Call base class implementation first
    
    if (DUE(BroadcastValue)) {
        static float value = 0.0;
        String json = "{\"value\":" + String(value, 1) + "}";
        
        if (this->webSocketServer) {
            this->webSocketServer->broadcast(json.c_str());
        }
        
        value += 0.1;
        if (value > 100.0) value = 0.0;
    }
}

void knxapp::knx_setup() {
    // Initialize KNX group objects
    setGroupObjectCount(1);
    knx.getGroupObject(1).dataPointType(DPT_Value_Temp);
    knx.getGroupObject(1).value(20.0);
}
```

### 7. Debugging

The library uses ArduinoLog for debugging. Enable verbose logging in your setup:

```cpp
Log.begin(LOG_LEVEL_VERBOSE, &Serial);
```

Common debug messages to watch for:
- "Created async web server instance"
- "WebSocket handler registered at /ws"
- "WebSocket client connected/disconnected"
- "Broadcasting to X clients"

### 8. Migration from Sync Mode

If migrating from synchronous mode:

1. Update platformio.ini:
   - Add `USE_ASYNC_WEB` build flag
   - Add async library dependencies
   - Update board configuration if needed

2. Code Changes:
   - No changes needed if using base class properly
   - If manually managing servers, remove that code
   - Use `this->webSocketServer` instead of global variable

3. Common Issues:
   - WebSocket broadcasts not working: Check `this->webSocketServer` is set
   - Connection problems: Check initialization sequence in logs
   - Data not updating: Verify JSON format and client-side parsing

For a complete working example, see the `examples/webpage_async` directory in this repository.

### 9. File Structure

Required files for a minimal async implementation:

```
your_project/
├── platformio.ini
├── src/
│   ├── arduino.cpp    # Main entry point
│   └── knxapp.cpp     # Your application code
├── include/
│   └── knxapp.h       # Your application header
└── data/              # Web files served by LittleFS
    ├── index.html
    ├── style.css
    └── script.js
