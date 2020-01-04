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
#define POINTS_PER_PRESS 5

#define SPECIAL_TIMER (LEDBOX_BUTTONS_DEBOUNCE_TIME*4)

//**************************<Types and Variables>******************************
enum eGamemodes {
	gm1P	 = 0,
	gm2P		,
	MaxGameModes,
};
struct sBlink{
	uint8_t LEDNr : 3;
	uint8_t countdown : 2;
	uint8_t numberOfBlinks : 3;
};
enum eGroup {
	groupOff = 0,
	group1 = 1,
	group2 = 2,
};
struct sTeamGroup{
	enum eGroup status;
	uint8_t firstLED;
	uint8_t special_timer;
    enum eColor special_color;
};
struct sTeam {
	uint8_t points;
	uint8_t trys;
	struct sTeamGroup groups[3];
	uint8_t offGroup;

	enum eColor teamColor;

    enum eColor error_color;
    enum eColor good_color;
};

volatile struct sTeam team[2];

enum eTeamNumber {
	team1,
	team2
};

struct sGlobalLED{
	uint8_t teamNr : 2;
	uint8_t GroupNr : 3;
	uint8_t LEDNr : 3;
};
struct sGlobalLED LEDToTeam[LEDBOX_COUNT_MAX];

enum eGamemodes usedGamemode;
enum eOperationModes operationMode;
//**************************<Methods>******************************************
void pushButton(uint8_t number);
void setLEDs(void);

void gamemode_init(void){
	maxGameModes=MaxGameModes;
	random_init();

	usedGamemode=gm1P;
	gamemode_start(usedGamemode, omTest);

}

uint8_t gamemode_start(uint8_t gameMode, enum eOperationModes oM){
	if ((ledbox_state == full_field && gameMode == gm2P)||
		((ledbox_state == full_field || ledbox_state == half_field ) && gameMode == gm1P)){

		usedGamemode=gameMode;
		operationMode=oM;

		if (operationMode == omTeamprobe){
			   firstNumber = 1;
			   secondNumber = 2;
		}

		team[team1].teamColor = TEAM1COLOR;
		team[team1].offGroup = 3;
		team[team1].trys = 0;
		team[team1].points = 0;
		team[team1].error_color = clPurple;
		team[team1].good_color = TEAM1COLOR;

		team[team2].teamColor = TEAM2COLOR;
		team[team2].offGroup = 3;
		team[team2].trys = 0;
		team[team2].points = 0;
		team[team2].error_color = clPurple;
		team[team2].good_color = TEAM2COLOR;

		uint8_t  teamNr;
		for (teamNr=0;teamNr<2;teamNr++){
			uint8_t i;
			for(i=0;i<3;i++){
				team[teamNr].groups[i].status = groupOff;
			}
		}
		uint16_t randomNumber= random();
		for (teamNr=0;teamNr<((gameMode==gm2P)?2:1);teamNr++){
			uint8_t i;
			for(i=0;i<3;i++){
				team[teamNr].groups[i].status = randomNumber % 2 + 1;
				randomNumber /= 2;
			}
		}
		//init Led to team with respect to field size
		uint8_t i;
		for(i=0;i< ledbox_count_current / 4;i++){
			const uint8_t teamNr = ((ledbox_state == half_field) || (i < 1) || (i > 3)) ? 0 : 1;
			const uint8_t groupNr = ((ledbox_state == half_field) || (i < 1)) ? i : (i < 4) ? i - 1 : i - 3;
			uint8_t j;
			for(j=0;j< 4;j++){
				uint8_t number = i * 4 + j;
				switch(j){
					case 0:
					case 3:
								LEDToTeam[number].teamNr = 2;
								break;
					case 1:
								team[teamNr].groups[groupNr].firstLED = number;
					case 2:
								LEDToTeam[number].teamNr = teamNr;
								LEDToTeam[number].GroupNr= groupNr;
								LEDToTeam[number].LEDNr= j - 1;
								break;
				}
			}
		}
		return 0;
	}else{
		return 1;
	}
}

void gamemode_update(){
	uint8_t i;
	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		if (buttons_get(i)) {
			pushButton(i);
		}
	}
	//code to update RGB and IR LED
	setLEDs();
	showPoints(team[team1].points, team[team2].points);
}

void gamemode_finalize(uint8_t count, uint8_t mode){
	//game end

	uint8_t points1MaxVal= MAX_TRYS * POINTS_PER_PRESS;
	uint8_t points2MaxVal= MAX_TRYS * POINTS_PER_PRESS;
	
	uint8_t i;
	uint8_t numberMax = (full_field)? ledbox_count_current / 2 : ledbox_count_current;
	uint8_t number = (count > numberMax)? numberMax: count;

	//for (i = 0; i < ledbox_count_current; i++) {
	//	rgb_set(i, clBlack);
	//}
	
	showPoints(team[team1].points, team[team2].points);


	if(ledbox_state == full_field){
		//0 -> no leds
		//76 -> all leds
		//1-75 -> >=one led on, >=1 led off
		for (i = 0; i < number; i++) {
	  	 if(((i * points1MaxVal)/(numberMax - 1U)) < (team[team1].points)){
				rgb_set(i, team[team1].teamColor);
			} else {
				rgb_set(i, clBlack);
			}
		}
		for (i = 0; i < number; i++) {
			if(((i * points2MaxVal)/(numberMax - 1U)) < (team[team2].points)){
				rgb_set(ledbox_count_current-1-i, team[team2].teamColor);
			} else {
				rgb_set(ledbox_count_current-1-i, clBlack);
			}
		}
	} else { 
		for (i = 0; i < number; i++) {
			uint8_t led = (ledbox_count_current - i + 3) % ledbox_count_current;
			if(((i * points1MaxVal)/(numberMax - 1U)) < (team[team1].points)){
				rgb_set(led, team[team1].teamColor);
			} else {
				rgb_set(led, clBlack);
			}
		}
	}
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

//Game priate

void setLEDs(void){
	rgb_setAll(NOCOLOR);

	uint8_t  teamNr;
	for (teamNr=0;teamNr<2;teamNr++){
		uint8_t i;
		const enum eColor color = team[teamNr].teamColor;
		for(i=0;i<3;i++){
			const uint8_t status = team[teamNr].groups[i].status;
			const uint8_t number = team[teamNr].groups[i].firstLED;
			const uint8_t special = team[teamNr].groups[i].special_timer;
			const uint8_t special_color = team[teamNr].groups[i].special_color;
			uint8_t num;
			for(num=0;num<2;num++){
				ir_set(number + num, status & (num + 1));
				if(status & (num + 1)) rgb_set(number + num, color);
			}
			if (special){
				team[teamNr].groups[i].special_timer--;
        	    rgb_set(number-1,special_color);
    	        rgb_set(number,special_color);
	            rgb_set(number+1,special_color);
            	rgb_set(number+2,special_color);
        	}
		}
	}
}

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
			uint8_t i;
			for(i=0;i<3;i++){
				team[teamNr].groups[i].status = groupOff;
			}
			team[teamNr].offGroup = 4;
		}

		team[teamNr].groups[GroupNr].status = groupOff;
  
		if (operationMode == omTeamprobe) team[teamNr].offGroup = GroupNr;
  
		if (team[teamNr].offGroup < 3){
			team[teamNr].groups[team[teamNr].offGroup].status = random() % 2 + 1;
		}

		team[teamNr].offGroup = GroupNr;
	}
}
