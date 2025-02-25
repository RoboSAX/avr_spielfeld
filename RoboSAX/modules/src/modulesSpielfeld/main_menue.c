/******************************************************************************
 * mainMenue.c                                                                *
 * ===========                                                                *
 *                                                                            *
 * Version: 2.0.1                                                             *
 * Date   : 01.06.18                                                          *
 * Author : Peter Weissig, Leander Herr                                       *
 *                                                                            *
 *****************************************************************************/
// ms per round
#define STARTTIME ((uint32_t)(10UL * 1000UL))
#define YEARTIME  ((uint32_t)(1234UL))
#define LASTSEC   ((uint32_t)(15UL * 1000UL))

#define SWITCHTIME ((uint32_t)(5UL * 1000UL))
#define UPDATETIME ((uint32_t)(100UL))
// ms for blinking of activ changed elements
#define BLINKTIMEON                           ((uint32_t)(420UL))
#define BLINKTIMEOFF                          ((uint32_t)(200UL))
#define LEDBOX_ROLLING_RAINBOW_SWITCH_TIME_MS 500

//**************************<Included files>***********************************
#include <avr/io.h>
#include <inttypes.h>

#include "robosax/game_modes/game_modes.h"
#include "robosax/interface/display.h"
#include "robosax/interface/ledbox.h"
#include "robosax/interface/master_buttons.h"
#include "robosax/interface/menue_helper.h"
#include "robosax/interface/systick.h"
#include "robosax/spielfeld/display_chars.h"

//**************************<Prototypes>***************************************
int  main(void);
void init(void);
void default_display(void);

enum eRunningState {
    rsNone = 0,
    rsSelectMasterMode,
    rsSelectSubMode,
    rsStartMode,
    rsYearMode,
    rsTestModeRunning,
    rsGameModeStarting,
    rsGameModeRunning,
    rsGameModeFinished
};

//**************************[init]*********************************************
void yearSelect() {
    uint8_t maxMasterModes = maxGameNr;
    uint8_t masterMode     = 0;

    uint32_t currentTime      = systick_get();
    uint32_t rainbowStartTime = currentTime;
    uint8_t  rainbowNumber    = 0;
    uint8_t  newPress         = 0;
    while (1) {
        if ((rainbowStartTime + LEDBOX_ROLLING_RAINBOW_SWITCH_TIME_MS <
             currentTime) ||
            (currentTime < rainbowStartTime)) {
            rainbowNumber++;
            rainbowNumber %= NUM_RAINBOWS;
            rainbowStartTime = currentTime;
            rgb_setAll(clRainbows[rainbowNumber]);
        }
        currentTime = systick_get();
        if (master_button_ok_state()) newPress = 1;
        if (newPress) {
            if (master_button_ok()) {
                change_gameNr(masterMode + 1);
                return;
            }
            if (master_button_up()) {
                masterMode++;
                masterMode %= maxMasterModes;
            }
            if (master_button_down()) {
                (masterMode <= 0) ? masterMode = maxMasterModes - 1
                                  : masterMode--;
            }
            writeModesToDisplay(masterMode + DefaultMasterModes, 0);
        }
        waitAndUpdate();
    }
}

void init() {
    // initialization
    master_init();
    ledbox_init();
    init_display();
    systick_init();
    change_gameNr(0);

    waitMsAndUpdate(100);

    default_display();
    if (master_button_ok_state()) yearSelect();
    default_display();
}

//**************************[default display]**********************************
void default_display() {
    display_double_dot = 0;
    display_setSuperSegment(RoboSax, 0);
    display_setSuperSegment(Pokeball, 1);
    rgb_setAll(clRain);
}

void scanmode_start(enum eScanModes scanMode) {
    if (scanMode == scLedboxCount) {
        ledbox_setup_module_count();
        showPoints(ledbox_count_current, LEDBOX_COUNT_MAX);
        // waitMsAndUpdate(50);
    } else {
        while (1) {
            static uint8_t number = 0;
            // rainbow LED
            static uint8_t rainbowNumber    = 0;
            uint32_t       currentTime      = systick_get();
            uint32_t       rainbowStartTime = currentTime;
            if ((rainbowStartTime + LEDBOX_ROLLING_RAINBOW_SWITCH_TIME_MS <
                 currentTime) ||
                (currentTime < rainbowStartTime)) {
                rainbowNumber++;
                rainbowNumber %= NUM_RAINBOWS;
                rainbowStartTime = currentTime;
            }
            rgb_clearAll();
            rgb_set(number, clRainbows[rainbowNumber]);
            // IR LED
            ir_clearAll();
            ir_set(number, 1);
            // Buttontest&next
            if (buttons_get(number)) {
                number++;
                if (number >= ledbox_count_current) number = 0;
            }
            if (master_button_up())
                if (number < ledbox_count_current - 1)
                    number++;
                else
                    number = 0;
            if (master_button_down())
                if (number > 0)
                    number--;
                else
                    number = ledbox_count_current - 1;
            if (master_button_ok()) break;
            showPoints(number, ledbox_count_current);
            waitAndUpdate();
        }
    }
}

