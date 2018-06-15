/******************************************************************************
* display.h                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.06.18                                                           *
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

#define SEGMENTS_COUNT 3

//**************************<Types and Variables>******************************
uint8_t display_double_dot;


//**************************<Prototypes>***************************************

void init_display(void);

//display
//display_setSegment: pict: 7 columns of 5(last) bits,segmentnumber:0(out),1(out),2(out),3(niu),..
/*bsp:([
1* 00011111,
2* 00010101,
3* 00011111,
4* 00011011,
5* 00001110,
6* 00010101,
7* 00011011],0)
 */
void display_setSegment(uint8_t *pict,uint8_t segmentnumber);
void display_shiftright(uint8_t number);
void display_shiftleft(uint8_t number);

void  display_fill_col(uint8_t row, uint8_t col, uint8_t seg);
void  display_row_reset(void);
void  display_row_next(void);
void display_fill_current_row(void);
void display_show(void);
void display_show_ms(uint16_t ms);

#endif //#ifndef __DISPLAY_H
