#ifndef __ROBOLIB_H
#define __ROBOLIB_H

//**************************<visual studio code>*******************************
#ifndef MCU
  #define MCU atmega328p
#endif

//**************************<Precompiler switches>*****************************
#define F_CPU 20000000

//**************************<Included files>***********************************
//#include<robolib/delay.h>
//#include<robolib/string.h>
//#include<robolib/assembler.h>

#include<inttypes.h> 
#include<avr/io.h> 
#include<avr/interrupt.h> 

#define nop()  __asm__ __volatile__ ("nop" ::)

void delay_ms(uint16_t mseconds);
void delay_us(uint16_t useconds);
#endif //#ifndef __ROBOLIB_INI
