#include <knxp_platformio.h>

void submenuline(char mode, int base)
{
    char first = (base == 0 ? '1' : '0');

    switch (mode)
    {
    case 'P':
        Printf("[0..9] for Parameter [%c0..%c9]\n", base / 10 + '0', base / 10 + '0');
        break;
    case 'G':
        Printf("[%c..9] for GroupObject [%c%c..%c9]\n",
               first, base / 10 + '0', first, base / 10 + '0');
        break;
    case 'V':
        Printf("[0..6] to set Log Verbosity level\n");
        break;
    }
}

void _knxapp::menu()
{
    KnxYieldGuard yieldGuard;
    const static char valid[] = "?zabcPGESTV0123456789";
    static char mode = 'P';
    static int base = 0;

    if (!console->available()) {
        return;
    }

    byte b = console->read();
    while (strchr(valid, b) == NULL)
    {
        yieldGuard.check();
        if (console->available())
            b = console->read();
        else
            return;
    }

    switch (b)
    {
    case '?':
        help();
        break;
    case 'z':
        base = 0;
        submenuline(mode, base);
        break;
    case 'a':
        base = 10;
        submenuline(mode, base);
        break;
    case 'b':
        base = 20;
        submenuline(mode, base);
        break;
    case 'c':
        base = 30;
        submenuline(mode, base);
        break;
    case 'E':
        dumpEEPROM();
        break;
    case 'G': // starts at 1
        base = 0;
        mode = 'G';
        submenuline(mode, base);
        break;
    case 'P': // starts at 0
        base = 0;
        mode = 'P';
        submenuline(mode, base);
        break;
    case 'S':
        this->status();  // Call status through this instance
        break;
    case 'T':
        knx.toggleProgMode();
        knx.loop();
        break;
    case 'V':
        base = 0;
        mode = 'V';
        submenuline(mode, base);
        break;
    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
        switch (mode)
        {
        case 'P':
            dumpParameter(b - '0' + base);
            break;
        case 'G':
            dumpGroupObject(b - '0' + base);
            break;
        case 'V':
            if (b - '0' < 7 && b - '0' >= 0)
                Log.setLevel(b - '0');
            switch (b - '0')
            {
            case 0:
                Println("Log level set to SILENT");
                break;
            case 1:
                Println("Log level set to FATAL");
                break;
            case 2:
                Println("Log level set to ERROR");
                break;
            case 3:
                Println("Log level set to WARNING");
                break;
            case 4:
                Println("Log level set to INFO");
                break;
            case 5:
                Println("Log level set to TRACE");
                break;
            case 6:
                Println("Log level set to VERBOSE");
                break;
            }
            break;
        }
        break;
    default:
        Log.error("Unknown command [%C] | [%x]\n", b, b);
    }
}
