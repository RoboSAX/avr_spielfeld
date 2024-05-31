/******************************************************************************
* gamemodes.c                                                                    *
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
enum eGamemodes {
    gm        = 1,
    MaxGameModes
};

//**************************<Methods>******************************************

void gamemode_init_2024(void){
    maxGameModes=1;
    gamemode_start(gm, omTest, bsSpielfeld);
}

uint8_t gamemode_start_2024(uint8_t gameMode, enum eOperationModes operationMode, enum eBaseSystem system){
	if ((ledbox_state != full_field) || (gameMode != gm)){
		return 1;
	}
	if(system==bsTeamprobe)
	{
		return 1;
	}
	gamemode=gameMode;
	return 0;
}


#define TimeSlice 1UL*60UL*1000UL
void gamemode_update_2024(){
	// LED count down?
	
	enum eColor color1;
	enum eColor color2;
	uint32_t gamTimeSub=gameTimeLeft % TimeSlice;
	uint32_t gamTimeMain=gameTimeLefti / TimeSlice;

	uint8_t color1Last=0;
	color1Last=(ledbox_count_current * gameTimeSub)/TimeSlice;
	if(gameTimeMain==0UL){
		color1=clRed;
		color2=clBlack;
	}else if(gameTimeMain==1UL){
		color1=clYellow;
		color2=clRed;
	}else{
		color1=clGreen;
		color2=clYellow;
	}

	for (uint8_t i = 0; i<ledbox_count_current; i++){
		if(i>color1Last)rgb_set(i,color2);
		else rgb_set(i,color1);
	}
}

struct Points gamemode_points_2024(uint8_t mode){
	//game end
	struct Points points;
	points.team1=0;
	points.team2=0;
	points.maxPoints=0;
	points.type=ptTeam1;
	return points;
}

void gamemode_to_display_2024(uint8_t gameMode, uint8_t const** displayOut1, uint8_t const** displayOut2){
	switch (gameMode) {
	case gm:
		*displayOut1=space;
		*displayOut2=space;
		break;
	default:
		*displayOut1=numbers[gameMode / 10];
		*displayOut2=numbers[gameMode % 10];
		break;
	}
}



//Game priate
