/******************************************************************************
 * systickBase.c                                                              *
 * =============                                                              *
 *                                                                            *
 * Version: 1.0.1                                                             *
 * Date   : 22.02.25                                                          *
 * Author : Peter Weissig                                                     *
 *                                                                            *
 * See also:                                                                  *
 *   https://github.com/RoboAG/projekt_gluecksrad                             *
 *****************************************************************************/

//*********************************<Included files>****************************
#include "display.h"
#include "ledbox.h"
#include "master.h"
#include "systick.h"



//*********************************<Types>*************************************



//*********************************<Constants>*********************************


//*********************************<Variables>*********************************
volatile uint32_t systick_count;
volatile uint8_t  makeUpdate;


//*********************************<Macros>************************************



//*********************************<Prototypes>********************************

//*********************************[systick_reset]*****************************
void systick_reset() {

    uint8_t mSREG = SREG;
    cli();
    systick_count = 0;
    makeUpdate    = 0;
    SREG          = mSREG;
}

//*********************************[systick_delay]*****************************
void systick_delay(uint16_t mseconds) {

    uint32_t start;

    start = systick_get();

    while (systick_get() - start < (uint32_t)mseconds) {
        sei();
    }
}

//*********************************[systick_get]*******************************
uint32_t systick_get() {

    uint32_t result;
    uint8_t  mSREG = SREG;
    cli();
    result = systick_count;
    SREG   = mSREG;

    return result;
}

//*********************************[systick_toHour]****************************
uint8_t systick_toHour(uint32_t time) { return time / (uint32_t)3600000; }

//*********************************[systick_toMin]*****************************
uint8_t systick_toMin(uint32_t time) {

    return (time / (uint32_t)60000) % (uint32_t)60;
}

//*********************************[systick_toSec]*****************************
uint8_t systick_toSec(uint32_t time) {

    return (time / (uint32_t)1000) % (uint32_t)60;
}

//*********************************[systick_toMsec]****************************
uint16_t systick_toMsec(uint32_t time) { return (time % (uint32_t)1000); }

//**************************<UPDATE>*******************************************
uint8_t systick_freezUpdate(enum eUpdate update) {
    if (makeUpdate & update_activ) return 0;
    makeUpdate &= ~update;
    return 1;
}

void systick_unFreezUpdate(enum eUpdate update) { makeUpdate |= update; }

void update() {
    // count up
    systick_count++;

    makeUpdate |= update_activ;

    if (makeUpdate & update_Display) {
        display_show();
    }
    if (makeUpdate & update_RGBLeds) {
        _ledbox_rgb_update();
    }
    if (makeUpdate & update_buttons) {
        _ledbox_buttons_and_ir_update();
        _master_buttons_update();
    }
    makeUpdate &= ~update_activ;
}
