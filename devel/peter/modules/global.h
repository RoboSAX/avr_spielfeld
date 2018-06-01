/******************************************************************************
* gloabel.h                                                                   *
* =========                                                                   *
*                                                                             *
* Version: 1.0.1                                                              *
* Date   : 31.05.18                                                           *
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
    clBlack  = 0,
    clRed    = 1,
    clGreen  = 2,
    clYellow = 3,
    clBlue   = 4,
    clPurple = 5,
    clCyan   = 6,
    clWhite  = 7
};

struct sRGB {
    uint8_t r; ///< red
    uint8_t g; ///< green
    uint8_t b; ///< blue
};

// buttons
struct sButtonState {
    uint8_t state     : 1; ///< boolean
    uint8_t flank     : 1; ///< boolean
    uint8_t countdown : 6; ///< 0..63 (* 10ms)
};


#endif //#ifndef __GLOBAL_H
