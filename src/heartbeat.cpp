
#include "heartbeat.h"

DECLARE_TIMER(watchDogTimer, 2)  //-- fire every 2 seconds

void handleHeartbeat()
{
#ifndef NOHEARTBEAT
#ifdef PIN_HEARTBEAT
  if ( DUE (watchDogTimer) )   
    digitalWrite(PIN_HEARTBEAT, !digitalRead(PIN_HEARTBEAT));
#else
#error DEFINE PIN_HEARTBEAT OR NOHEARTBEAT
#endif
#endif
} 