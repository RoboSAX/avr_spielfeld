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
#include "modulesInterface/random.h"
#include "modulesInterface/master.h"

#include "modulesInterface/global.h"
#include "modulesInterface/ledbox.h"
#include "modulesInterface/menueHelper.h"
//optics:
//blink if pressed correct
//blink red[/color] if error


//**************************<Macros>*******************************************
//**************************<Types and Variables>******************************
//**************************<Methods>******************************************

void setCrossPairs(void){
    numberOfBlocks=BLOCK_COUNT_MAX;

    uint8_t  blockNr;
	for (blockNr=0;blockNr<numberOfBlocks;blockNr++){
        uint8_t firstNumber=shift_num(blockNr);
        uint8_t secondNumber=shift_num(blockNr+12);

        BlockToLED[blockNr].ledCount=2;
        BlockToLED[blockNr].LEDNr[0]=firstNumber;
        BlockToLED[blockNr].LEDNr[1]=secondNumber;
        LEDToBlock[firstNumber].LEDNr=0;
        LEDToBlock[secondNumber].LEDNr=1;
        LEDToBlock[firstNumber].GroupNr=blockNr;
        LEDToBlock[secondNumber].GroupNr=blockNr;
    }
}
void setPairsToSection(void){
    numberOfBlocks=BLOCK_COUNT_MAX/2;

	uint8_t  blockNr;
	for (blockNr=0;blockNr<numberOfBlocks;blockNr++){
        BlockToLED[blockNr].ledCount=4;
        BlockToLED[blockNr].status=groupOff;
        BlockToLED[blockNr].color=TEAM1COLOR;        
        uint8_t  ledNr;
        for (ledNr=0;ledNr<BlockToLED[blockNr].ledCount;ledNr++){
            uint8_t number=shift_num(blockNr*4+ledNr);

            BlockToLED[blockNr].LEDNr[ledNr]=number;
            LEDToBlock[number].LEDNr=ledNr;
            LEDToBlock[number].GroupNr=blockNr;
        }
    }
}
void setPairsToWall(void){
    numberOfBlocks=4;

	uint8_t  blockNr;
	for (blockNr=0;blockNr<numberOfBlocks;blockNr+=2){
        BlockToLED[blockNr].ledCount=8;
        BlockToLED[blockNr].status=groupOff;
        BlockToLED[blockNr].color=TEAM1COLOR;        
        uint8_t  ledNr;
        for (ledNr=0;ledNr<BlockToLED[blockNr].ledCount;ledNr++){
            uint8_t number=shift_num(blockNr*6+4);

            BlockToLED[blockNr].LEDNr[ledNr]=number;
            LEDToBlock[number].LEDNr=ledNr;
            LEDToBlock[number].GroupNr=blockNr;
        }
        for (ledNr=0;ledNr<4;ledNr++){
            uint8_t number=shift_num(blockNr*6+4+ledNr);

            BlockToLED[blockNr].LEDNr[ledNr+2]=number;
            LEDToBlock[number].LEDNr=ledNr+2;
            LEDToBlock[number].GroupNr=blockNr;
        }
    }
	for (blockNr=1;blockNr<numberOfBlocks;blockNr+=2){
        BlockToLED[blockNr].ledCount=8;
        BlockToLED[blockNr].status=groupOff;
        BlockToLED[blockNr].color=TEAM1COLOR;        
        uint8_t  ledNr;
        for (ledNr=0;ledNr<BlockToLED[blockNr].ledCount;ledNr++){
            uint8_t number=shift_num(blockNr*6-6+8+ledNr);

            BlockToLED[blockNr].LEDNr[ledNr]=number;
            LEDToBlock[number].LEDNr=ledNr;
            LEDToBlock[number].GroupNr=blockNr;
        }
    }
}
void setLEDsGameGeneric(void){
	rgb_setAll(NOCOLOR);

	uint8_t  blockNr;
	for (blockNr=0;blockNr<numberOfBlocks;blockNr++){
		uint8_t i;
		const enum eColor color = BlockToLED[blockNr].color;
        const uint8_t status = BlockToLED[blockNr].status;
        const uint8_t special = BlockToLED[blockNr].special_timer;
        const uint8_t special_color = BlockToLED[blockNr].special_color;
        uint8_t num;
        for(num=0;num<BlockToLED[blockNr].ledCount;num++){
            uint8_t statusFlag=status & (1 << num);
            ir_set(BlockToLED[blockNr].LEDNr[num], statusFlag);
            if (special){
                rgb_set(lockToLED[blockNr].LEDNr[num],special_color);
            }else if(statusFlag) rgb_set(lockToLED[blockNr].LEDNr[num], color);
        }
        if (special){
            BlockToLED[blockNr].special_timer.special_timer--;
        }
    }
}

