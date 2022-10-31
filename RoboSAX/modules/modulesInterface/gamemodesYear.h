/******************************************************************************
* gamemodes.h                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __GAMEMODES_YEARS_H
#define __GAMEMODES_YEARS_H


//**************************<Included files>***********************************

#include <avr/io.h>
#include <inttypes.h>


//**************************<PORTDEF>*******************************************
//**************************<Macros>*******************************************
//**************************<Types and Variables>******************************
// leds

//the last Year in the list counts as the current Year
#define ProcessYears \
		YEAR_ACTION(2018)\
		YEAR_ACTION(2019)\
		YEAR_ACTION(2020)\
		YEAR_ACTION(2022)\
		YEAR_ACTION(2023)\



enum eOperationModes {
    omGame,
    omTest,
};

enum eBaseSystem {
    bsSpielfeld,
    bsTeamprobe,
};

//**************************<Prototypes>***************************************
#define YEAR_ACTION(YEAR) \
void gamemode_init_##YEAR(void);\
uint8_t gamemode_start_##YEAR(uint8_t gameMode, enum eOperationModes operationMode, enum eBaseSystem system);\
void gamemode_update_##YEAR(void);\
void gamemode_finalize_##YEAR(uint8_t count, uint8_t mode);\
\
void gamemode_to_display_##YEAR(uint8_t gameMode, uint8_t const** displayOut1, uint8_t const** displayOut2);

ProcessYears
#undef YEAR_ACTION

#define YEAR_ACTION(YEAR) GameOfYear##YEAR,
enum eAvailibleYears {
GameOfCurrentYear,
ProcessYears
NumberOfAvailibleYears
};
static const uint8_t maxGameNr=NumberOfAvailibleYears-1;
#undef YEAR_ACTION

#endif //#ifndef __GAMEMODES_H
