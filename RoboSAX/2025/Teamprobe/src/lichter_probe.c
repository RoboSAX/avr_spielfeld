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

#define LED_GREEN 0
#define LED_RED   1
#define LED_BLUE  2
#define LED_WHITE 3

//**************************<Prototypes>***************************************
void led_control(uint8_t color_led1, uint8_t color_led2);
void modus_random_led(void);

void blink_multicolored_alternating(void);
void blink_different_alternating(void);
void blink_green_sync(void);

int main(void);


//**************************<Methods>******************************************

//**************************[led_control]**************************************
void led_control(uint8_t color_led1, uint8_t color_led2) {

    // color_led: Aus = 0, Weiß = 1, Grün = 2, Rot = 3

    uint8_t state_led1 = color_led1;
    uint8_t state_led2 = color_led2;

    buttons_clearAll();
    leds_setLED(1, color_led1);
    leds_setLED(2, color_led2);
    delay_ms(100);
    while (!buttonMode_readFlank()) {

        // check buttons
        uint8_t flank_led1 = button1_readFlank();
        uint8_t flank_led2 = button2_readFlank();

        // check if led1 is pushed
        if (flank_led1) {
            if (state_led1) {
                // correct push
                leds_setLED(1, 2);  // green
                leds_setLED(2, 2);
            } else {
                // incorrect push
                leds_setLED(1, 3);  // red
                leds_setLED(2, 3);
            }
        }

        // check if led2 is pushed
        if (flank_led2) {
            if (state_led2) {
                // correct push
                leds_setLED(1, 2);  // green
                leds_setLED(2, 2);
            } else {
                // incorrect push
                leds_setLED(1, 3);  // red
                leds_setLED(2, 3);
            }
        }

        delay_licht(0);  // 10ms ;-)
    }
    delay_ms(500);
    leds_clearAll();
    buttons_reset();
}


//**************************[modus_random_led]**************************
void modus_random_led() {

    uint16_t rnd_num  = random();
    uint8_t  comb_num = rnd_num % 2;
    if (comb_num) {
        led_control(0, 1);
    } else {
        led_control(1, 0);
    }
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
