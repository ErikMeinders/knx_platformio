# KNX Temperature Monitor Web Interface

This directory contains two versions of the web interface for different WebSocket implementations:

## 1. WebSocket with /ws prefix (index-ws.html)
- Uses the standard HTTP port with a `/ws` path prefix for WebSocket connections
- Suitable for ESP32 using standard WebServer
- Files:
  - `index-ws.html`: Main HTML file
  - `script-ws.js`: WebSocket client implementation

## 2. WebSocket on Port 81 (index-port81.html)
- Uses a dedicated WebSocket server on port 81
- Suitable for ESP8266 or when using AsyncWebServer
- Files:
  - `index-port81.html`: Main HTML file
  - `script-port81.js`: WebSocket client implementation

## Usage
1. Choose the appropriate HTML file based on your ESP device and web server implementation:
   - For standard WebServer with `/ws` prefix: Use `index-ws.html`
   - For dedicated WebSocket server on port 81: Use `index-port81.html`
2. Upload the corresponding files to your device's filesystem
3. Access the web interface through your device's IP address

Both versions provide the same functionality:
- Real-time temperature display
- Connection status indication
- Automatic reconnection attempts
- JSON message parsing
