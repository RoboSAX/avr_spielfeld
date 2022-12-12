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
#include "../modulesInterface/ledbox.h"
#include "../modulesInterface/systick.h"


//**************************<Macros>*******************************************

// bus
#define BUS_BTN_DATA_PORT       PIND
#define BUS_BTN_DATA_DDR        DDRD
#define BUS_BTN_DATA_PIN        _BV(2)
#define bus_btn_data(x)         ((BUS_BTN_DATA_PORT & BUS_BTN_DATA_PIN) == 0x00)
#define _bus_btn_data_init()    (BUS_BTN_DATA_DDR&= ~BUS_BTN_DATA_PIN)

#define BUS_BTN_LOAD_PORT       PORTD
#define BUS_BTN_LOAD_DDR        DDRD
#define BUS_BTN_LOAD_PIN        _BV(3)
#define bus_btn_load(x)         (x ? (BUS_BTN_LOAD_PORT|= BUS_BTN_LOAD_PIN) : \
                                    (BUS_BTN_LOAD_PORT&= ~BUS_BTN_LOAD_PIN))
#define _bus_btn_load_enable()  (BUS_BTN_LOAD_DDR|= BUS_BTN_LOAD_PIN)


#define BUS_LED_DATA_PORT       PORTD
#define BUS_LED_DATA_DDR        DDRD
#define BUS_LED_DATA_PIN        _BV(4)
#define bus_led_data(x)         (x ? (BUS_LED_DATA_PORT|= BUS_LED_DATA_PIN) : \
                                    (BUS_LED_DATA_PORT&= ~BUS_LED_DATA_PIN))
#define _bus_led_data_enable()  (BUS_LED_DATA_DDR|= BUS_LED_DATA_PIN)

#define BUS_LED_LOAD_PORT       PORTD
#define BUS_LED_LOAD_DDR        DDRD
#define BUS_LED_LOAD_PIN        _BV(7)
#define bus_led_load(x)         (x ? (BUS_LED_LOAD_PORT|= BUS_LED_LOAD_PIN) : \
                                    (BUS_LED_LOAD_PORT&= ~BUS_LED_LOAD_PIN))
#define _bus_led_load_enable()  (BUS_LED_LOAD_DDR|= BUS_LED_LOAD_PIN)


#define BUS_IR_CLK_PORT         PORTD
#define BUS_IR_CLK_DDR          DDRD
#define BUS_IR_CLK_PIN          _BV(5)
#define bus_ir_clk(x)           (x ? (BUS_IR_CLK_PORT|= BUS_IR_CLK_PIN) : \
                                    (BUS_IR_CLK_PORT&= ~BUS_IR_CLK_PIN))
#define _bus_ir_clk_enable()    (BUS_IR_CLK_DDR|= BUS_IR_CLK_PIN)

#define BUS_CLK_PORT        PORTD
#define BUS_CLK_DDR         DDRD
#define BUS_CLK_PIN         _BV(6)
#define bus_clk(x)          (x ? (BUS_CLK_PORT|= BUS_CLK_PIN) : \
                                (BUS_CLK_PORT&= ~BUS_CLK_PIN))
#define _bus_clk_enable()   (BUS_CLK_DDR|= BUS_CLK_PIN)


#define BUS_RGB_PORT        PORTD
#define BUS_RGB_DDR         DDRD
#define BUS_RGB_PIN         _BV(1)
#define bus_rgb(x)          (x ? (BUS_RGB_PORT&= ~BUS_RGB_PIN) : \
                                (BUS_RGB_PORT|= BUS_RGB_PIN))
#define _bus_rgb_enable()   (BUS_RGB_DDR|= BUS_RGB_PIN)

#define RETURN_RGB_PORT     PIND
#define RETURN_RGB_DDR      DDRD
#define RETURN_RGB_PIN      _BV(0)
#define return_rgb(x)       (RETURN_RGB_PORT & RETURN_RGB_PIN)
#define _return_rgb_init()  (RETURN_RGB_DDR&= ~RETURN_RGB_PIN)

#define RETURN_BTN_PORT     PORTB
#define RETURN_BTN_DDR      DDRB
#define RETURN_BTN_PIN      _BV(0)
#define return_btn(x)       (x ? (RETURN_BTN_PORT|= RETURN_BTN_PIN) : \
                                (RETURN_BTN_PORT&= ~RETURN_BTN_PIN))
#define _return_btn_enable() (RETURN_BTN_DDR|= RETURN_BTN_PIN)


//#define ledbox_buttons_get ledbox_buttons[working_buffer]
//#define ledbox_buttons_set ledbox_buttons[display_buffer]

