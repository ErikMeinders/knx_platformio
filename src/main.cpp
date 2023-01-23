#define DEBUG_OUT Serial

#include <knxp_platformio.h>

DECLARE_TIMER( YourCodeShoutOut, 5 );

void knxapp_setup() {

  // put your setup code here, to run oncen after platform setup:

  Debugln(">> YOUR CODE SETUP START<< ");

  Debugln(">> YOUR CODE SETUP DONE << ");
}

void knxapp_loop() {

  // put your main code here, to run repeatedly:

  if (DUE(YourCodeShoutOut ))
    Debugln(">> YOUR CODE LOOP << ");
}

char * knxapp_hostname()
{
  return (char*) "TesP";
}

void knxapp_pinsetup()
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

void knxapp_report(int a, const char*b)
{
  // put your report code here, to report on progress during platform setup:
  Serial.println(b);
}
