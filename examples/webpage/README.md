# KNX Temperature Monitor Example

This example demonstrates three different approaches to implementing WebSocket-based temperature monitoring for KNX devices using ESP8266/ESP32 boards.

## Features

- Real-time temperature updates via WebSocket
- Three different WebSocket connection methods:
  1. Standard WebSocket at `/ws` path
  2. WebSocket with explicit `/ws` prefix
  3. WebSocket on port 81
- Connection status monitoring
- Automatic reconnection handling
- Responsive design with CSS styling
- LittleFS filesystem for web files

## Hardware Requirements

Supported boards:
- ESP32-S3-DevKitC-1-N32R8 (default)
- ESP32 (Wemos D1 Mini32)
- ESP8266 (D1)

Additional requirements:
- KNX-IP interface connection

## Project Structure

```
/data/
├── index.html          # Standard WebSocket implementation
├── index-ws.html       # WebSocket with /ws prefix
├── index-port81.html   # Port 81 implementation
├── script.js           # Standard WebSocket connection
├── script-ws.js        # WebSocket with /ws prefix
├── script-port81.js    # Port 81 connection
└── style.css          # Shared styles

/src/
├── arduino.cpp         # Arduino framework initialization
└── knxapp.cpp         # Main application logic with WebSocket broadcasting
```

## Implementation Details

### Backend (knxapp.cpp)
- Broadcasts simulated temperature values every second
- Temperature range: 19.0°C to 25.0°C
- Uses JSON format for data transmission
- Includes KNX group object setup with DPT_Value_Temp

### Frontend
All three implementations provide:
- Real-time temperature display
- Connection status indicator
- Automatic reconnection (up to 5 attempts)
- Error handling and logging

#### Connection Methods
1. **Standard WebSocket** (index.html)
   - Connects to `/ws` path on the same port as HTTP
   - Default implementation

2. **Explicit WebSocket** (index-ws.html)
   - Uses explicit `/ws` prefix for connection
   - Useful for specific routing requirements

3. **Port 81** (index-port81.html)
   - Connects to WebSocket server on port 81
   - Demonstrates multi-port capability

## Setup Instructions

1. Configure platformio.ini:
   ```ini
   build_flags = 
       -DKNXP_FEATURE_WEB    ; Enable web server
       -DKNXP_FEATURE_WEBS   ; Enable WebSocket server
   board_build.filesystem = littlefs
   ```

2. Select your board environment in platformio.ini:
   - ESP32-S3: `webpage-esp32s3` (default)
   - ESP32: `webpage-esp32`
   - ESP8266: `webpage-esp8266`

3. Configure your network settings in the code

4. Upload the code to your board:
   ```bash
   pio run -t upload
   ```

5. Upload the filesystem:
   ```bash
   pio run -t uploadfs
   ```

## Build Commands

The project includes a Makefile with useful commands:

```bash
make all              # Build and upload both firmware and filesystem
make build-firmware   # Build and upload only the firmware
make build-filesystem # Build and upload only the filesystem
make force-firmware   # Force rebuild and upload firmware
make force-filesystem # Force rebuild and upload filesystem
make clean           # Clean build artifacts
make check           # Check what needs to be rebuilt
```

## Usage

Access the different implementations at:
- Standard: `http://[device-ip]/index.html`
- WS Prefix: `http://[device-ip]/index-ws.html`
- Port 81: `http://[device-ip]/index-port81.html`

## Notes

- All implementations include automatic reconnection logic
- Connection status is visually indicated
- Temperature updates occur every second
- WebSocket connections handle both HTTP and HTTPS (WSS) protocols
- Required feature flags: KNXP_FEATURE_WEB and KNXP_FEATURE_WEBS
- Web files are stored in LittleFS filesystem
