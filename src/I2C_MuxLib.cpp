/*
***************************************************************************  
**
**  File    : I2C_MuxLib.cpp
**  Version : v0.6.6
**
**  Copyright 2019, 2020, 2021, 2022 Willem Aandewiel
**
**  TERMS OF USE: MIT License. See bottom of file.                                                            
***************************************************************************      
*/

#include "I2C_MuxLib.h"

// Constructor
I2CMUX::I2CMUX() { 
  _statusTimer = millis();
  _status = 0;
  _I2Cbus = NULL;
  _I2Caddress = 0;
}

// Initializes the I2C_Multiplexer
// Returns false if I2C_Multiplexer is not detected
//-------------------------------------------------------------------------------------
bool I2CMUX::begin(TwoWire &wireBus, uint8_t deviceAddress)
{
  _I2Cbus = &wireBus;
  _I2Cbus->begin(); 
  _I2Cbus->setClock(100000L); // <-- don't be smart! This is the max you can get

  _I2Caddress = deviceAddress;

  if (connectedToMux() == false)
    return false; // Check for I2C_Relay_Multiplexer presence
  
  return true; // Everything is OK!

} // begin()

//-------------------------------------------------------------------------------------
bool I2CMUX::connectedToMux()
{
  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  if (_I2Cbus->endTransmission() != 0)
    return false; // I2C Slave did not ACK
  return true;
} 

//-------------------------------------------------------------------------------------
byte I2CMUX::getMajorRelease()
{
  while ((millis() - _statusTimer) < _READDELAY) {
    delay(1);
  }
  _statusTimer = millis();
  return (readReg1Byte(I2CMUX_MAJORRELEASE));
}
//-------------------------------------------------------------------------------------
byte I2CMUX::getMinorRelease()
{
  while ((millis() - _statusTimer) < _READDELAY) {
    delay(1);
  }
  _statusTimer = millis();
  return (readReg1Byte(I2CMUX_MINORRELEASE));
}

//-------------------------------------------------------------------------------------
byte I2CMUX::getWhoAmI()
{
  while ((millis() - _statusTimer) < _READDELAY) {
    delay(1);
  }
  _statusTimer = millis();
  return (readReg1Byte(I2CMUX_WHOAMI));
}

//-------------------------------------------------------------------------------------
byte I2CMUX::getStatus()
{
  while ((millis() - _statusTimer) < _READDELAY) {
    delay(1);
  }
  _statusTimer = millis();
  uint8_t tmpStatus = (byte)readReg1Byte(I2CMUX_STATUS);
  _status |= (byte)tmpStatus;
  return (tmpStatus);
}

//-------------------------------------------------------------------------------------
bool I2CMUX::writeCommand(byte command)
{
  Serial.printf("Command [%d]\n", command);
  return writeReg1Byte(I2CMUX_COMMAND, command);
}

//-------------------------------------------------------------------------------------
bool I2CMUX::pinMode(byte GPIO_PIN, byte PINMODE)
{
  return writeCommand3Bytes(_BV(CMD_PINMODE), GPIO_PIN, PINMODE) ;
}

//-------------------------------------------------------------------------------------
bool I2CMUX::digitalRead(byte GPIO_PIN)
{
  if (writeCommand2Bytes(_BV(CMD_DIGITALREAD), GPIO_PIN)) {
    delay(2);
    return readReg1Byte(I2CMUX_LASTGPIOSTATE);
  }
  return false;
}

//-------------------------------------------------------------------------------------
bool I2CMUX::digitalWrite(byte GPIO_PIN, byte HIGH_LOW)
{
  return(writeCommand3Bytes(_BV(CMD_DIGITALWRITE), GPIO_PIN, HIGH_LOW));
}

// Change the I2C address of this I2C Slave address to newAddress
//-------------------------------------------------------------------------------------
bool I2CMUX::setI2Caddress(uint8_t newAddress)
{
  if (writeReg1Byte(I2CMUX_WHOAMI, newAddress)) {
    // Once the address is changed, we need to change it in the library
    _I2Caddress = newAddress;
    // -->> writeReg1Byte(I2CMUX_COMMAND, I2CMUX_WHOAMI);
    return true;
  }
  return false;

} // newAddress()


