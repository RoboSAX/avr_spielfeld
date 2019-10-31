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

uint8_t firstNumber=0;
uint8_t secondNumber=1;
//**************************<Macros>*******************************************
#define leds_setIr1(x)	  ( x ? (PORTD|= _BV(3)) : (PORTD&= ~_BV(3)))
#define leds_setIr2(x)	  ( x ? (PORTB|= _BV(3)) : (PORTB&= ~_BV(3)))

#define buttons_getBtn1()	 ((PINB & _BV(0)) == 0x00)
#define buttons_getBtn2()	 ((PINC & _BV(3)) == 0x00)

#define led1_setRed(x)	 ( x ? (PORTB|= _BV(1)) : (PORTB&= ~_BV(1)))
#define led1_setGreen(x) ( x ? (PORTB|= _BV(2)) : (PORTB&= ~_BV(2)))
#define led1_setBlue(x)  ( x ? (PORTD|= _BV(5)) : (PORTD&= ~_BV(5)))

#define led1_readRed()	 ((PORTB & _BV(1)) != 0x00)
#define led1_readGreen() ((PORTB & _BV(2)) != 0x00)
#define led1_readBlue()  ((PORTD & _BV(5)) != 0x00)

#define led2_setRed(x)	 ( x ? (PORTC|= _BV(0)) : (PORTC&= ~_BV(0)))
#define led2_setGreen(x) ( x ? (PORTC|= _BV(1)) : (PORTC&= ~_BV(1)))
#define led2_setBlue(x)  ( x ? (PORTC|= _BV(2)) : (PORTC&= ~_BV(2)))

#define led2_readRed()	 ((PORTC & _BV(0)) != 0x00)
#define led2_readGreen() ((PORTC & _BV(1)) != 0x00)
#define led2_readBlue()  ((PORTC & _BV(2)) != 0x00)


//#define ledbox_buttons_get ledbox_buttons[working_buffer]
//#define ledbox_buttons_set ledbox_buttons[display_buffer]

//#define ledbox_ir_get ledbox_ir[working_buffer]
//#define ledbox_ir_set ledbox_ir[display_buffer]

//#define ledbox_ir_get ledbox_ir[working_buffer]
//#define ledbox_ir_set ledbox_ir[display_buffer]
//**************************<Types and Variables>******************************
struct sIRLed{
	uint8_t read  : 1;
	uint8_t write : 1;
	uint8_t empty : 6;
};

// buttons
volatile struct sButtonState ledbox_buttons[LEDBOX_COUNT_MAX];

// rgb leds
volatile struct sRGB ledbox_rgb[LEDBOX_COUNT_MAX];

// ir leds (order is invers!)
volatile struct sIRLed ledbox_ir[LEDBOX_COUNT_MAX];

//**************************<Methods>******************************************

// clock and load
void toggle_clk(void);
void toggle_led_load(void);


//**************************[ledbox_init]**************************************
void ledbox_init(void) {
	// set leds to output
	DDRB = _BV(1) |  // led1 red
		   _BV(2) |  // led1 green
		   _BV(3);	 // led2 IR
	DDRC = _BV(0) |  // led2 red
		   _BV(1) |  // led2 green
		   _BV(2);	 // led2 blue
	DDRD = _BV(3) |  // led1 IR
		   _BV(5);	 // led1 blue

	// set buttons to input (disable pullups)
	DDRB &= ~_BV(0);
	DDRC &= ~_BV(3);

	// Timer 2 (IR-led)
	TCCR2A = _BV(WGM21);  // Output-Mode: none
						  // Operation-Mode: CTC til OCRA
	TCCR2B = _BV(CS20);   // Operation-Mode: CTC til OCRA
						  // Prescaler: 1
	OCR2A = 210;		  // Top: 210 ==> 76KHz
	TIMSK2 = _BV(OCIE2A); // enable compare match interrupt
	//Timer 2 -> IR led basetakt(38KHz)
	TCCR2A	= 0
			 //|_BV(COM2A1)  // OC0A disconnected
			 //|_BV(COM2A0)  // OC0A disconnected
			 //|_BV(COM2B1)  // OC0B toggel mode
			 |_BV(COM2B0)	 // OC0B toggel mode
			 |_BV(WGM21)	 // Timer modus CTC top at OCR0A
			 //|_BV(WGM20)	 // =Mode 2
			 ;
	TCCR2B	= 0
			 //|_BV(WGM22)	 // Timer modus CTC top at OCR0A
			 //|_BV(CS22)	 // Speed = 38KHz
			 |_BV(CS21)		 // => 20MHz/2(half speed by toggeling)/38KHz~aller 263 takte
			 //|_BV(CS20)	 // => prescaler=8,OCRA=32=>37,87KHz
			 ;
	OCR2A	= 32;


	// startup time
	delay_ms(50);

	// variables
	// leds
	rgb_clearAll();
	ir_clearAll();

	// buttons
	buttons_reset();
	sei();

	ledbox_setup_module_count();
}

