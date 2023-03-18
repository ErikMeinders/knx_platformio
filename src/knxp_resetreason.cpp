#include "knxp_resetreason.h"

static char txtReason[32];

char * resetReasonStr(int8_t reason)
{
  int txtReasonLen = sizeof(txtReason);

  switch ( reason)
  {
    case  1 : snprintf(txtReason, txtReasonLen, "[%d] POWERON_RESET", reason);    break; // < 1, Vbat power on reset
    case  3 : snprintf(txtReason, txtReasonLen, "[%d] SW_RESET", reason);         break; // < 3, Software reset digital core
    case  4 : snprintf(txtReason, txtReasonLen, "[%d] OWDT_RESET", reason);       break; // < 4, Legacy watch dog reset digital core
    case  5 : snprintf(txtReason, txtReasonLen, "[%d] DEEPSLEEP_RESET", reason);  break; // < 5, Deep Sleep reset digital core
    case  6 : snprintf(txtReason, txtReasonLen, "[%d] SDIO_RESET", reason);       break; // < 6, Reset by SLC module, reset digital core
    case  7 : snprintf(txtReason, txtReasonLen, "[%d] TG0WDT_SYS_RESET", reason); break; // < 7, Timer Group0 Watch dog reset digital core
    case  8 : snprintf(txtReason, txtReasonLen, "[%d] TG1WDT_SYS_RESET", reason); break; // < 8, Timer Group1 Watch dog reset digital core
    case  9 : snprintf(txtReason, txtReasonLen, "[%d] RTCWDT_SYS_RESET", reason); break; // < 9, RTC Watch dog Reset digital core
    case 10 : snprintf(txtReason, txtReasonLen, "[%d] INTRUSION_RESET", reason);  break; // <10, Instrusion tested to reset CPU
    case 11 : snprintf(txtReason, txtReasonLen, "[%d] TGWDT_CPU_RESET", reason);  break; // <11, Time Group reset CPU
    case 12 : snprintf(txtReason, txtReasonLen, "[%d] SW_CPU_RESET", reason);     break; // <12, Software reset CPU
    case 13 : snprintf(txtReason, txtReasonLen, "[%d] RTCWDT_CPU_RESET", reason); break; // <13, RTC Watch dog Reset CPU
    case 14 : snprintf(txtReason, txtReasonLen, "[%d] EXT_CPU_RESET", reason);    break; // <14, for APP CPU, reseted by PRO CPU
    case 15 : snprintf(txtReason, txtReasonLen, "[%d] RTCWDT_BROWN_OUT_RESET", reason); break;// <15, Reset when the vdd voltage is not stable
    case 16 : snprintf(txtReason, txtReasonLen, "[%d] RTCWDT_RTC_RESET", reason); break; // <16, RTC Watch dog reset digital core and rtc module
    default : snprintf(txtReason, txtReasonLen, "[%d] NO_MEAN", reason);
  }
  return txtReason;
} 
