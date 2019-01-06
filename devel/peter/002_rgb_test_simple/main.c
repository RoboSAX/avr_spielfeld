/******************************************************************************
* rgb_test_simple/main.c                                                      *
* ======================                                                      *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.06.18                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "master.h"
#include "ledbox.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<Prototypes>***************************************
int main(void);
void init(void);

//**************************<Methods>******************************************

//**************************[init]*********************************************
void init () {

    uint8_t i;
    for (i = 0; i < 10; i++) {
        debug_led1(1); debug_led2(0);  delay_ms(200);
        debug_led1(0); debug_led2(1);  delay_ms(200);
    }

    debug_led1(0);
    debug_led2(0);
}

//**************************[main]*********************************************
int main () {

    // initialization
    master_init();
    ledbox_init();

    init();

    uint8_t i;

    while (1) {

        if (master_button1()) {
            debug_led1(1);

            rgb_setAll(clYellow);
        }
        if (master_button2()) {
            debug_led2(1);

            rgb_setAll(clBlue);
        }
        if (master_button3()) {
            debug_led1(0);
            debug_led2(0);

            rgb_setAll(clGreen);
            ir_clearAll();
        }


        for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
            if (buttons_get(i)) {
                rgb_set(i, clWhite);
                ir_set (i,    0xFF);
            }
        }

        _master_buttons_update();
        _ledbox_buttons_and_ir_update();
        _ledbox_rgb_update();
        delay_ms(10);
    }

    return (0);
}