//**************************[main]*********************************************
int main() {

    init();

    uint8_t maxMasterModes = DefaultMasterModes;  //+maxGameNr;
    uint8_t maxModes[maxMasterModes];
    uint8_t i;
    for (i = 0; i < maxMasterModes; i++) {
        switch (i) {
            case mmGameMode:
            case mmTestMode:
                maxModes[i] = maxGameModes;
                break;
            case mmScanMode:
                maxModes[i] = MaxScanModes;
                break;
            case mmOldGameMode:
            default:
                maxModes[i] = 1;
                break;
        }
    }

    enum eRunningState menuemode = rsNone;

    uint32_t currentTime       = systick_get();
    uint32_t endtime           = currentTime;
    uint32_t pointsNextTime    = currentTime + SWITCHTIME;
    uint32_t rainbowSwitchTime = currentTime;

    uint32_t display_next_blink_time = currentTime + BLINKTIMEOFF;
    uint8_t  display_blink_status    = 0;

    uint32_t          submode               = 0;
    enum eMasterModes masterMode            = 0;
    uint8_t           rainbowNumber         = 0;
    uint8_t           itteration            = 0;
    uint8_t           gameRunningShowPoints = 0;
    uint8_t           pointsMode            = 1;
    uint8_t           invalidMode           = 0;
    while (1) {
        // zeitsteuerung:
        currentTime = systick_get();
        // todo time overflow -> alle 49Tage... hinreichend irrelevant
        if (currentTime > pointsNextTime) {
            pointsNextTime = currentTime + SWITCHTIME;
            pointsMode++;
        }
        if (currentTime > display_next_blink_time) {
            if (display_blink_status == 1) {
                if (menuemode == rsSelectMasterMode) {
                    // writeModesToDisplay(masterMode, submode);
                    // display_invertSegment(0);
                    // display_invertSegment(1);
                    // display_setSegment(qestM,0);
                    // display_setSegment(qestM,1);
                    display_setSegment(space, 0);
                    display_setSegment(space, 1);
                    display_setSegment(space, 2);
                    display_setSegment(space, 3);
                    display_blink_status    = 0;
                    display_next_blink_time = currentTime + BLINKTIMEOFF;
                } else if (menuemode == rsSelectSubMode) {
                    // writeModesToDisplay(masterMode, submode);
                    // display_invertSegment(2);
                    // display_invertSegment(3);
                    // display_setSegment(qestM,2);
                    // display_setSegment(qestM,3);
                    display_setSegment(space, 2);
                    display_setSegment(space, 3);
                    display_blink_status    = 0;
                    display_next_blink_time = currentTime + BLINKTIMEOFF;
                } else {
                    display_next_blink_time = currentTime + BLINKTIMEON;
                }
            } else {
                if (menuemode == rsSelectMasterMode) {
                    writeModesToDisplay(masterMode, -1);
                } else if (menuemode == rsSelectSubMode) {
                    writeModesToDisplay(masterMode, submode);
                }
                display_blink_status    = 1;
                display_next_blink_time = currentTime + BLINKTIMEON;
            }
        }
        if ((currentTime > rainbowSwitchTime) &&
            ((menuemode <= rsSelectSubMode) ||
             (menuemode == rsGameModeStarting))) {
            rainbowNumber++;
            rainbowNumber %= NUM_RAINBOWS;
            rgb_setAll(clRainbows[rainbowNumber]);
            if (menuemode == rsGameModeStarting) {
                rainbowSwitchTime += STARTTIME / ledbox_count_current;
                itteration++;
                uint32_t i;
                for (i = 0; i < itteration; i++) rgb_set(i, clBlack);
            } else {
                rainbowSwitchTime =
                  currentTime + LEDBOX_ROLLING_RAINBOW_SWITCH_TIME_MS;
            }
        }
        // led controll
        if ((menuemode <= rsSelectSubMode) && invalidMode) {
            for (int i = 0; i < 5; i++) {
                rgb_setAll(clRed);
                waitMsAndUpdate(333);
                rgb_setAll(clBlack);
                waitMsAndUpdate(333);
            }
            invalidMode = 0;
        }
        // Knoepfe
        if (master_button_full3()) {
            invalidMode = 0;
            switch (menuemode) {
                case rsNone:
                case rsStartMode:
                case rsTestModeRunning:
                    menuemode = rsSelectMasterMode;
                    break;
                case rsGameModeFinished:
                    if (master_button_state1() && master_button_state2()) {
                        menuemode = rsSelectMasterMode;
                    }
                    break;
                case rsSelectMasterMode:
                    submode = 0;
                    if (maxModes[masterMode] > 1) {
                        menuemode = rsSelectSubMode;
                    } else if (maxModes[masterMode] == 1) {
                        menuemode = rsStartMode;
                    }
                    break;
                case rsSelectSubMode:
                    menuemode = rsStartMode;
                    if (master_button_state1() && master_button_state2()) {
                        menuemode = rsSelectMasterMode;
                    }
                    break;
                case rsGameModeStarting:
                case rsGameModeRunning:
                    if (master_button_state1() && master_button_state2()) {
                        menuemode      = rsGameModeFinished;
                        pointsMode     = 0;
                        pointsNextTime = currentTime + SWITCHTIME;
                    } else {
                        gameRunningShowPoints = !gameRunningShowPoints;
                    }
                    break;
            }
            if (menuemode == rsStartMode) {
                if ((masterMode == mmGameMode) || (masterMode == mmTestMode)) {
                    if (submode >= maxGameModes)
                        invalidMode = 1;
                    else if (masterMode == mmGameMode)
                        invalidMode =
                          gamemode_start(submode, omGame, bsSpielfeld);
                    else if (masterMode == mmTestMode)
                        invalidMode =
                          gamemode_start(submode, omTest, bsSpielfeld);
                    buttons_reset();
                }
                if (invalidMode) {
                    if (maxModes[masterMode] > 1) {
                        menuemode = rsSelectSubMode;
                    } else if (maxModes[masterMode] == 1) {
                        menuemode = rsSelectMasterMode;
                    }
                } else if (masterMode == mmGameMode) {
                    menuemode = rsYearMode;
                    endtime   = currentTime + YEARTIME;
                } else if (masterMode == mmTestMode) {
                    menuemode = rsYearMode;
                    endtime   = currentTime + YEARTIME;
                } else if (masterMode == mmOldGameMode) {
                    pointsMode     = 0;
                    pointsNextTime = currentTime + SWITCHTIME;
                    menuemode      = rsGameModeFinished;
                } else if (masterMode == mmScanMode) {
                    scanmode_start(submode);
                } else {
                    change_gameNr(masterMode - DefaultMasterModes + 1);
                    for (i = 0; i < maxMasterModes; i++) {
                        switch (i) {
                            case mmGameMode:
                            case mmTestMode:
                                maxModes[i] = maxGameModes + 1;
                                break;
                            case mmScanMode:
                                maxModes[i] = MaxScanModes;
                                break;
                            case mmOldGameMode:
                            default:
                                maxModes[i] = 1;
                                break;
                        }
                    }
                    menuemode = rsSelectMasterMode;
                }
            }
        }
        if (master_button_up()) {
            if (menuemode == rsSelectMasterMode) {
                masterMode++;
                masterMode %= maxMasterModes;
            } else if (menuemode == rsSelectSubMode) {
                submode++;
                submode %= maxModes[masterMode];
            }
        }
        if (master_button_down()) {
            if (menuemode == rsSelectMasterMode) {
                (masterMode <= 0) ? masterMode = maxMasterModes - 1
                                  : masterMode--;
            } else if (menuemode == rsSelectSubMode) {
                (submode <= 0) ? submode = maxModes[masterMode] - 1 : submode--;
            }
        }
        //(Re)aktionen
        if (menuemode == rsYearMode) {
            if (currentTime < endtime) {
                uint8_t const* year[4];
                curr_game_to_display(&year[0], &year[1], &year[2], &year[3]);
                display_clearSuperSegment(0);
                display_addRight(year[0], 0, 1);
                display_addRight(year[1], 0, 1);
                display_clearSuperSegment(1);
                display_addRight(year[2], 1, 1);
                display_addRight(year[3], 1, 1);
            } else if (masterMode == mmGameMode) {
                menuemode  = rsGameModeStarting;
                itteration = 0;
                systick_reset();
                currentTime             = systick_get();
                display_next_blink_time = currentTime;
                endtime                 = currentTime + STARTTIME;
                rainbowSwitchTime =
                  currentTime + STARTTIME / ledbox_count_current;
            } else if (masterMode == mmTestMode) {
                menuemode = rsTestModeRunning;
            } else
                menuemode = rsSelectSubMode;
        } else if (menuemode == rsGameModeStarting) {
            if (currentTime < endtime) {
                showtime((endtime - currentTime) / 10, 0);
            } else {
                menuemode             = rsGameModeRunning;
                gameRunningShowPoints = 0;
                buttons_reset();
                endtime = currentTime + gameTimeMax;
            }
        } else if (menuemode == rsGameModeRunning) {
            gameTimeLeft = endtime - currentTime;
            gamemode_update();
            if (endtime < currentTime) {
                menuemode      = rsGameModeFinished;
                pointsMode     = 0;
                pointsNextTime = currentTime + SWITCHTIME;
                default_display();
            } else if (!gameRunningShowPoints ||
                       (endtime - LASTSEC < currentTime)) {
                showtime(gameTimeLeft / (1000UL), 1);
            } else {
                pointMagic(gamemode_points(pointsMode));
            }
        } else if (menuemode == rsTestModeRunning) {
            gamemode_update();
            pointMagic(gamemode_points(pointsMode));
        } else if (menuemode == rsGameModeFinished) {
            pointLedMagic(gamemode_points(pointsMode),
                          (currentTime - endtime) / UPDATETIME);
        }

        waitAndUpdate();
    }
    return (0);
}
