#define DEBUG_OUT Serial

#include <knxmcastplatform.h>

WebServer httpServer;

void apiInit(WebServer *_srv);

void knxapp_setup() {
  // put your setup code here, to run once:
  Debugln(" >> ERIX SETUP << ");
}

void knxapp_loop() {
  // put your main code here, to run repeatedly:
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