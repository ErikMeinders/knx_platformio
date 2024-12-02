#include "knxp_timeinfo.h"

#define MY_NTP_SERVER "nl.pool.ntp.org" 
#define MY_TZ         "CET-1CEST,M3.5.0,M10.5.0/3"
#ifdef ESP32
#include "esp_sntp.h"
#endif

static char timeString[36];
static char calcUptime[80];

static const char* _myctime(time_t *when)
{
    strcpy(timeString, ctime(when));
    timeString[strlen(timeString) - 1] = '\0'; // remove trailing newline
    return timeString;
}

const char* timeNowString()
{
    time_t now;
    time(&now);
    return _myctime(&now);
}

void timeInit()
{
    configTime(0, 0, MY_NTP_SERVER); // 0, 0 because we will use TZ in the next line
    setenv("TZ", MY_TZ, 1); // Set environment variable with your time zone
    tzset();
#ifdef ESP32
    sntp_set_sync_interval(12 * 60 * 60 * 1000UL); // 12 hours
    sntp_restart();  
#endif
}

static time_t startTime;

void resetUptime()
{
    time(&startTime);
}

const char* uptime()
{
    uint32_t upTimeNow = (millis() / 1000); 

    sprintf(calcUptime, "boottime %s - %d[d] %02d:%02d",
            _myctime(&startTime),
            (int)((upTimeNow / (60 * 60 * 24)) % 365),
            (int)((upTimeNow / (60 * 60)) % 24), // hours
            (int)((upTimeNow / 60) % 60));       // minutes
    return calcUptime;
}
