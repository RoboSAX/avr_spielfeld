/******************************************************************************
* gloabel.h                                                                   *
* =========                                                                   *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 31.05.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __GLOBAL_H
#define __GLOBAL_H


//**************************<Included files>***********************************
#include <inttypes.h>


//**************************<Types and Variables>******************************

// buttons
struct sButtonState {
    uint8_t state     : 1; ///< boolean
    uint8_t flank     : 1; ///< boolean
    uint8_t countdown : 6; ///< 0..63 (* 10ms)
};


#endif //#ifndef __GLOBAL_H