void reandomSetLEDActiveForBlock(uint8_t blockNr, uint8_t numberLedsPerBlock,enum eStatus validLedsPerBlock){
    BlockToLED[blockNr].status=groupOff;
	uint8_t  ledNr;
	for (ledNr=0;ledNr<numberLedsPerBlock;ledNr++){
        uint8_t randomFlag=0;
        while(!randomFlag) randomFlag=((1<<(random()%8))&validLedsPerBlock)&~BlockToLED[blockNr].status;
        BlockToLED[blockNr].status|=randomFlag;
    }
}
void reandomSetLEDActiveForBlocks(uint8_t numberLedsPerBlock,enum eStatus validLedsPerBlock){
	uint8_t  blockNr;
	for (blockNr=0;blockNr<numberOfBlocks;blockNr++){
        reandomSetLEDActiveForBlock(blockNr,numberLedsPerBlock,validLedsPerBlock);
    }
}
void reandomSetLEDActiveForBlocksStable(uint8_t numberLedsPerBlock,enum eStatus validLedsPerBlock){
	uint8_t  blockNr;
	for (blockNr=0;blockNr<numberOfBlocks;blockNr++){
        if(BlockToLED[blockNr].status==groupOff) reandomSetLEDActiveForBlock(blockNr,numberLedsPerBlock,validLedsPerBlock);
    }
}

//Game priate


void pushButton(uint8_t number){
	const uint8_t teamNr = LEDToTeam[number].teamNr;
	const uint8_t GroupNr = LEDToTeam[number].GroupNr;
	const uint8_t LEDNr = LEDToTeam[number].LEDNr;
	if (teamNr>1) return;

	if (team[teamNr].groups[GroupNr].status){
		team[teamNr].trys++;
		if (team[teamNr].groups[GroupNr].status == LEDNr + 1){
			team[teamNr].points += POINTS_PER_PRESS;
			team[teamNr].groups[GroupNr].special_color=team[teamNr].good_color;
		}else{
			team[teamNr].groups[GroupNr].special_color=team[teamNr].error_color;
		}
		team[teamNr].groups[GroupNr].special_timer = SPECIAL_TIMER;

		if ((operationMode == omGame) && (team[teamNr].trys>=MAX_TRYS)){
			uint8_t i,j,k;
			if (baseSystem == bsSpielfeld){
				for(i=0;i<3;i++){
					team[teamNr].groups[i].status = groupOff;
				}
				team[teamNr].offGroup = 4;
			}else{
				enum eColor colorFinalize;
				uint8_t numberFinalize;
				numberFinalize=team[teamNr].points/POINTS_PER_PRESS;
				colorFinalize=team[teamNr].teamColor;
				for (i=0; i<numberFinalize;i++){
					for (j=1;j<ledbox_count_current;j+=4){
						for (k=0;k<2;k++){
							rgb_clearAll();
							rgb_set(j+k,colorFinalize);
							waitMsAndUpdate(50);
						}
					}
				}
				team[teamNr].points = 0;
				team[teamNr].trys = 0;
			}
		}

		team[teamNr].groups[GroupNr].status = groupOff;
  
		if (team[teamNr].offGroup < 3){
			team[teamNr].groups[team[teamNr].offGroup].status = random() % 2 + 1;
		}

		team[teamNr].offGroup = GroupNr;
	}
}