//#define ledbox_ir_get ledbox_ir[working_buffer]
//#define ledbox_ir_set ledbox_ir[display_buffer]

//#define ledbox_ir_get ledbox_ir[working_buffer]
//#define ledbox_ir_set ledbox_ir[display_buffer]
//**************************<Types and Variables>******************************

// buttons
struct sButtonState {
    uint8_t stateWrite: 1; ///< boolean
    uint8_t flankWrite: 1; ///< boolean
    uint8_t flankRead : 1; ///< boolean
    uint8_t countdown : 5; ///< 0..31 (* 1ms)
};
volatile struct sButtonState ledbox_buttons[LEDBOX_COUNT_MAX];

// rgb leds
struct sRGB {
    uint8_t r; ///< red
    uint8_t g; ///< green
    uint8_t b; ///< blue
    uint8_t dataOut[8];///< data for the USART
};
volatile struct sRGB ledbox_rgb[LEDBOX_COUNT_MAX];

// ir leds (order is invers!)
struct sIRLed{
	uint8_t read  : 1;
	uint8_t write : 1;
	uint8_t empty : 6;
};
volatile struct sIRLed ledbox_ir[LEDBOX_COUNT_MAX];
volatile enum eUpdate needsUpdate; 

//**************************<Methods>******************************************

// clock and load
void toggle_clk(void);
void toggle_led_load(void);


//**************************[ledbox_init]**************************************
void ledbox_init(void) {

	// hardware
	// bus
	_bus_btn_data_init();
	_bus_btn_load_enable(); bus_btn_load(0);

	_bus_led_data_enable(); bus_led_data(0);
	_bus_led_load_enable(); bus_led_load(0);

	_bus_ir_clk_enable();	bus_ir_clk(0);
	_bus_clk_enable();		bus_clk(0);

	_bus_rgb_enable();		bus_rgb(0);
	_return_rgb_init();
	_return_btn_enable();	return_btn(0);

	// uart
	UCSR0A	= _BV(TXC0 ) |	 // clear TX-Complete-Flag
			  _BV(U2X0 );	 // enable double speed
			  //_BV(MPCM0)	 // no multi-processor communication mode
	UCSR0B	= _BV(TXEN0);	 // enable transmitter
			  //_BV(RXCIE)	 // no RX-Complete-Interrupt
			  //_BV(UDRIE)	 // no Date-Register-Empty-Interrupt
			  //_BV(TXCIE)	 // no TX-Complete-Interrupt
			  //_BV(RXEN0)	 // disable receiver
			  //_BV(UCSZ02)  // 7 bits in UART frame (UCSZ0x = 0x010)
			  //_BV(TXB80)	 // 9th data bit for transmission
	UCSR0C	= _BV(UCSZ01);	 // 7 bits in UART frame (UCSZ0x = 0x010)
			  //_BV(UMSEL01) // asynchronous mode (UMSEL0x = 0x00)
			  //_BV(UMSEL00) // asynchronous mode (UMSEL0x = 0x00)
			  //_BV(UPM01)	 // no parity check (UPM0x = 0x00)
			  //_BV(UPM00)	 // no parity check (UPM0x = 0x00)
			  //_BV(USBS0)	 // 1 stop bit
			  //_BV(UCSZ00)  // 7 bits in UART frame (UCSZ0x = 0x010)
			  //_BV(UCPOL0)  // clock polarity (not used)

	UBRR0H = 0; // set baudrate to F_CPU
	UBRR0L = 0; // ==> with double speed this will be 2.5 MHz

	DDRD|= _BV(1); // activate strong drivers for TX

	//Timer 0 -> IR led basetakt(38KHz)
	TCCR0A	= 0
			 //|_BV(COM0A1)  // OC0A disconnected
			 //|_BV(COM0A0)  // OC0A disconnected
			 //|_BV(COM0B1)  // OC0B toggel mode
			 |_BV(COM0B0)	 // OC0B toggel mode
			 |_BV(WGM01)	 // Timer modus CTC top at OCR0A
			 //|_BV(WGM00)	 // =Mode 2
			 ;
	TCCR0B	= 0
			 //|_BV(WGM02)	 // Timer modus CTC top at OCR0A
			 //|_BV(CS02)	 // Speed = 38KHz
			 |_BV(CS01)		 // => 20MHz/2(half speed by toggeling)/38KHz~aller 263 takte
			 //|_BV(CS00)	 // => prescaler=8,OCRA=32=>37,87KHz
			 ;
	OCR0A	= 32;

	// variables
	// leds
	rgb_clearAll();
	ir_clearAll();

	// buttons
	buttons_reset();
	sei();

	ledbox_setup_module_count();

	needsUpdate=update_all;
}

