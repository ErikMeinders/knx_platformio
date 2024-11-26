#ifndef KNXP_NETWORK_H
#define KNXP_NETWORK_H

#include <Arduino.h>
#include <knxp_espcompat.h>
#include <WiFiManager.h>
#ifndef NO_TELNET
#include <TelnetStream.h>
#endif

/**
 * @file knxp_network.h
 * @brief Network management functionality for KNX PlatformIO
 * 
 * This module handles WiFi connectivity, Telnet debugging, and mDNS services.
 * It supports both station and access point modes, with automatic fallback
 * and connection monitoring.
 */

/**
 * @brief Get current network ready status
 * @return true if network is ready, false otherwise
 */
bool isNetworkReady();

/**
 * @brief Initialize and start WiFi connection
 * 
 * @param hostname Device hostname for network identification
 * 
 * @details
 * - In normal mode, attempts to connect to configured WiFi network
 * - In NETWORK_ONDEMAND mode, only starts when PROG button is pressed
 * - Supports fallback to AP mode for configuration
 * - Automatically restarts device on connection timeout
 * 
 * Usage example:
 * @code
 * startWiFi("my-device");
 * @endcode
 */
void startWiFi(const char *hostname);

#ifndef NO_TELNET
/**
 * @brief Initialize and start Telnet server
 * 
 * @details
 * - Enables remote debugging and monitoring via Telnet
 * - When STDIO_TELNET is defined, redirects standard I/O to Telnet
 * - Requires active network connection
 * 
 * Usage example:
 * @code
 * if (isNetworkReady()) startTelnet();
 * @endcode
 */
void startTelnet();
#endif

/**
 * @brief Initialize and start mDNS responder
 * 
 * @param hostname Device hostname for mDNS resolution
 * 
 * @details
 * - Sets up mDNS for local network device discovery
 * - Advertises HTTP service on port 80
 * - Requires active network connection
 * - Device will be accessible as hostname.local
 * 
 * Usage example:
 * @code
 * if (isNetworkReady()) startMDNS("my-device");
 * @endcode
 */
void startMDNS(const char *hostname);

/**
 * @brief Process network-related tasks
 * 
 * @details
 * - Monitors WiFi connection status
 * - Attempts automatic reconnection when needed
 * - Should be called regularly from the main loop
 * 
 * Usage example:
 * @code
 * void loop() {
 *     processNetwork();
 *     // other loop tasks
 * }
 * @endcode
 */
void processNetwork();

#ifndef NO_HTTP
/**
 * @brief Global HTTP server instance
 * @details Available when NO_HTTP is not defined
 */
extern WebServer httpServer;
#endif

#endif // KNXP_NETWORK_H
