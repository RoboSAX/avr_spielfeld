/******************************************************************************
* rgb_test_simple/main.c                                                      *
* ======================                                                      *
*                                                                             *
* Version: 2.0.0                                                              *
* Date   : 01.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/
//ms per round
#define ROUNDTIME ((uint32_t)(3UL*60UL*1000UL))
#define STARTTIME ((uint32_t)(10UL*1000UL))
#define LASTSEC ((uint32_t)(15UL*1000UL))

#define SWITCHTIME ((uint32_t)(3UL*1000UL))
#define UPDATETIME ((uint32_t)(100UL))
//ms for blinking of activ changed elements
#define BLINKTIMEON ((uint32_t)(420UL))
#define BLINKTIMEOFF ((uint32_t)(200UL))
#define LEDBOX_ROLLING_RAINBOW_SWITCH_TIME_MS 500

//**************************<Included files>***********************************
#include "master.h"
#include "ledbox.h"
#include "menueHelper.h"
#include "gamemodes.h"
#include "display.h"
#include "systick.h"
#include "display_chars.h"

#include <avr/io.h>
#include <inttypes.h>

//**************************<Prototypes>***************************************
int main(void);
void init(void);
void default_display(void);

enum eRunningState {
	rsNone = 0,
	rsSelectMasterMode ,
	rsSelectSubMode ,
	rsStartMode ,
	rsTestModeRunning ,
	rsGameModeStarting ,
	rsGameModeRunning ,
	rsGameModeFinished
};
//**************************[init]*********************************************
void init () {
	// initialization
	master_init();
	ledbox_init();
	init_display();
	systick_init();
	gamemode_init();

	waitAndUpdate();

	default_display();
}
//**************************[default display]**********************************
void default_display(){
	display_double_dot=0;
	display_setSuperSegment(RoboSax,0);
	display_setSuperSegment(Pokeball,1);
	rgb_setAll(clRain);
}

void scanmode_start(enum eScanModes scanMode){
	switch (scanMode){
	case scLedboxCount:
		ledbox_setup_module_count();
		showPoints(ledbox_count_current,LEDBOX_COUNT_MAX);
		break;
		
	case scTesting:
		{
			static uint8_t number=0;
			//rainbow LED
			static uint8_t rainbowNumber=0;
			uint32_t currentTime = systick_get();
			uint32_t rainbowStartTime = currentTime;
			if((rainbowStartTime + LEDBOX_ROLLING_RAINBOW_SWITCH_TIME_MS < currentTime)
							||(currentTime<rainbowStartTime)){
				rainbowNumber++;
				rainbowNumber %= NUM_RAINBOWS;
				rainbowStartTime = currentTime;
			}
			rgb_clearAll();
            rgb_set(number,clRainbows[rainbowNumber]);
			//IR LED
			ir_clearAll();
            ir_set(number,1);
			//Buttontest&next
			if (buttons_get(number)) number++;
			if (master_button_up()) {
				number++;
			}
			if (master_button_down()) {
				if (number>0)	number--;
			}
		}
		break;
	default:
		break;
	}
 }
//**************************[main]*********************************************
int main () {

	init();

	uint8_t maxModes[MaxMasterModes];
	uint8_t i;
	for (i=0;i<MaxMasterModes;i++){
		switch (i){
			case mmGameMode:
			case mmTestMode:
				maxModes[i]=MaxGameModes;
			break;
			case mmScanMode:
				maxModes[i]=MaxScanModes;
			break;
			case mmOldGameMode:
			default:
				maxModes[i]=1;
			break;
		}
	}

	uint32_t currentTime = systick_get();
	enum eRunningState menuemode=rsNone;
	uint32_t starttime = currentTime;
	uint32_t rainbowStartTime = currentTime;
	uint32_t submode = 0;
	enum eMasterModes masterMode = 0;
	uint32_t display_blink_time = currentTime;
	uint8_t display_blink_status = 0;
	uint8_t rainbowNumber = 0;
	uint8_t itteration = 0;
	uint8_t gameRunningShowPoints = 0;
	uint8_t pointsMode = 1;
	while (1) {
		switch (menuemode){
			case rsNone:
			case rsSelectMasterMode:
			case rsSelectSubMode:
				if((rainbowStartTime + LEDBOX_ROLLING_RAINBOW_SWITCH_TIME_MS < currentTime)||(currentTime<rainbowStartTime)){
					rainbowNumber++;
					rainbowNumber %= NUM_RAINBOWS;
					rainbowStartTime = currentTime;
					rgb_setAll(clRainbows[rainbowNumber]);
				}
			break;
			case rsGameModeStarting:
				if((((itteration + 1)*STARTTIME)/(ledbox_count_current))<(currentTime-starttime)){
					rainbowNumber++;
					itteration++;
					rainbowNumber %= NUM_RAINBOWS;
					rainbowStartTime = currentTime;
					rgb_setAll(clRainbows[rainbowNumber]);
					uint32_t i;
					for (i = 0; i < itteration; i++) {
							rgb_set(i, clBlack);
					}
				}
			break;
			case rsStartMode:
			case rsGameModeRunning:
			case rsTestModeRunning:
			case rsGameModeFinished:
			break;
		}
		currentTime = systick_get();
		if (master_button_full3()) {
			switch (menuemode){
				case rsNone:
				case rsStartMode:
				case rsTestModeRunning:
					menuemode = rsSelectMasterMode;
				break;
				case rsGameModeFinished:
					if(master_button_state1() && master_button_state2()){
						menuemode = rsSelectMasterMode;
					}
				break;
				case rsSelectMasterMode:
					submode = 0;
					if (maxModes[masterMode]>1){
						menuemode = rsSelectSubMode;
					} else if (maxModes[masterMode]==1){
						menuemode = rsStartMode;
					}
				break;
				case rsSelectSubMode:
					menuemode = rsStartMode;
				break;
				case rsGameModeStarting:
				case rsGameModeRunning:
					if(master_button_state1() && master_button_state2()){
						menuemode = rsGameModeFinished;
						pointsMode = 0;
					}
					else{
						gameRunningShowPoints=!gameRunningShowPoints;
					}
				break;
			}
		}
		switch (menuemode){
			case rsSelectMasterMode:
				if (master_button1()) {
					masterMode++;
					masterMode %= MaxMasterModes;
					//writeModesToDisplay(masterMode, submode);
				}
				if (master_button2()) {
					(masterMode <=	0)? masterMode = MaxMasterModes - 1 : masterMode-- ;
					//writeModesToDisplay(masterMode, submode);
				}
			break;
			case rsSelectSubMode:
				if (master_button1()) {
					submode++;
					submode %= maxModes[masterMode];
					//writeModesToDisplay(masterMode, submode);
				}
				if (master_button2()) {
					(submode <=  0)? submode = maxModes[masterMode] - 1 : submode-- ;
					//writeModesToDisplay(masterMode, submode);
				}
			break;
			case rsStartMode:
				if ((masterMode == mmGameMode) || (masterMode == mmTestMode))
				{
					gamemode_start(submode);
					buttons_reset();
				}
				if (masterMode == mmGameMode)
				{
					menuemode=rsGameModeStarting;
					itteration = 0;
					systick_reset();
					currentTime = systick_get();
					starttime=currentTime;
				}
				if (masterMode == mmTestMode)
				{
					menuemode=rsTestModeRunning;
				}
				if (masterMode == mmOldGameMode)
				{
					pointsMode = 0;
					menuemode=rsGameModeFinished;
				}
				if (masterMode == mmScanMode)
				{
					scanmode_start(submode);
				}
			break;
			case rsGameModeStarting:
				if (currentTime>(STARTTIME+starttime)){
					menuemode=rsGameModeRunning;
					buttons_reset();
					starttime=currentTime;
				}
				else{
					showtime((STARTTIME+starttime-currentTime)/10,0);
				}
			break;
			case rsGameModeRunning:
				gamemode_update();
				if(ROUNDTIME+starttime-LASTSEC<currentTime){
					gameRunningShowPoints = 0;
				}
				if(!gameRunningShowPoints){
					showtime((ROUNDTIME+starttime-currentTime)/(1000UL),1);
				}
				if(ROUNDTIME+starttime<currentTime){
					menuemode=rsGameModeFinished;
					pointsMode = 0;
					default_display();
				}
			break;
			case rsTestModeRunning:
				gamemode_update();
			break;
			case rsGameModeFinished:
				if (currentTime>(SWITCHTIME+starttime)){
					starttime=currentTime;
					pointsMode++;
				}
				else{
					gamemode_finalize((currentTime-starttime) / UPDATETIME, pointsMode);
				}
			break;
			case rsNone:
			default:
		break;
		}

		if(display_blink_status==1){
			if((display_blink_time+BLINKTIMEON<currentTime)||(currentTime<display_blink_time)){
				switch (menuemode){
					case rsSelectMasterMode:
						//writeModesToDisplay(masterMode, submode);
						//display_invertSegment(0);
						//display_invertSegment(1);
						//display_setSegment(qestM,0);
						//display_setSegment(qestM,1);
						display_setSegment(space,0);
						display_setSegment(space,1);
						display_blink_status = 0;
					//break;
					case rsSelectSubMode:
						//writeModesToDisplay(masterMode, submode);
						//display_invertSegment(2);
						//display_invertSegment(3);
						//display_setSegment(qestM,2);
						//display_setSegment(qestM,3);
						display_setSegment(space,2);
						display_setSegment(space,3);
						display_blink_status = 0;
					break;
					case rsNone:
					case rsStartMode:
					case rsTestModeRunning:
					case rsGameModeStarting:
					case rsGameModeRunning:
					case rsGameModeFinished:
					default:
					break;
				}
				display_blink_time = currentTime;
			}
		}
		if(display_blink_status==0){
			if((display_blink_time+BLINKTIMEOFF<currentTime)||(currentTime<display_blink_time)){
				switch (menuemode){
					case rsSelectMasterMode:
						writeModesToDisplay(masterMode, -1);
						display_blink_status = 1;
					break;
					case rsSelectSubMode:
						writeModesToDisplay(masterMode, submode);
						display_blink_status = 1;
					break;
					case rsNone:
					case rsStartMode:
					case rsTestModeRunning:
					case rsGameModeStarting:
					case rsGameModeRunning:
					case rsGameModeFinished:
					default:
					break;
				}
				display_blink_time = currentTime;
			}
		}
		waitAndUpdate();
	}
	return (0);
}
