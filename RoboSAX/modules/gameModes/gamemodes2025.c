/******************************************************************************
 * gamemodes.c                                                                 *
 * ========                                                                    *
 *                                                                             *
 * Version: 1.0.0                                                              *
 * Date   : 01.06.18                                                           *
 * Author : Peter Weissig, Leander Herr                                        *
 *                                                                             *
 ******************************************************************************/


//**************************<Included files>***********************************
#include "modulesInterface/gamemodes.h"
#include "modulesInterface/gamemodesGeneric.h"
#include "modulesInterface/global.h"
#include "modulesInterface/ledbox.h"
#include "modulesInterface/master.h"
#include "modulesInterface/menueHelper.h"
#include "modulesInterface/random.h"
// optics:
// blink if pressed correct
// blink red[/color] if error


//**************************<Macros>*******************************************
#define TEAM1COLOR        clGreen
#define TEAM1FLASHCOLOR   clBGreen
#define TEAM2COLOR        clRed
#define TEAM2FLASHCOLOR   clBRed
#define NOCOLOR           clBlack
#define ERRORCOLOR        clPurple
#define POINTSFOROWNCOLOR 1

#define NUMBEROFBLINKINGS 5
#define BLINKCOUNTDOWNMAX 3
#define BLINKCOUNTMULTI   6

#define MAX_TRYS         6
#define POINTS_PER_PRESS 1

#define SPECIAL_TIMER (LEDBOX_BUTTONS_DEBOUNCE_TIME * 4)


//**************************<Macros>*******************************************
//**************************<Types and Variables>******************************
uint8_t numberOfBlocks;
struct sGlobalLED {
    uint8_t GroupNr : 3;
    uint8_t LEDNr : 3;
};
struct sGlobalLED LEDToBlock[LEDBOX_COUNT_MAX];

struct sGlobalBlock {
    uint8_t      ledCount:4;
    uint8_t      LEDNr[8];
    enum eStatus status;
    enum eColor  color;
    uint8_t      special_timer;
    enum eColor  special_color;
};
struct sGlobalBlock BlockToLED[BLOCK_COUNT_MAX];

//**************************<Methods>******************************************

