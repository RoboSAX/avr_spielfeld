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
// buttons

#define master_button1() (master_buttons_get_pushed(1))
#define master_button2() (master_buttons_get_pushed(2))
#define master_button_down() (master_buttons_get_pushed(2))
#define master_button_up() (master_buttons_get_pushed(1))
#define master_button3() (master_buttons_get_pushed(3))
#define master_button_ok() (master_buttons_get_pushed(3))

#define master_button_full1() (master_buttons_get_push_and_released(1))
#define master_button_full2() (master_buttons_get_push_and_released(2))
#define master_button_down_full() (master_buttons_get_push_and_released(2))
#define master_button_up_full() (master_buttons_get_push_and_released(1))
#define master_button_full3() (master_buttons_get_push_and_released(3))
#define master_button_ok_full() (master_buttons_get_push_and_released(3))

#define master_button_state1() (master_buttons_get_state(1))
#define master_button_state2() (master_buttons_get_state(2))
#define master_button_down_state() (master_buttons_get_state(2))
#define master_button_up_state() (master_buttons_get_state(1))
#define master_button_state3() (master_buttons_get_state(3))
#define master_button_ok_state() (master_buttons_get_state(3))


//**************************<Types and Variables>******************************


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
uint8_t _master_button(uint8_t number);
void _master_buttons_update(void);

#endif //#ifndef __MASTER_H
