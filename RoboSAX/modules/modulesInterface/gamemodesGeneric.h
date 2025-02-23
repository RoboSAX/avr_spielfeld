/******************************************************************************
 * gamemodes.h *
 * ========                                                                    *
 *                                                                             *
 * Version: 1.0.0                                                              *
 * Date   : 01.06.18                                                           *
 * Author : Peter Weissig, Leander Herr                                        *
 *                                                                             *
 ******************************************************************************/

#ifndef __GAMEMODES_GENERIC_H
#define __GAMEMODES_GENERIC_H


//**************************<Included files>***********************************

#include <avr/io.h>
#include <inttypes.h>

#include "modulesInterface/ledbox.h"


//**************************<PORTDEF>*******************************************
//**************************<Macros>*******************************************
#define TEAM1COLOR      clGreen
#define TEAM1FLASHCOLOR clBGreen
#define TEAM2COLOR      clRed
#define TEAM2FLASHCOLOR clBRed
#define NOCOLOR         clBlack
#define ERRORCOLOR      clPurple

#define NUMBEROFBLINKINGS 5
#define BLINKCOUNTDOWNMAX 3
#define BLINKCOUNTMULTI   6

#define BLOCK_COUNT_MAX LEDBOX_COUNT_MAX / 2

#define SPECIAL_TIMER (LEDBOX_BUTTONS_DEBOUNCE_TIME * 4)

//**************************<Types and Variables>******************************
enum eStatus {
    groupOff  = 0,
    groupLed1 = 1,
    groupLed2 = 2,
    groupLed3 = 4,
    groupLed4 = 8,
    groupLed5 = 0x10,
    groupLed6 = 0x20,
    groupLed7 = 0x40,
    groupLed8 = 0x80,
};

struct sGlobalLED {
    uint8_t GroupNr : 3;
    uint8_t LEDNr : 3;
};
static struct sGlobalLED LEDToBlock[LEDBOX_COUNT_MAX];

struct sGlobalBlock {
    uint8_t      ledCount;
    uint8_t      LEDNr[8];
    enum eStatus status;
    enum eColor  color;
    uint8_t      special_timer;
    enum eColor  special_color;
};
static struct sGlobalBlock BlockToLED[BLOCK_COUNT_MAX];

uint8_t numberOfBlocks;
//**************************<Prototypes>***************************************
void setCrossPairs(void);
void setPairsToSection(void);
void setPairsToWall(void);
void setLEDsGameGeneric(void);
void reandomSetLEDActiveForBlock(uint8_t blockNr, uint8_t numberLedsPerBlock,
                                 enum eStatus validLedsPerBlock);
void reandomSetLEDActiveForBlocks(uint8_t      numberLedsPerBlock,
                                  enum eStatus validLedsPerBlock);
void reandomSetLEDActiveForBlocksStable(uint8_t      numberLedsPerBlock,
                                        enum eStatus validLedsPerBlock);

#endif  // #ifndef __GAMEMODES_H
