/******************************************************************************
* master.h                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 31.05.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __MASTER_H
#define __MASTER_H


//**************************<Included files>***********************************
#include "robolib.h"
#include "global.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<Macros>*******************************************
// leds (only for testing - will be removed)
#define LED1_PORT      PORTD
#define LED1_DDR       DDRD
#define LED1_PIN       _BV(1)
#define debug_led1(x)  (x ? (LED1_PORT&= ~LED1_PIN) : (LED1_PORT|= LED1_PIN))
#define _debug_led1_enable() (LED1_DDR|= LED1_PIN)

#define LED2_PORT      PORTD
#define LED2_DDR       DDRD
#define LED2_PIN       _BV(4)
#define debug_led2(x)  (x ? (LED2_PORT|= LED2_PIN) : (LED2_PORT&= ~LED2_PIN))
#define _debug_led2_enable() (LED2_DDR|= LED2_PIN)

// buttons
#define MASTER_BUTTONS_DEBOUNCE_TIME 25

#define master_button1() (master_buttons_get(1))
#define master_button2() (master_buttons_get(2))
#define master_button3() (master_buttons_get(3))


//**************************<Prototypes>***************************************
void master_init(void);

//buttons
void master_buttons_reset(void);
void master_buttons_clear(void);
uint8_t master_buttons_get(uint8_t number);

void _master_buttons_update(void);
#endif //#ifndef __MASTER_H
