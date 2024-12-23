#include "knxapp.h"

void setup()
{
    Println("Your setup");
    knxAppInstance.setup();
    Println("Your setup completed");
}

void loop()
{
    knxAppInstance.loop();
}
