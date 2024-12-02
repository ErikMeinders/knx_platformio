#ifndef KNXP_YIELD_H
#define KNXP_YIELD_H

#include <Arduino.h>
#include "knxp_heartbeat.h"

// Combined yield and heartbeat function
inline void knxYield() {
    yield();  // System yield
#ifndef NO_HEARTBEAT
    handleHeartbeat();  // Handle heartbeat
#endif
}

// Helper for loops that might block
inline void knxLoopYield(unsigned long &lastYield) {
    unsigned long now = millis();
    if (now - lastYield >= 100) {  // Yield every 100ms
        knxYield();
        lastYield = now;
    }
}

// RAII-style yield guard for scoped sections
class KnxYieldGuard {
    unsigned long lastYield;
public:
    KnxYieldGuard() : lastYield(millis()) {}
    void check() {
        knxLoopYield(lastYield);
    }
};

#endif // KNXP_YIELD_H
