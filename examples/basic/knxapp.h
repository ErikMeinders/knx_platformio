#ifndef KNXAPP_H
#define KNXAPP_H

#include <knxp_platformio.h>

// your application override of the _knxapp class methods


class knxapp : public _knxapp
{
  public:

    void setup() ;          // after KNX configuration before KNX start  
    void loop();            // application loop
    void status();          // callback for additonal status information Menu item 'S'

};

extern knxapp knxApp;

#endif
