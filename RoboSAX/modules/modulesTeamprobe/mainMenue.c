/******************************************************************************
* module/ProbenplatineMain                                                    *
* ========================                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.11.19                                                           *
* Author : Leander Herr                                                       *
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
#include "../modulesInterface/master.h"
#include "../modulesInterface/ledbox.h"
#include "../modulesInterface/menueHelper.h"
#include "../modulesInterface/gamemodes.h"
#include "../modulesInterface/display.h"
#include "../modulesInterface/systick.h"
#include "../modulesInterface/display_chars.h"

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
			//reset
			if (master_button3()) {
				number=0;
			}
		}
		break;
	default:
		break;
	}
 }
//**************************[main]*********************************************
int main () {

	//todo: select mode....
	init();
	uint8_t gamemode = 0;
	gamemode_start(gamemode, omTeamprobe);
	buttons_reset();
	while (1) {
		gamemode_update();
		waitAndUpdate();
	}
	return (0);
}
