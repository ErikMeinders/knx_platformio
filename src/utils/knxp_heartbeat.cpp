
#include "utils/knxp_heartbeat.h"

#ifndef KNXP_NO_HEARTBEAT
DECLARE_TIMER(watchDogTimer, 2)  //-- fire every 2 seconds

void handleHeartbeat()
{
#ifdef KNXP_HEARTBEAT_PIN
  if ( DUE (watchDogTimer) )   
    digitalWrite(KNXP_HEARTBEAT_PIN, !digitalRead(KNXP_HEARTBEAT_PIN));
#else
#error DEFINE KNXP_HEARTBEAT_PIN OR KNXP_NO_HEARTBEAT
#endif
}
#endif
