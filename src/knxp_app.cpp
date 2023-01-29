#define DEBUG_OUT Serial

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

    Serial.printf("Button pin: %d\n", knx.buttonPin());

}

void _knxapp::conf()
{

    knx.readMemory();
    uint16_t ia = knx.individualAddress();

    Serial.printf("Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    Serial.printf("Configured: %s\n", knx.configured() ? "true" : "false");

    if (!knx.configured())
    {
        Serial.printf("Waiting for KNX to be configured \n");

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

  // put your setup code here, to run oncen after platform setup:

  Debugln(">> YOUR CODE SETUP START<< ");

  Debugln(">> YOUR CODE SETUP DONE << ");
}

void _knxapp::loop() {

  // put your main code here, to run repeatedly:

  if (DUE(YourCodeShoutOut ))
    Debugln(">> YOUR CODE LOOP << ");
}

void _knxapp::status()
{
  // put your status code here, to run repeatedly when called from Menu

  Debugln(">> YOUR CODE STATUS << ");
}

char * _knxapp::hostname()
{
  return (char*) ( HOSTNAME ) ;
}

void _knxapp::progress(int a, const char*b)
{
  // put your progess message here

  Serial.printf(">> Startup step [%02d]: %s\n", a, b);
}

_knxapp _knxApp;
knxapp knxApp;
