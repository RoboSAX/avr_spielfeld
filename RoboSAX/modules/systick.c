/*******************************************************************************
* systick.c                                                                    *
* =========                                                                    *
*                                                                              *
* Version: 1.0.0                                                               *
* Date   : 21.03.18                                                            *
* Author : Peter Weissig                                                       *
*                                                                              *
* See also:                                                                    *
*   https://github.com/RoboAG/projekt_gluecksrad                               *
*******************************************************************************/

//*********************************<Included files>*****************************
#include "systick.h"



//*********************************<Types>**************************************



//*********************************<Constants>**********************************



//*********************************<Variables>**********************************
volatile uint32_t systick_count;



//*********************************<Macros>*************************************



//*********************************<Prototypes>*********************************
ISR(TIMER0_COMPA_vect);



//*********************************[systick_init]*******************************
void systick_init(void) {

    // 16-bit Timer
    // Mode 2 (CTC until ICR)
    TCCR1A = 0;
        // Bit 6-7 (COM2Ax) =   00 output mode (none)
        // Bit 4-5 (COM2Bx) =   00 output mode (none)
        // Bit 2-3 (  -   ) =      reserved
        // Bit 0-1 (WGM2x ) =   10 select timer mode [WGM02 in TCCR0B]

    TCCR1B = 0|_BV(WGM13)|_BV(WGM12)|_BV(CS10);
        // Bit 6-7 (FOC2n)  =    0 noise cancel (none)
        // Bit 5   (  -   ) =      reserved
        // Bit 4-3 (WGM12/3)=    3 select timer mode [WGM1x in TCCR0A]
        // Bit 0-2 (CS1x  ) =    1 no prescaler

    TCNT1 = 0;
        // Timer/Counter Register - current value of timer

    ICR1  = (uint16_t)(F_CPU/1000);
        // Output Compare Register - top for timer

    TIMSK1 = _BV(ICIE1);

    TIFR1 = _BV(ICF1);
    sei();
}

//*********************************[systick_reset]******************************
void systick_reset() {

    uint8_t mSREG = SREG;
    cli();
    systick_count = 0;
    SREG = mSREG;
}

//*********************************[systick_delay]******************************
void systick_delay(uint16_t mseconds) {

    uint32_t start;

    start = systick_get();

    while (systick_get() - start < (uint32_t)mseconds) {
        sei();
    }
}

//*********************************[systick_get]********************************
uint32_t systick_get() {

    uint32_t result;
    uint8_t mSREG = SREG;
    cli();
    result = systick_count;
    SREG = mSREG;

    return result;
}

//**************************[ISR(TIMER0_COMPA_vect)]****************************
ISR(TIMER1_CAPT_vect) {

    // count up
    systick_count++;

}
