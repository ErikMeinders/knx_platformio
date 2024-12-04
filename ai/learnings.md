# Learnings

## ESP32 Support (2024-01-09)

### Key Principles

1. Keep ESP8266 Code Intact:
   - Existing ESP8266 code works and should not be modified
   - Add ESP32 support through platform-specific guards
   - Use #ifdef ESP8266 / #elif defined(ESP32) pattern

2. Platform Differences:
   - ESP8266: Uses ESPAsyncTCP and ESPAsyncWebServer
   - ESP32: Uses AsyncTCP and ESPAsyncWebServer
   - WiFi API differences (hostname vs setHostname)
   - Different include paths for WiFi libraries

3. Async Wrapper Strategy:
   - Keep ESP8266 HTTP method handling intact
   - Add separate ESP32 section that doesn't need HTTP method handling
   - Use platform-specific guards to include correct libraries

### Implementation Details

1. Network Layer:
   ```cpp
   #ifdef ESP8266
   #include <ESP8266WiFi.h>
   #include <ESP8266mDNS.h>
   WiFi.hostname(hostname);
   #elif defined(ESP32)
   #include <WiFi.h>
   #include <ESPmDNS.h>
   WiFi.setHostname(hostname);
   #endif
   ```

2. Async Wrapper:
   ```cpp
   #ifdef ESP8266
   // Original ESP8266 code with HTTP method handling
   #elif defined(ESP32)
   // Simple includes for ESP32
   #include <AsyncTCP.h>
   #include <ESPAsyncWebServer.h>
   #endif
   ```

### Successful Build Results
- ESP32 build now succeeds
- RAM usage: 24497 bytes
- Flash usage: 930729 bytes text + 223688 bytes data
- Total size: 1178914 bytes

### Next Steps
1. Test WebSocket functionality on ESP32
2. Verify WiFi connection handling
3. Test async web server performance
