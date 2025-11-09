#ifndef KNXP_OTA_H
#define KNXP_OTA_H

#include <ArduinoOTA.h>

#ifndef KNXP_NO_OTA
/**
 * @brief Initialize Over-The-Air updates
 */
void otaInit();

/**
 * @brief Handle OTA update process in the main loop
 */
void otaLoop();
#endif // KNXP_NO_OTA

#endif // KNXP_OTA_H
