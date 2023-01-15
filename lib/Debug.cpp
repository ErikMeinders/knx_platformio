/*
   Debug Beginning Of Line (BOL)

   Outputs the BeginOfLine of a DebugT* line
   as per macro's in Debug.h

   Includes times when USE_NTP_TIME is configured

*/
#include "Debug.h"

void _debugBOL(const char *fn, int line)
{

  char _bol[30];

  struct tm tInfo;
  time_t now;

  return; // disable for now
  time(&now); // read the current time

  if (localtime_r(&now, &tInfo)!=NULL)
  {
    sprintf(_bol, "[%02d:%02d:%02d] %-12.12s(%4d): ",
            tInfo.tm_hour, tInfo.tm_min, tInfo.tm_sec,
            fn, line);
  } else {
    sprintf(_bol, "%-12.12s(%4d): ",
            fn, line);
  }

  Debug (_bol);
 
}
