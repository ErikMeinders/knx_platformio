#ifndef KNXP_TIMEINFO_H
#define KNXP_TIMEINFO_H

#include <Arduino.h>
#include <time.h>

void timeInit();
const char* timeNowString();

void resetUptime();
const char* uptime();

#endif // KNXP_TIMEINFO_H
