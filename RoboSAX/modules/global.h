/******************************************************************************
* gloabel.h                                                                   *
* =========                                                                   *
*                                                                             *
* Version: 1.2.1                                                              *
* Date   : 02.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __GLOBAL_H
#define __GLOBAL_H


//**************************<Included files>***********************************
#include <inttypes.h>


//**************************<Types and Variables>******************************

// leds
enum eColor {
    clPurple = 0,
    clBlue   = 1,
    clCyan   = 2,
    clGreen  = 3,
    clYellow = 4,
    clRed    = 5,
    clWhite  = 6,
    clBlack  = 7,
    clLBlue  = 8,
    clLGreen = 9,
    clRain0  = 10,
    clRain1  = 11,
    clRain2  = 12,
    clRain3  = 13,
    clRain4  = 14,
    clRain5  = 15,
    clRain   = clRain0
};

struct sRGB {
    uint8_t r; ///< red
    uint8_t g; ///< green
    uint8_t b; ///< blue
    uint8_t dataOut[8];///< data for the USART
};

// buttons
struct sButtonState {
    uint8_t state     : 1; ///< boolean
    uint8_t flank     : 1; ///< boolean
    uint8_t countdown : 6; ///< 0..63 (* 10ms)
};


#endif //#ifndef __GLOBAL_H
