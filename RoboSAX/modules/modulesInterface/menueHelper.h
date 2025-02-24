/******************************************************************************
 * menueHelper.h                                                              *
 * =============                                                              *
 *                                                                            *
 * Version: 1.0.1                                                             *
 * Date   : 22.02.25                                                          *
 * Author : Peter Weissig, Leander Herr                                       *
 *                                                                            *
 *****************************************************************************/
#ifndef __MENUE_HELPER_H
#define __MENUE_HELPER_H

//**************************<Included files>***********************************
#include "display.h"
#include "display_chars.h"
#include "gamemodes.h"
#include "ledbox.h"
#include "systick.h"

//**************************<Prototypes>***************************************
enum eMasterModes {
    mmBlokMode = 0,
    mmGameMode,
    mmTestMode,
    mmOldGameMode,
    mmScanMode,
    DefaultMasterModes
};
enum eScanModes { scLedboxCount = 0, scTesting, MaxScanModes };

//**************************<Methods>******************************************
void waitAndUpdate(void);
void waitMsAndUpdate(uint16_t msecs);

void pointMagic(struct Points points);
void pointLedMagic(struct Points points, uint8_t count);
void showtime(uint16_t time, uint8_t dot);
void showPoints(uint8_t team1Points, uint8_t team2Points);
void showOnePoints(uint8_t teamPoints);
void showPointsLed(uint8_t team1Points, enum eColor color1, uint8_t team2Points,
                   enum eColor color2, uint8_t maxPoints, uint8_t numberLeds);
void showPointsLedVs(uint8_t team1Points, enum eColor color1,
                     uint8_t team2Points, enum eColor color2, uint8_t maxPoints,
                     uint8_t numberLeds);
void showOnePointsLed(uint8_t teamPoints, uint8_t maxPoints, enum eColor color,
                      uint8_t numberLeds);
void writeModesToDisplay(enum eMasterModes masterMode, int8_t subMode);
#endif
