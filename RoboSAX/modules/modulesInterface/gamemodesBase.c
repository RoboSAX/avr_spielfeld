/******************************************************************************
 * gamemodesBase.c                                                            *
 * ===============                                                            *
 *                                                                            *
 * Version: 1.0.1                                                             *
 * Date   : 22.02.25                                                          *
 * Author : Peter Weissig, Leander Herr                                       *
 *                                                                            *
 *****************************************************************************/

//**************************<Included files>***********************************
#include "currentYear.h"
#include "gamemodes.h"
#include "menueHelper.h"

//**************************<Types and Variables>******************************
gamemode_init_pointer       gamemode_init;
gamemode_start_pointer      gamemode_start;
gamemode_update_pointer     gamemode_update;
gamemode_points_pointer     gamemode_points;
gamemode_to_display_pointer gamemode_to_display;

uint8_t  maxGameModes;
uint32_t gameTimeMax;
uint32_t gameTimeLeft;
uint8_t  currGameNr;

#define ROUNDTIME         ((uint32_t)(3UL * 60UL * 1000UL))
#define YEAR_ACTION(YEAR) YEAR,
const uint16_t availibleYears[] = {ProcessYears};
#undef YEAR_ACTION

//**************************<Methods>******************************************
void game_to_display(uint8_t gameNr, uint8_t const** displayOut1,
                     uint8_t const** displayOut2, uint8_t const** displayOut3,
                     uint8_t const** displayOut4) {
    if (gameNr == 0) gameNr = CURRENT_YEAR;
    uint16_t year = availibleYears[gameNr - 1];

    *displayOut1 = numbers[year / 1000];
    year %= 1000;
    *displayOut2 = numbers[year / 100];
    year %= 100;
    *displayOut3 = numbers[year / 10];
    *displayOut4 = numbers[year % 10];
}
void curr_game_to_display(uint8_t const** displayOut1,
                          uint8_t const** displayOut2,
                          uint8_t const** displayOut3,
                          uint8_t const** displayOut4) {
    game_to_display(currGameNr, displayOut1, displayOut2, displayOut3,
                    displayOut4);
}

void change_gameNr(uint8_t gameNr) {
    currGameNr = gameNr;
    if (gameNr == 0) gameNr = CURRENT_YEAR;
    switch (gameNr) {
#define YEAR_ACTION(YEAR)                                 \
    case GameOfYear##YEAR:                                \
        gamemode_init       = gamemode_init_##YEAR;       \
        gamemode_start      = gamemode_start_##YEAR;      \
        gamemode_update     = gamemode_update_##YEAR;     \
        gamemode_points     = gamemode_points_##YEAR;     \
        gamemode_to_display = gamemode_to_display_##YEAR; \
        break;

        ProcessYears
#undef YEAR_ACTION
    }
    // default time might be overriden by specific year init
    gameTimeMax = ROUNDTIME;
    gamemode_init();
}
