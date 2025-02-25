/******************************************************************************
 * gamemodes2024.c                                                            *
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
#define TimeSlice ((uint32_t)(1UL * 60UL * 1000UL))

//**************************<Types and Variables>******************************
enum eGamemodes { gm = 0, MaxGameModes };

static enum eGamemodes gamemode;
//**************************<Methods>******************************************

void gamemode_init_2024(void) {
    maxGameModes = MaxGameModes;
    gamemode_start(gm, omTest, bsSpielfeld);
}

uint8_t gamemode_start_2024(uint8_t              gameMode,
                            enum eOperationModes operationMode,
                            enum eBaseSystem     system) {
    if ((ledbox_state != full_field) || (gameMode != gm)) {
        return 1;
    }
    if (system == bsTeamprobe) {
        return 1;
    }
    gamemode = gameMode;
    return 0;
}


void gamemode_update_2024() {
    // LED count down?
    int i;
    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        if (buttons_get(i)) continue;
    }
    ir_setAll(0);
    rgb_setAll(clBlack);

    enum eColor color1;
    enum eColor color2;
    uint32_t    gameTimeSub  = gameTimeLeft % TimeSlice;
    uint32_t    gameTimeMain = gameTimeLeft / TimeSlice;

    uint8_t color1Last = 0;
    color1Last =
      (uint8_t)(((uint32_t)(ledbox_count_current)*gameTimeSub) / TimeSlice);
    if (gameTimeMain == 0UL) {
        color1 = clRed;
        color2 = clBlack;
    } else if (gameTimeMain == 1UL) {
        color1 = clYellow;
        color2 = clRed;
    } else if (gameTimeMain == 2UL) {
        color1 = clGreen;
        color2 = clYellow;
    } else {
        color1 = clBlue;
        color2 = clGreen;
    }

    for (uint8_t i = 0; i < ledbox_count_current; i++) {
        if (i > color1Last)
            rgb_set(i, color2);
        else
            rgb_set(i, color1);
    }
}

struct Points gamemode_points_2024(uint8_t mode) {
    // game end
    struct Points points;
    points.team1     = 100;
    points.team2     = 100;
    points.maxPoints = 100;
    points.color1    = clGreen;
    points.color2    = clRed;
    points.type      = ptBeide;
    return points;
}

void gamemode_to_display_2024(uint8_t gameMode, uint8_t const** displayOut1,
                              uint8_t const** displayOut2) {
    switch (gameMode) {
        case gm:
            *displayOut1 = space;
            *displayOut2 = space;
            break;
        default:
            *displayOut1 = numbers[gameMode / 10];
            *displayOut2 = numbers[gameMode % 10];
            break;
    }
}



// Game priate
