/******************************************************************************
 * systick.c                                                                  *
 * =========                                                                  *
 *                                                                            *
 * Version: 1.0.1                                                             *
 * Date   : 22.02.25                                                          *
 * Author : Peter Weissig                                                     *
 *                                                                            *
 * See also:                                                                  *
 *   https://github.com/RoboAG/projekt_gluecksrad                             *
 *****************************************************************************/

//*********************************<Included files>****************************
#include "robosax/interface/systick.h"

//*********************************<Types>*************************************

//*********************************<Constants>*********************************

//*********************************<Variables>*********************************

//*********************************<Macros>************************************

//*********************************<Prototypes>********************************
ISR(TIMER2_COMPA_vect);

//*********************************[systick_init]******************************
void systick_init(void) {

#if F_CPU / 1000 / 1024 > 256
#error "can't setup systick with TIMER0 - prescaler maximum is 1024"

#elif F_CPU / 1000 / 256 > 256
#define TICK_CS  0b111
#define TICK_MAX ((uint8_t)(F_CPU / 1000 / 1024))

#elif F_CPU / 1000 / 128 > 256
#define TICK_CS  0b110
#define TICK_MAX ((uint8_t)(F_CPU / 1000 / 256))

#elif F_CPU / 1000 / 64 > 256
#define TICK_CS  0b101
#define TICK_MAX ((uint8_t)(F_CPU / 1000 / 128))

#elif F_CPU / 1000 / 32 > 256
#define TICK_CS  0b100
#define TICK_MAX ((uint8_t)(F_CPU / 1000 / 64))

#elif F_CPU / 1000 / 8 > 256
#define TICK_CS  0b011
#define TICK_MAX ((uint8_t)(F_CPU / 1000 / 32))

#elif F_CPU / 1000 / 1 > 256
#define TICK_CS  0b010
#define TICK_MAX ((uint8_t)(F_CPU / 1000 / 8))

#else
#define TICK_CS  0b001
#define TICK_MAX ((uint8_t)(F_CPU / 1000 / 1))
#endif

    ASSR = 0;

    // 8-bit Timer
    // Mode 2 (CTC until OCRA)
    TCCR2A = _BV(WGM21);
    // Bit 6-7 (COM2Ax) =   00 output mode (none)
    // Bit 4-5 (COM2Bx) =   00 output mode (none)
    // Bit 2-3 (  -   ) =      reserved
    // Bit 0-1 (WGM2x ) =   10 select timer mode [WGM22 in TCCR0B]

    TCCR2B = (TICK_CS & 0x07);
    // Bit 6-7 (FOC2n)  =    0 force output compare (none)
    // Bit 4-5 (  -   ) =      reserved
    // Bit 3   (WGM22 ) =    0 select timer mode [WGM0x in TCCR2A]
    // Bit 0-2 (CS2x  ) =  ??? [calculated]

    TCNT2 = 0;
    // Timer/Counter Register - current value of timer

    OCR2A = TICK_MAX;
    // Output Compare Register - top for timer

    TIMSK2 = _BV(OCIE2A);
    // Bit 3-7 (  -   ) =      reserved
    // Bit 2   (OCIE2B) =    0 interrupt for compare match B
    // Bit 1   (OCIE2A) =    1 interrupt for compare match A (systick)
    // Bit 0   (TOIE2 ) =    0 interrupt for overflow

    TIFR2 = _BV(OCF2A);
    // Bit 3-7 (  -   ) =      reserved
    // Bit 2   (OCF2B ) =    0 interrupt for compare match B
    // Bit 1   (OCF2A ) =    1 interrupt for compare match A (systick)
    // Bit 0   (TOV2  ) =    0 interrupt for overflow

    systick_reset();
}

//**************************[ISR(TIMER0_COMPA_vect)]***************************
ISR(TIMER2_COMPA_vect) {
    // turn on interrupts
    TIMSK2 &= ~_BV(OCIE2A);
    sei();

    update();

    // turn off interrupts
    cli();
    TIMSK2 |= _BV(OCIE2A);
}
