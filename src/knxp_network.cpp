
#include "knxp_platformio.h"

void startWiFi(const char *Hostname)
{
  WiFiManager manageWiFi;

  wifi_mode_t myWifiMode = WIFI_STA;
  int myWifiTimeout = 30;

  String thisAP = String(Hostname) + "-" + WiFi.macAddress();

#ifdef NETWORK_ONDEMAND
  
  int cnt = 0;

  if (!digitalRead(PROGMODE_PIN) == LOW)
  {
    Log.info("PROG button not pressed, not starting WiFi\n");
    return;
  } else {
    Log.info("PROG button pressed, starting WiFi\n");
    while(digitalRead(PROGMODE_PIN) == LOW){
      delay(100);
      cnt++;
    }
    if(cnt > 100){
      Log.info("PROG button pressed for more than 10 seconds, starting AP upon release\n");
      myWifiMode = WIFI_AP_STA;
      myWifiTimeout = 120;
    }
  }
  
#endif

  WiFi.hostname(Hostname);
  WiFi.mode(myWifiMode);

  manageWiFi.setDebugOutput(true);
  manageWiFi.setHostname(Hostname);

  //--sets timeout until configuration portal gets turned off
  //--useful to make it all retry or go to sleep in seconds

  manageWiFi.setTimeout(myWifiTimeout); // 2 minuten

  //--fetches ssid and pass and tries to connect
  //--if it does not connect it starts an access point with the specified name
  //--and goes into a blocking loop awaiting configuration

  if (!manageWiFi.autoConnect(thisAP.c_str()))
  {
    Log.fatal("failed to connect and hit timeout\n");

    // reset and try again, or maybe put it to deep sleep
    ESP.restart();
    delay(2000);
  }

  Log.info("Connected with IP-address [%s]\n", WiFi.localIP().toString().c_str());

  networkReady = true;

} // startWiFi()

#ifndef NO_TELNET
void startTelnet()
{
  TelnetStream.begin();
  Log.trace("Telnet server started ..");
  // TelnetStream.flush();
#ifdef STDIO_TELNET
  stdIn = &TelnetStream;
  stdOut = &TelnetStream;
  Log.begin(LOG_LEVEL_VERBOSE, &TelnetStream);

#endif
} // startTelnet()
#endif

void startMDNS(const char *Hostname)
{
  Log.info("mDNS setup as [%s.local]\r\n", Hostname);
  if (MDNS.begin(Hostname))
  { 
    // Start the mDNS responder for Hostname.local
    Log.trace("mDNS responder started as [%s.local]\r\n", Hostname);
  }
  else
  {
    Log.error("Error setting up MDNS responder!\r\n");
  }
  MDNS.addService("http", "tcp", 80);

} // startMDNS()

/***************************************************************************
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
 * OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
 * THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 ***************************************************************************/
