/******************************************************************************
 * lichter_intern.h                                                           *
 * ================                                                           *
 *                                                                            *
 * Version: 1.1.0                                                             *
 * Date   : 07.02.19                                                          *
 * Author : Peter Weissig, Daniel Gunkel, Tina Lenk, Leander Herr             *
 *                                                                            *
 * Original source code was taken and modified from:                          *
 *   https://github.com/RoboAG/projekt_gluecksrad                             *
 *                                                                            *
 * See also:                                                                  *
 *   https://github.com/RoboSAX/avr_spielfeld                                 *
 *****************************************************************************/

#ifndef __LICHTER_INTERN_H
#define __LICHTER_INTERN_H

//**************************<Included files>***********************************
#include <avr/io.h>
#include <inttypes.h>

#include "robolib.h"


//**************************<Macros>*******************************************
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


//**************************<Prototypes>***************************************
void init_hardware(void);
void leds_clearAll(void);
void leds_clearLED(uint8_t led_num);
void leds_setLED(uint8_t led_num, uint8_t color);
void leds_initTest(void);

void led1_setIr(uint8_t state);
void led2_setIr(uint8_t state);

void buttons_reset(void);
void buttons_clearAll(void);

uint8_t button1_readFlank(void);
uint8_t button2_readFlank(void);
uint8_t buttonMode_readFlank(void);

void delay_licht(uint16_t mseconds);

#endif  // #ifndef __LICHTER_INTERN_H
