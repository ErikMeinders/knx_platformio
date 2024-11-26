#ifndef _KNXP_APP_H
#define _KNXP_APP_H

extern unsigned long _cyclicKnxTimer_interval;
/**
 * @brief Base class for application
 * 
 * Implements arduino setup() and loop(). Calls pinsetup(), conf(), setup() and loop() of derived class.
 * To use cyclic() call setCyclicTimer() with the interval in seconds and setGroupObjectCount() with the number of group objects.
 * 
 */
class _knxapp {
  public:
    virtual ~_knxapp() = default;  // Virtual destructor for proper cleanup

    /**
     * @brief First application call to setup pins
     * 
     */
    virtual void pinsetup();
    /**
     * @brief Second application call to configure KNX
     * Unlikely your application will override this
     *
    */
    virtual void conf();      // unlikely to be overridden
    /**
     * @brief Third application call to setup your application
     * Now the network is active and your application functional setup can run
     * This is where you setup your Group Objects (DPTs & callbacks)
     * 
     */
    virtual void setup();
    /**
     * @brief Your application loop
     * All other services are handled in the background by the library
     * 
     */
    virtual void loop();
    /**
     * @brief Send KNX values cyclically
     * Unlikely your application will override this
     * 
     */
    void cyclic();
    /**
     * @brief Get the Hostname object
     * 
     * @return const char* - expects HOSTNAME to be defined in platformio.ini
     */
    virtual char *hostname();
    /**
     * @brief Dump the internal status of the application
     * Override to add your own status information
     * 
     */
    virtual void status();
    /**
     * @brief Progress indicator showing the application setup progress
     * 
     * @param step 
     * @param msg 
     */
    virtual void progress(int step, const char *msg);

    /**
     * @brief Set the Cyclic Timer (how ofter to send cyclic values - 0 = off)
     * 
     * @param interval - in seconds
     */
    void setCyclicTimer(unsigned long interval);
    /**
     * @brief Set the Group Object Count (how many group objects to consider to send cyclic values)
     * 
     * @param count 
     */
    void setGroupObjectCount(int count) { _groupObjectCount = count; }
    /**
     * @brief To be called to present the menu and process the user input
     * 
     */
    void menu();

  private:
    void help();
    void dumpParameter(int i);
    void dumpGroupObject(int i);
    void dumpEEPROM();

    void status_8266();
    void status_32();

    int _groupObjectCount = 0;
};

#endif
