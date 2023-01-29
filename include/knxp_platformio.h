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
#include <knxp_app.h>

// define timeout for programming mode after startup
// 0 = only programming mode if button is pressed
// >0 = timeout in ms
// default is 20.000ms = 20s

void setup();
void loop();

void knxpMenu();
void dumpEEPROM();
void dumpParameter(int i);
void dumpGroupObject(int i);
void help();

#ifndef HOSTNAME
#define HOSTNAME "knx_device"
#endif