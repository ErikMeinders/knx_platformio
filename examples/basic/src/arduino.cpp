#include "knxapp.h"

void setup()
{
    Serial.begin(115200);
    Serial.println("Your setup");
    Println("Your setup");
    //knxAppInstance.setup();
    Println("Your setup completed");
}

void loop()
{
    knxAppInstance.loop();
    Serial.println("Your loop");
}
