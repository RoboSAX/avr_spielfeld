/******************************************************************************
 * gamemodes2023.c                                                            *
 * ===============                                                            *
 *                                                                            *
 * Version: 1.0.1                                                             *
 * Date   : 22.02.25                                                          *
 * Author : Peter Weissig, Leander Herr                                       *
 *                                                                            *
 *****************************************************************************/


//**************************<Included files>***********************************
#include "modulesInterface/gamemodes.h"
#include "modulesInterface/global.h"
#include "modulesInterface/ledbox.h"
#include "modulesInterface/master.h"
#include "modulesInterface/menueHelper.h"
#include "modulesInterface/random.h"

// optics:
// blink if pressed correct
// blink red[/color] if error


//**************************<Macros>*******************************************
#define LED_PER_TEAM   6
#define TEAM1COLOR     clGreen
#define TEAM2COLOR     clBlue
#define TEAM1NOCOLOR   clBlack
#define TEAM2NOCOLOR   clBlack
#define NEUTRALCOLOR   clBlack
#define LED_TEAM1_0_to 4

//**************************<Types and Variables>******************************
enum eGamemodes { gm2P = 0, gm1P = 1, MaxGameModes };

struct sTeam {
    uint8_t activate_LEDS[LED_PER_TEAM];
};
static struct sTeam    team1, team2;
static enum eGamemodes gamemode;
//**************************<Methods>******************************************
static void switchLED(struct sTeam *myteam, struct sTeam *enemy,
                      uint8_t mynumber);

uint8_t team1NrToLEDNr(uint8_t nr) {
    if (nr < 3) return nr;
    return nr + 14;
}
uint8_t team2NrToLEDNr(uint8_t nr) {
    if (nr < 3) return nr + 12;
    return nr + 2;
}
//		0,1,2,3  		4,5,6,7
// 20,21,22,23		         8,9,10,11
//		19,18,17,16 	15,14,13,12
int8_t LEDToTeamNr(uint8_t nr) {
    if (nr < 3) return nr + 1;
    if (nr > 16 && nr <= 19) return nr - 14 + 1;
    if (nr > 4 && nr <= 7) return -(nr - 2 + 1);
    if (nr > 11 && nr <= 14) return -(nr - 12 + 1);
    return 0;
}

void gamemode_init_2023(void) {
    maxGameModes = MaxGameModes;
    gamemode_start(gm2P, omTest, bsSpielfeld);
}

uint8_t gamemode_start_2023(uint8_t              gameMode,
                            enum eOperationModes operationMode,
                            enum eBaseSystem     system) {
    if ((ledbox_state != full_field) ||
        (gameMode != gm1P && gameMode != gm2P)) {
        return 1;
    }
    if (system == bsTeamprobe) {
        firstNumber  = 0;
        secondNumber = 12;
    }
    gamemode = gameMode;
    uint8_t i;
    for (i = 0; i < LED_PER_TEAM; i++) {
        team1.activate_LEDS[i] = 1;
        if (gameMode == gm2P)
            team2.activate_LEDS[i] = 1;
        else
            team2.activate_LEDS[i] = 0;
    }
    return 0;
}

static void setLEDs(void) {
    uint8_t i;
    rgb_setAll(NEUTRALCOLOR);
    ir_setAll(0);
    for (i = 0; i < LED_PER_TEAM; i++) {
        uint8_t number1 = team1NrToLEDNr(i);
        if (team1.activate_LEDS[i]) {
            rgb_set(number1, TEAM1COLOR);
            ir_set(number1, 1);
        } else {
            if ((gamemode != gm1P) && (team2.activate_LEDS[i]))
                rgb_set(number1, TEAM1NOCOLOR);
            else
                rgb_set(number1, NEUTRALCOLOR);
            ir_set(number1, 0);
        }
        uint8_t number2 = team2NrToLEDNr(i);
        if (team2.activate_LEDS[i]) {
            rgb_set(number2, TEAM2COLOR);
            ir_set(number2, 1);
        } else {
            if ((gamemode != gm1P) && (team1.activate_LEDS[i]))
                rgb_set(number2, TEAM2NOCOLOR);
            else
                rgb_set(number2, NEUTRALCOLOR);
            ir_set(number2, 0);
        }
    }
}

static void pushButton(uint8_t rawNumber) {
    struct sTeam *myteam, *enemy;
    uint8_t       mynumber;
    int8_t        number = LEDToTeamNr(rawNumber);
    if (number > 0) {
        myteam   = &team1;
        enemy    = &team2;
        mynumber = number - 1;
    } else if (number < 0) {
        number   = -number;
        myteam   = &team2;
        enemy    = &team1;
        mynumber = number - 1;
    }
    switchLED(myteam, enemy, mynumber);
}
void switchLED(struct sTeam *myteam, struct sTeam *enemy, uint8_t mynumber) {
    myteam->activate_LEDS[mynumber] = 0;
    if (gamemode == gm2P) enemy->activate_LEDS[mynumber] = 1;
}

void gamemode_update_2023() {
    int i;
    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        if (buttons_get(i)) {
            pushButton(i);
        }
    }
    setLEDs();
}

struct Points gamemode_points_2023(uint8_t mode) {
    // game end

    struct Points points;
    points.team1 = 0;
    points.team2 = 0;
    if (gamemode == gm1P) {
        uint8_t i;
        for (i = 0; i < LED_PER_TEAM; i++) {
            if (!team1.activate_LEDS[i]) {
                points.team1 += 3;
            }
        }

        points.color1    = TEAM1COLOR;
        points.maxPoints = 3 * LED_PER_TEAM;
        points.type      = ptTeam1;

        // bälle?
    } else {
        uint8_t i;

        for (i = 0; i < LED_PER_TEAM; i++) {
            if (!team1.activate_LEDS[i]) {
                points.team1 += 3;
            }
            if (!team2.activate_LEDS[i]) {
                points.team2 += 3;
            }
        }

        points.color1    = TEAM1COLOR;
        points.color2    = TEAM2COLOR;
        points.maxPoints = 3 * LED_PER_TEAM;
        points.type      = ptMaxIstSumme;
        // bälle?
    }
    return points;
}

void gamemode_to_display_2023(uint8_t gameMode, uint8_t const **displayOut1,
                              uint8_t const **displayOut2) {
    switch (gameMode) {
        case gm1P:
            *displayOut1 = numbers[1];
            *displayOut2 = alpaP;
            break;
        case gm2P:
            *displayOut1 = numbers[2];
            *displayOut2 = alpaP;
            break;
        default:
            *displayOut1 = numbers[gameMode / 10];
            *displayOut2 = numbers[gameMode % 10];
            break;
    }
}



// Game priate
