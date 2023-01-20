#include <Arduino.h>
#include <knxp_espcompat.h>

#include <knx.h>

#include <timers.h>
#include <profiling.h>

#include <knxp_ota.h>
#include <knxp_network.h>
#include <knxp_timeinfo.h>
#include <knxp_heartbeat.h>
#include <knxp_resetreason.h>

// define timeout for programming mode after startup
// 0 = only programming mode if button is pressed
// >0 = timeout in ms
// default is 20.000ms = 20s
#define PROGMODE_TIMEOUT 0000

void setup();
void loop();
