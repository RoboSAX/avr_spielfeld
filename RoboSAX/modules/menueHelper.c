/******************************************************************************
* moduls/main.c                                                               *
* ======================                                                      *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/
//**************************<Included files>***********************************
#include "mainMenue.h"


//**************************<Methods>******************************************
void waitAndUpdate () {

    // update asyncron mode
    // switch working and showing buffers
    systick_freezUpdate(update_Display);
    display_switchBuffer();
    systick_unFreezUpdate(update_Display);
    systick_freezUpdate(update_others);
    _ledbox_switchBuffer();
    systick_unFreezUpdate(update_others);

    systick_delay(1);
}

void showtime (uint16_t time,uint8_t dot) {
    if(dot)
    {
//        display_double_dot=1;
        display_double_dot=0;
        display_setSegment(numbers[time%10],3);
        time/=10;
        display_setSegment(numbers[time%6],2);
        time/=6;
        display_setSegment(numbers[time%10],1);
        time/=10;
        display_setSegment(numbers[time%10],0);
    }
    else
    {
        display_double_dot=0;
        display_setSegment(numbers[time%10],3);
        time/=10;
        display_setSegment(numbers[time%10],2);
        time/=10;
        display_setSegment(numbers[time%10],1);
        time/=10;
        display_setSegment(numbers[time%10],0);
    }
}

void showPoints (uint8_t team1Points,uint8_t team2Points) {
    display_double_dot=1;

    if (team1Points>99){
        display_setSegment(qestM,0);
        display_setSegment(qestM,1);
    }
    else {
        display_setSegment(numbers[team1Points/10],0);
        display_setSegment(numbers[team1Points%10],1);
    }

    if (team2Points>99){
        display_setSegment(qestM,2);
        display_setSegment(qestM,3);
    }
    else {
        display_setSegment(numbers[team2Points/10],2);
        display_setSegment(numbers[team2Points%10],3);
    }
}

void writeModesToDisplay (enum eMasterModes masterMode, uint8_t subMode){
    display_double_dot=0;
    switch (masterMode){
        case mmGameMode:
            display_clearSuperSegment(0);
            display_addRight(alpaG,0,1);
            display_addRight(alpaM,0,1);
            display_setSegment(numbers[subMode / 10],2);
            display_setSegment(numbers[subMode % 10],3);
        break;
        case mmTestMode:
            display_clearSuperSegment(0);
            display_addRight(alpaT,0,0);
            display_addRight(alpaM,0,0);
            display_setSegment(numbers[subMode / 10],2);
            display_setSegment(numbers[subMode % 10],3);
        break;
        default:
            display_clearSuperSegment(0);
            display_addRight(qestM,0,1);
            display_addRight(qestM,0,1);
            display_clearSuperSegment(1);
            display_addRight(qestM,1,1);
            display_addRight(qestM,1,1);
        break;
    }
}
