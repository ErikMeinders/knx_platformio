#ifndef KNXAPP_H
#define KNXAPP_H

#include "knxp_platformio.h"

class knxapp : public _knxapp {
public:
    void setup() override;
    void loop() override;
    void status() override;
};

extern knxapp knxApp;

#endif
