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
#define TEAMCOLOR clGreen
#define ONCOLOR clGreen
#define TEAMFLASHCOLOR clBGreen
#define NOCOLOR clBlack
#define ERRORCOLOR clPurple
#define POINTSFOROWNCOLOR 3

#define NUMBEROFBLINKINGS 5
#define BLINKCOUNTDOWNMAX 0

#define POINTS_PER_PRESS 3

//**************************<Types and Variables>******************************
enum eGamemodes {
	gm1P	 = 0,
	MaxGameModes,
};
struct sBlink{
	uint8_t countdown : 4;
	uint8_t numberOfBlinks : 4;
};
struct sTeam {
	uint8_t points;
};

static volatile struct sTeam team;

struct sGlobalLED{
	uint8_t on : 1;
	struct sBlink blinking;
};
static struct sGlobalLED LEDs[LEDBOX_COUNT_MAX];

static enum eGamemodes usedGamemode;
static enum eOperationModes operationMode;
static enum eBaseSystem baseSystem;

//**************************<Methods>******************************************
static void pushButton(uint8_t number);
static void setLEDs(void);

void gamemode_init_2022(void){
	maxGameModes=MaxGameModes;
	random_init();

	usedGamemode=gm1P;
	gamemode_start(usedGamemode, omTest, bsSpielfeld);

}

uint8_t gamemode_start_2022(uint8_t gameMode, enum eOperationModes oM, enum eBaseSystem system){
	if (ledbox_state == full_field && gameMode == gm1P){

		usedGamemode=gameMode;
		operationMode=oM;
		baseSystem=system;

		team.points = 0;

		//init Led to team with respect to field size
		uint8_t i;
		for(i=0;i< ledbox_count_current;i++){
			LEDs[i].on = 1;
			LEDs[i].blinking.numberOfBlinks = NUMBEROFBLINKINGS;
		}
		return 0;
	}else{
		return 1;
	}
}

void gamemode_update_2022(){
	uint8_t i;
	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		if (buttons_get(i)) {
			pushButton(i);
		}
	}
	//code to update RGB and IR LED
	setLEDs();
	showOnePoints(team.points);
}

void gamemode_finalize_2022(uint8_t count, uint8_t mode){
	//game end

	uint8_t points1MaxVal= ledbox_count_current * POINTS_PER_PRESS;

	
	uint8_t i;
	uint8_t numberMax = ledbox_count_current;
	
	uint8_t number = (count > numberMax)? numberMax: count;

	showOnePoints(team.points);

	for (i = 0; i < number; i++) {
		if(((i * points1MaxVal)/(numberMax - 1U)) < (team.points)){
			rgb_set(shift_num(i), TEAMFLASHCOLOR);
		} else {
			rgb_set(shift_num(i), NOCOLOR);
		}
	}
}

void gamemode_to_display_2022(uint8_t gameMode, uint8_t const** displayOut1, uint8_t const** displayOut2){
	switch (gameMode) {
	case gm1P:
		*displayOut1=numbers[1];
		*displayOut2=alpaP;
		break;
	default:
		*displayOut1=numbers[gameMode / 10];
		*displayOut2=numbers[gameMode % 10];
		break;
	}
}

//Game priate

void setLEDs(void){
	rgb_setAll(NOCOLOR);
	for(uint8_t i=0;i< ledbox_count_current;i++){
		if(LEDs[i].on){
			rgb_set(i, ONCOLOR);
			ir_set(i, 1);
		}else{
			ir_set(i, 1);
			if(LEDs[i].blinking.numberOfBlinks > 0){
				if (LEDs[i].blinking.numberOfBlinks & 0x01){
					rgb_set(i, TEAMFLASHCOLOR);
				}else{
					rgb_set(i, NOCOLOR);
				}
				if (LEDs[i].blinking.countdown) LEDs[i].blinking.countdown--;
				else if(LEDs[i].blinking.numberOfBlinks) {
					LEDs[i].blinking.numberOfBlinks--;
					LEDs[i].blinking.countdown = BLINKCOUNTDOWNMAX;
				}
			}else{
				rgb_set(i, NOCOLOR);
			}
		}
	}
}

void pushButton(uint8_t number){
	LEDs[number].on=0;
	team.points += POINTS_PER_PRESS;
}
