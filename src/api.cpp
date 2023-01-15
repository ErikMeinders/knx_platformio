#include <knxmcastplatform.h>

void apiToggleProgMode()
{
  knx.toggleProgMode();
  knx.loop();
}

void apiInit(WebServer *_srv)
{

  _srv->on("/api/knxprog",  apiToggleProgMode );
}