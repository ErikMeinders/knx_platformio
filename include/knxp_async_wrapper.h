#ifndef KNXP_ASYNC_WRAPPER_H
#define KNXP_ASYNC_WRAPPER_H

#ifdef ESP8266
// Save existing HTTP method definitions if any
#ifdef HTTP_GET
#define SAVED_HTTP_GET HTTP_GET
#undef HTTP_GET
#endif

#ifdef HTTP_POST
#define SAVED_HTTP_POST HTTP_POST
#undef HTTP_POST
#endif

#ifdef HTTP_DELETE
#define SAVED_HTTP_DELETE HTTP_DELETE
#undef HTTP_DELETE
#endif

#ifdef HTTP_PUT
#define SAVED_HTTP_PUT HTTP_PUT
#undef HTTP_PUT
#endif

#ifdef HTTP_PATCH
#define SAVED_HTTP_PATCH HTTP_PATCH
#undef HTTP_PATCH
#endif

#ifdef HTTP_HEAD
#define SAVED_HTTP_HEAD HTTP_HEAD
#undef HTTP_HEAD
#endif

#ifdef HTTP_OPTIONS
#define SAVED_HTTP_OPTIONS HTTP_OPTIONS
#undef HTTP_OPTIONS
#endif

#ifdef HTTP_ANY
#define SAVED_HTTP_ANY HTTP_ANY
#undef HTTP_ANY
#endif

// Include async libraries
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>

// Restore original HTTP method definitions
#ifdef SAVED_HTTP_GET
#define HTTP_GET SAVED_HTTP_GET
#undef SAVED_HTTP_GET
#endif

#ifdef SAVED_HTTP_POST
#define HTTP_POST SAVED_HTTP_POST
#undef SAVED_HTTP_POST
#endif

#ifdef SAVED_HTTP_DELETE
#define HTTP_DELETE SAVED_HTTP_DELETE
#undef SAVED_HTTP_DELETE
#endif

#ifdef SAVED_HTTP_PUT
#define HTTP_PUT SAVED_HTTP_PUT
#undef SAVED_HTTP_PUT
#endif

#ifdef SAVED_HTTP_PATCH
#define HTTP_PATCH SAVED_HTTP_PATCH
#undef SAVED_HTTP_PATCH
#endif

#ifdef SAVED_HTTP_HEAD
#define HTTP_HEAD SAVED_HTTP_HEAD
#undef SAVED_HTTP_HEAD
#endif

#ifdef SAVED_HTTP_OPTIONS
#define HTTP_OPTIONS SAVED_HTTP_OPTIONS
#undef SAVED_HTTP_OPTIONS
#endif

#ifdef SAVED_HTTP_ANY
#define HTTP_ANY SAVED_HTTP_ANY
#undef SAVED_HTTP_ANY
#endif

#elif defined(ESP32)
// ESP32 doesn't need HTTP method handling
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#endif

#endif // KNXP_ASYNC_WRAPPER_H
