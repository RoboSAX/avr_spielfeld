/******************************************************************************
* moduls/main.h                                                               *
* ======================                                                      *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.01.19                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/
#ifndef __MAIN_MENUE_H
#define __MAIN_MENUE_H

//ms per round
#define ROUNDTIME ((uint32_t)(3UL*60UL*1000UL))

//**************************<Included files>***********************************
#include "display.h"
#include "gamemodes.h"
#include "display_chars.h"

//**************************<Prototypes>***************************************
enum eMasterModes {
    mmGameMode   = 0,
    mmTestMode      ,
    MaxMasterModes
};
//**************************<Methods>******************************************
void showtime (uint16_t time,uint8_t dot);
void showPoints (uint8_t team1Points,uint8_t team2Points);
void writeModesToDisplay (enum eMasterModes masterMode, uint8_t subMode);
#endif
