#include <Arduino.h>
#include <EEPROM.h>

#include <knxp_espcompat.h>

#include <knx.h>

#include <timers.h>
#include <profiling.h>

#include <knxp_ota.h>
#include <knxp_network.h>
#include <knxp_timeinfo.h>
#include <knxp_heartbeat.h>
#include <knxp_resetreason.h>
#include <knxp_app.h>

#include "knxapp.h"

extern Stream *stdIn;
extern Stream *stdOut;

void setup();
void loop();

#ifndef HOSTNAME
#define HOSTNAME "knx_device"
#endif
