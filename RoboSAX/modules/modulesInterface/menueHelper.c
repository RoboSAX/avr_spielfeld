/******************************************************************************
* moduls/main.c                                   *
* ======================                              *
*                                         *
* Version: 1.0.0                                  *
* Date   : 01.06.18                               *
* Author : Peter Weissig, Leander Herr                        *
*                                         *
******************************************************************************/
//**************************<Included files>***********************************
#include "menueHelper.h"
#include "gamemodes.h"


//**************************<Methods>******************************************

void pointMagic(struct Points points){
	if(points.type==ptTeam1){
			showOnePoints(points.team1);
	}else{
			showPoints(points.team1,points.team2);
	}
}
void pointLedMagic(struct Points points, uint8_t count){
	uint8_t numberMax = ledbox_count_current;
   	uint8_t number = (count > numberMax)? numberMax: count;
	if(points.type==ptTeam1){
		showOnePoints(points.team1);
		showOnePointsLed(points.team1, points.color1,
					   points.maxPoints, number);
	}else{
		showPoints(points.team1,points.team2);
		if(points.type==ptBeide){
			showPointsLed (points.team1, points.color1,
					   points.team2, points.color2,
					   points.maxPoints, number);
		}else{
			showPointsLedVs(points.team1, points.color1,
					   points.team2, points.color2,
					   points.maxPoints, number);
		}
	}
}

void waitAndUpdate () {
	waitMsAndUpdate(0);
}
void waitMsAndUpdate(uint16_t msecs){

	// update asyncron mode
	// switch working and showing buffers
    uint32_t start;

    start = systick_get();

	do{
		display_switchBuffer();
		_ledbox_switchBuffer();
	}while (systick_get() - start < (uint32_t)msecs);
}

void showtime (uint16_t time,uint8_t dot) {
	if(dot)
	{
		display_double_dot=1;
	}
	else
	{
		display_double_dot=0;
	}
	uint8_t digitNot0 = 0;

	uint8_t digit = time%10;
	time/=10;
	display_setSegment(numbers[digit],3);
	if (digit){
		digitNot0 |= 0x08;
	}

	if(dot)
	{
		digit = time%6;
		time/=6;
	}
	else
	{
		digit = time%10;
		time/=10;
	}
	display_setSegment(numbers[digit],2);
	if (digit){
		digitNot0 |= 0x04;
	}

	digit = time%10;
	time/=10;
	display_setSegment(numbers[digit],1);
	if (digit){
		digitNot0 |= 0x02;
	}

	digit = time%10;
	display_setSegment(numbers[digit],0);



	if (!digit){
		display_setSegment(space,0);
		if (!(digitNot0 & 0x02)){
			display_setSegment(space,1);
			if (!(digitNot0 & 0x04)){
				display_setSegment(space,2);
			}
		}
	}
}

void showPoints (uint8_t team1Points,uint8_t team2Points) {
	display_double_dot=1;

	if (team1Points>99){
		display_setSegment(qestM,0);
		display_setSegment(qestM,1);
	}
	else {
		display_setSegment(numbers[team1Points/10],0);
		display_setSegment(numbers[team1Points%10],1);
	}

	if (team2Points>99){
		display_setSegment(qestM,2);
		display_setSegment(qestM,3);
	}
	else {
		display_setSegment(numbers[team2Points/10],2);
		display_setSegment(numbers[team2Points%10],3);
	}
}

void showOnePoints (uint8_t teamPoints) {
	display_double_dot=0;
	display_setSegment(space,0);
	display_setSegment(space,1);

	if (teamPoints>99){
		display_setSegment(qestM,2);
		display_setSegment(qestM,3);
	}
	else {
		display_setSegment(numbers[teamPoints/10],2);
		display_setSegment(numbers[teamPoints%10],3);
	}
}

void showPointsLed (uint8_t team1Points,enum eColor color1, uint8_t team2Points, enum eColor color2, uint8_t maxPoints, uint8_t numberLeds){
	showPoints(team1Points,team2Points);
	if(numberLeds>ledbox_count_current/2)numberLeds=ledbox_count_current/2;
    for (uint8_t i = 0; i < numberLeds; i++) {
       if(((i * maxPoints)/(numberLeds - 1U)) < (team1Points)){
            rgb_set(i, color1);
        } else {
            rgb_set(i, clBlack);
        }
    }
    for (uint8_t i = 0; i < numberLeds; i++) {
       if(((i * maxPoints)/(numberLeds - 1U)) < (team2Points)){
            rgb_set(ledbox_count_current-1-i, color2);
        } else {
            rgb_set(ledbox_count_current-1-i, clBlack);
        }
    }
}
void showPointsLedVs (uint8_t team1Points,enum eColor color1, uint8_t team2Points, enum eColor color2, uint8_t maxPoints, uint8_t numberLeds){
	showPoints(team1Points,team2Points);
	rgb_setAll(clBlack);
	if(numberLeds>ledbox_count_current)numberLeds=ledbox_count_current;
    for (uint8_t i = 0; i < numberLeds; i++) {
       if(((i * maxPoints)/(numberLeds - 1U)) < (team1Points)){
            rgb_set(i, color1);
        }
    }
    for (uint8_t i = 0; i < numberLeds; i++) {
       if(((i * maxPoints)/(numberLeds - 1U)) < (team2Points)){
            rgb_set(ledbox_count_current-1-i, color2);
        }
    }
}
void showOnePointsLed (uint8_t teamPoints,uint8_t maxPoints, enum eColor color, uint8_t numberLeds){
	showOnePoints(teamPoints);
    for (uint8_t i = 0; i < numberLeds; i++) {
       if(((i * maxPoints)/(numberLeds - 1U)) < (teamPoints)){
            rgb_set(i, color);
        } else {
            rgb_set(i, clBlack);
        }
    }
}

