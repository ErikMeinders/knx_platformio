#include "knxp_platformio.h"

WebServer httpServer;

Stream *stdIn = &Serial;
Stream *stdOut = &Serial;

#define DECLARE_gTIMER(timerName, timerTime)     unsigned long timerName##_interval = timerTime * 1000,        timerName##_last = millis()+random(timerName##_interval);

DECLARE_gTIMER(_cyclicKnxTimer, 0);

/**
 * @brief HiJack the Arduino setup() function
 * Do not continue with application setup until KNX is ready
 * 
 */
void setup()
{
    int step=0;
    Serial.begin(115200);
    Log.begin(LOG_LEVEL_VERBOSE, &Serial);

    knxApp.progress(step++, "Starting pin setup");
    knxApp.pinsetup();

#ifndef NO_WIFI
    knxApp.progress(step++, "Starting WiFi");
    startWiFi(knxApp.hostname());
#endif

#ifndef NO_NTP
    knxApp.progress(step++, "Starting NTP");
    timeInit();
#endif

#ifndef NO_TELNET
    knxApp.progress(step++, "Starting Telnet");
    startTelnet();
#endif

    knxApp.progress(step++, "Starting KNX configuration");
    knxApp.conf();

 #ifndef NO_MDNS   
    knxApp.progress(step++, "Starting MDNS");
    startMDNS(knxApp.hostname());
#endif

#ifndef NO_OTA
    knxApp.progress(step++, "Starting OTA");
    otaInit();
#endif

#ifndef NO_HTTP
    knxApp.progress(step++, "Starting Webserver");
    httpServer.begin();
#endif

    knxApp.progress(step++, "Starting KNX Application Setup");
    knxApp.setup();
    
    knxApp.progress(step++, "Starting KNX");
    knx.start();

    delay(1000);
    
    resetUptime();
    Log.trace("Platform Startup Completed at %s in %u ms.\n\n", timeNowString(), millis());

}


/**
 * @brief HiJack the Arduino loop() function
 * Do not continue with application loop until KNX is ready and not in programming mode
 * 
 */
void loop()
{
    knx.loop();
    
    if (stdIn->available() >0 ) knxApp.menu();

 #ifndef NO_HEARTBEAT
    timeThis ( handleHeartbeat() );
    // knx.loop();
#endif   

    if(knx.progMode()) return;

#ifndef NO_OTA
    timeThis ( otaLoop() );
    // knx.loop();
#endif

#ifndef NO_HTTP
    timeThis ( httpServer.handleClient() );
    // knx.loop();
#endif

    timeThis ( knxApp.loop() );
    // knx.loop();

    if( !DUE (_cyclicKnxTimer)) return;

    timeThis( knxApp.cyclic() );
    knx.loop();
}
