/******************************************************************************
* master.h                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.1                                                              *
* Date   : 31.05.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __MASTER_H
#define __MASTER_H


//**************************<Included files>***********************************
#include "global.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<Macros>*******************************************
// leds (only for testing - will be removed)
#define MASTER_LED1_PORT      PORTD
#define MASTER_LED1_DDR       DDRD
#define MASTER_LED1_PIN       _BV(1)
#define debug_led1(x)        (x ? (MASTER_LED1_PORT&= ~MASTER_LED1_PIN) : \
                             (MASTER_LED1_PORT|= MASTER_LED1_PIN))
#define _debug_led1_enable() (MASTER_LED1_DDR|= MASTER_LED1_PIN)

#define MASTER_LED2_PORT      PORTD
#define MASTER_LED2_DDR       DDRD
#define MASTER_LED2_PIN       _BV(4)
#define debug_led2(x)        (x ? (MASTER_LED2_PORT|= MASTER_LED2_PIN) : \
                             (MASTER_LED2_PORT&= ~MASTER_LED2_PIN))
#define _debug_led2_enable() (MASTER_LED2_DDR|= MASTER_LED2_PIN)

// buttons
#define MASTER_BUTTONS_DEBOUNCE_TIME 25

#define master_button1() (master_buttons_get_pushed(1))
#define master_button2() (master_buttons_get_pushed(2))
#define master_button3() (master_buttons_get_pushed(3))

#define master_button_full1() (master_buttons_get_pushed(1))
#define master_button_full2() (master_buttons_get_pushed(2))
#define master_button_full3() (master_buttons_get_pushed(3))

#define master_button_state1() (master_buttons_get_state(1))
#define master_button_state2() (master_buttons_get_state(2))
#define master_button_state3() (master_buttons_get_state(3))

//**************************<Prototypes>***************************************

// init
void master_init(void);

//buttons
void master_buttons_reset(void);
void master_buttons_clear(void);
uint8_t master_buttons_get_pushed(uint8_t number);
uint8_t master_buttons_get_push_and_released(uint8_t number);
uint8_t master_buttons_get_state(uint8_t number);

// update
void _master_buttons_update(void);

#endif //#ifndef __MASTER_H
