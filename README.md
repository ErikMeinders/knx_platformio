# KNX PlatformIO Library

A comprehensive platform for building KNX applications for ESP8266/ESP32 devices with advanced networking, web interfaces, OTA updates, and monitoring capabilities.

## Features

- KNX IP routing and tunneling
- Web interface with optional WebSocket support
- OTA updates
- Network monitoring and management
- Telnet debugging
- Heartbeat monitoring
- Async and sync web server support

## Basic Usage

1. Create a project using PlatformIO
2. Add the library to your `platformio.ini`:
```ini
lib_deps = 
    knx_platformio
```

3. Create your application class:
```cpp
// knxapp.h
class knxapp : public _knxapp {
public:
    void loop() override;
    void knx_setup() override;
};
```

## Compile Options

The library supports various compile-time options to customize functionality. The base KNX functionality requires no options - use NO_* options to strip features, and FEATURE_* options to add them.

### Feature Disabling Options

- `-DKNXP_NO_WIFI`: Disable WiFi functionality
- `-DKNXP_NO_HEARTBEAT`: Disable heartbeat monitoring
- `-DKNXP_NO_TELNET`: Disable Telnet debugging
- `-DKNXP_NO_NTP`: Disable NTP time synchronization
- `-DKNXP_NO_MDNS`: Disable mDNS service discovery
- `-DKNXP_NO_OTA`: Disable OTA updates
- `-DKNXP_NO_KNX`: Disable KNX functionality (for testing)

### Feature Enabling Options

- `-DKNXP_FEATURE_WEB`: Enable web server
- `-DKNXP_FEATURE_WEBS`: Enable WebSocket server (requires KNXP_FEATURE_WEB)
- `-DKNXP_USE_ASYNC_WEB`: Use async web server instead of sync

### Configuration Options

- `-DKNXP_HOSTNAME="device-name"`: Set device hostname
- `-DKNXP_NETWORK_ONDEMAND`: Enable on-demand network connectivity
- `-DKNXP_PROG_PIN=0`: Set programming mode pin
- `-DKNXP_NEOPIXEL_PIN=x`: Configure NeoPixel LED pin

## Examples

The library includes several example projects:

- `examples/basic/`: Minimal KNX application
- `examples/webpage/`: Web interface with sync server
- `examples/webpage_async/`: Web interface with async server

Each example demonstrates different library features and configurations.

## Project Structure

Minimal project structure:
```
your_project/
├── platformio.ini
├── src/
│   ├── arduino.cpp    # Main entry point
│   └── knxapp.cpp     # Your application code
└── include/
    └── knxapp.h       # Your application header
```

For web interface projects, add:
```
your_project/
└── data/              # Web files served by LittleFS
    ├── index.html
    ├── style.css
    └── script.js
```

## Example platformio.ini

```ini
[env]
framework = arduino
lib_deps = 
    knx_platformio

build_flags = 
    -DKNXP_HOSTNAME='"knx-device"'
    -DKNXP_FEATURE_WEB
    -DKNXP_FEATURE_WEBS
    -DMASK_VERSION=0x57B0

[env:esp32]
platform = espressif32
board = esp32dev
build_flags = 
    ${env.build_flags}
    -DESP32
board_build.filesystem = littlefs
```

## Documentation

For more detailed documentation and examples, visit the [examples](examples/) directory in this repository.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
