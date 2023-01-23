#include "knxp_platformio.h"
#include <EEPROM.h>

/* To Be Implemented in Your Application */

void knxapp_pinsetup();
void knxapp_report(int step, const char *msg);
void knxapp_setup();
void knxapp_loop();
char *knxapp_hostname();

WebServer httpServer;



/**
 * @brief Loop until KNX is configured or timeout is reached 
 * 
 */
void getKNXconfigured()
{
    uint16_t ia = knx.individualAddress();
    time_t configstart = millis();
    
    Serial.printf("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    Serial.printf("Configured: %s\n", knx.configured() ? "true" : "false");

    Serial.printf("Waiting for KNX to be configured (%d seconds in prog mode)\n", PROGMODE_TIMEOUT/1000);

    // if(knx.individualAddress() == 0xFFFF) 

    knx.progMode(true);
    knx.loop();

    while( !knx.configured())
    {
        knx.loop();
        if (millis() > PROGMODE_TIMEOUT + configstart && knx.progMode() ){
           
            Serial.printf("Timeout, exiting prog mode\n");
            break;
        }
        delay(5);
    }
    delay(50);
    knx.progMode(false);
    knx.loop();

    ia = knx.individualAddress();
    
    Serial.printf("Individual Address: %d.%d.%d [%04x]\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF, ia);
    Serial.printf("Configured: %s\n", knx.configured() ? "true" : "false");
    if( knx.configured()) {
        dumpParameter(1);
        dumpGroupObject(1);
    }
}

void dumpGroupObject(int i)
{
    GroupObject *go;
    unsigned long b;

    if( i == 0 ) return;
    
    Serial.printf("GO %02d ",i);
    go = &knx.getGroupObject(i);

    Serial.printf("Flags: ");
    Serial.printf(go->communicationEnable() ? "C" : "-");
    Serial.printf(go->readEnable() ? "R" : "-");
    Serial.printf(go->writeEnable() ? "W" : "-");
    Serial.printf(go->transmitEnable() ? "T" : "-");
    Serial.printf(go->responseUpdateEnable() ? "U" : "-");
    Serial.printf(go->valueReadOnInit() ? "I" : "-");

    Serial.printf(" DPT: [%02x] ",go->dataPointType());

    Serial.printf("valueSize: %d ",go->valueSize());
    
    Serial.printf("%d ", (go->value()));
        
    Serial.printf("Com Flag: %d \n",go->commFlag());
}

void dumpParameter(int i)
{
    if( i == 0 ) return;

    Serial.printf("P%02d: %2X\n",i,knx.paramByte(i));
    
}

void dumpEEPROM()
{
    Serial.print("0000: ");
    int i=0;
    while ( i < EEPROM.length()   )
    {
        byte b=EEPROM.read(i);
        Serial.printf("%02X [%c]", b, b >= 32  && b <= 127 ? b : ' ');
        i++;
        if (i % 16 == 0 )
        { 
            Serial.println();
            if ( i < EEPROM.length())
                Serial.printf("%04X: ", i);
        }
    }
}

void help()
{
    Serial.printf("GO size: %d\n", sizeof(GroupObject));
    // Serial.printf("P size: %d\n", sizeof(Parameter));
    Serial.printf("EEPROM size: %d\n", EEPROM.length());
}

void knxpMenu()
{

    byte b = Serial.read();
    static char mode='P';
    static int base=0;
    uint16_t ia = knx.individualAddress();
    
    char basechar = base/10 + '0';
    char basestart = base == 0 ? '1' : '0';

    switch (b)
    {
    case '?' : help();
        break;
    case 'z' : base=0;
        Serial.printf("Base 00 [1..9] for %s [1..9]\n", mode == 'P' ? "Parameter" : "GroupObject");
        break;
    case 'a' : base=10;
        Serial.printf("Base 10 [0..9] for %s [10..19]\n", mode == 'P' ? "Parameter" : "GroupObject");
        break;
    case 'b' : base=20;
        Serial.printf("Base 20 [0..9] for %s [20..29]\n", mode == 'P' ? "Parameter" : "GroupObject");
        break;
    case 'E':
        dumpEEPROM();
        break;
    case 'P':
        base = 0;
        basechar = base/10 + '0';
        basestart = base == 0 ? '1' : '0';
        mode = 'P';
        Serial.printf("[%c..9] for GroupObject %c%c..%c9 | ", basestart, basechar, basestart, basechar) ;
        Serial.printf("Display Mode %c | Programming mode %c\n", mode, knx.progMode() ? 'E' : 'D');
        break;
    case 'G':
        base=0;
        basechar = base/10 + '0';
        basestart = base == 0 ? '1' : '0';
        mode = 'G';
        Serial.printf("[%c..9] for GroupObject %c%c..%c9 | ", basestart, basechar, basestart, basechar) ;
        Serial.printf("Display Mode %c | Programming mode %c\n", mode, knx.progMode() ? 'E' : 'D');
        break;
    case 'T':
        knx.toggleProgMode();
        knx.loop();
        Serial.printf("Programming mode %s\n", knx.progMode() ? "Enabled" : "Disabled");
        Serial.printf("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
        Serial.printf("Configured: %s\n", knx.configured() ? "true" : "false"); 
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        if (mode == 'P' )
            dumpParameter(b-'0'+base);
        else
            dumpGroupObject(b-'0'+base);
        break;
    default:
        Serial.println("[P] parameters [G] groupObject [T] toggleProgMode [E] EEPROM [abz] base [0..9] dump [?] help");

    }

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
    
    knx.loop();

    knxapp_report(step++, "Starting MDNS");
    startMDNS(knxapp_hostname());
    knx.loop();

    knxapp_report(step++, "Starting OTA");
    otaInit();
    knx.loop();

    knxapp_report(step++, "Starting Webserver");
    httpServer.begin();
    knx.loop();

    knxapp_report(step++, "Starting Telnet");
    startTelnet();
    knx.loop();

    knxapp_report(step++, "Starting KNX Application");
    knxapp_setup();
    knx.loop();

    resetUptime();
    Debugf("-- [%s] Startup completed in %d ms.\n\n", timeNowString(), millis());

}
/**
 * @brief HiJack the Arduino loop() function
 * Do not continue with application loop until KNX is ready and not in programming mode
 * 
 */
void loop()
{
    knx.loop();
    
    if (Serial.available()) knxpMenu();

    if(!knx.configured() || knx.progMode()) return;

    otaLoop();
    knx.loop();

    handleHeartbeat();
    knx.loop();

    httpServer.handleClient();
    knx.loop();

    timeThis ( knxapp_loop() );
    knx.loop();
}
