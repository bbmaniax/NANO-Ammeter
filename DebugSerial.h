/*
  DebugSerial Output Library

  A library for debug serial communication.
*/

#ifndef __DEBUG_SERIAL_H__
#define __DEBUG_SERIAL_H__

#include <Arduino.h>

#if defined(DEBUG)
#define DEBUG_SERIAL_BEGIN(x) Serial.begin(x)
#define DEBUG_SERIAL_WAIT_FOR() delay(100)
#define DEBUG_SERIAL_PRINT(x) Serial.print(x)
#define DEBUG_SERIAL_PRINTLN(x) Serial.println(x)
#define DEBUG_SERIAL_PRINTHEX(x) Serial.print(x, HEX)
#define DEBUG_SERIAL_PRINTLNHEX(x) Serial.println(x, HEX)
#else
#define DEBUG_SERIAL_BEGIN(x)
#define DEBUG_SERIAL_WAIT_FOR()
#define DEBUG_SERIAL_PRINT(x)
#define DEBUG_SERIAL_PRINTLN(x)
#define DEBUG_SERIAL_PRINTHEX(x)
#define DEBUG_SERIAL_PRINTLNHEX(x)
#endif

#endif  // __DEBUG_SERIAL_H__
