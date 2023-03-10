
#include "knxp_platformio.h"

void startWiFi(const char *Hostname)
{
  WiFiManager manageWiFi;

  String thisAP = String(Hostname) + "-" + WiFi.macAddress();

  WiFi.hostname(Hostname);
  WiFi.mode(WIFI_STA);

  manageWiFi.setDebugOutput(true);
  manageWiFi.setHostname(Hostname);

  //--sets timeout until configuration portal gets turned off
  //--useful to make it all retry or go to sleep in seconds
  manageWiFi.setTimeout(120); // 2 minuten

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
