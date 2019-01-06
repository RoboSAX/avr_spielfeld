/******************************************************************************
* display_chars.h                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __DISPLAY_CHARS_H
#define __DISPLAY_CHARS_H
#include <avr/io.h>
#include <inttypes.h>

 uint8_t space[7]={
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000};

 uint8_t pic0[7]={
0b00011111,
0b00010101,
0b00011111,
0b00011011,
0b00001110,
0b00010101,
0b00011011};

 uint8_t Smile[7]={
0b00001010,
0b00001010,
0b00001010,
0b00000100,
0b00010001,
0b00011111,
0b00001110};

 uint8_t Robol[7]={
0b00000011,
0b00000011,
0b00010101,
0b00001111,
0b00000001,
0b00000010,
0b00001100};

 uint8_t Robor[7]={
0b00011000,
0b00011000,
0b00010010,
0b00011100,
0b00010010,
0b00001000,
0b00000110};

//stuff
 uint8_t alpaT[7]={
0b00011111,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
0b00000100};

 uint8_t alpaW[7]={
0b00010001,
0b00010001,
0b00010101,
0b00010101,
0b00010101,
0b00010101,
0b00001110};

 uint8_t alpaS[7]={
0b00001110,
0b00010001,
0b00010000,
0b00001110,
0b00000001,
0b00010001,
0b00001110};

 uint8_t qestM[7]={
0b00001110,
0b00010001,
0b00001000,
0b00000100,
0b00000000,
0b00001100,
0b00001100};

//numbers
 uint8_t num1[7]={
0b00000010,
0b00000110,
0b00001010,
0b00000010,
0b00000010,
0b00000010,
0b00000010};

 uint8_t num2[7]={
0b00000110,
0b00001001,
0b00000001,
0b00000010,
0b00000100,
0b00001000,
0b00001111};

 uint8_t num3[7]={
0b00001110,
0b00010001,
0b00000001,
0b00001110,
0b00000001,
0b00010001,
0b00001110};

 uint8_t num4[7]={
0b00000010,
0b00000100,
0b00001010,
0b00011111,
0b00000010,
0b00000010,
0b00000010};

 uint8_t num5[7]={
0b00001110,
0b00010000,
0b00010000,
0b00001110,
0b00000001,
0b00000001,
0b00001110};

 uint8_t num6[7]={
0b00001110,
0b00010000,
0b00010000,
0b00011110,
0b00010001,
0b00010001,
0b00001110};

 uint8_t num7[7]={
0b00011111,
0b00000010,
0b00000100,
0b00001000,
0b00001000,
0b00001000,
0b00001000};

 uint8_t num8[7]={
0b00001110,
0b00010001,
0b00010001,
0b00001110,
0b00010001,
0b00010001,
0b00001110};

 uint8_t num9[7]={
0b00001110,
0b00010001,
0b00010001,
0b00001111,
0b00000001,
0b00000001,
0b00001110};

 uint8_t num0[7]={
0b00000110,
0b00001001,
0b00001001,
0b00001001,
0b00001001,
0b00001001,
0b00000110};

#endif //#ifndef __DISPLAY_CHARS_H
