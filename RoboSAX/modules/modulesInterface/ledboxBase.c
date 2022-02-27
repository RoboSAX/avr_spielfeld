/******************************************************************************
* ledbox.c                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.2.0                                                              *
* Date   : 02.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "ledbox.h"

//**************************<Types and Variables>******************************

volatile uint8_t ledbox_count_current;
volatile enum fieldState ledbox_state;

volatile uint8_t firstNumber;
volatile uint8_t secondNumber;
volatile uint8_t teamprobeIncrementPerMasterPress;

//**************************<Methods>******************************************

//**************************[rgb_set]******************************************
void rgb_set(uint8_t number, enum eColor color) {

	if (number >= LEDBOX_COUNT_MAX) {
		return;
	}

	#define H2 30
	switch (color) {
		case clPurple: rgb_set2(number, 255,   0, 255); break;
		case clPB	 : rgb_set2(number,  H2,   0, 255); break;
		case clBlue  : rgb_set2(number,   0,   0, 255); break;
		case clBC	 : rgb_set2(number,   0,  H2, 255); break;
		case clCyan  : rgb_set2(number,   0, 255, 255); break;
		case clCG	 : rgb_set2(number,   0, 255,  H2); break;
		case clGreen : rgb_set2(number,   0, 255,	0); break;
		case clGY	 : rgb_set2(number,  H2, 255,	0); break;
		case clYellow: rgb_set2(number, 255, 255,	0); break;
		case clYR	 : rgb_set2(number, 255,  H2,	0); break;
		case clRed	 : rgb_set2(number, 255,   0,	0); break;
		case clRP	 : rgb_set2(number, 255,   0,  H2); break;

		case clBlack : rgb_set2(number,   0,   0,	0); break;
		case clWhite : rgb_set2(number, 255, 255, 255); break;

		case clLBlue : rgb_set2(number,  10,  10,  50); break;
		case clLGreen: rgb_set2(number,  10,  50,  10); break;
		case clLRed  : rgb_set2(number,  50,  10,  10); break;

		case clBBlue : rgb_set2(number,   0,   0, 100); break;
		case clBGreen: rgb_set2(number,   0, 100,	0); break;
		case clBRed  : rgb_set2(number, 100,   0,	0); break;

		case clRain0 : rgb_set (number,(number +11)%12); break;
		case clRain1 : rgb_set (number,(number +10)%12); break;
		case clRain2 : rgb_set (number,(number + 9)%12); break;
		case clRain3 : rgb_set (number,(number + 8)%12); break;
		case clRain4 : rgb_set (number,(number + 7)%12); break;
		case clRain5 : rgb_set (number,(number + 6)%12); break;
		case clRain6 : rgb_set (number,(number + 5)%12); break;
		case clRain7 : rgb_set (number,(number + 4)%12); break;
		case clRain8 : rgb_set (number,(number + 3)%12); break;
		case clRain9 : rgb_set (number,(number + 2)%12); break;
		case clRain10: rgb_set (number,(number + 1)%12); break;
		case clRain11: rgb_set (number,(number + 0)%12); break;
		default		 :									break;
	}
}

//**************************[rgb_setAll]***************************************
void rgb_setAll(enum eColor color) {

	uint8_t i;

	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		rgb_set(i, color);
	}
}

//**************************[rgb_setAll2]**************************************
void rgb_setAll2(uint8_t r, uint8_t g, uint8_t b) {

	uint8_t i;

	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		rgb_set2(i, r, g, b);
	}
}

//**************************[rgb_clearAll]*************************************
void rgb_clearAll() {

	uint8_t i;

	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		rgb_set2(i, 0, 0, 0);
	}
}


//**************************[ir_setAll]****************************************
void ir_setAll(uint8_t x) {

	uint8_t i;

	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		ir_set(i, x);
	}
}

//**************************[ir_clearAll]**************************************
void ir_clearAll(void) {

	ir_setAll(0);
}
