#ifndef _KNXP_APP_H
#define _KNXP_APP_H

class _knxapp {
  public:
    virtual void pinsetup();
    virtual void conf();
    virtual void setup();
    virtual void loop();

    virtual char *hostname();
    virtual void status();
    virtual void progress(int step, const char *msg);

    void menu();

  private:
    void help();
    void dumpParameter(int i);
    void dumpGroupObject(int i);
    void dumpEEPROM();
};

extern _knxapp _knxApp;

#endif  
