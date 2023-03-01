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
    knx.buttonPin(PROGMODE_PIN);

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

void _knxapp::status_8266()
{
#ifdef ESP8266
    // esp8266 specific status

    Printf("ESP8266 STATUS \n");
    Printf("  Now: %s\n", timeNowString());
    Printf("  Free Heap: %d\n", ESP.getFreeHeap());
    Printf("  Large Free Heap: %d\n", ESP.getFreeContStack());
    Printf("  Chip ID: %08X\n", ESP.getChipId());
    Printf("  CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
    Printf("  Flash Chip ID: %08X\n", ESP.getFlashChipId());
    Printf("  Flash Chip Size: %d\n", ESP.getFlashChipSize());
    Printf("  Flash Chip Speed: %d\n", ESP.getFlashChipSpeed());
    Printf("  Sketch Size: %d\n", ESP.getSketchSize());
    Printf("  Free Sketch Space: %d\n", ESP.getFreeSketchSpace());
    Printf("  Reset Reason: %s\n", ESP.getResetReason().c_str());
    Printf("  Reset Info: %s\n", ESP.getResetInfo().c_str());
    Printf("  Boot Version: %d\n", ESP.getBootVersion());
    Printf("  Boot Mode: %d\n", ESP.getBootMode());
    Printf("  Core Version: %s\n", ESP.getCoreVersion().c_str());
    Printf("  SDK Version: %s\n", ESP.getSdkVersion());
    Printf("  Lib build %s %s\n", __DATE__, __TIME__); 
    
#endif
}

void _knxapp::status_32()
{
#ifdef ESP32
    // esp32 specific status
    Log.trace("ESP32 STATUS\n");
    Printf("  Now: %s\n", timeNowString());
    Printf("  SDK Version: %s\n", ESP.getSdkVersion());
    Printf("  Free Heap: %d\n", ESP.getFreeHeap());
    Printf("  Min Free Heap: %d\n", ESP.getMinFreeHeap());
    Printf("  CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
    Printf("  Flash Chip Size: %d\n", ESP.getFlashChipSize());
    Printf("  Flash Chip Speed: %d\n", ESP.getFlashChipSpeed());
    Printf("  Flash Chip Mode: %s\n", ESP.getFlashChipMode() == FM_QIO ? "QIO" : ESP.getFlashChipMode() == FM_QOUT ? "QOUT" : ESP.getFlashChipMode() == FM_DIO ? "DIO" : ESP.getFlashChipMode() == FM_DOUT ? "DOUT" : "UNKNOWN");
    Printf("  Sketch Size: %d\n", ESP.getSketchSize());
    Printf("  Free Sketch Space: %d\n", ESP.getFreeSketchSpace());
    Printf("  Lib build %s %s\n", __DATE__, __TIME__);
    
#endif
}

void _knxapp::status()
{
    
    uint16_t ia = knx.individualAddress();

    Printf("Uptime: %s\n", uptime());

    status_8266();
    status_32();

    Printf("Wifi: %s\n", WiFi.isConnected() ? "Connected" : "Disconnected");

    Printf("Hostname: %s\n", knxApp.hostname());
    Printf("IP: %s\n", WiFi.localIP().toString().c_str());

    Printf("EEPROM size: %d\n", EEPROM.length());
    Printf("Configured: %s\n", knx.configured() ? "true" : "false");
    Printf("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);

    Printf("Programming mode %s\n", knx.progMode() ? "Enabled" : "Disabled");
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

    if (i == 0 || i > _groupObjectCount)
    {
        Printf("GO %02d out of range\n", i);
        return;
    }

    Printf("GO %02d ", i);
    go = &knx.getGroupObject(i);

    Printf("Flags: ");
    Printf(go->communicationEnable() ? "C" : "-");
    Printf(go->readEnable() ? "R" : "-");
    Printf(go->writeEnable() ? "W" : "-");
    Printf(go->transmitEnable() ? "T" : "-");
    Printf(go->responseUpdateEnable() ? "U" : "-");
    Printf(go->valueReadOnInit() ? "I" : "-");

    Printf(" DPT: [%d.%d] ", go->dataPointType().mainGroup, go->dataPointType().subGroup);

    Printf("valueSize: %d valueRaw(hex): ", go->valueSize());

    for (int i = 0; i < go->valueSize(); i++)
        Printf("%02x ", (uint8_t)go->valueRef()[i]);

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

void submenuline(char mode, int base)
{
    char first = (base == 0 ? '1' : '0');

    switch (mode)
    {
    case 'P':
        Printf("[0..9] for Parameter [%c0..%c9]\n", base / 10 + '0', base / 10 + '0');
        break;
    case 'G':
        Printf("[%c..9] for GroupObject [%c%c..%c9]\n",
               first, base / 10 + '0', first, base / 10 + '0');
        break;
    case 'V':
        Printf("[0..6] to set Log Verbosity level\n");
        break;
    }
}

void _knxapp::menu()
{

    byte b = stdIn->read();
    const static char valid[] = "?zabcPGESTV0123456789";

    while (strchr(valid, b) == NULL)
    {
        if (stdIn->available())
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
        submenuline(mode, base);
        break;
    case 'a':
        base = 10;
        submenuline(mode, base);
        break;
    case 'b':
        base = 20;
        submenuline(mode, base);
        break;
    case 'c':
        base = 30;
        submenuline(mode, base);
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
        switch (mode)
        {
        case 'P':
            dumpParameter(b - '0' + base);
            break;
        case 'G':
            dumpGroupObject(b - '0' + base);
            break;
        case 'V':
            if (b - '0' < 7 && b - '0' >= 0)
                Log.setLevel(b - '0');
            switch (b - '0')
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

void _knxapp::cyclic()
{
    Log.trace("Entering cyclic\n");

    for (int16_t g = 1; g <= _groupObjectCount; g++)
    {
        if (knx.getGroupObject(g).asap() != g)
        {
            Log.error("ASAP mismatch %d %d\n", g, knx.getGroupObject(g).asap());
            break;
        }

        if (knx.getGroupObject(g).readEnable() && knx.getGroupObject(g).communicationEnable())
        {
            Log.info("GO %d FLGS %c%c\n", g, knx.getGroupObject(g).readEnable() ? 'R' : '-', knx.getGroupObject(g).communicationEnable() ? 'C' : '-');

            Log.info("  cyclic sent\n");

            knx.getGroupObject(g).objectWritten();

            knx.loop();
            delay(5);
        
        }
    }
}

_knxapp _knxApp;
