/*******************************************************************************
* systick.h                                                                    *
* =========                                                                    *
*                                                                              *
* Version: 1.0.0                                                               *
* Date   : 21.03.18                                                            *
* Author : Peter Weissig                                                       *
*                                                                              *
* See also:                                                                    *
*   https://github.com/RoboAG/projekt_gluecksrad                               *
*******************************************************************************/

#ifndef _systick_h_
#define _systick_h_

//*******************************<Included files>*******************************
#include <avr/io.h>
#include "robolib.h"
#include <inttypes.h>



//*********************************<Types>**************************************



//*********************************<Constants>**********************************



//*********************************<Variables>**********************************

enum eUpdate {
    update_Display	= 0b00000001,
    update_RGBLeds 	= 0b00000100,
    update_buttons 	= 0b00010000,
    update_all    	= 0b00010101,
    update_activ  	= 0b10000000,
};

//*********************************<Macros>*************************************



//**********************************<Prototypes>********************************
void systick_init(void);
void systick_reset(void);

uint8_t systick_freezUpdate(enum eUpdate update);//bool
void systick_unFreezUpdate(enum eUpdate update);

void systick_delay(uint16_t mseconds);

uint32_t systick_get(void);
uint8_t systick_toHour (uint32_t time);
uint8_t systick_toMin  (uint32_t time);
uint8_t systick_toSec  (uint32_t time);
uint16_t systick_toMsec(uint32_t time);

void update(void);


#endif // #ifndef _systick_h_
