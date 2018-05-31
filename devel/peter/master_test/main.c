/******************************************************************************
* master_test/main.c                                                          *
* ==================                                                          *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 31.05.18                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "master.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<Prototypes>***************************************
int main(void);
void init(void);

//**************************<Methods>******************************************

//**************************[main]*********************************************
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

    init();

    while (1) {

        if (master_button1()) {
            debug_led1(1);
        }
        if (master_button2()) {
            debug_led2(1);
        }
        if (master_button3()) {
            debug_led1(0);
            debug_led2(0);
        }

        _master_buttons_update();
        delay_ms(10);
    }

    return (0);
}