void setCrossPairs(void) {
    /*numberOfBlocks = BLOCK_COUNT_MAX;

    uint8_t blockNr;
    for (blockNr = 0; blockNr < numberOfBlocks; blockNr++) {
        uint8_t firstNumber  = shift_num(blockNr);
        uint8_t secondNumber = shift_num(blockNr + 12);

        //BlockToLED[blockNr].ledCount     = 2;
        //BlockToLED[blockNr].LEDNr[0]     = firstNumber;
        //BlockToLED[blockNr].LEDNr[1]     = secondNumber;
        LEDToBlock[firstNumber].LEDNr    = 0;
        LEDToBlock[secondNumber].LEDNr   = 1;
        LEDToBlock[firstNumber].GroupNr  = blockNr;
        LEDToBlock[secondNumber].GroupNr = blockNr;
    }*/
}
void setPairsToSection(void) {
  /*  numberOfBlocks = BLOCK_COUNT_MAX / 2;

    uint8_t blockNr;
    for (blockNr = 0; blockNr < numberOfBlocks; blockNr++) {
        BlockToLED[blockNr].ledCount = 4;
        BlockToLED[blockNr].status   = groupOff;
        BlockToLED[blockNr].color    = TEAM1COLOR;
        uint8_t ledNr;
        for (ledNr = 0; ledNr < BlockToLED[blockNr].ledCount; ledNr++) {
            uint8_t number = shift_num(blockNr * 4 + ledNr);

            //BlockToLED[blockNr].LEDNr[ledNr] = number;
            LEDToBlock[number].LEDNr         = ledNr;
            LEDToBlock[number].GroupNr       = blockNr;
        }
    }
*/}
void setPairsToWall(void) {
    numberOfBlocks = 4;

    uint8_t blockNr;
        uint8_t ledNr;
        uint8_t number;
    for (blockNr = 0; blockNr < numberOfBlocks; blockNr += 1) {
        BlockToLED[blockNr].ledCount = 8;
        BlockToLED[blockNr].status   = groupOff;
        BlockToLED[blockNr].color    = TEAM1COLOR;
            BlockToLED[blockNr].LEDNr[0] = 25;
            BlockToLED[blockNr].LEDNr[1] = 25;
            BlockToLED[blockNr].LEDNr[6] = 25;
            BlockToLED[blockNr].LEDNr[7] = 25;
	}
        for (ledNr = 0; ledNr < 24; ledNr++) {
				if(ledNr<8){
            BlockToLED[0].LEDNr[ledNr] = ledNr;
            LEDToBlock[ledNr].LEDNr         = ledNr;
            LEDToBlock[ledNr].GroupNr       = 0;
				}else if(ledNr<12){
						number=ledNr-8+2;
            BlockToLED[1].LEDNr[number] = ledNr;
            LEDToBlock[ledNr].LEDNr         = number;
            LEDToBlock[ledNr].GroupNr       = 1;
				}else if(ledNr<20){
						number=ledNr-12;
            BlockToLED[2].LEDNr[number] = ledNr;
            LEDToBlock[ledNr].LEDNr         = number;
            LEDToBlock[ledNr].GroupNr       = 2;
				}else {
						number=ledNr-20+2;
            BlockToLED[3].LEDNr[number] = ledNr;
            LEDToBlock[ledNr].LEDNr         = number;
            LEDToBlock[ledNr].GroupNr       = 3;
				}

        }
    /*for (blockNr = 0; blockNr < numberOfBlocks; blockNr += 2) {
        BlockToLED[blockNr].ledCount = 8;
        BlockToLED[blockNr].status   = groupOff;
        BlockToLED[blockNr].color    = TEAM1COLOR;
        for (ledNr = 0; ledNr < BlockToLED[blockNr].ledCount; ledNr++) {
            number = (blockNr * 6 + 4);

            BlockToLED[blockNr].LEDNr[ledNr] = number;
            LEDToBlock[number].LEDNr         = ledNr;
            LEDToBlock[number].GroupNr       = blockNr;
        }
        for (ledNr = 0; ledNr < 4; ledNr++) {
            number = (blockNr * 6 + 4 + ledNr);

            BlockToLED[blockNr].LEDNr[ledNr + 2] = number;
            LEDToBlock[number].LEDNr             = ledNr + 2;
            LEDToBlock[number].GroupNr           = blockNr;
        }
    }
    for (blockNr = 1; blockNr < numberOfBlocks; blockNr += 2) {
        BlockToLED[blockNr].ledCount = 8;
        BlockToLED[blockNr].status   = groupOff;
        BlockToLED[blockNr].color    = TEAM1COLOR;
        for (ledNr = 0; ledNr < BlockToLED[blockNr].ledCount; ledNr++) {
            number = (blockNr * 6 - 6 + 8 + ledNr);

            BlockToLED[blockNr].LEDNr[ledNr] = number;
            LEDToBlock[number].LEDNr         = ledNr;
            LEDToBlock[number].GroupNr       = blockNr;
        }
    }*/
}
void setLEDsGameGeneric(void) {
    rgb_setAll(NOCOLOR);

    uint8_t blockNr;
    for (blockNr = 0; blockNr < numberOfBlocks; blockNr++) {
        const enum eColor color         = BlockToLED[blockNr].color;
        const uint8_t     status        = BlockToLED[blockNr].status;
        const uint8_t     special       = BlockToLED[blockNr].special_timer;
        const uint8_t     special_color = BlockToLED[blockNr].special_color;
        uint8_t           num;
        for (num = 0; num < BlockToLED[blockNr].ledCount; num++) {
            uint8_t statusFlag = status & (1 << num);
            ir_set(BlockToLED[blockNr].LEDNr[num], statusFlag);
            if (special) {
                rgb_set(BlockToLED[blockNr].LEDNr[num], special_color);
            } else if (statusFlag)
                rgb_set(BlockToLED[blockNr].LEDNr[num], color);
        }
        if (special) {
            BlockToLED[blockNr].special_timer--;
        }
    }
}

