/******************************************************************************
 * ledbox.c                                                                   *
 * ========                                                                   *
 *                                                                            *
 * Version: 1.2.0                                                             *
 * Date   : 02.06.18                                                          *
 * Author : Peter Weissig, Leander Herr                                       *
 *                                                                            *
 *****************************************************************************/


//**************************<Included files>***********************************
#include "../modulesInterface/ledbox.h"

#include "../modulesInterface/systick.h"


//**************************<Macros>*******************************************
#define leds_setIr1(x) (x ? (PORTD |= _BV(3)) : (PORTD &= ~_BV(3)))
#define leds_setIr2(x) (x ? (PORTB |= _BV(3)) : (PORTB &= ~_BV(3)))

#define buttons_getBtn1() ((PINB & _BV(0)) == 0x00)
#define buttons_getBtn2() ((PINC & _BV(3)) == 0x00)

#define led1_setRed(x)   (x ? (PORTB |= _BV(1)) : (PORTB &= ~_BV(1)))
#define led1_setGreen(x) (x ? (PORTB |= _BV(2)) : (PORTB &= ~_BV(2)))
#define led1_setBlue(x)  (x ? (PORTD |= _BV(5)) : (PORTD &= ~_BV(5)))

#define led1_readRed()   ((PORTB & _BV(1)) != 0x00)
#define led1_readGreen() ((PORTB & _BV(2)) != 0x00)
#define led1_readBlue()  ((PORTD & _BV(5)) != 0x00)

#define led2_setRed(x)   (x ? (PORTC |= _BV(0)) : (PORTC &= ~_BV(0)))
#define led2_setGreen(x) (x ? (PORTC |= _BV(1)) : (PORTC &= ~_BV(1)))
#define led2_setBlue(x)  (x ? (PORTC |= _BV(2)) : (PORTC &= ~_BV(2)))

#define led2_readRed()   ((PORTC & _BV(0)) != 0x00)
#define led2_readGreen() ((PORTC & _BV(1)) != 0x00)
#define led2_readBlue()  ((PORTC & _BV(2)) != 0x00)


// #define ledbox_buttons_get ledbox_buttons[working_buffer]
// #define ledbox_buttons_set ledbox_buttons[display_buffer]

// #define ledbox_ir_get ledbox_ir[working_buffer]
// #define ledbox_ir_set ledbox_ir[display_buffer]

// #define ledbox_ir_get ledbox_ir[working_buffer]
// #define ledbox_ir_set ledbox_ir[display_buffer]
//**************************<Types and Variables>******************************

// buttons
struct sButtonState {
    uint8_t state : 1;      ///< boolean
    uint8_t flank : 1;      ///< boolean
    uint8_t countdown : 6;  ///< 0..63 (* 1ms)
};
volatile struct sButtonState ledbox_buttons[LEDBOX_COUNT_MAX];

// rgb leds
struct sRGB {
    uint8_t r;  ///< red
    uint8_t g;  ///< green
    uint8_t b;  ///< blue
};
volatile struct sRGB ledbox_rgb[LEDBOX_COUNT_MAX];

// ir leds
struct sIRLed {
    uint8_t state : 1;
    uint8_t empty : 7;
};
volatile struct sIRLed ledbox_ir[LEDBOX_COUNT_MAX];

struct sIrLedsIntern {
    uint8_t state1 : 1;
    uint8_t state2 : 1;
    uint8_t counter : 1;
};
volatile struct sIrLedsIntern ir_leds;

volatile enum eUpdate needsUpdate;


//**************************<Methods>******************************************

//**************************[ledbox_init]**************************************
void ledbox_init(void) {
    // set leds to output
    DDRB = _BV(1) |  // led1 red
           _BV(2) |  // led1 green
           _BV(3);   // led2 IR
    DDRC = _BV(0) |  // led2 red
           _BV(1) |  // led2 green
           _BV(2);   // led2 blue
    DDRD = _BV(3) |  // led1 IR
           _BV(5);   // led1 blue

    // set buttons to input (disable pullups)
    DDRB &= ~_BV(0);
    DDRC &= ~_BV(3);

    // Timer 2 (IR-led)
    TCCR2A = _BV(WGM21);   // Output-Mode: none
                           // Operation-Mode: CTC til OCRA
    TCCR2B = _BV(CS20);    // Operation-Mode: CTC til OCRA
                           // Prescaler: 1
    OCR2A  = 210;          // Top: 210 ==> 76KHz
    TIMSK2 = _BV(OCIE2A);  // enable compare match interrupt

    // startup time
    delay_ms(50);

    // buttons
    sei();

    ledbox_setup_module_count();

    firstNumber                      = 0;
    secondNumber                     = 1;
    teamprobeIncrementPerMasterPress = 1;

    // variables
    // leds
    rgb_clearAll();
    ir_clearAll();

    buttons_reset();

    needsUpdate = update_all;
}

//**************************[ledbox_set_modul_count]***************************
void ledbox_setup_module_count(void) {
    ledbox_count_current = LEDBOX_COUNT_MAX;
    ledbox_state         = full_field;
}