//-------------------------------------------------------------------------------------
//-------------------------- READ FROM REGISTERS --------------------------------------
//-------------------------------------------------------------------------------------

// Reads a uint8_t from a register @addr
//-------------------------------------------------------------------------------------
uint8_t I2CMUX::readReg1Byte(uint8_t addr)
{  
  while ((millis() - _statusTimer) < _READDELAY) {
    delay(1);
  }
  _statusTimer = millis();

  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  yield();
  _I2Cbus->write(addr);
  yield();
  if (_I2Cbus->endTransmission() != 0) {
    return (0); // Slave did not ack
  }

  _I2Cbus->requestFrom((uint8_t)_I2Caddress, (uint8_t) 1);
  delay(5);
  if (_I2Cbus->available()) {
    yield();
    return (_I2Cbus->read());
    yield();
  }

  return (0); // Slave did not respond
}

// Reads an int16_t from a register @addr
//-------------------------------------------------------------------------------------
int16_t I2CMUX::readReg2Byte(uint8_t addr)
{
  while ((millis() - _statusTimer) < _READDELAY) {
    delay(1);
  }
  _statusTimer = millis();

  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  yield();
  _I2Cbus->write(addr);
  yield();
  if (_I2Cbus->endTransmission() != 0) {
    return (0); // Slave did not ack
  }

  _I2Cbus->requestFrom((uint8_t)_I2Caddress, (uint8_t) 2);
  delay(5);
  if (_I2Cbus->available()) {
    yield();
    uint8_t LSB = _I2Cbus->read();
    yield();
    uint8_t MSB = _I2Cbus->read();
    yield();
    return ((int16_t)MSB << 8 | LSB);
  }

  return (0); // Slave did not respond
}

// Reads an int32_t from a register @addr
//-------------------------------------------------------------------------------------
int32_t I2CMUX::readReg4Byte(uint8_t addr)
{
  while ((millis() - _statusTimer) < _READDELAY) {
    delay(1);
  }
  _statusTimer = millis();

  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  yield();
  _I2Cbus->write(addr);
  yield();
  if (_I2Cbus->endTransmission() != 0) {
    return (0); // Slave did not ack
  }

  _I2Cbus->requestFrom((uint8_t)_I2Caddress, (uint8_t) 4);
  delay(5);
  if (_I2Cbus->available()) {
    yield();
    uint8_t LSB   = _I2Cbus->read();
    yield();
    uint8_t mLSB  = _I2Cbus->read();
    yield();
    uint8_t mMSB  = _I2Cbus->read();
    yield();
    uint8_t MSB   = _I2Cbus->read();
    yield();
    // uint32_t comb = MSB << 24 | mMSB << 16 | mLSB << 8 | LSB;
    return ((int32_t)MSB << 24 | mMSB << 16 | mLSB << 8 | LSB);
  }

  return (0); // Slave did not respond
}

//-------------------------------------------------------------------------------------
//-------------------------- WRITE TO REGISTERS ---------------------------------------
//-------------------------------------------------------------------------------------

// Write a 1 byte value to a register
//-------------------------------------------------------------------------------------
bool I2CMUX::writeReg1Byte(uint8_t addr, uint8_t val)
{
  while ((millis() - _statusTimer) < _WRITEDELAY) {
    delay(1);
  }
  _statusTimer = millis();

  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  yield();
  _I2Cbus->write(addr);
  yield();
  _I2Cbus->write(val);
  yield();
  if (_I2Cbus->endTransmission() != 0) {
    return (false); // Slave did not ack
  }

  return (true);
}

// Write a 2 byte value to a register
//-------------------------------------------------------------------------------------
bool I2CMUX::writeReg2Byte(uint8_t addr, int16_t val)
{
  while ((millis() - _statusTimer) < _WRITEDELAY) {
    delay(1);
  }
  _statusTimer = millis();

  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  yield();
  _I2Cbus->write(addr);
  yield();
  _I2Cbus->write(val & 0xFF); // LSB
  yield();
  _I2Cbus->write(val >> 8);   // MSB
  yield();
  if (_I2Cbus->endTransmission() != 0) {
    return (false); // Slave did not ack
  }

  return (true);
}


