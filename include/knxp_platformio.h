#include <Arduino.h>
#include <ArduinoLog.h>

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

void setup();
void loop();

#ifndef HOSTNAME
#define HOSTNAME knx_device
#endif

#define Printf(...)     { stdOut->printf(__VA_ARGS__); }
#define Println(...)    { stdOut->println(__VA_ARGS__); }

extern Stream *stdIn;
extern Stream *stdOut;

#ifndef STD_DELAY
#define STD_DELAY 100
#endif

#define DELAY delay(STD_DELAY)