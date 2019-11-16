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
#define LED_PER_TEAM LEDBOX_COUNT_MAX/3
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

//**************************<Types and Variables>******************************
enum eGamemodes {
	gm1P	 = 0,
	gm2P		,
	MaxGameModes,
};

enum eGamemodes usedGamemode;
//**************************<Methods>******************************************
void gamemode_init(void){
	maxGameModes=MaxGameModes;
	random_init();

	usedGamemode=gm1P;
	gamemode_start(usedGamemode);

}

uint8_t gamemode_start(uint8_t gameMode){
	if ((ledbox_state == full_field && gameMode == gm2P)||
		((ledbox_state == full_field || ledbox_state == half_field ) && gameMode == gm1P)){

		usedGamemode=gameMode;

		//init Led to team with respect to field size

		return 0;
	}else{
		return 1;
	}
}

void gamemode_update(){
}

void gamemode_finalize(uint8_t count, uint8_t mode){
}

void gamemode_to_display(uint8_t gameMode, uint8_t const** displayOut1, uint8_t const** displayOut2){
	switch (gameMode) {
	case gm1P:
		*displayOut1=numbers[1];
		*displayOut2=alpaP;
		break;
	case gm2P:
		*displayOut1=numbers[2];
		*displayOut2=alpaP;
		break;
	default:
		*displayOut1=numbers[gameMode / 10];
		*displayOut2=numbers[gameMode % 10];
		break;
	}
}
