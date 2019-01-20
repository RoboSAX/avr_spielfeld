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
#include "global.h"
#include "ledbox.h"
#include "mainMenue.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<PORTDEF>*******************************************
//**************************<Macros>*******************************************
//**************************<Types and Variables>******************************
// leds
enum eGamemodes {
    gmNothing        = 0,
    gmEnemy          ,
    gmPunishAndEnemy ,
    MaxGameModes
};

//**************************<Prototypes>***************************************
void gamemode_init(void);
void gamemode_start(enum eGamemodes gameMode);
void gamemode_update();
void gamemode_finalize();


#endif //#ifndef __GAMEMODES_H
