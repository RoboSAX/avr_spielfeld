/******************************************************************************
* moduls/main.h                                                               *
* ======================                                                      *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.01.19                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/
#ifndef __MENUE_HELPER_H
#define __MENUE_HELPER_H

//**************************<Included files>***********************************
#include "display.h"
#include "display_chars.h"
#include "systick.h"
#include "ledbox.h"

//**************************<Prototypes>***************************************
enum eMasterModes {
    mmGameMode   = 0,
    mmTestMode      ,
    MaxMasterModes
};
//**************************<Methods>******************************************
void waitAndUpdate(void);

void showtime (uint16_t time,uint8_t dot);
void showPoints (uint8_t team1Points,uint8_t team2Points);
void writeModesToDisplay (enum eMasterModes masterMode, uint8_t subMode);
#endif
