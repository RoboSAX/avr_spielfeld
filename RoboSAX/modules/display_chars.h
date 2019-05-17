/*************static const *****************************************************************
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

 static const uint8_t space[7]={
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000,
0b00000000};

 static const uint8_t pic0[8]={
0b00011111,
0b00010101,
0b00011111,
0b00011011,
0b00001110,
0b00010101,
0b00011011,
(7<<4)|5};

 static const uint8_t Smile[8]={
0b00001010,
0b00001010,
0b00001010,
0b00000100,
0b00010001,
0b00011111,
0b00001110,
(7<<4)|5};

 static const uint16_t RoboF[8]={
0b0000000001111000,
0b0000000001111000,
0b0000001010110010,
0b0000000111111100,
0b0000000000110010,
0b0000000001001000,
0b0000000110000110,
(7<<8)|10};

 static const uint16_t Pokeball[8]={
0b0000000001111000,
0b0000000111111110,
0b0000001111111111,
0b0000001011001101,
0b0000001000110001,
0b0000000110000110,
0b0000000001111000,
(7<<8)|10};

 static const uint16_t RoboSax[8]={
0b0000001010110101,
0b0000000101001010,
0b0000000010110100,
0b0000000001111000,
0b0000000000110000,
0b0000000001111000,
0b0000000001001000,
(7<<8)|10};

 static const uint8_t Robol[8]={
0b00000011,
0b00000011,
0b00010101,
0b00001111,
0b00000001,
0b00000010,
0b00001100,
(7<<4)|5};

 static const uint8_t Robor[8]={
0b00011000,
0b00011000,
0b00010010,
0b00011100,
0b00010010,
0b00001000,
0b00000110,
(7<<4)|5};

//letters
 static const uint8_t alpaA[8]={
0b00000110,
0b00001001,
0b00001001,
0b00001111,
0b00001001,
0b00001001,
0b00001001,
(7<<4)|4};

 static const uint8_t alpaE[8]={
0b00001110,
0b00001000,
0b00001000,
0b00001111,
0b00001000,
0b00001000,
0b00001111,
(7<<4)|4};

 static const uint8_t alpaG[8]={
0b00000111,
0b00001001,
0b00001000,
0b00001000,
0b00001011,
0b00001001,
0b00000111,
(7<<4)|4};

 static const uint8_t alpaI[8]={
0b00001111,
0b00000110,
0b00000110,
0b00000110,
0b00000110,
0b00000110,
0b00001111,
(7<<4)|4};

 static const uint8_t alpaL[8]={
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001000,
0b00001111,
(7<<4)|4};

 static const uint8_t alpaM[8]={
0b00010001,
0b00011011,
0b00011111,
0b00010101,
0b00010001,
0b00010001,
0b00010001,
(7<<4)|5};

 static const uint8_t alpaN[8]={
0b00010001,
0b00011001,
0b00011101,
0b00010111,
0b00010011,
0b00010001,
0b00010001,
(7<<4)|5};

 static const uint8_t alpaS[8]={
0b00000110,
0b00001001,
0b00001000,
0b00000110,
0b00000001,
0b00001001,
0b00000110,
(7<<4)|4};

 static const uint8_t alpaT[8]={
0b00011111,
0b00010101,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
(7<<4)|5};

 static const uint8_t alpaW[8]={
0b00010001,
0b00010001,
0b00010001,
0b00010101,
0b00011111,
0b00011011,
0b00010001,
(7<<4)|5};

//others
static const uint8_t qestM[8]={
0b00000110,
0b00001001,
0b00000001,
0b00000010,
0b00000010,
0b00000000,
0b00000010,
(7<<4)|4};

//numbers
 static const uint8_t num1[8]={
0b00000010,
0b00000110,
0b00001010,
0b00000010,
0b00000010,
0b00000010,
0b00000010,
(7<<4)|4};

 static const uint8_t num2[8]={
0b00000110,
0b00001001,
0b00000001,
0b00000010,
0b00000100,
0b00001000,
0b00001111,
(7<<4)|4};

 static const uint8_t num3[8]={
0b00000110,
0b00001001,
0b00000001,
0b00000110,
0b00000001,
0b00001001,
0b00000110,
(7<<4)|4};

 static const uint8_t num4[8]={
0b00000010,
0b00000100,
0b00001010,
0b00001111,
0b00000010,
0b00000010,
0b00000010,
(7<<4)|4};

 static const uint8_t num5[8]={
0b00001111,
0b00001000,
0b00001110,
0b00000001,
0b00000001,
0b00001001,
0b00000110,
(7<<4)|4};

 static const uint8_t num6[8]={
0b00000110,
0b00001000,
0b00001000,
0b00001110,
0b00001001,
0b00001001,
0b00000110,
(7<<4)|4};

 static const uint8_t num7[8]={
0b00001111,
0b00000001,
0b00000010,
0b00000100,
0b00000100,
0b00000100,
0b00000100,
(7<<4)|4};

 static const uint8_t num8[8]={
0b00000110,
0b00001001,
0b00001001,
0b00000110,
0b00001001,
0b00001001,
0b00000110,
(7<<4)|4};

 static const uint8_t num9[8]={
0b00000110,
0b00001001,
0b00001001,
0b00000111,
0b00000001,
0b00000001,
0b00000110,
(7<<4)|4};

 static const uint8_t num0[8]={
0b00000110,
0b00001001,
0b00001001,
0b00001001,
0b00001001,
0b00001001,
0b00000110,
(7<<4)|4};

//**************************<Constants>****************************************
static const uint8_t *const numbers[10]={num0,num1,num2,num3,num4,num5,num6,num7,num8,num9};

#endif //#ifndef __DISPLAY_CHARS_H
