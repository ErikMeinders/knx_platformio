# KNX Temperature Monitor Web Interface Files

This directory contains three different implementations of the web interface demonstrating different WebSocket connection methods:

## 1. Standard WebSocket (index.html)
- Uses the standard HTTP port with `/ws` path
- Default implementation
- Files:
  - `index.html`: Main HTML file
  - `script.js`: WebSocket client implementation

## 2. WebSocket with /ws prefix (index-ws.html)
- Uses explicit `/ws` path prefix for WebSocket connections
- Useful for specific routing requirements
- Files:
  - `index-ws.html`: Main HTML file
  - `script-ws.js`: WebSocket client implementation

## 3. WebSocket on Port 81 (index-port81.html)
- Uses a dedicated WebSocket server on port 81
- Demonstrates multi-port capability
- Files:
  - `index-port81.html`: Main HTML file
  - `script-port81.js`: WebSocket client implementation

## Shared Files
- `style.css`: Common styles for all implementations

## Implementation Details

All versions provide identical functionality:
- Real-time temperature display (19.0°C to 25.0°C range)
- Connection status indication with visual feedback
- Automatic reconnection (up to 5 attempts)
- Error handling and console logging
- JSON message parsing for temperature updates

## Usage

Access the different implementations at:
- Standard: `http://[device-ip]/index.html`
- WS Prefix: `http://[device-ip]/index-ws.html`
- Port 81: `http://[device-ip]/index-port81.html`

## Notes
- All implementations support both HTTP and HTTPS (WSS)
- Temperature updates occur every second
- Connection status is visually indicated with color changes
- Each implementation demonstrates a different approach to WebSocket connectivity
