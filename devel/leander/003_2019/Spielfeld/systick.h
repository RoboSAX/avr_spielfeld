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
#include <inttypes.h>
#include <avr/interrupt.h>
#include "master.h"
//#include "robolib.h"

#define F_CPU 20000000

//*********************************<Types>**************************************



//*********************************<Constants>**********************************



//*********************************<Variables>**********************************



//*********************************<Macros>*************************************



//**********************************<Prototypes>********************************
void systick_init(void);
void systick_reset(void);

void systick_delay(uint16_t mseconds);

uint32_t systick_get(void);



#endif // #ifndef _systick_h_
