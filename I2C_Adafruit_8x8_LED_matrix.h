/*
  I2C_Adafruit_8x8_LED_matrix.h - for ESP-WROOM-02 ( esp8266 )
  Beta version 1.0
  This is the Arduino IDE library of Adafruit I2C 8x8 LED matrix.
  
The MIT License (MIT)

Copyright (c) 2016 Mgo-tec
Blog URL ---> https://www.mgo-tec.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef I2C_Adafruit_8x8_LED_matrix_h_
#define I2C_Adafruit_8x8_LED_matrix_h_
#include "Arduino.h"
#include "Wire.h"

class I2C_Adafruit_8x8_LED_matrix
{
public:
  I2C_Adafruit_8x8_LED_matrix();
  void LED_Driver_Initialize(uint8_t __LDaddrs);
  void LED_Driver_Setup(uint8_t __LDaddrs, uint8_t __on_off);
  void LED_Driver_Blink(uint8_t __LDaddrs, uint8_t __on_off, uint8_t __blink_Hz);
  void LED_Driver_Brightness(uint8_t __LDaddrs, uint8_t __brightness);
  void LED_Driver_DisplayInt(uint8_t __LDaddrs);
  void LED_Dot_Rotation(int16_t __angle, uint8_t* __buf, uint8_t* __RotBuf);
  void LED_Black_White_Reversal(boolean __rev, uint8_t* __buf, uint8_t* __buf_cnv);
  void LED_8x8mini_Disp_Out(uint8_t __LDaddrs, uint8_t* __Bdot1);
  void LED_8x8BiColor_Disp_Out(uint8_t __LDaddrs, uint8_t __color, byte* __Bdot1);
  void LED_Driver_DisplayOutput(uint8_t __LDaddrs, uint8_t* __DotB1, uint8_t* __DotB2);
private:
};

#endif