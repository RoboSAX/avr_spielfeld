/******************************************************************************
* Teamprobe/tht/lichter_probe.c                                               *
* =============================                                               *
*                                                                             *
* Version: 2.0.0                                                              *
* Date   : 06.02.19                                                           *
* Author : Peter Weissig, Tina Lenk, Daniel Gunkel                            *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include <avr/io.h>
#include <inttypes.h>

#include "lichter.h"
#include "lichter_intern.h"
//#include "random.h"

//**************************<Prototypes>***************************************
void modus_competition(void);
void modus_competition_simple(void);
uint8_t modus_single_field(uint8_t initial_error_count);

void blink_multicolored_alternating(void);
void blink_different_alternating(void);
void blink_green_sync(void);

int main (void);


//**************************<Methods>******************************************

//**************************[modus_setcolor]********************************
void modus_setcolor(uint8_t color_led1, uint8_t color_led2) {

    //color_led: Green = 0, Red = 1, Blue = 2

    uint8_t error_led1 = 0;
    uint8_t error_led2 = 0;
    uint8_t countdown_led1 = 0;
    uint8_t countdown_led2 = 0;
    uint8_t state_led1 = 1;
    uint8_t state_led2 = 1; 

    // 0 = led1 & led2 on; 1 = led1 off; 2 = led2 off; 3 = both led off
    uint8_t state = 0; 

    buttons_clearAll();
    leds_setLED(1, color_led1, 1);
    leds_setLED(1, color_led2, 1);

    while (!buttonMode_readFlank()) {

        // check buttons
        uint8_t flank_led1 = button1_readFlank();
        uint8_t flank_led2 = button2_readFlank();

        switch (state) {
            
            case 1:
                if (flank_led1) {
                    error_led1++;
                    if (error_led1 >= 3) {
                        error_led1 = 0;
                        
                        // extinguish another TeamLED 
                        if (color_led1 == color_led2) {
                            // insert random check here
                            state_led2 = 0;
                            state = 3;
                        }
                    }

                    countdown_led1 = 25;
                }
                if (flank_led2) {
                    state = 3;
                    state_led2 = 0;
                }
                break;

            case 2:
                if (flank_led1) {
                    state = 3;
                    state_led1 = 0;
                }
                if (flank_led2) {
                    error_led2++;
                    if (error_led2 >= 3) {
                        error_led2 = 0;

                        // extinguish another TeamLED
                        if (color_led1 == color_led2) {
                            // insert random check here
                            state_led1 = 0;
                            state = 3;
                        }
                    }

                    countdown_led2 = 25;
                }
                break;

            case 3:
                // only error output and count 
                if (flank_led1) {
                    error_led1++;
                    if (error_led1 >= 3) {
                        error_led1 = 0;
                    }

                    countdown_led1 = 25;
                }
                if (flank_led2) {
                    error_led2++;
                    if (error_led2 >= 3) {
                        error_led2 = 0;
                    }

                    countdown_led2 = 25;
                }
                break;

            default:
                if (flank_led2 && flank_led1) {
                    break;
                }
                if (flank_led1) {
                    state = 1;
                    state_led1 = 0;
                } 
                if (flank_led2) {
                    state = 2;
                    state_led2 = 0;
                }
                break;
        }

        // output warnings
        if (countdown_led1) {
            countdown_led1--;

            if (error_led1 > 0) {
                leds_setLED(1, 3, 0);
            }

            // switch(error_led1) {
            //     case  1: led1_setBlue (1); led1_setRed(1); break;
            //     case  2: led1_setGreen(1); led1_setRed(1); break;
            //     default:                   led1_setRed(1); break;
            // }
        } else if (!state_led1) {
            leds_clearLED(1);
        }

        if (countdown_led2) {
            countdown_led2--;

            if (error_led1 > 0) {
                leds_setLED(2, 3, 0);
            }

            // switch(error_led2) {
            //     case  1: led2_setGreen(1); led2_setRed(1); break;
            //     case  2: led2_setBlue (1); led2_setRed(1); break;
            //     default:                   led2_setRed(1); break;
            // }

        } else if (!state_led2) {
            leds_clearLED(2);
        }

        delay_licht(0); // 10ms ;-)
    }
}

//**************************[modus_competition]********************************
void modus_competition() {

    uint8_t error_team1 = 0;
    uint8_t error_team2 = 0;
    uint8_t countdown_team1 = 0;
    uint8_t countdown_team2 = 0;

    uint8_t state = 0; // 0 = none; 1 = Team 1; 2 = Team 2

    leds_clearAll();
    buttons_clearAll();

    while (!buttonMode_readFlank()) {

        // check buttons
        uint8_t flank_team1 = button1_readFlank();
        uint8_t flank_team2 = button2_readFlank();

        switch (state) {
            case 1:
                if (flank_team1) {
                    error_team1++;
                    if (error_team1 >= 3) {
                        error_team1 = 0;
                        state = 2;
                    }

                    countdown_team1 = 25;
                }
                if (flank_team2) {
                    state = 2;
                }
                break;

            case 2:
                if (flank_team2) {
                    error_team2++;
                    if (error_team2 >= 3) {
                        error_team2 = 0;
                        state = 1;
                    }

                    countdown_team2 = 25;
                }
                if (flank_team1) {
                    state = 1;
                }
                break;

            default:
                if (flank_team2 && flank_team1) {
                    break;
                }
                if (flank_team1) {
                    state = 1;
                } else if (flank_team2) {
                    state = 2;
                }
                break;
        }

        // set leds
        leds_clearAll();

        if (countdown_team1) {
            countdown_team1--;

            switch(error_team1) {
                case  1: led1_setBlue (1); led1_setRed(1); break;
                case  2: led1_setGreen(1); led1_setRed(1); break;
                default:                   led1_setRed(1); break;
            }
        } else if (state == 1) {
            led1_setBlue(1);
        }

        if (countdown_team2) {
            countdown_team2--;

            switch(error_team2) {
                case  1: led2_setGreen(1); led2_setRed(1); break;
                case  2: led2_setBlue (1); led2_setRed(1); break;
                default:                   led2_setRed(1); break;
            }
        } else if (state == 2) {
            led2_setGreen(1);
        }

        // set ir leds
        led1_setIr(state == 1);
        led2_setIr(state == 2);

        delay_licht(0); // 10ms ;-)
    }
}

//**************************[modus_competition_simple]*************************
void modus_competition_simple() {

    uint8_t state = 0; // 0 = none; 1 = Team 1; 2 = Team 2

    leds_clearAll();
    buttons_clearAll();

    while (!buttonMode_readFlank()) {

        // check buttons
        uint8_t flank_team1 = button1_readFlank();
        uint8_t flank_team2 = button2_readFlank();

        switch (state) {
            case 1:
                if (flank_team2) {
                    state = 2;
                }
                break;

            case 2:
                if (flank_team1) {
                    state = 1;
                }
                break;

            default:
                if (flank_team2 && flank_team1) {
                    break;
                }
                if (flank_team1) {
                    state = 1;
                } else if (flank_team2) {
                    state = 2;
                }
                break;
        }

        // set leds
        leds_clearAll();

        if (state == 1) {
            led1_setBlue(1);
        }

        if (state == 2) {
            led2_setGreen(1);
        }

        // set ir leds
        led1_setIr(state == 1);
        led2_setIr(state == 2);

        delay_licht(0); // 10ms ;-)
    }
}

//**************************[modus_single_field]*******************************
uint8_t modus_single_field(uint8_t initial_error_count) {


    uint8_t error_team = initial_error_count;
    uint8_t countdown_team = 0;

    uint8_t state1 = 0;
    uint8_t state2 = 0;

    leds_clearAll();
    buttons_clearAll();

    while (!buttonMode_readFlank()) {

        // check buttons
        uint8_t flank1 = button1_readFlank();
        uint8_t flank2 = button2_readFlank();

        if (flank1) {
            if (state1) {
                error_team++;
                if (error_team >= 3) {
                    error_team = 0;
                    state1 = 0;
                }

                countdown_team = 25;
            } else {
                state1 = 1;
            }
        }

        if (flank2) {
            if (state2) {
                error_team++;
                if (error_team >= 3) {
                    error_team = 0;
                    state2 = 0;
                }

                countdown_team = 25;
            } else {
                state2 = 1;
            }
        }

        // set leds
        leds_clearAll();

        if (countdown_team) {
            countdown_team--;

            switch(error_team) {
                case  1: led1_setGreen(1); led1_setRed(1);
                         led2_setGreen(1); led2_setRed(1); break;
                case  2: led1_setBlue (1); led1_setRed(1);
                         led2_setBlue (1); led2_setRed(1); break;
                default:                   led1_setRed(1);
                                           led2_setRed(1); break;
            }
        } else {
            if (state1) { led1_setGreen(1);}
            if (state2) { led2_setGreen(1);}
        }

        // set ir leds
        led1_setIr(state1);
        led2_setIr(state2);

        delay_licht(0); // 10ms ;-)
    }

    return error_team;
}

//**************************[blink_multicolored_alternating]*******************
void blink_multicolored_alternating() {

    for (uint8_t i = 0; i < 3; i++) {
        led1_setRed(1); led1_setGreen(1); led1_setBlue(1);
        delay_licht(300);
        leds_clearAll();

        led2_setRed(1); led2_setGreen(1); led2_setBlue(1);
        delay_licht(300);
        leds_clearAll();
    }
}

//**************************[blink_different_alternating]**********************
void blink_different_alternating(){

    for (uint8_t i = 0; i < 3; i++) {
        led1_setBlue(1);
        delay_licht(300);
        leds_clearAll();

        led2_setGreen(1);
        delay_licht(300);
        leds_clearAll();
    }
}

//**************************[blink_green_sync]*********************************
void blink_green_sync() {

    for (uint8_t i = 0; i < 3; i++) {
        led1_setGreen(1);
        led2_setGreen(1);
        delay_licht(300);

        leds_clearAll();
        delay_licht(300);
    }
}

//**************************[main]*********************************************
int main (void) {

    // initialize hardware
    init_hardware();

    delay_licht(10);

    if (!buttonMode_readFlank()) {
        // initial test
        leds_initTest();

        //random generated colors
        uint8_t led1 = 0;
        uint8_t led2 = 0;

        while (1) {
            modus_setcolor(led1, led2);
        }
    }

    while (1) {

        blink_multicolored_alternating();
        modus_competition();

        blink_different_alternating();
        modus_competition_simple(); // ok (no strafe)

        blink_green_sync();
        modus_single_field(0);
    }

    return (0);
}
