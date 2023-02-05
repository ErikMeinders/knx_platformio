#include <knxp_platformio.h>

DECLARE_TIMER(BaseCodeShoutOut, 5);

void _knxapp::pinsetup()
{
    // put your pin setup code here, to run once before platform setup:

    // pin or GPIO the programming led is connected to. Default is LED_BUILTIN
    knx.ledPin(LED_BUILTIN);

    // is the led active on HIGH or low? Default is LOW
    knx.ledPinActiveOn(LED_BUILTIN_ON);
   
    // pin or GPIO programming button is connected to. Default is 0
    knx.buttonPin(27);

    Log.info("Button pin: %d\n", knx.buttonPin());
}

void _knxapp::conf()
{
    knx.readMemory();
    uint16_t ia = knx.individualAddress();

    Log.info("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    Log.info("Configured: %s\n", knx.configured() ? "true" : "false");

    if (!knx.configured())
    {
        Log.trace("Waiting for KNX to be configured \n");

        knx.start();
        while (!knx.configured())
        {
            if (stdIn->available())
                menu();

            knx.loop();
        }
        // knx.stop();
        ESP.restart();
    }
}

void _knxapp::setup()
{
    Println(">> BASE CODE SETUP START -- OVERRIDE THIS ! << ");

    Println(">> BASE CODE SETUP DONE << ");
}

void _knxapp::loop()
{
    if (DUE(BaseCodeShoutOut))

        Println(">> BASE CODE LOOP -- OVERRIDE THIS << ");
}

void _knxapp::status()
{
    Log.trace(">> BASE CODE STATUS << \n");
    
    uint16_t ia = knx.individualAddress();

    Printf("EEPROM size: %d\n", EEPROM.length());
    Printf("Configured: %s\n", knx.configured() ? "true" : "false");
    Printf("Programming mode %s\n", knx.progMode() ? "Enabled" : "Disabled");
    Printf("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    Printf("Wifi: %s\n", WiFi.isConnected() ? "Connected" : "Disconnected");
    Printf("IP: %s\n", WiFi.localIP().toString().c_str());
#ifdef ESP8266
    Printf("Last reset reason: %s\n", ESP.getResetReason().c_str());
#endif
}

char *_knxapp::hostname()
{
    return (char *)(HOSTNAME);
}

void _knxapp::progress(int nr, const char *text)
{
    Log.trace(">> Startup step [%d]: %s\n", nr, text);
}

void _knxapp::dumpParameter(int i)
{
    Printf("P%02d: %2X\n", i, knx.paramByte(i));
}

void _knxapp::dumpGroupObject(int i)
{
    GroupObject *go;

    if (i == 0)
        return;

    Printf("GO %02d ", i);
    go = &knx.getGroupObject(i);

    Printf("Flags: ");
    Printf(go->communicationEnable() ? "C" : "-");
    Printf(go->readEnable() ? "R" : "-");
    Printf(go->writeEnable() ? "W" : "-");
    Printf(go->transmitEnable() ? "T" : "-");
    Printf(go->responseUpdateEnable() ? "U" : "-");
    Printf(go->valueReadOnInit() ? "I" : "-");

    Printf(" DPT: [%04x][%04x] ", go->dataPointType().mainGroup, go->dataPointType().subGroup);

    Printf("valueSize: %d ", go->valueSize());

    Printf("%02x ", (uint8_t)go->value());

    Printf("Com Flag: %d \n", go->commFlag());
}

void _knxapp::dumpEEPROM()
{
    Printf("0000: ");
    size_t i = 0;

    while (i < EEPROM.length())
    {
        byte b = EEPROM.read(i);
        
        Printf("[%02X] %c ", b, b >= 32 && b <= 127 ? b : ' ');

        if (++i % 16 == 0)
        {
            Println();
            if (i < EEPROM.length())
                Printf("%04X: ", i);
            delay(10);
        }
        
    }
}

// static char menuLine[]="[E] EEPROM [P] parameters [G] groupObject [V] Verbosity [T] toggleProgMode [S] Status [abcz] base [0..9] dump [?] help\n";

void submenuline(char mode, int base)
{
    char first = (base == 0 ? '1' : '0');

    switch(mode)
    {
    case 'P':
        Printf("[0..9] for Parameter [%c0..%c9]\n", base/10+'0', base/10+'0');
        break;
    case 'G':
        Printf("[%c..9] for GroupObject [%c%c..%c9]\n", 
            first, base/10+'0', first, base/10+'0');
        break;
    case 'V':
        Printf("[0..6] to set Log Verbosity level\n");
        break;
    
    }
}

void _knxapp::menu()
{

    byte b = stdIn->read();
    char valid[] = "?zabcPGESTV0123456789";

    while ( strchr(valid,b) == NULL )
    {
        if(stdIn->available())
            b = stdIn->read();
        else
            return;
    } 

    static char mode = 'P';
    static int base = 0;

    switch (b)
    {
    case '?':
        help();
        break;
    case 'z':
        base = 0;
        submenuline(mode,base);
        break;
    case 'a':
        base = 10;
        submenuline(mode,base);
        break;
    case 'b':
        base = 20;
        submenuline(mode,base);
        break;
    case 'c':
        base = 30;
        submenuline(mode,base);
        break;
    case 'E':
        dumpEEPROM();
        break;
    case 'G': // starts at 1
        base = 0;
        mode = 'G';
        submenuline(mode, base);
        break;
    case 'P': // starts at 0
        base = 0;
        mode = 'P';
        submenuline(mode, base);
        break;
    case 'S':
        knxApp.status();
        break;
    case 'T':
        knx.toggleProgMode();
        knx.loop();
        break;
    case 'V':
        base = 0;
        mode = 'V';
        submenuline(mode, base);
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
        switch(mode) 
        {
        case 'P':
            dumpParameter(b - '0' + base);
            break; 
        case 'G':
            dumpGroupObject(b - '0' + base);
            break;
        case 'V':
            if( b - '0' < 7 && b - '0' >= 0)
                Log.setLevel(b - '0');
            switch(b - '0')
            {
            case 0:
                Println("Log level set to SILENT");
                break;
            case 1:
                Println("Log level set to FATAL");
                break;
            case 2:
                Println("Log level set to ERROR");
                break;
            case 3:
                Println("Log level set to WARNING");
                break;
            case 4:
                Println("Log level set to INFO");
                break;
            case 5:
                Println("Log level set to TRACE");
                break;
            case 6:
                Println("Log level set to VERBOSE");
                break;
            }
            break;
        }
        break;
    default:
        Log.error("Unknown command [%C] | [%x]\n", b, b);
    }
}

void _knxapp::help()
{
    Println("[E] EEPROM: dump the EEPROM content");
    Println("[P] Switch to Parameters dump mode");
    Println("[G] Switch to GroupObject dump mode");
    Println("[V] Switch to set Verbosity level mode");
    Println("[T] ToggleProgMode: toggle the programming mode");
    Println("[S] Status: dump the status of the KNX stack");
    Println("[abcz] Base: set the 10base for the dump [z=0 a=10 b=20 c=30]");
    Println("[0..9] Dump: dump the selected Parameter or Group object or set the Verbosity level");
    Println("[?] Help: print this help");
}

_knxapp _knxApp;
