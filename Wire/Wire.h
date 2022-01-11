/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
  Modified 2015 by Chuck Todd <ctodd@cableone.net> to implement 
    1. repeated onRequestEvent() callbacks, this allows over 32bytes to be sent in Slave Read mode
	2. Timeouts in endTransmission(), requestFrom() incase bus is hung
	3. extend error reporting
	4. multiple Wire.write() statements in onRequestEvent(), fixes bug that only transmitted last Wire.write()
	5. Added I2C_anthing template from Nick Gammon
	6. Modified write(long),write
	
*/

#ifndef TwoWire_h
#define TwoWire_h

#include <inttypes.h>
#include "Stream.h"

#define BUFFER_LENGTH 32

class TwoWire : public Stream
{
  private:
    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static uint8_t transmitting;
    static void (*user_onRequest)(void);
    static void (*user_onReceive)(int);
    static void onRequestService(void);
    static void onReceiveService(uint8_t*, int);
  public:
    TwoWire();
    void begin();
    void begin(uint8_t);
    void begin(int);
    void setClock(uint32_t);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );
	uint8_t lastError( void); // chuck 17JUN2015 added to access twi_error
      // see #include <compat/twi.h> for meaning of lastError Values.
    inline size_t write(unsigned long n) { return write((uint8_t*)&n,sizeof n); } //chuck 21JUN2015
    inline size_t write(long n) { return write((uint8_t*)&n,sizeof n); }          //chuck 21JUN2015
    inline size_t write(unsigned int n) { return write((uint8_t*)&n, sizeof n); }
    inline size_t write(int n) { return write((uint8_t*)&n,sizeof n); }
    using Print::write;
};

extern TwoWire Wire;
// I2C anything templates Nick Gammon < http://www.gammon.com.au/electronics  >

template <typename T> int I2C_writeAnything (const T& value)
 {
   const uint8_t * p = (const uint8_t*) &value;
   unsigned int i;
   for (i = 0; i < sizeof value; i++)
         Wire.write(*p++);
   return i;
 }  // end of I2C_writeAnything

template <typename T> int I2C_readAnything(T& value)
 {
   uint8_t * p = (uint8_t*) &value;
   unsigned int i;
   for (i = 0; i < sizeof value; i++)
         *p++ = Wire.read();
   return i;
 }  // end of I2C_readAnything


#endif

