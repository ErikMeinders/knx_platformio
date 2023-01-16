/*
***************************************************************************  
**  Program  : Debug.h, part of FloorTempMonitor
**  Version  : v0.5.0
**
**  Copyright 2019, 2020, 2021, 2022 Willem Aandewiel
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

#ifndef DEBUG_H
#define DEBUG_H
#include <Arduino.h>

#ifndef DEBUG_OUT

   #define Debug(...) {}
   #define Debugln(...) {}
   #define Debugf(...) {}

   #define DebugT(...) {}
   #define DebugTln(...) {}
   #define DebugTf(...) {}

#else

   #define OUT1            DEBUG_OUT

   #define Debug(...)      ({ OUT1.print(__VA_ARGS__); })
   #define Debugln(...)    ({ OUT1.println(__VA_ARGS__);})
   #define Debugf(...)     ({ OUT1.printf(__VA_ARGS__);})

   #define DebugFlush()    ({ OUT1.flush();})

   void _debugBOL(const char *, int );

   #define DebugT(...)     ({ _debugBOL(__FUNCTION__, __LINE__);  \
                              Debug(__VA_ARGS__);                 \
                           })
   #define DebugTln(...)   ({ _debugBOL(__FUNCTION__, __LINE__);  \
                              Debugln(__VA_ARGS__);        \
                           })
   #define DebugTf(...)    ({ _debugBOL(__FUNCTION__, __LINE__);  \
                              Debugf(__VA_ARGS__);                \
                           })
#endif
#endif
/***************************************************************************
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the
* following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT
* OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR
* THE USE OR OTHER DEALINGS IN THE SOFTWARE.
* 
***************************************************************************/
