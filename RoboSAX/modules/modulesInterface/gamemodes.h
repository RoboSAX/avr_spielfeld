/******************************************************************************
* gamemodes.h                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __GAMEMODES_H
#define __GAMEMODES_H


//**************************<Included files>***********************************
//#include "robolib.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<PORTDEF>*******************************************
//**************************<Macros>*******************************************
//**************************<Types and Variables>******************************
// leds

uint8_t maxGameModes;

//**************************<Prototypes>***************************************
void gamemode_init(void);
uint8_t gamemode_start(uint8_t gameMode);
void gamemode_update(void);
void gamemode_finalize(uint8_t count, uint8_t mode);


void gamemode_to_display(uint8_t gameMode, uint8_t const** displayOut1, uint8_t const** displayOut2);

#endif //#ifndef __GAMEMODES_H