//**************************[rgb_set2]*****************************************
void rgb_set2(uint8_t number, uint8_t r, uint8_t g, uint8_t b) {

    if (number >= LEDBOX_COUNT_MAX) {
        return;
    }

    ledbox_rgb[number].r = r;
    ledbox_rgb[number].g = g;
    ledbox_rgb[number].b = b;

    needsUpdate |= update_RGBLeds;
}

//**************************[ir_set]*******************************************
void ir_set(uint8_t number, uint8_t x) {

    if (number >= LEDBOX_COUNT_MAX) {
        return;
    }

    ledbox_ir[number].state = x ? 1 : 0;
    needsUpdate |= update_buttons;
}

//**************************[buttons_reset]************************************
void buttons_reset(void) {

    uint8_t i;

    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        ledbox_buttons[i].state     = 0;
        ledbox_buttons[i].flank     = 0;
        ledbox_buttons[i].countdown = LEDBOX_BUTTONS_DEBOUNCE_TIME;
    }

    buttons_clear();
}

//**************************[buttons_clear]************************************
void buttons_clear(void) {

    uint8_t i;

    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        ledbox_buttons[i].flank = 0;
    }
}

//**************************[buttons_get]**************************************
uint8_t buttons_get(uint8_t number) {

    if (number >= LEDBOX_COUNT_MAX) {
        return 0x00;
    }

    uint8_t result               = ledbox_buttons[number].flank;
    ledbox_buttons[number].flank = 0;

    return result;
}

//**************************[_ledbox_rgb_update]*******************************
void _ledbox_rgb_update(void) {


    static uint8_t X = 1;
    X += 0x10;

    led1_setRed(ledbox_rgb[firstNumber].r >= X);
    led1_setGreen(ledbox_rgb[firstNumber].g >= X);
    led1_setBlue(ledbox_rgb[firstNumber].b >= X);

    led2_setRed(ledbox_rgb[secondNumber].r >= X);
    led2_setGreen(ledbox_rgb[secondNumber].g >= X);
    led2_setBlue(ledbox_rgb[secondNumber].b >= X);
}

//**************************[_ledbox_buttons_and_ir_update]********************
void _ledbox_buttons_and_ir_update(void) {

    uint8_t state;

    static uint8_t global_button_countdown = 0;
    if (global_button_countdown) {
        global_button_countdown--;
    } else {
        global_button_countdown = LEDBOX_BUTTONS_GLOBAL_TIME;
    }

    // set ir_led
    ir_leds.state1 = ledbox_ir[firstNumber].state;

    state = buttons_getBtn1();

    // check for changed state
    if (state != ledbox_buttons[firstNumber].state) {
        ledbox_buttons[firstNumber].state = state;

        // state did change
        if (ledbox_buttons[firstNumber].countdown == 0x00) {
            if (!state) {
                // button released
                ledbox_buttons[firstNumber].countdown =
                  LEDBOX_BUTTONS_DEBOUNCE_TIME;
            } else {  // pushed button and no countdown
                ledbox_buttons[firstNumber].flank = 1;
                needsUpdate |= update_buttons;
            }
        }
    }

    if (!global_button_countdown) {
        if (ledbox_buttons[firstNumber].countdown) {
            ledbox_buttons[firstNumber].countdown--;
        }
    }

    ir_leds.state2 = ledbox_ir[secondNumber].state;

    state = buttons_getBtn2();

    // check for changed state
    if (state != ledbox_buttons[secondNumber].state) {
        ledbox_buttons[secondNumber].state = state;

        // state did change
        if (ledbox_buttons[secondNumber].countdown == 0x00) {
            if (!state) {
                // button released
                ledbox_buttons[secondNumber].countdown =
                  LEDBOX_BUTTONS_DEBOUNCE_TIME;
            } else {  // pushed button and no countdown
                ledbox_buttons[secondNumber].flank = 1;
            }
        }
    }

    if (!global_button_countdown) {
        if (ledbox_buttons[secondNumber].countdown) {
            ledbox_buttons[secondNumber].countdown--;
        }
    }
}

void _ledbox_switchBuffer() {
    if (needsUpdate & update_buttons)
        if (systick_freezUpdate(update_buttons)) {
            needsUpdate &= ~update_buttons;
            systick_unFreezUpdate(update_buttons);
        }
    if (needsUpdate & update_RGBLeds)
        if (systick_freezUpdate(update_RGBLeds)) {
            needsUpdate &= ~update_RGBLeds;
            systick_unFreezUpdate(update_RGBLeds);
        }
}

//**************************[delay_licht]**************************************
ISR(TIMER2_COMPA_vect) {
    if (ir_leds.counter) {
        ir_leds.counter = 0;
        leds_setIr1(ir_leds.state1);
        leds_setIr2(ir_leds.state2);
    } else {
        ir_leds.counter = 1;
        leds_setIr1(0);
        leds_setIr2(0);
    }
}
