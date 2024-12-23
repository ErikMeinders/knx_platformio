#ifndef KNXP_SERVICES_H
#define KNXP_SERVICES_H

#include <knxp_platformio.h>
#include "../utils/knxp_progress.h"
#include "../utils/knxp_timeinfo.h"
#include "../utils/knxp_ota.h"
#include "../core/app.h"

/**
 * @brief Initialize network-dependent services
 * @param app The knxapp instance
 * @param step Progress step counter
 * @return Updated step counter
 */
int initializeNetworkServices(_knxapp* app, int step);

/**
 * @brief Process network-dependent services in the main loop
 */
void processNetworkServices();

#endif // KNXP_SERVICES_H