//**************************[ledbox_set_modul_count]**************************************
void ledbox_setup_module_count(void) {

	// load button values
	return_btn(0);

	bus_btn_load(1);
	delay_us(100);

	uint8_t i;
	uint8_t state;

	//send all 1s
	for (i = 0; i < LEDBOX_COUNT_MAX*10; i++) {

		// set button
		return_btn(1);

		// read current state
		state = bus_btn_data();
		bus_btn_load(0);

		// toggle clock
		toggle_clk();
	}

	//recieve 1
	//send 01010??
	//count till recieve 01010
	//set count as moudule count
	ledbox_count_current=0;
	while(!state) {

		// send 0 button
		return_btn(0);

		// toggle clock
		toggle_clk();

		ledbox_count_current++;
		if(ledbox_count_current>LEDBOX_COUNT_MAX)
			break;

		// read current state
		state = bus_btn_data();
		bus_btn_load(0);
	}
	
	if(ledbox_count_current==LEDBOX_COUNT_MAX)
		ledbox_state=full_field;
	else if(ledbox_count_current==LEDBOX_COUNT_MAX / 2)
		ledbox_state=half_field;
	else
		ledbox_state=unknown_field;
	if(ledbox_count_current>LEDBOX_COUNT_MAX)
		ledbox_count_current=LEDBOX_COUNT_MAX;
}

//**************************[rgb_set2]*****************************************
void rgb_set2(uint8_t number, uint8_t r, uint8_t g, uint8_t b) {

	if (number >= LEDBOX_COUNT_MAX) {
		return;
	}

	ledbox_rgb[number].r = r;
	ledbox_rgb[number].g = g;
	ledbox_rgb[number].b = b;

	needsUpdate|=update_RGBLeds;
}

//**************************[ir_set]*******************************************
void ir_set(uint8_t number, uint8_t x) {

	if (number >= ledbox_count_current) {
		return;
	}

	// order is invers!
	ledbox_ir[ledbox_count_current - number - 1].write = x ? 1 : 0;
	needsUpdate|=update_buttons;
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


	bus_rgb(0);

	// transmit data
	cli();
	uint8_t i,j;
	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
		for (j = 0; j < 8; j++) {
			while ((UCSR0A & _BV(UDRE0)) == 0) {
				nop();
			}
			UDR0 = ledbox_rgb[i].dataOut[j];
		}
	}
	sei();

}

//**************************[_ledbox_buttons_and_ir_update]********************
void _ledbox_buttons_and_ir_update(void) {

	// load button values
	return_btn(0);

	bus_btn_load(1);
	delay_us(1);

	uint8_t i;
	uint8_t state;

	static uint8_t global_button_countdown = 0;
	if (global_button_countdown){global_button_countdown--;}
	else{
		global_button_countdown=LEDBOX_BUTTONS_GLOBAL_TIME;
	}
 
	for (i = 0; i < LEDBOX_COUNT_MAX; i++) {

		// set ir_led
		bus_led_data(ledbox_ir[i].read);

		// toggle clock
		toggle_clk();
		// read current state
		state = bus_btn_data();
		bus_btn_load(0);

		// check for changed state
		if (state != ledbox_buttons[i].stateWrite) {
			ledbox_buttons[i].stateWrite = state;

			// state did change
			if (ledbox_buttons[i].countdown == 0x00) {
				if (!state) {
					// button released
					ledbox_buttons[i].countdown = LEDBOX_BUTTONS_DEBOUNCE_TIME;
				} else {				 // pushed button and no countdown
					ledbox_buttons[i].flankWrite = 1;
					needsUpdate|=update_buttons;
				}
			}
			continue;
		}

		if (!global_button_countdown){
			if (ledbox_buttons[i].countdown) {
				ledbox_buttons[i].countdown--;
			}
		}
	}
	toggle_led_load();
}

