#define DEBUG_OUT Serial

#include "knx_platformio.h"
#include "api.h"

WebServer httpServer;

void knxapp_setup() {
  // put your setup code here, to run once:

  Debugln(" >> ERIX SETUP << ");

  apiInit(&httpServer);
  loop();
}

void knxapp_loop() {

  // put your main code here, to run repeatedly:

  httpServer.handleClient();

  Debugln(" >> ERIX LOOP << ");
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