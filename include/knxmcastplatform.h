#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

#include <ota.h>

#include <knx.h>

#include <network.h>
#include <mytimeinfo.h>
#include <timers.h>
#include <profiling.h>
#include <heartbeat.h>
#include <resetreason.h>
#include <LittleFSmanager.h>

// define timeout for programming mode after startup
// 0 = only programming mode if button is pressed
// >0 = timeout in ms
// default is 20.000ms = 20s
#define PROGMODE_TIMEOUT 10000

