#include "api.h"

static DynamicJsonDocument toReturnJSON(1024);  // generic doc to return, clear() before use!
static WebServer * _srv;

void _returnJSON(JsonObject obj)

{
  String toReturn;

  serializeJson(obj, toReturn);

  _srv->send(200, "application/json", toReturn);
}

void apiToggleProgMode()
{
  knx.toggleProgMode();
  knx.loop();

  toReturnJSON.clear();

  toReturnJSON["knxprogmode"] = knx.progMode();

  _returnJSON(toReturnJSON.as<JsonObject>());
}

void apiInit(WebServer *server)
{
  _srv = server;
  _srv->on("/api/knxprog",  apiToggleProgMode );
}