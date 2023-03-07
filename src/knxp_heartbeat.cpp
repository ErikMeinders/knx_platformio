
#include "knxp_heartbeat.h"

#ifndef NO_HEARTBEAT
DECLARE_TIMER(watchDogTimer, 2)  //-- fire every 2 seconds

void handleHeartbeat()
{
#ifdef PIN_HEARTBEAT
  if ( DUE (watchDogTimer) )   
    digitalWrite(PIN_HEARTBEAT, !digitalRead(PIN_HEARTBEAT));
#else
#error DEFINE PIN_HEARTBEAT OR NO_HEARTBEAT
#endif
}
#endif
