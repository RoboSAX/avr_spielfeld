/******************************************************************************
 * mainMenue.c                                                                *
 * ===========                                                                *
 *                                                                            *
 * Version: 1.0.1                                                             *
 * Date   : 22.02.25                                                          *
 * Author : Leander Herr                                                      *
 *                                                                            *
 *****************************************************************************/
// ms per round
#define ROUNDTIME ((uint32_t)(3UL * 60UL * 1000UL))
#define STARTTIME ((uint32_t)(10UL * 1000UL))
#define LASTSEC   ((uint32_t)(15UL * 1000UL))

#define SWITCHTIME ((uint32_t)(3UL * 1000UL))
#define UPDATETIME ((uint32_t)(100UL))
// ms for blinking of activ changed elements
#define BLINKTIMEON                           ((uint32_t)(420UL))
#define BLINKTIMEOFF                          ((uint32_t)(200UL))
#define LEDBOX_ROLLING_RAINBOW_SWITCH_TIME_MS 500

//**************************<Included files>***********************************
#include <avr/io.h>
#include <inttypes.h>

#include "../modulesInterface/display.h"
#include "../modulesInterface/display_chars.h"
#include "../modulesInterface/gamemodes.h"
#include "../modulesInterface/ledbox.h"
#include "../modulesInterface/master.h"
#include "../modulesInterface/menueHelper.h"
#include "../modulesInterface/systick.h"

//**************************<Prototypes>***************************************
int  main(void);
void init(void);
void default_display(void);
void Modus1(void);
void Modus2(void);

enum eRunningState {
    rsNone = 0,
    rsSelectMasterMode,
    rsSelectSubMode,
    rsStartMode,
    rsTestModeRunning,
    rsGameModeStarting,
    rsGameModeRunning,
    rsGameModeFinished
};


//**************************<Methods>******************************************

//**************************[init]*********************************************
void init() {
    // initialization
    master_init();
    ledbox_init();
    init_display();
    systick_init();
    change_gameNr(0);

    uint8_t i;

    for (i = 0; i < 3; i++) waitMsAndUpdate(100);
}
//**************************[Modi]**********************
void Modus1() {

    rgb_clearAll();
    ir_setAll(1);

    rgb_setAll(clRed);
    waitMsAndUpdate(500);
    rgb_setAll(clGreen);
    waitMsAndUpdate(500);
    rgb_setAll(clBlue);
    waitMsAndUpdate(500);

    ir_clearAll();
    rgb_clearAll();
}
void Modus2() {

    uint8_t oldFirst = firstNumber;
    firstNumber      = 0;
    uint8_t oldSec   = secondNumber;
    secondNumber     = 1;

    rgb_clearAll();

    rgb_set(0, clRed);
    ir_set(0, 1);
    waitMsAndUpdate(250);
    rgb_clearAll();
    ir_clearAll();

    rgb_set(1, clYellow);
    ir_set(1, 1);
    waitMsAndUpdate(250);
    rgb_clearAll();
    ir_clearAll();

    rgb_set(0, clGreen);
    ir_set(0, 1);
    waitMsAndUpdate(250);
    rgb_clearAll();
    ir_clearAll();

    rgb_set(1, clCyan);
    ir_set(1, 1);
    waitMsAndUpdate(250);
    rgb_clearAll();
    ir_clearAll();

    rgb_set(0, clBlue);
    ir_set(0, 1);
    waitMsAndUpdate(250);
    rgb_clearAll();
    ir_clearAll();

    rgb_set(1, clPurple);
    ir_set(1, 1);
    waitMsAndUpdate(250);
    rgb_clearAll();
    ir_clearAll();

    firstNumber  = oldFirst;
    secondNumber = oldSec;
}
//**************************[main]*********************************************
int main() {

    // todo: select mode....
    init();
    uint8_t gamemode = 0;
    // ledbox_count_current=12;
    // ledbox_state=half_field;
    if (/*!master_button_ok_state()*/ 1) {
        Modus1();
        gamemode_start(gamemode, omTest, bsTeamprobe);
    } else {
        Modus2();
        gamemode_start(gamemode, omGame, bsTeamprobe);
        gamemode = 1;
    }
    buttons_reset();
    while (1) {
        gamemode_update();
        waitAndUpdate();
        if master_button_ok_full () {
            if (gamemode) {
                firstNumber += teamprobeIncrementPerMasterPress;
                firstNumber %= ledbox_count_current;
                secondNumber += teamprobeIncrementPerMasterPress;
                secondNumber %= ledbox_count_current;
            } else {
                gamemode_start(gamemode, omTest, bsTeamprobe);
            }
        }
    }
    return (0);
}
