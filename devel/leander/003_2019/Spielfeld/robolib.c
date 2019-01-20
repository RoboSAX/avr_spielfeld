#include"robolib.h" 

#define nop()  __asm__ __volatile__ ("nop" ::)

void delay_ms(uint16_t mseconds){

    #if (((F_CPU + 4000) / 8000) > 255)
        uint16_t a;
    #else // if (((F_CPU + 4000) / 8000) > 255)
        uint8_t a;
    #endif // if (((F_CPU + 4000) / 8000) > 255)

    while (mseconds) {
        #if (((F_CPU + 4000) / 8000) > 255)
            for (a = (uint16_t) ((F_CPU + 4000) / 8000); a; a--) {
                nop();
                nop();
                nop();
                nop();
            }
        #else // if (((F_CPU + 4000) / 8000) > 255)
            for (a = (uint8_t) ((F_CPU + 4000) / 8000); a; a--) {
                nop();
                nop();
                nop();
                nop();
                nop();
            }
        #endif // if (((F_CPU + 4000) / 8000) > 255)
        mseconds--;
    }
}

/* Frome file: /util/delay_basic.h
   Copyright (c) 2002, Marek Michalkiewicz
   Copyright (c) 2007 Joerg Wunsch
*/
__inline__ void loopdelay(uint8_t __count) {
    // waits for (__count * 3 - 1) cylces
    __asm__ volatile (
        "1: dec %0" "\n\t"
        "brne 1b"
        : "=r" (__count)
        : "0" (__count)
    );
}


void delay_us(uint16_t useconds){

    // + 7/8 cylces for call and return
    // +   3 cycles for if and correction
    // =  10 cycles to many

    if (useconds >= ((2000000 / (F_CPU + 50000) - 1) / 2)) {
        useconds-= ((2000000 / (F_CPU + 50000) - 1) / 2);
    }

    #if (((F_CPU - 350000) / 300000) > 5)
        while (useconds) {
            // while-schleife dauert 4 Takte
            loopdelay((F_CPU - 350000) / 300000);
            useconds--;
        }
    #else // if (((F_CPU - 350000) / 300000) > 5)
        while (useconds) {
            nop();
            #if ((((F_CPU - 350000) / 100000) & 0x01) == 0x01)
                nop();
            #endif
            #if ((((F_CPU - 350000) / 100000) & 0x02) == 0x02)
                nop(); nop();
            #endif
            #if ((((F_CPU - 350000) / 100000) & 0x04) == 0x04)
                nop(); nop(); nop(); nop();
            #endif
            #if ((((F_CPU - 350000) / 100000) & 0x08) == 0x08)
                nop(); nop(); nop(); nop(); nop(); nop(); nop(); nop();
            #endif
            useconds--;
        }
    #endif
}

