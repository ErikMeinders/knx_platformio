
#include <knxp_platformio.h>

DECLARE_TIMER( YourCodeShoutOut, 5 );

void _knxapp::pinsetup()
{
  // put your pin setup code here, to run once before platform setup:

  // pin or GPIO the programming led is connected to. Default is LED_BUILTIN
    knx.ledPin(2);

    // is the led active on HIGH or low? Default is LOW
    knx.ledPinActiveOn(HIGH);

    // pin or GPIO programming button is connected to. Default is 0
    knx.buttonPin(27);

    stdOut->printf("Button pin: %d\n", knx.buttonPin());

}

void _knxapp::conf()
{

    knx.readMemory();
    uint16_t ia = knx.individualAddress();

    stdOut->printf("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    stdOut->printf("Configured: %s\n", knx.configured() ? "true" : "false");

    if (!knx.configured())
    {
        stdOut->printf("Waiting for KNX to be configured \n");

        knx.start();
        while(!knx.configured())
        {
            if (Serial.available()) knxpMenu();

            knx.loop();
            
        }
        //knx.stop();
        ESP.restart();
    }
    
}

void _knxapp::setup()
{
  Debugln(">> YOUR CODE SETUP START<< ");

  Debugln(">> YOUR CODE SETUP DONE << ");
}

void _knxapp::loop() 
{
  if (DUE(YourCodeShoutOut ))

    Debugln(">> YOUR CODE LOOP << ");
}

void _knxapp::status()
{
  Debugln(">> YOUR CODE STATUS << ");
}

char * _knxapp::hostname()
{
  return (char*) ( HOSTNAME ) ;
}

void _knxapp::progress(int a, const char*b)
{
  stdOut->printf(">> Startup step [%02d]: %s\n", a, b);
}


void _knxapp::help()
{
    uint16_t ia = knx.individualAddress();
    
    stdOut->printf("EEPROM size: %d\n", EEPROM.length());
    stdOut->printf("Programming mode %s\n", knx.progMode() ? "Enabled" : "Disabled");
    stdOut->printf("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    stdOut->printf("Configured: %s\n", knx.configured() ? "true" : "false"); 
    stdOut->println("[P] parameters [G] groupObject [T] toggleProgMode [E] EEPROM [abz] base [0..9] dump [?] help");
}

void _knxapp::dumpParameter(int i)
{
    stdOut->printf("P%02d: %2X\n",i,knx.paramByte(i));   
}


void _knxapp::dumpGroupObject(int i)
{
    GroupObject *go;

    if( i == 0 ) return;
    
    stdOut->printf("GO %02d ",i);
    go = &knx.getGroupObject(i);

    stdOut->printf("Flags: ");
    stdOut->printf(go->communicationEnable() ? "C" : "-");
    stdOut->printf(go->readEnable() ? "R" : "-");
    stdOut->printf(go->writeEnable() ? "W" : "-");
    stdOut->printf(go->transmitEnable() ? "T" : "-");
    stdOut->printf(go->responseUpdateEnable() ? "U" : "-");
    stdOut->printf(go->valueReadOnInit() ? "I" : "-");

    stdOut->printf(" DPT: [%04x][%04x] ", go->dataPointType().mainGroup, go->dataPointType().subGroup);

    stdOut->printf("valueSize: %d ",go->valueSize());
    
    stdOut->printf("%02x ", (uint8_t) go->value());
        
    stdOut->printf("Com Flag: %d \n",go->commFlag());
}


void _knxapp::dumpEEPROM()
{
    stdOut->print("0000: ");
    size_t i=0;

    while ( i < EEPROM.length()   )
    {
        byte b=EEPROM.read(i);
        stdOut->printf("%02X [%c]", b, b >= 32  && b <= 127 ? b : ' ');
        i++;
        if (i % 16 == 0 )
        { 
            stdOut->println();
            if ( i < EEPROM.length())
                stdOut->printf("%04X: ", i);
        }
    }
}


void _knxapp::knxpMenu()
{

    byte b = stdIn->read();

    if ( b == '\n' || b == 0 ) return;
    
    static char mode='P';
    static int base=0;
    
    char basechar = base/10 + '0';
    char basestart = base == 0 ? '1' : '0';

    switch (b)
    {
    case '?' : help();
        break;
    case 'z' : base=0;
        stdOut->printf("Base 00 [1..9] for %s [1..9]\n", mode == 'P' ? "Parameter" : "GroupObject");
        break;
    case 'a' : base=10;
        stdOut->printf("Base 10 [0..9] for %s [10..19]\n", mode == 'P' ? "Parameter" : "GroupObject");
        break;
    case 'b' : base=20;
        stdOut->printf("Base 20 [0..9] for %s [20..29]\n", mode == 'P' ? "Parameter" : "GroupObject");
        break;
    case 'E':
        dumpEEPROM();
        break;
    case 'P':
        base = 0;
        basechar = base/10 + '0';
        basestart = base == 0 ? '1' : '0';
        mode = 'P';
        stdOut->printf("[%c..9] for Parameter %c%c..%c9 | ", basestart, basechar, basestart, basechar) ;
        stdOut->printf("Display Mode %c | Programming mode %c\n", mode, knx.progMode() ? 'E' : 'D');
        break;
    case 'G':
        base=0;
        basechar = base/10 + '0';
        basestart = base == 0 ? '1' : '0';
        mode = 'G';
        stdOut->printf("[%c..9] for GroupObject %c%c..%c9 | ", basestart, basechar, basestart, basechar) ;
        stdOut->printf("Display Mode %c | Programming mode %c\n", mode, knx.progMode() ? 'E' : 'D');
        break;
    case 'T':
        knx.toggleProgMode();
        knx.loop();
        help();       
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
    case 'S' :
        knxApp.status();
        break;
    default:
        stdOut->println("[P] parameters [G] groupObject [T] toggleProgMode [E] EEPROM [abz] base [0..9] dump [?] help");

    }
}

_knxapp _knxApp;
