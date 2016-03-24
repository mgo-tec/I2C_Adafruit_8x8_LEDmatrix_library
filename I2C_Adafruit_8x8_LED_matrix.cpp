/*
  I2C_Adafruit_8x8_LED_matrix.cpp - for ESP-WROOM-02 ( esp8266 )
  Beta version 1.1
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

#include "Arduino.h"
#include "I2C_Adafruit_8x8_LED_matrix.h"

I2C_Adafruit_8x8_LED_matrix::I2C_Adafruit_8x8_LED_matrix(){}

//********************** HT16K33 Initialize *******************
void I2C_Adafruit_8x8_LED_matrix::LED_Driver_Initialize(uint8_t __LDaddrs)
{
  I2C_Adafruit_8x8_LED_matrix::LED_Driver_Setup( __LDaddrs, 1); //HT16K33 System oscillator ON
  I2C_Adafruit_8x8_LED_matrix::LED_Driver_Blink( __LDaddrs, 1, 0); //__blink_Hz=0=off, 1=2Hz, 2=1Hz, 3=0.5Hz, __on_off=0=OFF, 1=ON
  I2C_Adafruit_8x8_LED_matrix::LED_Driver_Brightness( __LDaddrs, 0 ); // __brightness= 0～15
  I2C_Adafruit_8x8_LED_matrix::LED_Driver_DisplayInt( __LDaddrs ); //Display Black OUT
}

//********************** HT16K33 SetUp *******************
void I2C_Adafruit_8x8_LED_matrix::LED_Driver_Setup(uint8_t __LDaddrs, uint8_t __on_off)
{
  Wire.beginTransmission(__LDaddrs);
  Wire.write(0x20 | __on_off);  //HT16K33 System oscillator ON
  Wire.endTransmission();
}
//********************** HT16K33 Blink SetUp *******************
void I2C_Adafruit_8x8_LED_matrix::LED_Driver_Blink(uint8_t __LDaddrs, uint8_t __on_off, uint8_t __blink_Hz)
{
  //__blink_Hz=0=off, 1=2Hz, 2=1Hz, 3=0.5Hz, __on_off=0=OFF, 1=ON
  Wire.beginTransmission(__LDaddrs);
  Wire.write(0x80 | (__blink_Hz<<1) | __on_off); 
  Wire.endTransmission();
}
//********************** HT16K33 Brightness *********************
void I2C_Adafruit_8x8_LED_matrix::LED_Driver_Brightness(uint8_t __LDaddrs, uint8_t __brightness)
{
  // __brightness= 0～15
  Wire.beginTransmission(__LDaddrs);
  Wire.write(0xE0 | __brightness);
  Wire.endTransmission();
}
//********************** HT16K33 Display Initialize *********************
void I2C_Adafruit_8x8_LED_matrix::LED_Driver_DisplayInt(uint8_t __LDaddrs)
{
  Wire.beginTransmission(__LDaddrs);
  Wire.write(0x00);
  for(int __i=0;__i<8;__i++){  
    Wire.write(B00000000);
    Wire.write(B00000000);
  }
  Wire.endTransmission();
}
//**************************8X8 Dot Rotation****************************************
void I2C_Adafruit_8x8_LED_matrix::LED_Dot_Rotation(int16_t __angle, uint8_t* __buf, uint8_t* __RotBuf)
{
  uint8_t __col,__row;

  switch(__angle){
    case 0:
      for( __row = 0; __row < 8; __row++){
        __RotBuf[__row] = __buf[__row];
      }
      break;
    case 90:
      for( __col = 0; __col < 8; __col++ ) {
        for( __row = 0; __row < 8; __row++ ) {
          bitWrite( __RotBuf[7-__row], 7-__col , bitRead( __buf[__col], 7-__row ) );
        }
      }
      break;
    case 180:
      for( __col = 0; __col < 8; __col++ ) {
        for( __row = 0; __row < 8; __row++ ) {
          bitWrite( __RotBuf[7-__col], 7-__row , bitRead( __buf[__col], __row ) );
        }
      }
      break;
    case -90: //-90=270
      for( __col = 0; __col < 8; __col++ ) {
        for( __row = 0; __row < 8; __row++ ) {
          bitWrite( __RotBuf[__row], __col , bitRead( __buf[__col], 7-__row ) );
        }
      }
      break;
    default:
      for( __row = 0; __row < 8; __row++){
        __RotBuf[__row] = __buf[__row];
      }
      break;
  }
}
//*************************Black_White_Reversal**************************************
void I2C_Adafruit_8x8_LED_matrix::LED_Black_White_Reversal(boolean __rev, uint8_t* __buf, uint8_t* __buf_cnv)
{
  if(__rev == true){
    for(int __i = 0; __i < 8; __i++){
       __buf[__i] = ~__buf_cnv[__i];
    }
  }
}

//*************************Dot Scroll Replace**************************************
void I2C_Adafruit_8x8_LED_matrix::Scroller_Dot_Replace(uint8_t __drection, uint8_t* __next_buff, uint8_t* __scl_buff_1, uint8_t* __Orign_buff)
{
  uint8_t __i;

  switch( __drection ){
    case 0:        // R <--- L
      for(__i=0 ; __i<8 ; __i++){
        bitWrite( __next_buff[__i],7, bitRead( __scl_buff_1[__i],7));
        __scl_buff_1[__i] = __scl_buff_1[__i]<<1;
        bitWrite( __scl_buff_1[__i],0, bitRead( __Orign_buff[__i],7));
        __Orign_buff[__i] = __Orign_buff[__i]<<1;
      }
      break;
    case 1:        // L ---> R
      for(__i=0 ; __i<8 ; __i++){
        bitWrite( __next_buff[__i],0, bitRead( __scl_buff_1[__i],0));
        __scl_buff_1[__i] = __scl_buff_1[__i]>>1;
        bitWrite( __scl_buff_1[__i],7, bitRead( __Orign_buff[__i],0));
        __Orign_buff[__i] = __Orign_buff[__i]>>1;
      }
      break;
    case 2:      // Lower ---> Upper
      __next_buff[0] = __scl_buff_1[0];
      for(__i=0 ; __i<7 ; __i++){
        __scl_buff_1[__i] = __scl_buff_1[__i+1];
      }
      __scl_buff_1[7] = __Orign_buff[0];
      for(__i=0 ; __i<7 ; __i++){
        __Orign_buff[__i] = __Orign_buff[__i+1];
      }
      break;
    case 3:        // Upper ---> Lower
      __next_buff[7] = __scl_buff_1[7];
      for(__i=7 ; __i>0 ; __i--){
        __scl_buff_1[__i] = __scl_buff_1[__i-1];
      }
      __scl_buff_1[0] = __Orign_buff[7];
      for(__i=7 ; __i>0 ; __i--){
        __Orign_buff[__i] = __Orign_buff[__i-1];
      }
      break;
  }
}

//**********************Adafruit Mini 8x8 Dot Bit Convert & Display OUT******************************
void I2C_Adafruit_8x8_LED_matrix::LED_8x8mini_Disp_Out(uint8_t __LDaddrs, uint8_t* __Bdot1)
{
  uint8_t __Bdot2[8];
  uint8_t __Bdot_0[8] = {0,0,0,0,0,0,0,0};
  for(uint8_t __i=0; __i<8; __i++){
    for(uint8_t __j=0; __j<7; __j++){
     bitWrite(__Bdot2[__i],6-__j,bitRead(__Bdot1[__i],__j));
    }
    bitWrite( __Bdot2[__i],7,bitRead(__Bdot1[__i],7));  
  }
  I2C_Adafruit_8x8_LED_matrix::LED_Driver_DisplayOutput(__LDaddrs, __Bdot2, __Bdot_0);
}
//**********************Adafruit Bi-Color Dot Bit Convert & Display OUT******************************
void I2C_Adafruit_8x8_LED_matrix::LED_8x8BiColor_Disp_Out(uint8_t __LDaddrs, uint8_t __color, byte* __Bdot1)
{
  uint8_t __Bdot2[8];
  uint8_t __Bdot_0[8] = {0,0,0,0,0,0,0,0};
  for(uint8_t __i=0; __i<8; __i++){
    for(uint8_t __j=0; __j<8; __j++){
     bitWrite(__Bdot2[__j], __i, bitRead(__Bdot1[__i],__j));
    }
  }
  switch(__color){
    case 0: //green
      I2C_Adafruit_8x8_LED_matrix::LED_Driver_DisplayOutput(__LDaddrs, __Bdot2, __Bdot_0);
      break;
    case 1: //red
      I2C_Adafruit_8x8_LED_matrix::LED_Driver_DisplayOutput(__LDaddrs, __Bdot_0, __Bdot2);
      break;
    case 2: //orange
      I2C_Adafruit_8x8_LED_matrix::LED_Driver_DisplayOutput(__LDaddrs, __Bdot2, __Bdot2);
      break;
  }
  
}
//********************** HT16K33 8X8 I2C Data Send *******************
void I2C_Adafruit_8x8_LED_matrix::LED_Driver_DisplayOutput(uint8_t __LDaddrs, uint8_t* __DotB1, uint8_t* __DotB2)
{
  int __i;
    Wire.beginTransmission(__LDaddrs);
    Wire.write(B00000000);
    for(__i = 0; __i<8; __i++){
      Wire.write(__DotB1[__i]);
      Wire.write(__DotB2[__i]);
    }
    Wire.endTransmission();
}