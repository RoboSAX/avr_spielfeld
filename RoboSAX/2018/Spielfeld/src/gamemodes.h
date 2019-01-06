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
#include "robolib.h"
#include "global.h"
#include "ledbox.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<PORTDEF>*******************************************
//**************************<Macros>*******************************************
//**************************<Types and Variables>******************************
// leds
enum eGamemodes {
    gmNothing        = 0,
    gmEnemy          = 1,
    gmPunishAndEnemy = 2
};

enum eGamemodes gamemode;

//**************************<Prototypes>***************************************
void gamemode_init(void);
void gamemode_reset(void);
void pushButton(uint8_t number);
void setLEDs(void);


#endif //#ifndef __GAMEMODES_H
