/******************************************************************************
 * lichter_probe.c                                                            *
 * ===============                                                            *
 *                                                                            *
 * Version: 1.1.0                                                             *
 * Date   : 07.02.19                                                          *
 * Author : Peter Weissig, Daniel Gunkel, Tina Lenk                           *
 *                                                                            *
 * See also:                                                                  *
 *   https://github.com/RoboSAX/avr_spielfeld                                 *
 *****************************************************************************/


//**************************<Included files>***********************************
#include <avr/io.h>
#include <inttypes.h>

#include "lichter_intern.h"
#include "robolib.h"
#include "robosax/common/random.h"


//**************************<Definitions>**************************************

#define LED_GREEN 0
#define LED_RED   1
#define LED_BLUE  2
#define LED_WHITE 3

//**************************<Prototypes>***************************************
void led_control(uint8_t color_led1, uint8_t color_led2);
void modus_gamefield_combination(void);
void modus_random_combinaton(void);

void blink_multicolored_alternating(void);
void blink_different_alternating(void);
void blink_green_sync(void);

int main(void);


//**************************<Methods>******************************************

//**************************[led_control]**************************************
void led_control(uint8_t color_led1, uint8_t color_led2) {

    // color_led: Green = 0, Red = 1, Blue = 2

    uint8_t  error_count    = 7;
    uint8_t  countdown_led1 = 0;
    uint8_t  countdown_led2 = 0;
    uint8_t  state_led1     = 1;
    uint8_t  state_led2     = 1;
    uint16_t rnd_num        = 0;
    uint8_t  rnd_led_num    = 0;

    buttons_clearAll();
    leds_setLED(1, color_led1, 0);
    leds_setLED(2, color_led2, 0);
    delay_ms(1000);
    while (!buttonMode_readFlank()) {

        // check buttons
        uint8_t flank_led1 = button1_readFlank();
        uint8_t flank_led2 = button2_readFlank();

        // check if led1 is pushed
        if (flank_led1) {
            if (state_led1) {
                state_led1 = 0;
            } else {

                // check if other led is turned off and count error
                if (state_led2 && color_led1 == color_led2) {
                    rnd_num = random();
                    if (error_count) {
                        rnd_led_num = rnd_num % error_count;
                    } else {
                        rnd_led_num = 0;
                    }
                    if (!rnd_led_num) {
                        state_led2 = 0;
                    }
                    error_count--;
                }

                countdown_led1 = 25;
            }
        }

        // check if led2 is pushed
        if (flank_led2) {
            if (state_led2) {
                state_led2 = 0;
            } else {

                // check if other led is turned off and count error
                if (state_led1 && color_led1 == color_led2) {
                    rnd_num = random();
                    if (error_count) {
                        rnd_led_num = rnd_num % error_count;
                    } else {
                        rnd_led_num = 0;
                    }
                    if (!rnd_led_num) {
                        state_led1 = 0;
                    }
                    error_count--;
                }

                countdown_led2 = 25;
            }
        }

        // check if both leds are coupled and there is an error
        if (color_led1 == color_led2) {
            if (countdown_led1 > countdown_led2) {
                countdown_led2 = countdown_led1;
            } else {
                countdown_led1 = countdown_led2;
            }
        }

        // output warnings
        if (countdown_led1) {
            countdown_led1--;

            leds_setLED(1, color_led1, 1);
        } else if (state_led1) {
            leds_setLED(1, color_led1, 0);
        } else {
            leds_clearLED(1);
        }

        // output warnings
        if (countdown_led2) {
            countdown_led2--;

            leds_setLED(2, color_led2, 1);
        } else if (state_led2) {
            leds_setLED(2, color_led2, 0);
        } else {
            leds_clearLED(2);
        }

        delay_licht(0);  // 10ms ;-)
    }
}

//**************************[modus_gamefield_combination]**********************
void modus_gamefield_combination() {

    uint16_t rnd_num  = random();
    uint8_t  comb_num = rnd_num % 3;

    switch (comb_num) {
        case 0:
            led_control(LED_RED, LED_GREEN);
            break;
        case 1:
            led_control(LED_GREEN, LED_BLUE);
            break;
        case 2:
            led_control(LED_BLUE, LED_RED);
            break;
    }
}

//**************************[modus_random_combinaton]**************************
void modus_random_combinaton() {

    uint16_t rnd_num  = random();
    uint8_t  comb_num = rnd_num % 9;
    uint8_t  led1_num = comb_num % 3;
    uint8_t  led2_num = comb_num / 3;

    led_control(led1_num, led2_num);
}

//**************************[blink_different_alternating]**********************
void blink_different_alternating() {

    for (uint8_t i = 0; i < 3; i++) {
        leds_setLED(1, i, 0);
        delay_licht(300);
        leds_clearAll();

        leds_setLED(2, i, 0);
        delay_licht(300);
        leds_clearAll();
    }
}

//**************************[main]*********************************************
int main(void) {

    // initialize hardware
    random_init();
    init_hardware();

    delay_licht(10);

    if (!buttonMode_readFlank()) {
        // initial test
        leds_initTest();

        while (1) {
            modus_gamefield_combination();
        }
    }

    blink_different_alternating();

    while (1) {
        modus_random_combinaton();
    }

    return (0);
}
