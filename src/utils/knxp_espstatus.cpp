#include "knxp_platformio.h"
#include "utils/knxp_yield.h"
void _knxapp::esp_status()
{
#ifdef ESP8266
    // esp8266 specific status
    KnxYieldGuard yieldGuard;

    Printf("ESP8266 STATUS \n");
    Printf("  Now: %s\n", timeNowString());
    Printf("  Free Heap: %d\n", ESP.getFreeHeap());
    Printf("  Large Free Heap: %d\n", ESP.getFreeContStack());
    Printf("  Chip ID: %08X\n", ESP.getChipId());
    Printf("  CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
    yieldGuard.check();
    Printf("  Flash Chip ID: %08X\n", ESP.getFlashChipId());
    Printf("  Flash Chip Size: %d\n", ESP.getFlashChipSize());
    Printf("  Flash Chip Speed: %d\n", ESP.getFlashChipSpeed());
    Printf("  Sketch Size: %d\n", ESP.getSketchSize());
    yieldGuard.check();
    Printf("  Free Sketch Space: %d\n", ESP.getFreeSketchSpace());
    Printf("  Reset Reason: %s\n", ESP.getResetReason().c_str());
    Printf("  Reset Info: %s\n", ESP.getResetInfo().c_str());
    Printf("  Boot Version: %d\n", ESP.getBootVersion());
    Printf("  Boot Mode: %d\n", ESP.getBootMode());
    yieldGuard.check();
    Printf("  Core Version: %s\n", ESP.getCoreVersion().c_str());
    Printf("  SDK Version: %s\n", ESP.getSdkVersion());
    Printf("  Lib build %s %s\n", __DATE__, __TIME__); 
    
#endif

#ifdef ESP32
    // esp32 specific status
    KnxYieldGuard yieldGuard;
    Log.trace("ESP32 STATUS\n");
    Printf("  Now: %s\n", timeNowString());
    Printf("  SDK Version: %s\n", ESP.getSdkVersion());
    Printf("  Free Heap: %d\n", ESP.getFreeHeap());
    Printf("  Min Free Heap: %d\n", ESP.getMinFreeHeap());
    yieldGuard.check();
    Printf("  CPU Freq: %d MHz\n", ESP.getCpuFreqMHz());
    Printf("  Flash Chip Size: %d\n", ESP.getFlashChipSize());
    Printf("  Flash Chip Speed: %d\n", ESP.getFlashChipSpeed());
    /*
    Printf("  Flash Chip Mode: %s\n", ESP.getFlashChipMode() == FM_QIO ? "QIO" : 
                                        ESP.getFlashChipMode() == FM_QOUT ? "QOUT" : 
                                        ESP.getFlashChipMode() == FM_DIO ? "DIO" : 
                                        ESP.getFlashChipMode() == FM_DOUT ? "DOUT" : 
                                        "UNKNOWN");
    */
    yieldGuard.check();
    Printf("  Sketch Size: %d\n", ESP.getSketchSize());
    Printf("  Free Sketch Space: %d\n", ESP.getFreeSketchSpace());
    Printf("  Lib build %s %s\n", __DATE__, __TIME__);
    
#endif
}


void _knxapp::status()
{
    uint16_t ia = knx.individualAddress();

    Printf("Uptime: %s\n", uptime());

    esp_status();
    
    Printf("Network status\n");
    Printf("  Wifi: %s\n", WiFi.isConnected() ? "Connected" : "Disconnected");
    Printf("  Hostname: %s\n", this->hostname());
    Printf("  IP: %s\n", WiFi.localIP().toString().c_str());
    Printf("  Network ready: %s\n", isNetworkReady() ? "true" : "false");

    Printf("KNX status\n");
    Printf("  EEPROM size: %d\n", EEPROM.length());
    Printf("  Configured: %s\n", knx.configured() ? "true" : "false");
    Printf("  Individual Address: %d.%d.%d\n", ia >> 12, (ia >> 8) & 0x0F, ia & 0xFF);
    Printf("  Programming mode %s\n", knx.progMode() ? "Enabled" : "Disabled");
    Printf("  KNX Program Button pin: %d\n", knx.buttonPin());
    #ifdef KNXP_NEOPIXEL_PIN
    Printf("  Neopixel pin: %d\n", KNXP_NEOPIXEL_PIN);
    #endif
    
}
