#ifndef KNXAPP_H
#define KNXAPP_H

#include <knxp_platformio.h>

// your application override of the _knxapp class methods


class knxapp : public _knxapp
{
  public:
    // void progress(int step, const char *msg);  // callback for knxp_platformio

    // void  pinsetup();    // before anything else
    // void  conf();        // after Wifi, NTP and Telnet

    void setup() ;          // after KNX configuration before KNX start  
    void loop();            // application loop
    void cyclic();          // cyclic KNX feedback

    // char* hostname();    // callback by knxp_platformio | default: HOSTNAME / "knx_device"
    void  status();      // callback for additonal status information Menu item 'S'

};

extern knxapp knxApp;

#endif
