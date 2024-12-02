#ifndef KNXP_SERVICES_H
#define KNXP_SERVICES_H

#include <knxp_platformio.h>
#include <knxp_progress.h>
#include <knxp_timeinfo.h>
#include <knxp_ota.h>

/**
 * @brief Initialize network-dependent services
 * @param step Progress step counter
 * @return Updated step counter
 */
int initializeNetworkServices(int step);

/**
 * @brief Process network-dependent services in the main loop
 */
void processNetworkServices();

#endif // KNXP_SERVICES_H
