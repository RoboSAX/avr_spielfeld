/******************************************************************************
* display.h                                                                   *
* =========                                                                   *
*                                                                             *
* Version: 1.1.0                                                              *
* Date   : 27.12.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __DISPLAY_H
#define __DISPLAY_H


//**************************<Included files>***********************************
#include "robolib.h"
#include "global.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<PORTDEF>*******************************************
//**************************<Macros>*******************************************

#define SEGMENTS_COUNT 4
#define SUPER_SEGMENTS_COUNT 2

//**************************<Types and Variables>******************************
uint8_t display_double_dot;


//**************************<Prototypes>***************************************

void init_display(void);

//! display_setSegment:
//!     pict         : 7 columns of up tp 8 or 16(last) bits
//!			+(optional) Byte or word with first half= (max) hight
//!					    second half= width
//!     segmentnumber: 0..3
//!     e.g. display_setSegment([
//!         00011111,
//!         00010101,
//!         00011111,
//!         00011011,
//!         00001110,
//!         00010101,
//!         00011011],0);
void display_setSegment(const uint8_t *pict,uint8_t segmentnumber);
void display_invertSegment(uint8_t segmentnumber);
//void display_shiftright(uint8_t number);
//void display_shiftleft (uint8_t number);

//! display_addLeft/Right:
//!     pict         : 7 columns of up tp 8 or 16(last) bits
//!			+ Byte or word with first half= (max) hight
//!					    second half= width
//!     supersegmentnumber: 0..1
//!     sparse: true=one free column to the next char
//!     e.g. display_addLeft([
//!         00000000,
//!         00000000,
//!         00000001,
//!         00000011,
//!         00000101,
//!         00000001,
//!         00000001,
//!         (5<<4)|3],0);
void display_setSuperSegment(const uint16_t *pict,uint8_t supersegmentnumber);
void display_clearSuperSegment(uint8_t supersegmentnumber);
void display_invertSuperSegment(uint8_t supersegmentnumber);
void display_addLeft(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse);
void display_addRight(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse);
//void display_addAbove(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse);
//void display_addBelow(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse);
//void display_windowShiftright(uint8_t number);
//void display_windowShiftleft (uint8_t number);

void display_show(void);
void display_show_ms(uint16_t ms);

#endif //#ifndef __DISPLAY_H
