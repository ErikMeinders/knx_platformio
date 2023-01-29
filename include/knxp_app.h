class _knxapp {
  public:
    virtual void pinsetup();
    virtual void conf();
    virtual void setup();
    virtual void loop();

    virtual char *hostname();
    virtual void status();
    virtual void progress(int step, const char *msg);
};

// your application implementation of the _knxapp class methods

class knxapp : public _knxapp
{
  public:
    void setup() ;
    void loop();
};

extern _knxapp _knxApp;
extern knxapp knxApp;