// Write a 3 byte value to a register
//-------------------------------------------------------------------------------------
bool I2CMUX::writeReg3Byte(uint8_t addr, int32_t val)
{
  while ((millis() - _statusTimer) < _WRITEDELAY) {
    delay(1);
  }
  _statusTimer = millis();

  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  yield();
  _I2Cbus->write(addr);
  yield();
  _I2Cbus->write(val &0xFF);     // LSB
  yield();
  _I2Cbus->write(val >> 8);       // mLSB
  yield();
  _I2Cbus->write(val >> 16);      // mMSB
  yield();
  //_I2Cbus->write(val >> 24);    // MSB
  if (_I2Cbus->endTransmission() != 0) {
    return (false); // Slave did not ack
  }

  return (true);
}

// Write a 4 byte value to a register
//-------------------------------------------------------------------------------------
bool I2CMUX::writeReg4Byte(uint8_t addr, int32_t val)
{
  while ((millis() - _statusTimer) < _WRITEDELAY) {
    delay(1);
  }
  _statusTimer = millis();

  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  yield();
  _I2Cbus->write(addr);
  yield();
  _I2Cbus->write(val & 0xFF); // LSB
  yield();
  _I2Cbus->write(val >> 8);   // mLSB
  yield();
  _I2Cbus->write(val >> 16);  // mMSB
  yield();
  _I2Cbus->write(val >> 24);  // MSB
  if (_I2Cbus->endTransmission() != 0) {
    return (false); // Slave did not ack
  }

  return (true);
}

// Write a 2 byte's command I2C_Mux Slave
//-------------------------------------------------------------------------------------
bool I2CMUX::writeCommand2Bytes(byte CMD, byte GPIO_PIN)
{
  while ((millis() - _statusTimer) < _WRITEDELAY) {
    delay(5);
  }
  _statusTimer = millis();

  //Serial.printf("\nwriteCommand2Bytes: CMD[%d], GPIO[%d] \n", CMD, GPIO_PIN);
  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  yield();
  _I2Cbus->write(I2CMUX_COMMAND);
  yield();
  // val is [-------- cccccccc pppppppp]
  _I2Cbus->write(CMD);          // Command
  yield();
  _I2Cbus->write(GPIO_PIN);     // GPIO_PIN
  yield();
  if (_I2Cbus->endTransmission() != 0) {
    return (false); // Slave did not ack
  }
  return (true);
}

// Write a 3 byte's command I2C_Mux Slave
//-------------------------------------------------------------------------------------
bool I2CMUX::writeCommand3Bytes(byte CMD, byte GPIO_PIN, byte HIGH_LOW)
{
  while ((millis() - _statusTimer) < _WRITEDELAY) {
    delay(5);
  }
  _statusTimer = millis();

  //Serial.printf("\nwriteCommand3Bytes: CMD[%d], GPIO[%d], HL[%d] \n", CMD, GPIO_PIN, HIGH_LOW);
  _I2Cbus->beginTransmission((uint8_t)_I2Caddress);
  yield();
  _I2Cbus->write(I2CMUX_COMMAND);
  yield();
  // val is [-------- cccccccc pppppppp vvvvvvvv]
  _I2Cbus->write(CMD);          // Command
  yield();
  _I2Cbus->write(GPIO_PIN);     // GPIO_PIN
  yield();
  _I2Cbus->write(HIGH_LOW);     // HIGH_LOW
  yield();
  if (_I2Cbus->endTransmission() != 0) {
    return (false); // Slave did not ack
  }

  return (true);
}

//-------------------------------------------------------------------------------------
//-------------------------- HELPERS --------------------------------------------------
//-------------------------------------------------------------------------------------

//===========================================================================================
//assumes little endian
void I2CMUX::showRegister(size_t const size, void const * const ptr)
{
  unsigned char *b = (unsigned char*) ptr;
  unsigned char byte;
  int i, j;
  Serial.print("[");
  for (i=(int)(size-1); i>=0; i--) {
    if (i != (int)(size-1)) Serial.print(" ");
    for (j=7; j>=0; j--) {
      byte = (b[i] >> j) & 1;
      Serial.print(byte);
    }
  }
  Serial.print("] ");
} // showRegister()


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