void reandomSetLEDActiveForBlock(uint8_t blockNr, uint8_t numberLedsPerBlock,
                                 enum eStatus validLedsPerBlock) {
    BlockToLED[blockNr].status = groupOff;
    uint8_t ledNr;
    for (ledNr = 0; ledNr < numberLedsPerBlock; ledNr++) {
        uint8_t randomFlag = 0;
		// todo make it generic again + no repeat(destroys randomness!!)
		// => random & validLedsPerBlock
		// count leds on
		// shift rand & use to reduce/expand value
		// ...?
        randomFlag = ((1 << ((random() % 2)+3)) & validLedsPerBlock) &
                         ~BlockToLED[blockNr].status;
        BlockToLED[blockNr].status |= randomFlag;
    }
}
void reandomSetLEDActiveForBlocks(uint8_t      numberLedsPerBlock,
                                  enum eStatus validLedsPerBlock) {
    uint8_t blockNr;
    for (blockNr = 0; blockNr < numberOfBlocks; blockNr++) {
        reandomSetLEDActiveForBlock(blockNr, numberLedsPerBlock,
                                    validLedsPerBlock);
    }
}
void reandomSetLEDActiveForBlocksStable(uint8_t      numberLedsPerBlock,
                                        enum eStatus validLedsPerBlock) {
    uint8_t blockNr;
    for (blockNr = 0; blockNr < numberOfBlocks; blockNr++) {
        if (BlockToLED[blockNr].status == groupOff)
            reandomSetLEDActiveForBlock(blockNr, numberLedsPerBlock,
                                        validLedsPerBlock);
    }
}








//**************************<Types and Variables>******************************
enum eGamemodes {
    gm1P = 0,
    //	gm2P		,
    MaxGameModes,
};

uint8_t              currentPoints;
uint8_t              trys;
enum eOperationModes operationMode;


//**************************<Methods>******************************************
static void pushButton(uint8_t number);

void gamemode_init_2025(void) {
    maxGameModes = MaxGameModes;
    random_init();

    gamemode_start(gm1P, omTest, bsSpielfeld);
}

uint8_t gamemode_start_2025(uint8_t gameMode, enum eOperationModes oM,
                            enum eBaseSystem system) {
    if ((ledbox_state == full_field && gameMode == gm1P)) {
        currentPoints = 0;
        trys=0;
        setPairsToWall();
        reandomSetLEDActiveForBlocks(1, groupLed4 | groupLed5);
        operationMode = oM;
        return 0;
    } else {
        return 1;
    }
}

void gamemode_update_2025() {
    uint8_t i;
    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        if (buttons_get(i)) {
            pushButton(i);
        }
    }
    // code to update RGB and IR LED
    setLEDsGameGeneric();
    showOnePoints(currentPoints);
}

struct Points gamemode_points_2025(uint8_t mode) {

    struct Points points;
    points.team1     = currentPoints;
    points.team2     = trys;
    points.color1    = TEAM1COLOR;
    points.color2    = clLBlue;
    points.maxPoints = MAX_TRYS * POINTS_PER_PRESS;
    points.type      = ptBeide;
    return points;
}

void gamemode_to_display_2025(uint8_t gameMode, uint8_t const** displayOut1,
                              uint8_t const** displayOut2) {
    switch (gameMode) {
        case gm1P:
            *displayOut1 = numbers[1];
            *displayOut2 = alpaP;
            break;
        // case gm2P:
        //	*displayOut1=numbers[2];
        //	*displayOut2=alpaP;
        //	break;
        default:
            *displayOut1 = numbers[gameMode / 10];
            *displayOut2 = numbers[gameMode % 10];
            break;
    }
}

// Game priate

void pushButton(uint8_t number) {
    const uint8_t GroupNr = LEDToBlock[number].GroupNr;
    const uint8_t status  = BlockToLED[GroupNr].status;
    const uint8_t correct = (1<<LEDToBlock[number].LEDNr) & status;
    const uint8_t error =
      status && ((1<<LEDToBlock[number].LEDNr) & (groupLed4 | groupLed5));

    if (correct) {
        currentPoints += POINTS_PER_PRESS;
        BlockToLED[GroupNr].special_color = TEAM1FLASHCOLOR;
    } else if (error) {
        BlockToLED[GroupNr].special_color = ERRORCOLOR;
    }

    if (correct || error) {
        trys++;
        BlockToLED[GroupNr].special_timer = SPECIAL_TIMER;

        if ((operationMode == omGame) && (trys >= MAX_TRYS)) {
            uint8_t i;
            for (i = 0; i < 4; i++) {
                BlockToLED[i].status = groupOff;
            }
        } else {
            reandomSetLEDActiveForBlocksStable(1, groupLed4 | groupLed5);
            BlockToLED[GroupNr].status = groupOff;
        }
    }
}
