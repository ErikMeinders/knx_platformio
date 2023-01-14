#ifndef LITTLEFSMANAGER_H

#define LITTLEFSMANAGER_H

#define _MAX_LITTLEFS_FILES     20

#include <Arduino.h>
#include <LittleFS.h>
#include <WebServer.h>
#include <Debug.h>
#include "network.h"

void setupFSmanager();
bool handleList() ;
void deleteRecursive(const char *path) ;
bool handleFile(String &&path) ;
void handleUpload() ;
void formatFS()  ;   
void listFS() ;
void sendResponce() ;
const String formatBytes(size_t const& bytes) ;
void reBootESP();
void doRedirect(String msg, int wait, const char* URL, bool reboot,const char * hostname);
String getContentType(String filename);
int sortFunction(const void *cmp1, const void *cmp2);

#endif
