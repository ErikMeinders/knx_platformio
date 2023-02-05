#include "knxp_platformio.h"

WebServer httpServer;

Stream *stdIn = &Serial;
Stream *stdOut = &Serial;

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

    knxApp.progress(step++, "Starting WiFi");
    startWiFi(knxApp.hostname());

    knxApp.progress(step++, "Starting NTP");
    timeInit();

    knxApp.progress(step++, "Starting Telnet");
    startTelnet();

    knxApp.progress(step++, "Starting KNX configuration");
    knxApp.conf();
    
    knxApp.progress(step++, "Starting MDNS");
    startMDNS(knxApp.hostname());

    knxApp.progress(step++, "Starting OTA");
    otaInit();

    knxApp.progress(step++, "Starting Webserver");
    httpServer.begin();

    knxApp.progress(step++, "Starting KNX Application Setup");
    knxApp.setup();
    
    knxApp.progress(step++, "Starting KNX");
    knx.start();

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
    
    if(knx.progMode()) return;

    timeThis ( otaLoop() );
    knx.loop();

    timeThis ( handleHeartbeat() );
    knx.loop();

    timeThis ( httpServer.handleClient() );
    knx.loop();

    timeThis ( knxApp.loop() );
    knx.loop();
}
