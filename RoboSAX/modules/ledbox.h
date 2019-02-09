/******************************************************************************
* ledbox.h                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.2.0                                                              *
* Date   : 02.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __LEDBOX_H
#define __LEDBOX_H


//**************************<Included files>***********************************
#include "robolib.h"
#include "global.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<Macros>*******************************************
//TODO read LEDBOX_COUNT_ from number of LEDs in cycle
#define LEDBOX_COUNT_MAX 24

// buttons
#define LEDBOX_BUTTONS_DEBOUNCE_TIME 500

//**************************<Prototypes>***************************************

// init
void ledbox_init(void);

// rgb leds
void rgb_set(uint8_t number, enum eColor color);
void rgb_set2(uint8_t number, uint8_t r, uint8_t g, uint8_t b);
void rgb_setAll(enum eColor color);
void rgb_setAll2(uint8_t r, uint8_t g, uint8_t b);
void rgb_clearAll(void);

// ir leds
void ir_set(uint8_t number, uint8_t x);
void ir_setAll(uint8_t x);
void ir_clearAll(void);

// buttons
void buttons_reset(void);
void buttons_clear(void);
uint8_t buttons_get(uint8_t number);

// update
void _ledbox_rgb_update(void);
void _ledbox_buttons_and_ir_update(void);
void _ledbox_switchBuffer(void);

#endif //#ifndef __LEDBOX_H
