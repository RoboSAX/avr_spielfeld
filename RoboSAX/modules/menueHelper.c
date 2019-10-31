/******************************************************************************
* moduls/main.c									  *
* ======================							  *
*										  *
* Version: 1.0.0								  *
* Date	 : 01.06.18								  *
* Author : Peter Weissig, Leander Herr						  *
*										  *
******************************************************************************/
//**************************<Included files>***********************************
#include "menueHelper.h"
#include "gamemodes.h"


//**************************<Methods>******************************************
void waitAndUpdate () {

	// update asyncron mode
	// switch working and showing buffers
	systick_freezUpdate(update_Display);
	display_switchBuffer();
	systick_unFreezUpdate(update_Display);
	systick_freezUpdate(update_others);
	_ledbox_switchBuffer();
	systick_unFreezUpdate(update_others);

	systick_delay(1);
}

void showtime (uint16_t time,uint8_t dot) {
	if(dot)
	{
//	  display_double_dot=1;
		display_double_dot=0;
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

void writeModesToDisplay (enum eMasterModes masterMode, int8_t subMode){
	display_double_dot=0;
	switch (masterMode){
	case mmGameMode:
		if (subMode >= 0){
			display_clearSuperSegment(0);
			display_addRight(alpaG,0,1);
			display_addRight(alpaM,0,1);

			uint8_t const* displayLast2Chars[2];
			gamemode_to_display(subMode,displayLast2Chars);
			display_addRight(displayLast2Chars[0],1,1);
			display_addRight(displayLast2Chars[1],1,1);
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

			uint8_t const* displayLast2Chars[2];
			gamemode_to_display(subMode,displayLast2Chars);
			display_addRight(displayLast2Chars[0],1,1);
			display_addRight(displayLast2Chars[1],1,1);
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

			uint8_t const* displayLast2Chars[2];
			gamemode_to_display(subMode,displayLast2Chars);
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
				display_addRight(alpaN,0,1);
				display_addRight(alpaU,0,1);
				display_clearSuperSegment(1);
				display_addRight(alpaM,1,1);
				display_addRight(alpaDot,1,1);
			break;
			case scTesting:
				display_clearSuperSegment(0);
				display_addRight(alpaT,0,1);
				display_addRight(alpaE,0,1);
				display_clearSuperSegment(1);
				display_addRight(alpaS,1,1);
				display_addRight(alpaT,1,1);
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
	default:
		display_clearSuperSegment(0);
		display_addRight(qestM,0,1);
		display_addRight(qestM,0,1);
		display_clearSuperSegment(1);
		display_addRight(qestM,1,1);
		display_addRight(qestM,1,1);
	break;
	}
}
