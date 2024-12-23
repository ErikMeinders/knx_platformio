#include <knxp_platformio.h>

void _knxapp::dumpParameter(int i)
{
    Printf("P%02d: %2X\n", i, knx.paramByte(i));
}

void _knxapp::dumpGroupObject(int i)
{
    GroupObject *go;

    if (i == 0 || i > _groupObjectCount)
    {
        Printf("GO %02d out of range\n", i);
        return;
    }

    Printf("GO %02d ", i);
    go = &knx.getGroupObject(i);

    Printf("Flags: ");
    Printf(go->communicationEnable() ? "C" : "-");
    Printf(go->readEnable() ? "R" : "-");
    Printf(go->writeEnable() ? "W" : "-");
    Printf(go->transmitEnable() ? "T" : "-");
    Printf(go->responseUpdateEnable() ? "U" : "-");
    Printf(go->valueReadOnInit() ? "I" : "-");

    Printf(" DPT: [%d.%d] ", go->dataPointType().mainGroup, go->dataPointType().subGroup);

    Printf("valueSize: %d valueRaw(hex): ", go->valueSize());

    for (size_t i = 0; i < go->valueSize(); i++)
        Printf("%02x ", (uint8_t)go->valueRef()[i]);

    if( go->dataPointType().mainGroup == 9)
    {
        Printf("[ %7.2f ] ", (float) go->value());
    }
    Printf("Com Flag: %d \n", go->commFlag());
}

void _knxapp::dumpEEPROM()
{
    Printf("0000: ");
    size_t i = 0;
    KnxYieldGuard yieldGuard;

    while (i < EEPROM.length())
    {
        yieldGuard.check();
        byte b = EEPROM.read(i);

        Printf("[%02X] %c ", b, b >= 32 && b <= 127 ? b : ' ');

        if (++i % 16 == 0)
        {
            Println();
            if (i < EEPROM.length())
                Printf("%04X: ", i);
            knxYield();  // Explicit yield after each line
        }
    }
}