void _ledbox_switchBuffer(){

	if (needsUpdate & update_buttons) if(systick_freezUpdate(update_buttons)){
		uint8_t number;
		for(number=0;number<LEDBOX_COUNT_MAX;number++){
			ledbox_ir[number].read = ledbox_ir[number].write;

			ledbox_buttons[number].flankRead |= ledbox_buttons[number].flankWrite;
			ledbox_buttons[number].flankWrite = 0;
		}
		needsUpdate&=~update_buttons;
		systick_unFreezUpdate(update_buttons);
	}
	if (needsUpdate & update_RGBLeds) if(systick_freezUpdate(update_RGBLeds)){
		uint8_t number;
		for(number=0;number<LEDBOX_COUNT_MAX;number++){

			uint8_t j;

			// init data bytes
			// set all needed "1" (which will be "0" on the bus)
			for (j = 0; j < 8; j++){
				ledbox_rgb[number].dataOut[j] = 0x12;
				// S = 0  ==> 1 (high		part of databit 1)-
				// 0 = x  ==> !x(adjustable part of databit 1)?
				// 1 = 1  ==> 0 (low		part of databit 1)0x02
				// 2 = 0  ==> 1 (high		part of databit 2)0
				// 3 = x  ==> !x(adjustable part of databit 2)?
				// 4 = 1  ==> 0 (low		part of databit 2)0x10
				// 5 = 0  ==> 1 (high		part of databit 3)0
				// 6 = x  ==> !x(adjustable part of databit 3)?
				// P = 1  ==> 0 (low		part of databit 3)-
			}

			// load needed bits
			uint8_t d;

			d = ledbox_rgb[number].g;

			if ((d & 0x80) == 0x00) {ledbox_rgb[number].dataOut[0]|= 0x01;}
			if ((d & 0x40) == 0x00) {ledbox_rgb[number].dataOut[0]|= 0x08;}
			if ((d & 0x20) == 0x00) {ledbox_rgb[number].dataOut[0]|= 0x40;}

			if ((d & 0x10) == 0x00) {ledbox_rgb[number].dataOut[1]|= 0x01;}
			if ((d & 0x08) == 0x00) {ledbox_rgb[number].dataOut[1]|= 0x08;}
			if ((d & 0x04) == 0x00) {ledbox_rgb[number].dataOut[1]|= 0x40;}

			if ((d & 0x02) == 0x00) {ledbox_rgb[number].dataOut[2]|= 0x01;}
			if ((d & 0x01) == 0x00) {ledbox_rgb[number].dataOut[2]|= 0x08;}
			d = ledbox_rgb[number].r;
			if ((d & 0x80) == 0x00) {ledbox_rgb[number].dataOut[2]|= 0x40;}

			if ((d & 0x40) == 0x00) {ledbox_rgb[number].dataOut[3]|= 0x01;}
			if ((d & 0x20) == 0x00) {ledbox_rgb[number].dataOut[3]|= 0x08;}
			if ((d & 0x10) == 0x00) {ledbox_rgb[number].dataOut[3]|= 0x40;}

			if ((d & 0x08) == 0x00) {ledbox_rgb[number].dataOut[4]|= 0x01;}
			if ((d & 0x04) == 0x00) {ledbox_rgb[number].dataOut[4]|= 0x08;}
			if ((d & 0x02) == 0x00) {ledbox_rgb[number].dataOut[4]|= 0x40;}

			if ((d & 0x01) == 0x00) {ledbox_rgb[number].dataOut[5]|= 0x01;}
			d = ledbox_rgb[number].b;
			if ((d & 0x80) == 0x00) {ledbox_rgb[number].dataOut[5]|= 0x08;}
			if ((d & 0x40) == 0x00) {ledbox_rgb[number].dataOut[5]|= 0x40;}

			if ((d & 0x20) == 0x00) {ledbox_rgb[number].dataOut[6]|= 0x01;}
			if ((d & 0x10) == 0x00) {ledbox_rgb[number].dataOut[6]|= 0x08;}
			if ((d & 0x08) == 0x00) {ledbox_rgb[number].dataOut[6]|= 0x40;}

			if ((d & 0x04) == 0x00) {ledbox_rgb[number].dataOut[7]|= 0x01;}
			if ((d & 0x02) == 0x00) {ledbox_rgb[number].dataOut[7]|= 0x08;}
			if ((d & 0x01) == 0x00) {ledbox_rgb[number].dataOut[7]|= 0x40;}
		}
		needsUpdate&=~update_RGBLeds;
		systick_unFreezUpdate(update_RGBLeds);
	}
}


//**************************[toggle_clk]***************************************
void toggle_clk() {

	// 1 MHz
	bus_clk(1);
	nop(); nop(); nop(); nop();
	nop(); nop(); nop(); nop(); nop();

	bus_clk(0);
	nop(); nop(); nop(); nop();
	nop(); nop(); nop(); nop(); nop();
}

//**************************[toggle_led_load]**********************************
void toggle_led_load() {

	// 1 MHz
	bus_led_load(1);
	nop(); nop(); nop(); nop();
	nop(); nop(); nop(); nop(); nop();

	bus_led_load(0);
	nop(); nop(); nop(); nop();
	nop(); nop(); nop(); nop(); nop();
}
