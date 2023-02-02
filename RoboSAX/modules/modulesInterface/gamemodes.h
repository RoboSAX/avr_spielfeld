/******************************************************************************
* gamemodes.h                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __GAMEMODES_H
#define __GAMEMODES_H


//**************************<Included files>***********************************
//#include "robolib.h"
#include "gamemodesYear.h"
#include "ledbox.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<PORTDEF>*******************************************
//**************************<Macros>*******************************************
//**************************<Types and Variables>******************************
// leds

extern uint8_t maxGameModes;

enum ePType{
	ptBeide,
	ptMaxIstSumme,
	ptTeam1,
};
struct Points{
	uint8_t team1;
	enum eColor color1;
	uint8_t team2;
	enum eColor color2;
	uint8_t maxPoints;
	enum ePType type;
};

//**************************<Prototypes>***************************************
void game_to_display(uint8_t gameNr, 
					uint8_t const** displayOut1, uint8_t const** displayOut2, 
					uint8_t const** displayOut3, uint8_t const** displayOut4);
void curr_game_to_display( 
					uint8_t const** displayOut1, uint8_t const** displayOut2, 
					uint8_t const** displayOut3, uint8_t const** displayOut4);
void change_gameNr(uint8_t gameNr);

typedef void (*gamemode_init_pointer)(void);
extern gamemode_init_pointer gamemode_init;
typedef uint8_t (*gamemode_start_pointer)(uint8_t gameMode, enum eOperationModes operationMode, enum eBaseSystem system);
extern gamemode_start_pointer gamemode_start;
typedef void (*gamemode_update_pointer)(void);
extern gamemode_update_pointer gamemode_update;
typedef struct Points (*gamemode_points_pointer)(uint8_t mode);
extern gamemode_points_pointer gamemode_points;

typedef void (*gamemode_to_display_pointer)(uint8_t gameMode, uint8_t const** displayOut1, uint8_t const** displayOut2);
extern gamemode_to_display_pointer gamemode_to_display;

#endif //#ifndef __GAMEMODES_H
