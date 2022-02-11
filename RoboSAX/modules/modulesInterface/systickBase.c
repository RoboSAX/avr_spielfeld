/*******************************************************************************
* systick.c                                                                    *
* =========                                                                    *
*                                                                              *
* Version: 1.0.0                                                               *
* Date   : 21.03.18                                                            *
* Author : Peter Weissig                                                       *
*                                                                              *
* See also:                                                                    *
*   https://github.com/RoboAG/projekt_gluecksrad                               *
*******************************************************************************/

//*********************************<Included files>*****************************
#include "systick.h"
#include "master.h"
#include "display.h"
#include "ledbox.h"



//*********************************<Types>**************************************



//*********************************<Constants>**********************************

#define SYSTICK_LED_BUTTON_UPDATE_TIME 2


//*********************************<Variables>**********************************
volatile uint32_t systick_count;
volatile uint8_t makeUpdate;


//*********************************<Macros>*************************************



//*********************************<Prototypes>*********************************

//*********************************[systick_reset]******************************
void systick_reset() {

    uint8_t mSREG = SREG;
    cli();
    systick_count = 0;
	makeUpdate = 0;
    SREG = mSREG;
}

//*********************************[systick_delay]******************************
void systick_delay(uint16_t mseconds) {

    uint32_t start;

    start = systick_get();

    while (systick_get() - start < (uint32_t)mseconds) {
        sei();
    }
}

//*********************************[systick_get]********************************
uint32_t systick_get() {

    uint32_t result;
    uint8_t mSREG = SREG;
    cli();
    result = systick_count;
    SREG = mSREG;

    return result;
}

//*********************************[systick_toHour]*****************************
uint8_t systick_toHour(uint32_t time) {

    return time / (uint32_t) 3600000;
}

//*********************************[systick_toMin]******************************
uint8_t systick_toMin(uint32_t time) {

    return (time / (uint32_t) 60000) % (uint32_t) 60;
}

//*********************************[systick_toSec]******************************
uint8_t systick_toSec(uint32_t time) {

    return (time / (uint32_t) 1000) % (uint32_t) 60;
}

//*********************************[systick_toMsec]*****************************
uint16_t systick_toMsec(uint32_t time) {

    return (time % (uint32_t) 1000);
}

//**************************<UPDATE>********************************************
uint8_t systick_freezUpdate(enum eUpdate update){
    if (makeUpdate & (update_activ|(update<<1))) return 0;
	makeUpdate&=~update;
	return 1;
}
void systick_unFreezUpdate(enum eUpdate update){
    makeUpdate|=update;
}
void update () {
    // count up
    systick_count++;

    // update
    static uint8_t update_count=0;
    if (update_count){
        update_count--;
    }else{
        update_count=SYSTICK_LED_BUTTON_UPDATE_TIME;
    }
    makeUpdate|=update_activ;
    if (makeUpdate & update_Display){
        display_show();
    	makeUpdate&=~update_Display_planed;
    }else{
    	makeUpdate|=update_Display_planed;
	}
    if ((makeUpdate & update_others) && (update_count==0)){
        _ledbox_buttons_and_ir_update();
        _master_buttons_update();
    	_ledbox_rgb_update();
    	makeUpdate&=~update_others_planed;
    }else{
    	makeUpdate|=update_others_planed;
	}
    makeUpdate&=~update_activ;
}
