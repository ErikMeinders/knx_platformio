# KNX PlatformIO Web Example

This example demonstrates how to create a web interface with real-time updates using WebSocket communication.

## Features

- Web server serving static files from LittleFS
- WebSocket server for real-time updates
- Temperature value broadcasting
- Responsive file serving with proper chunked transfer

## Setup

1. Configure platformio.ini:
```ini
[env:webpage-esp8266]
platform = espressif8266
board = d1
framework = arduino
build_flags = 
    -DFEATURE_WEB      ; Enable web server
    -DFEATURE_WEBS     ; Enable WebSocket server
```

2. Create your application class:
```cpp
class knxapp : public _knxapp {
public:
    void loop() override {
        _knxapp::loop();  // Call base class implementation first
        
        if (DUE(BroadcastValue)) {
            static float temp = 19.0;
            String json = "{\"temperature\":" + String(temp, 1) + "}";
            _knxapp::webSocketServer.broadcast(json.c_str());
            
            // Update temperature for next broadcast
            temp += 0.1;
            if (temp > 25.0) temp = 19.0;
        }
    }
};
```

3. Prepare your web files:
- Place HTML, CSS, and JavaScript files in the `data` directory
- Upload filesystem using `make force-filesystem`

## Web Interface

1. HTML (data/index.html):
```html
<!DOCTYPE html>
<html>
<head>
    <title>KNX Temperature</title>
    <link rel="stylesheet" href="style.css">
</head>
<body>
    <div id="temperature">--.-°C</div>
    <script src="script.js"></script>
</body>
</html>
```

2. JavaScript (data/script.js):
```javascript
let ws = null;
const wsPort = 81;

function connect() {
    ws = new WebSocket(`ws://${window.location.hostname}:${wsPort}`);
    
    ws.onmessage = function(event) {
        const data = JSON.parse(event.data);
        document.getElementById('temperature').textContent = 
            `${data.temperature.toFixed(1)}°C`;
    };
    
    ws.onclose = function() {
        setTimeout(connect, 1000);  // Reconnect after 1 second
    };
}

connect();
```

## Implementation Details

1. Web Server:
- Serves static files from LittleFS
- Uses chunked transfer encoding for efficient delivery
- Maintains system responsiveness with regular yield() calls

2. WebSocket Server:
- Runs on port 81
- Broadcasts temperature updates every second
- Handles client connections automatically

## Notes

- Enable both FEATURE_WEB and FEATURE_WEBS in your build flags
- Upload filesystem after making changes to web files
- WebSocket server runs on port 81 to avoid conflicts with web server
