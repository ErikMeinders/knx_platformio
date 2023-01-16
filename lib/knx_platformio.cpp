#include "knx_platformio.h"

/* To Be Implemented in Your Application */

void knxapp_pinsetup();
void knxapp_report(int step, const char *msg);
void knxapp_setup();
void knxapp_loop();
char *knxapp_hostname();

//

void getKNXconfigured()
{
    unsigned long s=millis();
    bool configured = knx.configured() || knx.individualAddress() != 0xFFFF;

    // knx.progMode(true);
    // Serial.printf("Entering prog mode for %ds\n", (PROGMODE_TIMEOUT/1000));
    uint16_t ia = knx.individualAddress();
    
    Serial.printf("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    Serial.printf("Configured: %s\n", knx.configured() ? "true" : "false");

    Serial.printf("Waiting for KNX to be configured (20s in prog mode)\n");

    // if(knx.individualAddress() == 0xFFFF) 
        knx.progMode(true);

    knx.loop();
    while( !knx.configured())
    {
        knx.loop();
        if (millis() > 20000 && knx.progMode() ){
            knx.progMode(false);
           

            Serial.printf("Timeout, exiting prog mode\n");
            break;
        }
        delay(5);
    }
        
    ia = knx.individualAddress();
    
    Serial.printf("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    Serial.printf("Configured: %s\n", knx.configured() ? "true" : "false");
}
/**
 * @brief HiJack the Arduino setup() function
 * Do not continue with application setup until KNX is ready
 * 
 */
void setup()
{
    int step=0;
    Serial.begin(115200);
    
    knxapp_report(step++, "Starting pin setup");
    knxapp_pinsetup();

    knxapp_report(step++, "Starting WiFi");
    startWiFi(knxapp_hostname());

    knxapp_report(step++, "Starting NTP");
    timeInit();

    knxapp_report(step++, "Starting KNX");
    
    knx.readMemory();
    knx.start();

    knxapp_report(step++, "Starting KNX configuration");
    getKNXconfigured();
    knx.start();
    
    knxapp_report(step++, "Starting MDNS");
    startMDNS(knxapp_hostname());

    knxapp_report(step++, "Starting OTA");
    otaInit();

    knxapp_report(step++, "Starting Webserver");
    httpServer.begin();
    
    knxapp_report(step++, "Starting Telnet");
    startTelnet();

    knxapp_report(step++, "Starting KNX Application");
    knxapp_setup();

    resetUptime();
    Debugf("-- [%s] Startup completed in %d ms.\n\n", timeNowString(), millis());

}
/**
 * @brief HiJack the Arduino loop() function
 * Do not continue with application loop until KNX is ready and not in programming mode
 * 
 */
DECLARE_TIMERms(httpHandle, 500);
void loop()
{
    knx.loop();
    
    if( DUE(httpHandle)) httpServer.handleClient();

    if(!knx.configured() || knx.progMode()) return;

    otaLoop();
    handleHeartbeat();

    httpServer.handleClient();

    timeThis ( knxapp_loop() );
}
