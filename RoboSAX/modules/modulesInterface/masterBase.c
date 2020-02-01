/******************************************************************************
* master.c                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.1                                                              *
* Date   : 31.05.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "master.h"


//**************************<Macros>*******************************************
// leds (only for testing - will be removed)
#define MASTER_LED1_PORT      PORTD
#define MASTER_LED1_DDR       DDRD
#define MASTER_LED1_PIN       _BV(1)
#define debug_led1(x)        (x ? (MASTER_LED1_PORT&= ~MASTER_LED1_PIN) : \
                             (MASTER_LED1_PORT|= MASTER_LED1_PIN))
#define _debug_led1_enable() (MASTER_LED1_DDR|= MASTER_LED1_PIN)

#define MASTER_LED2_PORT      PORTD
#define MASTER_LED2_DDR       DDRD
#define MASTER_LED2_PIN       _BV(4)
#define debug_led2(x)        (x ? (MASTER_LED2_PORT|= MASTER_LED2_PIN) : \
                             (MASTER_LED2_PORT&= ~MASTER_LED2_PIN))
#define _debug_led2_enable() (MASTER_LED2_DDR|= MASTER_LED2_PIN)

// buttons
#define MASTER_BUTTONS_COUNT 3

//**************************<Types and Variables>******************************
// buttons
struct sButtonState {
    uint8_t state    	: 1; ///< boolean
    uint8_t flank    	: 1; ///< boolean
    uint8_t countdown	: 4; ///< 0..15 (* 1ms)
	uint8_t empty    	: 2;
};

volatile struct sButtonState master_buttons[MASTER_BUTTONS_COUNT];


//**************************<Methods>******************************************

//**************************[master_buttons_reset]*****************************
void master_buttons_reset(void) {

    master_buttons[0].state= 0;
    master_buttons[0].countdown = MASTER_BUTTONS_DEBOUNCE_TIME;
    master_buttons[0].flank= 0;

    master_buttons[1].state= 0;
    master_buttons[1].countdown = MASTER_BUTTONS_DEBOUNCE_TIME;
    master_buttons[1].flank= 0;

    master_buttons[2].state= 0;
    master_buttons[2].countdown = MASTER_BUTTONS_DEBOUNCE_TIME;
    master_buttons[2].flank= 0;

    master_buttons_clear();
}

//**************************[master_buttons_clear]*****************************
void master_buttons_clear(void) {

    master_buttons[0].flank=  0;
    master_buttons[1].flank=  0;
    master_buttons[2].flank=  0;
}

//**************************[master_buttons_get]*******************************
uint8_t master_buttons_get_pushed(uint8_t number) {

    number--;
    if (number >= MASTER_BUTTONS_COUNT) {
        return 0x00;
    }

    uint8_t result = master_buttons[number].flank;
    master_buttons[number].flank= 0;

    return result;
}

uint8_t master_buttons_get_push_and_released(uint8_t number) {

    number--;
    if (number >= MASTER_BUTTONS_COUNT) {
        return 0x00;
    }

    uint8_t result = (master_buttons[number].flank)&&(!master_buttons[number].state);
    if (result) master_buttons[number].flank= 0;

    return result;
}

uint8_t master_buttons_get_state(uint8_t number) {

    number--;
    if (number >= MASTER_BUTTONS_COUNT) {
        return 0x00;
    }

    return master_buttons[number].state;
}

//**************************[_master_buttons_update]***************************
void _master_buttons_update(void) {

    uint8_t i;
    uint8_t state;

    static uint8_t global_button_countdown = 0;
    if (global_button_countdown){global_button_countdown--;}
    else{
        global_button_countdown=MASTER_BUTTONS_GLOBAL_TIME;
    }


    for (i = 0; i < MASTER_BUTTONS_COUNT; i++) {

        // read current state
		state = _master_button(i);

        // check for changed state
        if (state != master_buttons[i].state) {
            master_buttons[i].state= state;

            if (master_buttons[i].countdown == 0x00) {
            // state did change
                if (!state) {
                    // button released
                    master_buttons[i].countdown = MASTER_BUTTONS_DEBOUNCE_TIME;
                } else {                // pushed button and no countdown
                    master_buttons[i].flank= 1;
                }
            }
            continue;
        }

        if (!global_button_countdown){
            if (master_buttons[i].countdown) {
                master_buttons[i].countdown--;
            }
        }
    }
}
