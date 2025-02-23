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
#include "modulesInterface/random.h"
#include "modulesInterface/master.h"

#include "modulesInterface/global.h"
#include "modulesInterface/ledbox.h"
#include "modulesInterface/menueHelper.h"
//optics:
//blink if pressed correct
//blink red[/color] if error


//**************************<Macros>*******************************************
#define TEAM1COLOR clGreen
#define TEAM1FLASHCOLOR clBGreen
#define TEAM2COLOR clRed
#define TEAM2FLASHCOLOR clBRed
#define NOCOLOR clBlack
#define ERRORCOLOR clPurple
#define POINTSFOROWNCOLOR 3

#define NUMBEROFBLINKINGS 5
#define BLINKCOUNTDOWNMAX 3
#define BLINKCOUNTMULTI 6

#define MAX_TRYS 5
#define POINTS_PER_PRESS 3

#define SPECIAL_TIMER (LEDBOX_BUTTONS_DEBOUNCE_TIME*4)

//**************************<Types and Variables>******************************
enum eGamemodes {
	gm1P	 = 0,
//	gm2P		,
	MaxGameModes,
};

uint8_t currentPoints;
uint8_t trys;
enum eOperationModes operationMode;


//**************************<Methods>******************************************
static void pushButton(uint8_t number);

void gamemode_init_2025(void){
	maxGameModes=MaxGameModes;
	random_init();

	gamemode_start(gm1P, omTest, bsSpielfeld);

}

uint8_t gamemode_start_2025(uint8_t gameMode, enum eOperationModes oM, enum eBaseSystem system){
	if ((ledbox_state == full_field && gameMode == gm1P)){
		setPairsToWall();
		reandomSetLEDActiveForBlocksStable(1,groupLed4|groupLed5);
		operationMode=oM;
		return 0;
	}else{
		return 1;
	}
}

void gamemode_update_2025(){
	uint8_t i;
	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		if (buttons_get(shift_num(i))) {
			pushButton(i);
		}
	}
	//code to update RGB and IR LED
	setLEDsGameGeneric();
	showOnePoints(currentPoints);
}

struct Points gamemode_points_2025(uint8_t mode){

	struct Points points;
	points.team1=currentPoints;
	points.team2=currentPoints;
	points.color1=TEAM1COLOR;
	points.color2=TEAM1COLOR;
	points.maxPoints=MAX_TRYS * POINTS_PER_PRESS;
	points.type=ptTeam1;
	return points;
}

void gamemode_to_display_2025(uint8_t gameMode, uint8_t const** displayOut1, uint8_t const** displayOut2){
	switch (gameMode) {
	case gm1P:
		*displayOut1=numbers[1];
		*displayOut2=alpaP;
		break;
	//case gm2P:
	//	*displayOut1=numbers[2];
	//	*displayOut2=alpaP;
	//	break;
	default:
		*displayOut1=numbers[gameMode / 10];
		*displayOut2=numbers[gameMode % 10];
		break;
	}
}

//Game priate

void pushButton(uint8_t number){
	const uint8_t GroupNr = LEDToBlock[number].GroupNr;
	const uint8_t status = BlockToLED[GroupNr].status;
	const uint8_t correct = LEDToBlock[number].LEDNr & status;
	const uint8_t error = status && (LEDToBlock[number].LEDNr & (groupLed4|groupLed5));

	if(correct){
		currentPoints += POINTS_PER_PRESS;
		BlockToLED[GroupNr].special_color=TEAM1FLASHCOLOR;
	}else if(error){
		BlockToLED[GroupNr].special_color=ERRORCOLOR;
	}

	if (correct || error){
		trys++;
		BlockToLED[GroupNr].special_timer = SPECIAL_TIMER;

		if ((operationMode == omGame) && (trys>=MAX_TRYS)){
			uint8_t i;
			for(i=0;i<3;i++){
				BlockToLED[GroupNr].status = groupOff;
			}
		}else{
			reandomSetLEDActiveForBlocksStable(1,groupLed4|groupLed5);
			BlockToLED[GroupNr].status=groupOff;
		}
	}
}
