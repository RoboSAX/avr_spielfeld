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
    clPB     ,
    clBlue   ,
    clBC     ,
    clCyan   ,
    clCG     ,
    clGreen  ,
    clGY     ,
    clYellow ,
    clYR     ,
    clRed    ,
    clRP     ,
    clWhite  ,
    clBlack  ,
    clLBlue  ,
    clLGreen ,
    clLRed   ,
    clRain0  ,
    clRain1  ,
    clRain2  ,
    clRain3  ,
    clRain4  ,
    clRain5  ,
    clRain6  ,
    clRain7  ,
    clRain8  ,
    clRain9  ,
    clRain10 ,
    clRain11 ,
    clRain   = clRain0
};

#define NUM_RAINBOWS 12
static const enum eColor clRainbows[NUM_RAINBOWS]={clRain0,clRain1,clRain2,clRain3,clRain4,clRain5,clRain6,clRain7,clRain8,clRain9,clRain10,clRain11};


struct sRGB {
    uint8_t r; ///< red
    uint8_t g; ///< green
    uint8_t b; ///< blue
    uint8_t dataOut[8];///< data for the USART
};

// buttons
struct sButtonState {
    uint16_t stateWrite: 1; ///< boolean
    uint16_t stateRead : 1; ///< boolean
    uint16_t flankWrite: 1; ///< boolean
    uint16_t flankRead : 1; ///< boolean
    uint16_t countdown :12; ///< 0..4095 (* 1ms)
};


#endif //#ifndef __GLOBAL_H
