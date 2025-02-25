/******************************************************************************
 * master.c                                                                   *
 * ========                                                                   *
 *                                                                            *
 * Version: 1.0.1                                                             *
 * Date   : 31.05.18                                                          *
 * Author : Peter Weissig, Leander Herr                                       *
 *                                                                            *
 *****************************************************************************/


//**************************<Included files>***********************************
#include "robosax/interface/master_buttons.h"


//**************************<Macros>*******************************************
// buttons
#define MASTER_BUTTONS_COUNT 3

#define MASTER_BUTTON3_PORT PIND
#define MASTER_BUTTON3_DDR  DDRD
#define MASTER_BUTTON3_PIN  _BV(4)

#define button3()       ((MASTER_BUTTON3_PORT & MASTER_BUTTON3_PIN) == 0x00)
#define _button3_init() (MASTER_BUTTON3_DDR &= ~MASTER_BUTTON3_PIN)


//**************************[master_init]**************************************
void master_init(void) {
    _button3_init();

    master_buttons_reset();
}


uint8_t _master_button(uint8_t number) {
    switch (number) {
        case 2:
            return button3();
        default:
            return 0;
    }
}
