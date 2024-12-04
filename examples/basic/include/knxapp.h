#ifndef KNXAPP_H
#define KNXAPP_H

#include <knxp_platformio.h>

// Application class that inherits from _knxapp
class knxapp : public _knxapp
{
  public:
    // Override methods to provide app-specific implementations
    void setup() override;
    void loop() override;
    void status() override;
    void knx_setup();
    virtual void handleWebSocketMessage(uint8_t num, uint8_t* payload, size_t length) { return; }
  private:
    unsigned long xx = 0;
};

// Declare the global instance
extern knxapp knxAppInstance;

#endif
