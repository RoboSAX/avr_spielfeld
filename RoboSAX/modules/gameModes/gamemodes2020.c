/******************************************************************************
 * gamemodes2020.c                                                            *
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
#define TEAM1COLOR        clGreen
#define TEAM1FLASHCOLOR   clBGreen
#define TEAM2COLOR        clRed
#define TEAM2FLASHCOLOR   clBRed
#define NOCOLOR           clBlack
#define ERRORCOLOR        clPurple
#define POINTSFOROWNCOLOR 3

#define NUMBEROFBLINKINGS 5
#define BLINKCOUNTDOWNMAX 3
#define BLINKCOUNTMULTI   6

#define MAX_TRYS         5
#define POINTS_PER_PRESS 3

#define SPECIAL_TIMER (LEDBOX_BUTTONS_DEBOUNCE_TIME * 4)

//**************************<Types and Variables>******************************
enum eGamemodes {
    gm1P = 0,
    gm2P,
    MaxGameModes,
};
struct sBlink {
    uint8_t LEDNr : 3;
    uint8_t countdown : 2;
    uint8_t numberOfBlinks : 3;
};
enum eGroup {
    groupOff = 0,
    group1   = 1,
    group2   = 2,
};
struct sTeamGroup {
    enum eGroup status;
    uint8_t     firstLED;
    uint8_t     special_timer;
    enum eColor special_color;
};
struct sTeam {
    uint8_t           points;
    uint8_t           trys;
    struct sTeamGroup groups[3];
    uint8_t           offGroup;

    enum eColor teamColor;

    enum eColor error_color;
    enum eColor good_color;
};

static volatile struct sTeam team[2];

enum eTeamNumber { team1, team2 };

struct sGlobalLED {
    uint8_t teamNr : 2;
    uint8_t GroupNr : 3;
    uint8_t LEDNr : 3;
};
static struct sGlobalLED LEDToTeam[LEDBOX_COUNT_MAX];

static enum eGamemodes      usedGamemode;
static enum eOperationModes operationMode;
static enum eBaseSystem     baseSystem;

//**************************<Methods>******************************************
static void pushButton(uint8_t number);
static void setLEDs(void);

void gamemode_init_2020(void) {
    maxGameModes = MaxGameModes;
    random_init();

    usedGamemode = gm1P;
    gamemode_start(usedGamemode, omTest, bsSpielfeld);
}

uint8_t gamemode_start_2020(uint8_t gameMode, enum eOperationModes oM,
                            enum eBaseSystem system) {
    if ((ledbox_state == full_field && gameMode == gm2P) ||
        ((ledbox_state == full_field || ledbox_state == half_field) &&
         gameMode == gm1P)) {

        usedGamemode  = gameMode;
        operationMode = oM;
        baseSystem    = system;

        firstNumber  = 1;
        secondNumber = 2;

        team[team1].teamColor   = TEAM1COLOR;
        team[team1].offGroup    = 3;
        team[team1].trys        = 0;
        team[team1].points      = 0;
        team[team1].error_color = clPurple;
        team[team1].good_color  = TEAM1COLOR;

        team[team2].teamColor   = TEAM2COLOR;
        team[team2].offGroup    = 3;
        team[team2].trys        = 0;
        team[team2].points      = 0;
        team[team2].error_color = clPurple;
        team[team2].good_color  = TEAM2COLOR;

        uint8_t teamNr;
        for (teamNr = 0; teamNr < 2; teamNr++) {
            uint8_t i;
            for (i = 0; i < 3; i++) {
                team[teamNr].groups[i].status = groupOff;
            }
        }
        uint16_t randomNumber = random();
        for (teamNr = 0; teamNr < ((gameMode == gm2P) ? 2 : 1); teamNr++) {
            uint8_t i;
            for (i = 0; i < 3; i++) {
                team[teamNr].groups[i].status = randomNumber % 2 + 1;
                randomNumber /= 2;
            }
        }
        // init Led to team with respect to field size
        uint8_t i;
        for (i = 0; i < ledbox_count_current / 4; i++) {
            const uint8_t teamNr  = i / (LEDBOX_COUNT_MAX / 2);
            const uint8_t groupNr = (i % (LEDBOX_COUNT_MAX / 2)) / 4;
            uint8_t       j;
            for (j = 0; j < 4; j++) {
                uint8_t number = i * 4 + j;
                switch (j) {
                    case 0:
                    case 3:
                        LEDToTeam[number].teamNr = 2;
                        break;
                    case 1:
                        team[teamNr].groups[groupNr].firstLED = number;
                    case 2:
                        LEDToTeam[number].teamNr  = teamNr;
                        LEDToTeam[number].GroupNr = groupNr;
                        LEDToTeam[number].LEDNr   = j - 1;
                        break;
                }
            }
        }
        return 0;
    } else {
        return 1;
    }
}

void gamemode_update_2020() {
    uint8_t i;
    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        if (buttons_get(shift_num(i))) {
            pushButton(i);
        }
    }
    // code to update RGB and IR LED
    setLEDs();
    showPoints(team[team1].points, team[team2].points);
}

struct Points gamemode_points_2020(uint8_t mode) {

    struct Points points;
    points.team1     = team[team1].points;
    points.team2     = team[team2].points;
    points.color1    = team[team1].teamColor;
    points.color2    = team[team2].teamColor;
    points.maxPoints = MAX_TRYS * POINTS_PER_PRESS;
    points.type      = ptBeide;
    return points;
}

void gamemode_to_display_2020(uint8_t gameMode, uint8_t const** displayOut1,
                              uint8_t const** displayOut2) {
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

void setLEDs(void) {
    rgb_setAll(NOCOLOR);

    uint8_t teamNr;
    for (teamNr = 0; teamNr < 2; teamNr++) {
        uint8_t           i;
        const enum eColor color = team[teamNr].teamColor;
        for (i = 0; i < 3; i++) {
            const uint8_t status        = team[teamNr].groups[i].status;
            const uint8_t number        = team[teamNr].groups[i].firstLED;
            const uint8_t special       = team[teamNr].groups[i].special_timer;
            const uint8_t special_color = team[teamNr].groups[i].special_color;
            uint8_t       num;
            for (num = 0; num < 2; num++) {
                ir_set(shift_num(number + num), status & (num + 1));
                if (status & (num + 1)) rgb_set(shift_num(number + num), color);
            }
            if (special) {
                team[teamNr].groups[i].special_timer--;
                rgb_set(shift_num(number - 1), special_color);
                rgb_set(shift_num(number), special_color);
                rgb_set(shift_num(number + 1), special_color);
                rgb_set(shift_num(number + 2), special_color);
            }
        }
    }
}

void pushButton(uint8_t number) {
    const uint8_t teamNr  = LEDToTeam[number].teamNr;
    const uint8_t GroupNr = LEDToTeam[number].GroupNr;
    const uint8_t LEDNr   = LEDToTeam[number].LEDNr;
    if (teamNr > 1) return;

    if (team[teamNr].groups[GroupNr].status) {
        team[teamNr].trys++;
        if (team[teamNr].groups[GroupNr].status == LEDNr + 1) {
            team[teamNr].points += POINTS_PER_PRESS;
            team[teamNr].groups[GroupNr].special_color =
              team[teamNr].good_color;
        } else {
            team[teamNr].groups[GroupNr].special_color =
              team[teamNr].error_color;
        }
        team[teamNr].groups[GroupNr].special_timer = SPECIAL_TIMER;

        if ((operationMode == omGame) && (team[teamNr].trys >= MAX_TRYS)) {
            uint8_t i, j, k;
            if (baseSystem == bsSpielfeld) {
                for (i = 0; i < 3; i++) {
                    team[teamNr].groups[i].status = groupOff;
                }
                team[teamNr].offGroup = 4;
            } else {
                enum eColor colorFinalize;
                uint8_t     numberFinalize;
                numberFinalize = team[teamNr].points / POINTS_PER_PRESS;
                colorFinalize  = team[teamNr].teamColor;
                for (i = 0; i < numberFinalize; i++) {
                    for (j = 1; j < ledbox_count_current; j += 4) {
                        for (k = 0; k < 2; k++) {
                            rgb_clearAll();
                            rgb_set(j + k, colorFinalize);
                            waitMsAndUpdate(50);
                        }
                    }
                }
                team[teamNr].points = 0;
                team[teamNr].trys   = 0;
            }
        }

        team[teamNr].groups[GroupNr].status = groupOff;

        if (team[teamNr].offGroup < 3) {
            team[teamNr].groups[team[teamNr].offGroup].status =
              random() % 2 + 1;
        }

        team[teamNr].offGroup = GroupNr;
    }
}
