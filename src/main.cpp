#define DEBUG_OUT Serial

#include <knxp_platformio.h>


DECLARE_TIMER( YourCodeShoutOut, 5 );

void knxapp_setup() {

  // put your setup code here, to run once:

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
  // put your pin setup code here, to run once:
}

void knxapp_report(int a, const char*b)
{
  // put your report code here, to run repeatedly:
  Serial.println(b);
}
