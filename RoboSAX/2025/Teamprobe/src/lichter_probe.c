/******************************************************************************
 * lichter_probe.c                                                            *
 * ===============                                                            *
 *                                                                            *
 * Version: 1.0.0                                                             *
 * Date   : 23.02.25                                                          *
 * Author : Peter Weissig, Daniel Gunkel, Tina Lenk, Janik Kaden              *
 *                                                                            *
 * See also:                                                                  *
 *   https://github.com/RoboSAX/avr_spielfeld                                 *
 *****************************************************************************/


//**************************<Included files>***********************************
#include <avr/io.h>
#include <inttypes.h>

#include "modulesTeamprobeSimple/lichter_intern.h"
#include "modulesTeamprobeSimple/random.h"
#include "robolib.h"


//**************************<Definitions>**************************************
enum eLedColor { clOff = 0, clWhite, clGreen, clRed };


//**************************<Prototypes>***************************************
void led_control(uint8_t const selected_led);
void modus_random_led(void);

void blink_multicolored_alternating(void);
void blink_different_alternating(void);
void blink_green_sync(void);

int main(void);


//**************************<Methods>******************************************

//**************************[led_control]**************************************
void led_control(uint8_t const selected_led) {

    buttons_clearAll();
    leds_setLED(1, selected_led == 1 ? clWhite : clOff);
    leds_setLED(2, selected_led == 2 ? clWhite : clOff);
    delay_ms(100);

    uint8_t active = 1;
    while (!buttonMode_readFlank()) {

        // check buttons
        uint8_t flank_led1 = button1_readFlank();
        uint8_t flank_led2 = button2_readFlank();

        // check if led1 is pushed
        if (active && flank_led1) {
            if (selected_led == 1) {
                // correct push
                leds_setLED(1, clGreen);
                leds_setLED(2, clGreen);
            } else {
                // incorrect push
                leds_setLED(1, clRed);
                leds_setLED(2, clRed);
            }
            active = 0;
            delay_ms(250);
            leds_clearAll();
        }

        // check if led2 is pushed
        if (active && flank_led2) {
            if (selected_led == 2) {
                // correct push
                leds_setLED(1, clGreen);
                leds_setLED(2, clGreen);
            } else {
                // incorrect push
                leds_setLED(1, clRed);
                leds_setLED(2, clRed);
            }
            active = 0;
            delay_ms(250);
            leds_clearAll();
        }

        delay_licht(0);  // 10ms ;-)
    }
}


//**************************[modus_random_led]**************************
void modus_random_led() {

    uint16_t const rnd_num  = random();
    uint8_t const  comb_num = (rnd_num % 2U) + 1U;

    led_control(comb_num);
}

//**************************[main]*********************************************
int main(void) {

    // initialize hardware
    random_init();
    init_hardware();

    delay_licht(10);

    // initial test
    leds_initTest();

    while (1) {
        modus_random_led();
    }

    return (0);
}
