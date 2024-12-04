#ifndef KNXP_NETWORK_H
#define KNXP_NETWORK_H

#include <Arduino.h>

#ifndef NO_WIFI
/**
 * @brief Start WiFi connection
 * 
 * @param hostname The hostname to use for the device
 * @return true if connection successful, false otherwise
 */
bool startWiFi(const char *hostname);

/**
 * @brief Check if network is ready
 * 
 * @return true if network is ready, false otherwise
 */
bool isNetworkReady();

/**
 * @brief Process network tasks
 * Called from loop() to handle network maintenance
 */
void processNetwork();

#ifndef NO_TELNET
/**
 * @brief Start telnet server
 */
void startTelnet();
#endif

#ifndef NO_MDNS
/**
 * @brief Start mDNS responder
 * 
 * @param hostname The hostname to use for mDNS
 */
void startMDNS(const char *hostname);
#endif

#endif // NO_WIFI
#endif // KNXP_NETWORK_H
