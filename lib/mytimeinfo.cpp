/*
**  mytimeinfo.cpp

*/

#include "mytimeinfo.h"

/*
**  struct tm {
**    int tm_sec;         // seconds,  range 0 to 59
**    int tm_min;         // minutes, range 0 to 59
**    int tm_hour;        // hours, range 0 to 23
**    int tm_mday;        // day of the month, range 1 to 31
**    int tm_mon;         // month, range 0 to 11
**    int tm_year;        // The number of years since 1900
**    int tm_wday;        // day of the week, range 0 to 6
**    int tm_yday;        // day in the year, range 0 to 365
**    int tm_isdst;       // daylight saving time 
**  };
*/

#define MY_NTP_SERVER "nl.pool.ntp.org" 
#define MY_TZ         "CET-1CEST,M3.5.0,M10.5.0/3"

#include "esp_sntp.h"

char * _myctime(time_t *when)
{
  static char timeString[36];

  strcpy(timeString, ctime(when));
  timeString[strlen(timeString) - 1] = '\0'; // remove trailing newline
  return timeString;
}

char * timeNowString()
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
  sntp_set_sync_interval(12 * 60 * 60 * 1000UL); // 12 hours
  sntp_restart();  
 
}

static time_t    startTime;

void resetUptime()
{
  time(&startTime);
}

char * uptime()
{
  static char calcUptime[80];

  uint32_t  upTimeNow = (millis() / 1000); 

  sprintf(calcUptime, "boottime %s - %d[d] %02d:%02d",
          _myctime(&startTime),
          (int)((upTimeNow / (60 * 60 * 24)) % 365),
          (int)((upTimeNow / (60 * 60)) % 24), // hours
          (int)((upTimeNow / 60) % 60));       // minuten
  return calcUptime;

} 


/***************************************************************************
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
* THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*
***************************************************************************/