//**************************[ledbox_set_modul_count]**************************************
void ledbox_setup_module_count(void) {
	ledbox_count_current=LEDBOX_COUNT_MAX;
	ledbox_state=full_field;
}


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

//**************************[rgb_set2]*****************************************
void rgb_set2(uint8_t number, uint8_t r, uint8_t g, uint8_t b) {

	if (number >= LEDBOX_COUNT_MAX) {
		return;
	}

	ledbox_rgb[number].r = r;
	ledbox_rgb[number].g = g;
	ledbox_rgb[number].b = b;
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



//**************************[ir_set]*******************************************
void ir_set(uint8_t number, uint8_t x) {

	if (number >= LEDBOX_COUNT_MAX) {
		return;
	}

	// order is invers!
	ledbox_ir[LEDBOX_COUNT_MAX - number - 1].write = x ? 1 : 0;
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



//**************************[buttons_reset]************************************
void buttons_reset(void) {

	uint8_t i;

	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		ledbox_buttons[i].stateWrite= 0;
		ledbox_buttons[i].flankWrite= 0;
		ledbox_buttons[i].countdown = LEDBOX_BUTTONS_DEBOUNCE_TIME;
	}

	buttons_clear();
}

//**************************[buttons_clear]************************************
void buttons_clear(void) {

	uint8_t i;

	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		ledbox_buttons[i].flankRead =  0;
	}
}

//**************************[buttons_get]**************************************
uint8_t buttons_get(uint8_t number) {

	if (number >= LEDBOX_COUNT_MAX) {
		return 0x00;
	}

	uint8_t result = ledbox_buttons[number].flankRead;
	ledbox_buttons[number].flankRead = 0;

	return result;
}

//**************************[_ledbox_rgb_update]*******************************
void _ledbox_rgb_update(void) {


	static uint8_t X=0;
	X++;
	bus_rgb(0);

	led1_setRed(ledbox_rgb[firstNumber].r>X);
	led1_setGreen(ledbox_rgb[firstNumber].g>X);
	led1_setBlue(ledbox_rgb[firstNumber].b>X);

	led2_setRed(ledbox_rgb[secondNumber].r>X);
	led2_setGreen(ledbox_rgb[secondNumber].g>X);
	led2_setBlue(ledbox_rgb[secondNumber].b>X);
}

//**************************[_ledbox_buttons_and_ir_update]********************
void _ledbox_buttons_and_ir_update(void) {

	uint8_t state;

	static uint8_t global_button_countdown = 0;
	if (global_button_countdown){global_button_countdown--;}
	else{
		global_button_countdown=LEDBOX_BUTTONS_GLOBAL_TIME;
	}
 
	// set ir_led
	leds_setIr1(ledbox_ir[firstNumber].read);

	state = buttons_getBtn1();

	// check for changed state
	if (state != ledbox_buttons[firstNumber].stateWrite) {
		ledbox_buttons[firstNumber].stateWrite = state;

		// state did change
		if (ledbox_buttons[firstNumber].countdown == 0x00) {
			if (!state) {
				// button released
				ledbox_buttons[firstNumber].countdown = LEDBOX_BUTTONS_DEBOUNCE_TIME;
			} else {				 // pushed button and no countdown
				ledbox_buttons[firstNumber].flankWrite = 1;
			}
		}
	}

	if (!global_button_countdown){
		if (ledbox_buttons[firstNumber].countdown) {
			ledbox_buttons[firstNumber].countdown--;
		}
	}

	leds_setIr2(ledbox_ir[secondNumber].read);

	state = buttons_getBtn2();

	// check for changed state
	if (state != ledbox_buttons[secondNumber].stateWrite) {
		ledbox_buttons[secondNumber].stateWrite = state;

		// state did change
		if (ledbox_buttons[secondNumber].countdown == 0x00) {
			if (!state) {
				// button released
				ledbox_buttons[secondNumber].countdown = LEDBOX_BUTTONS_DEBOUNCE_TIME;
			} else {				 // pushed button and no countdown
				ledbox_buttons[secondNumber].flankWrite = 1;
			}
		}
	}

	if (!global_button_countdown){
		if (ledbox_buttons[secondNumber].countdown) {
			ledbox_buttons[secondNumber].countdown--;
		}
	}
}

void _ledbox_switchBuffer(){

	uint8_t number;
	for(number=0;number<LEDBOX_COUNT_MAX;number++){
		ledbox_ir[number].read = ledbox_ir[number].write;

		ledbox_buttons[number].flankRead |= ledbox_buttons[number].flankWrite;
		ledbox_buttons[number].flankWrite = 0;
		ledbox_buttons[number].stateRead |= ledbox_buttons[number].stateWrite;
	}
}


//**************************[toggle_clk]***************************************
void toggle_clk() {
}

//**************************[toggle_led_load]**********************************
void toggle_led_load() {
}
