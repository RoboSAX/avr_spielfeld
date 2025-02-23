/******************************************************************************
 * ledbox.h                                                                   *
 * ========                                                                   *
 *                                                                            *
 * Version: 1.2.0                                                             *
 * Date   : 02.06.18                                                          *
 * Author : Peter Weissig, Leander Herr                                       *
 *                                                                            *
 *****************************************************************************/

#ifndef __LEDBOX_H
#define __LEDBOX_H


//**************************<Included files>***********************************
#include <avr/io.h>
#include <inttypes.h>

#include "global.h"
#include "robolib.h"


//**************************<Macros>*******************************************
#define LEDBOX_COUNT_MAX            24
#define LEDBOX_FULL_FIELD           24
#define LEDBOX_FULL_FIELD_ABNORMITY 4
#define LEDBOX_HALF_FIELD           12
#define LEDBOX_HALF_FIELD_ABNORMITY 4

// buttons

//**************************<Types and Variables>******************************
// leds
enum eColor {
    clPurple = 0,
    clPB,
    clBlue,
    clBC,
    clCyan,
    clCG,
    clGreen,
    clGY,
    clYellow,
    clYR,
    clRed,
    clRP,
    clWhite,
    clBlack,
    clLBlue,
    clLGreen,
    clLRed,
    clBBlue,
    clBGreen,
    clBRed,
    clRain0,
    clRain1,
    clRain2,
    clRain3,
    clRain4,
    clRain5,
    clRain6,
    clRain7,
    clRain8,
    clRain9,
    clRain10,
    clRain11,
    clRain = clRain0
};

#define NUM_RAINBOWS 12
static const enum eColor clRainbows[NUM_RAINBOWS] = {
  clRain0, clRain1, clRain2, clRain3, clRain4,  clRain5,
  clRain6, clRain7, clRain8, clRain9, clRain10, clRain11};


enum fieldState { unknown_field = 0, full_field = 1, half_field = 2 };

volatile extern uint8_t         ledbox_count_current;
volatile extern enum fieldState ledbox_state;

volatile extern uint8_t firstNumber;
volatile extern uint8_t secondNumber;
volatile extern uint8_t teamprobeIncrementPerMasterPress;
//**************************<Prototypes>***************************************

#define shift_num(number)                                                  \
    ((ledbox_state == full_field)                                          \
       ? ((number >= LEDBOX_FULL_FIELD)                                    \
            ? LEDBOX_FULL_FIELD                                            \
            : (number + LEDBOX_FULL_FIELD - LEDBOX_FULL_FIELD_ABNORMITY) % \
                LEDBOX_FULL_FIELD)                                         \
     : (ledbox_state == half_field)                                        \
       ? ((number >= LEDBOX_HALF_FIELD)                                    \
            ? LEDBOX_HALF_FIELD                                            \
            : (number + LEDBOX_HALF_FIELD - LEDBOX_HALF_FIELD_ABNORMITY) % \
                LEDBOX_HALF_FIELD)                                         \
       : number)

// init
void ledbox_init(void);
void ledbox_setup_module_count(void);

// rgb leds
void rgb_set(uint8_t number, enum eColor color);
void rgb_set2(uint8_t number, uint8_t r, uint8_t g, uint8_t b);
void rgb_setAll(enum eColor color);
void rgb_setAll2(uint8_t r, uint8_t g, uint8_t b);
void rgb_clearAll(void);
void rgb_setAllPercentage(enum eColor color, uint8_t percentageColor, enum eColor colorFill);
void rgb_setAllPercentageTime(enum eColor color, uint32_t timeLeft, uint32_t maxTime, enum eColor colorFill);

// ir leds
void ir_set(uint8_t number, uint8_t x);
void ir_setAll(uint8_t x);
void ir_clearAll(void);

// buttons
void    buttons_reset(void);
void    buttons_clear(void);
uint8_t buttons_get(uint8_t number);

// update
void _ledbox_rgb_update(void);
void _ledbox_buttons_and_ir_update(void);
void _ledbox_switchBuffer(void);

#endif  // #ifndef __LEDBOX_H