void writeModesToDisplay (enum eMasterModes masterMode, int8_t subMode){
	display_double_dot=0;
	switch (masterMode){
	case mmGameMode:
		if (subMode >= 0){
			display_clearSuperSegment(0);
			display_addRight(alpaG,0,1);
			display_addRight(alpaM,0,1);
  
			display_clearSuperSegment(1);
			if (subMode==maxGameModes){
				uint8_t const* display4Chars[4];
				curr_game_to_display(&display4Chars[0], &display4Chars[1], &display4Chars[2], &display4Chars[3]);
				display_addRight(display4Chars[2],1,1);
				display_addRight(display4Chars[3],1,1);
			}
			else{
				uint8_t const* displayLast2Chars[2];
				gamemode_to_display(subMode, &displayLast2Chars[0], &displayLast2Chars[1]);
				display_addRight(displayLast2Chars[0],1,1);
				display_addRight(displayLast2Chars[1],1,1);
			}
		} else {
			display_clearSuperSegment(0);
			display_addRight(alpaG,0,1);
			display_addRight(alpaA,0,1);
			display_clearSuperSegment(1);
			display_addRight(alpaM,1,1);
			display_addRight(alpaE,1,1);
		}
	break;
	case mmTestMode:
		if (subMode >= 0){
			display_clearSuperSegment(0);
			display_addRight(alpaT,0,0);
			display_addRight(alpaM,0,0);

			display_clearSuperSegment(1);
			if (subMode==maxGameModes){
				uint8_t const* display4Chars[4];
				curr_game_to_display(&display4Chars[0], &display4Chars[1], &display4Chars[2], &display4Chars[3]);
				display_addRight(display4Chars[2],1,1);
				display_addRight(display4Chars[3],1,1);
			}
			else{
				uint8_t const* displayLast2Chars[2];
				gamemode_to_display(subMode, &displayLast2Chars[0], &displayLast2Chars[1]);
				display_addRight(displayLast2Chars[0],1,1);
				display_addRight(displayLast2Chars[1],1,1);
			}
		} else {
			display_clearSuperSegment(0);
			display_addRight(alpaT,0,1);
			display_addRight(alpaE,0,1);
			display_clearSuperSegment(1);
			display_addRight(alpaS,1,1);
			display_addRight(alpaT,1,1);
		}
	break;
	case mmOldGameMode:
		if (subMode >= 0){
			display_clearSuperSegment(0);
			display_addRight(alpaL,0,1);
			display_addRight(alpaG,0,1);

			display_clearSuperSegment(1);
			uint8_t const* displayLast2Chars[2];
			gamemode_to_display(subMode, &displayLast2Chars[0], &displayLast2Chars[1]);
			display_addRight(displayLast2Chars[0],1,1);
			display_addRight(displayLast2Chars[1],1,1);
		} else {
			display_clearSuperSegment(0);
			display_addRight(alpaL,0,1);
			display_addRight(alpaA,0,1);
			display_clearSuperSegment(1);
			display_addRight(alpaS,1,1);
			display_addRight(alpaT,1,1);
		}
	break;
	case mmScanMode:
		switch (subMode){
			case scLedboxCount:
				display_clearSuperSegment(0);
				display_addRight(alpaS,0,1);
				display_addRight(alpaM,0,1);
				display_clearSuperSegment(1);
				display_addRight(alpaN,1,1);
				display_addRight(alpar,1,0);
				display_addRight(alpaSDot,1,0);
			break;
			case scTesting:
				display_clearSuperSegment(0);
				display_addRight(alpaS,0,1);
				display_addRight(alpaM,0,1);
				display_clearSuperSegment(1);
				display_addRight(alpaT,1,1);
				display_addRight(alpaM,1,1);
			break;
		
			default:
				display_clearSuperSegment(0);
				display_addRight(alpaS,0,1);
				display_addRight(alpaC,0,1);
				display_clearSuperSegment(1);
				display_addRight(alpaA,1,1);
				display_addRight(alpaN,1,1);
			break;
		}
	break;
	default:{
			uint8_t const* display4Chars[4];
			game_to_display(masterMode-DefaultMasterModes+1, &display4Chars[0], &display4Chars[1], &display4Chars[2], &display4Chars[3]);
			display_clearSuperSegment(0);
			display_addRight(display4Chars[0],0,1);
			display_addRight(display4Chars[1],0,1);
			display_clearSuperSegment(1);
			display_addRight(display4Chars[2],1,1);
			display_addRight(display4Chars[3],1,1);
		}
	break;
	}
}
