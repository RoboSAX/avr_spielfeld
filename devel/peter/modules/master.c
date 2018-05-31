/******************************************************************************
* master.c                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 31.05.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "master.h"


//**************************<Macros>*******************************************
// buttons
#define MASTER_BUTTONS_COUNT 3

#define BUTTON1_PORT PINC
#define BUTTON1_DDR  DDRC
#define BUTTON1_PIN  _BV(4)
#define button1()    ((BUTTON1_PORT & BUTTON1_PIN) == 0x00)
#define _button1_init() (BUTTON1_DDR&= ~BUTTON1_PIN)

#define BUTTON2_PORT PINC
#define BUTTON2_DDR  DDRC
#define BUTTON2_PIN  _BV(5)
#define button2()    ((BUTTON2_PORT & BUTTON2_PIN) == 0x00)
#define _button2_init() (BUTTON2_DDR&= ~BUTTON2_PIN)

#define BUTTON3_PORT PINB
#define BUTTON3_DDR  DDRB
#define BUTTON3_PIN  _BV(2)
#define button3()      ((BUTTON3_PORT & BUTTON3_PIN) == 0x00)
#define _button3_init() (BUTTON3_DDR&= ~BUTTON3_PIN)


//**************************<Types and Variables>******************************
// buttons
volatile struct sButtonState master_buttons[MASTER_BUTTONS_COUNT];


//**************************<Methods>******************************************

//**************************[master_init]**************************************
void master_init(void) {

    // hardware
        // leds
        _debug_led1_enable();
        _debug_led2_enable();

        // buttons
        _button1_init();
        _button2_init();
        _button3_init();

    // variables
        // leds
        debug_led1(0);
        debug_led2(0);

        // buttons
}

//**************************[master_buttons_reset]*****************************
void master_buttons_reset(void) {

    master_buttons[0].state     = 0;
    master_buttons[0].countdown = MASTER_BUTTONS_DEBOUNCE_TIME;

    master_buttons[1].state     = 0;
    master_buttons[1].countdown = MASTER_BUTTONS_DEBOUNCE_TIME;

    master_buttons[2].state     = 0;
    master_buttons[2].countdown = MASTER_BUTTONS_DEBOUNCE_TIME;
}

//**************************[master_buttons_clear]*****************************
void master_buttons_clear(void) {

    master_buttons[0].flank =  0;
    master_buttons[1].flank =  0;
    master_buttons[2].flank =  0;
}

//**************************[master_buttons_get]*******************************
uint8_t master_buttons_get(uint8_t number) {

    number--;
    if (number >= MASTER_BUTTONS_COUNT) {
        return 0x00;
    }

    uint8_t result = master_buttons[number].flank;
    master_buttons[number].flank = 0;

    return result;
}

//**************************[_master_buttons_update]***************************
void _master_buttons_update(void) {

    uint8_t i;
    uint8_t state;

    for (i = 0; i < MASTER_BUTTONS_COUNT; i++) {

        // read current state
        switch (i) {
            case  0: state = button1(); break;
            case  1: state = button2(); break;
            default: state = button3(); break;
        }

        // check for changed state
        if (state != master_buttons[i].state) {
            master_buttons[i].state = state;

            // state did change
            if (!state) {
                // button released
                master_buttons[i].countdown = MASTER_BUTTONS_DEBOUNCE_TIME;
            } else if (master_buttons[i].countdown == 0x00) {
                // pushed button and no countdown
                master_buttons[i].flank = 1;
            }
            continue;
        }

        if (master_buttons[i].countdown) {
            master_buttons[i].countdown--;
        }
    }
}
