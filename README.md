# KNX PlatformIO Library

A comprehensive platform for building KNX applications for ESP8266/ESP32 devices. This library provides a robust foundation for developing KNX-enabled IoT devices with features like OTA updates, network management, and monitoring capabilities.

## Features

- **Multi-Platform Support**: Compatible with ESP8266 and ESP32
- **Network Management**:
  - WiFi configuration via WiFiManager
  - MDNS support
  - Telnet debugging
  - HTTP server capabilities
- **KNX Integration**:
  - Full KNX protocol support
  - Programming mode handling
  - Cyclic updates
- **System Features**:
  - OTA (Over-The-Air) updates
  - Heartbeat monitoring
  - Time synchronization (NTP)
  - Serial debugging
  - Telnet monitoring

## Installation

1. Add this library to your PlatformIO project:
   ```ini
   lib_deps =
       https://github.com/ErikMeinders/knx_platformio.git
   ```

2. Include the main header in your code:
   ```cpp
   #include "knxp_platformio.h"
   ```

## Configuration

### Build Flags

The library supports various build flags to customize functionality:

- `NO_WIFI`: Disable WiFi functionality
- `NO_NTP`: Disable NTP time synchronization
- `NO_TELNET`: Disable Telnet debugging
- `NO_KNX`: Disable KNX functionality
- `NO_MDNS`: Disable MDNS support
- `NO_OTA`: Disable OTA updates
- `NO_HTTP`: Disable HTTP server
- `NO_HEARTBEAT`: Disable heartbeat monitoring

### Platform-Specific Settings

#### ESP8266
```ini
[env:esp8266]
platform = espressif8266
board = d1
```

#### ESP32
```ini
[env:esp32]
platform = espressif32
board = wemos_d1_mini32
```

## Usage

1. Create a new KNX application class:
```cpp
class MyKnxApp : public KnxApp {
public:
    void setup() override {
        // Your setup code
    }
    
    void loop() override {
        // Your loop code
    }
    
    void status() override {
        // Your status update code
    }
};
```

2. Initialize the application:
```cpp
MyKnxApp myApp;
```

## Dependencies

- [ArduinoLog](https://github.com/thijse/Arduino-Log)
- [Profiler](https://github.com/erikmeinders/profiler)
- [TelnetStream](https://github.com/jandrassy/TelnetStream)
- [KNX](https://github.com/thelsing/knx)
- [WiFiManager](https://github.com/tzapu/WiFiManager)

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.
