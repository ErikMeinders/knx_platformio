#ifndef KNXP_WEB_BASE_H
#define KNXP_WEB_BASE_H

#include <Arduino.h>

/**
 * Base class for web server implementations.
 * Provides common interface for both synchronous and asynchronous servers.
 */
class KNXWebServerBase {
public:
    virtual ~KNXWebServerBase() = default;
    
    /**
     * Initialize and start the web server
     * @return true if server started successfully
     */
    virtual bool begin() = 0;
    
    /**
     * Process server tasks in main loop
     */
    virtual void loop() = 0;

protected:
    /**
     * Get content type based on file extension
     * @param filename File name to get content type for
     * @return Content type string
     */
    virtual const char* getContentType(const String& filename) = 0;
};

// Factory function declaration
KNXWebServerBase* createWebServer();

#endif // KNXP_WEB_BASE_H
