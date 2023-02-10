#ifndef _KNXP_APP_H
#define _KNXP_APP_H

extern unsigned long _cyclicKnxTimer_interval;

class _knxapp {
  public:
    virtual void pinsetup();
    virtual void conf();      // unlikely to be overridden
    virtual void setup();
    virtual void loop();
    virtual void cyclic();

    virtual char *hostname();
    virtual void status();
    virtual void progress(int step, const char *msg);

    void menu();
    void setCyclicTimer(unsigned long interval) { _cyclicKnxTimer_interval = interval * 1000; }
    void setGroupObjectCount(int count) { _groupObjectCount = count; }

  private:
    void help();
    void dumpParameter(int i);
    void dumpGroupObject(int i);
    void dumpEEPROM();

    int _groupObjectCount = 0;
};

extern _knxapp _knxApp;

#endif  
