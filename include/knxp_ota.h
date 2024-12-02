#ifndef KNXP_OTA_H
#define KNXP_OTA_H

#include <ArduinoOTA.h>

#ifndef NO_OTA
/**
 * @brief Initialize Over-The-Air updates
 */
void otaInit();

/**
 * @brief Handle OTA update process in the main loop
 */
void otaLoop();
#endif // NO_OTA

#endif // KNXP_OTA_H
