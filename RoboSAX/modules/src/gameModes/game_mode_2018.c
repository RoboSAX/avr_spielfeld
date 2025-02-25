/******************************************************************************
 * gamemodes2018.c                                                            *
 * ===============                                                            *
 *                                                                            *
 * Version: 1.0.1                                                             *
 * Date   : 22.02.25                                                          *
 * Author : Peter Weissig, Leander Herr                                       *
 *                                                                            *
 *****************************************************************************/


//**************************<Included files>***********************************
#include "robosax/common/random.h"
#include "robosax/common/time_constants.h"
#include "robosax/game_modes/game_modes.h"
#include "robosax/interface/ledbox.h"
#include "robosax/interface/master_buttons.h"
#include "robosax/interface/menue_helper.h"
#include "robosax/spielfeld/display_chars.h"

// optics:
// blink if pressed correct
// blink red[/color] if error


//**************************<Macros>*******************************************
#define LED_PER_TEAM     LEDBOX_COUNT_MAX / 2
#define TEAM1COLOR       clGreen
#define TEAM2COLOR       clBlue
#define TEAM1NOCOLOR     clLBlue
#define TEAM2NOCOLOR     clLGreen
#define NEUTRALCOLOR     clBlack
#define LED_TEAM1_0_to   4
#define PUNISHERRORTIMER (LEDBOX_BUTTONS_DEBOUNCE_TIME * 4)

//**************************<Types and Variables>******************************
enum eGamemodes {
    gmNothing        = 0,
    gmEnemy          = 1,
    gmPunishAndEnemy = 2,
    MaxGameModes
};

struct sTeam {
    uint8_t     error_timer;
    uint8_t     error_countdown;
    enum eColor error_color;
    uint8_t     activate_LEDS[LED_PER_TEAM];
};
static struct sTeam    team1, team2;
static enum eGamemodes gamemode;
//**************************<Methods>******************************************
static uint8_t punishment(struct sTeam *myteam, struct sTeam *enemy,
                          uint8_t mynumber);
static void    switchLED(struct sTeam *myteam, struct sTeam *enemy,
                         uint8_t mynumber);

void gamemode_init_2018(void) {
    maxGameModes = MaxGameModes;
    gamemode_start(gmNothing, omTest, bsSpielfeld);
}

uint8_t gamemode_start_2018(uint8_t              gameModeIn,
                            enum eOperationModes operationMode,
                            enum eBaseSystem     system) {
    gamemode              = gameModeIn;
    team1.error_countdown = 0;
    team2.error_countdown = 0;
    uint8_t i;
    for (i = 0; i < LED_PER_TEAM; i++) {
        team1.activate_LEDS[i] = 0;
        team2.activate_LEDS[i] = 0;
    }
    return 0;
}

static void setLEDs(void) {
    uint8_t i;
    for (i = 0; i < LED_PER_TEAM; i++) {
        uint8_t number1 = (i < (LED_PER_TEAM - LED_TEAM1_0_to))
                            ? (i + (LED_PER_TEAM + LED_TEAM1_0_to))
                            : (i - (LED_PER_TEAM - LED_TEAM1_0_to));
        if (team1.error_timer) {
            team1.error_timer--;
            rgb_set(number1, team1.error_color);
        } else {
            if (team1.activate_LEDS[i]) {
                rgb_set(number1, TEAM1COLOR);
                ir_set(number1, 1);
            } else {
                if ((gamemode != gmNothing) && (team2.activate_LEDS[i]))
                    rgb_set(number1, TEAM1NOCOLOR);
                else
                    rgb_set(number1, NEUTRALCOLOR);
                ir_set(number1, 0);
            }
        }
        uint8_t number2 = i + LED_TEAM1_0_to;
        if (team2.error_timer) {
            team2.error_timer--;
            rgb_set(number2, team2.error_color);
        } else {
            if (team2.activate_LEDS[i]) {
                rgb_set(number2, TEAM2COLOR);
                ir_set(number2, 1);
            } else {
                if ((gamemode != gmNothing) && (team1.activate_LEDS[i]))
                    rgb_set(number2, TEAM2NOCOLOR);
                else
                    rgb_set(number2, NEUTRALCOLOR);
                ir_set(number2, 0);
            }
        }
    }
}

static void pushButton(uint8_t number) {
    struct sTeam *myteam, *enemy;
    uint8_t       mynumber;
    if (number < LED_TEAM1_0_to) {
        myteam   = &team1;
        enemy    = &team2;
        mynumber = number + (LED_PER_TEAM - LED_TEAM1_0_to);
    } else {
        number -= LED_TEAM1_0_to;
        if (number >= LED_PER_TEAM) {
            myteam   = &team1;
            enemy    = &team2;
            mynumber = number - LED_PER_TEAM;
        } else {
            myteam   = &team2;
            enemy    = &team1;
            mynumber = number;
        }
    }
    if (!punishment(myteam, enemy, mynumber))
        switchLED(myteam, enemy, mynumber);
}
uint8_t punishment(struct sTeam *myteam, struct sTeam *enemy,
                   uint8_t mynumber) {
    if ((gamemode == gmPunishAndEnemy) && myteam->activate_LEDS[mynumber]) {
        myteam->error_countdown++;
        switch (myteam->error_countdown) {
            case 1:
                myteam->error_timer = PUNISHERRORTIMER;
                myteam->error_color = clYellow;
                break;
            case 2:
                myteam->error_timer = PUNISHERRORTIMER;
                myteam->error_color = clPurple;
                break;
            case 3:
                myteam->error_timer             = PUNISHERRORTIMER;
                myteam->error_color             = clRed;
                myteam->error_countdown         = 0;
                myteam->activate_LEDS[mynumber] = 0;
                enemy->activate_LEDS[mynumber]  = 1;
                return 1;
                break;
            default:
                myteam->error_countdown = 0;
                break;
        }
    }
    return 0;
}
void switchLED(struct sTeam *myteam, struct sTeam *enemy, uint8_t mynumber) {
    myteam->activate_LEDS[mynumber] = 1;
    if ((gamemode == gmPunishAndEnemy) || (gamemode == gmEnemy))
        enemy->activate_LEDS[mynumber] = 0;
}

void gamemode_update_2018() {
    int i;
    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        if (buttons_get(i)) {
            pushButton(i);
        }
    }
    setLEDs();
}

struct Points gamemode_points_2018(uint8_t mode) {
    // game end

    uint8_t i;

    struct Points points;
    points.team1 = 0;
    points.team2 = 0;
    for (i = 0; i < LED_PER_TEAM; i++) {
        if (team1.activate_LEDS[i]) {
            points.team1 += 3;
        }
        if (team2.error_timer) {
            points.team2 += 3;
        }
    }

    points.color1    = TEAM1COLOR;
    points.color2    = TEAM2COLOR;
    points.maxPoints = LEDBOX_COUNT_MAX / 2 * 3;
    points.type      = ptBeide;
    return points;
}

void gamemode_to_display_2018(uint8_t gameMode, uint8_t const **displayOut1,
                              uint8_t const **displayOut2) {
    switch (gameMode) {
        case gmNothing:
            *displayOut1 = numbers[1];
            *displayOut2 = alpaP;
            break;
        case gmEnemy:
            *displayOut1 = numbers[2];
            *displayOut2 = alpaP;
            break;
        case gmPunishAndEnemy:
            *displayOut1 = alpaH;
            *displayOut2 = alpaM;
            break;
        default:
            *displayOut1 = numbers[gameMode / 10];
            *displayOut2 = numbers[gameMode % 10];
            break;
    }
}

// Game priate
