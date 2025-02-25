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
#include "../modulesInterface/master.h"


//**************************<Macros>*******************************************
// buttons
#define MASTER_BUTTONS_COUNT 3

#define MASTER_BUTTON1_PORT PINC
#define MASTER_BUTTON1_DDR  DDRC
#define MASTER_BUTTON1_PIN  _BV(4)
#define button1()           ((MASTER_BUTTON1_PORT & MASTER_BUTTON1_PIN) == 0x00)
#define _button1_init()     (MASTER_BUTTON1_DDR &= ~MASTER_BUTTON1_PIN)

#define MASTER_BUTTON2_PORT PINC
#define MASTER_BUTTON2_DDR  DDRC
#define MASTER_BUTTON2_PIN  _BV(5)
#define button2()           ((MASTER_BUTTON2_PORT & MASTER_BUTTON2_PIN) == 0x00)
#define _button2_init()     (MASTER_BUTTON2_DDR &= ~MASTER_BUTTON2_PIN)

#define MASTER_BUTTON3_PORT PINB
#define MASTER_BUTTON3_DDR  DDRB
#define MASTER_BUTTON3_PIN  _BV(2)
#define button3()           ((MASTER_BUTTON3_PORT & MASTER_BUTTON3_PIN) == 0x00)
#define _button3_init()     (MASTER_BUTTON3_DDR &= ~MASTER_BUTTON3_PIN)


//**************************[master_init]**************************************
void master_init(void) {

    // hardware
    // leds
    //_debug_led1_enable();
    //_debug_led2_enable();

    // debug_led1(0);
    // debug_led2(0);

    // buttons
    _button1_init();
    _button2_init();
    _button3_init();

    // variables
    // buttons
    master_buttons_reset();
}

uint8_t _master_button(uint8_t number) {
    switch (number) {
        case 0:
            return button1();
        case 1:
            return button2();
        case 2:
            return button3();
        default:
            return 0;
    }
}
