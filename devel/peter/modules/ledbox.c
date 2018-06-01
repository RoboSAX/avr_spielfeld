/******************************************************************************
* ledbox.c                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "ledbox.h"


//**************************<Macros>*******************************************

// bus
#define BUS_BTN_DATA_PORT PIND
#define BUS_BTN_DATA_DDR  DDRD
#define BUS_BTN_DATA_PIN  _BV(2)
#define bus_btn_data(x)   ((BUS_BTN_DATA_PORT & BUS_BTN_DATA_PIN) == 0x00)
#define _bus_btn_data_init() (BUS_BTN_DATA_DDR&= ~BUS_BTN_DATA_PIN)

#define BUS_BTN_LOAD_PORT PORTD
#define BUS_BTN_LOAD_DDR  DDRD
#define BUS_BTN_LOAD_PIN  _BV(3)
#define bus_btn_load(x)   (x ? (BUS_BTN_LOAD_PORT|= BUS_BTN_LOAD_PIN) : \
                          (BUS_BTN_LOAD_PORT&= ~BUS_BTN_LOAD_PIN))
#define _bus_btn_load_enable() (BUS_BTN_LOAD_DDR|= BUS_BTN_LOAD_PIN)


#define BUS_LED_DATA_PORT PORTD
#define BUS_LED_DATA_DDR  DDRD
#define BUS_LED_DATA_PIN  _BV(4)
#define bus_led_data(x)   (x ? (BUS_LED_DATA_PORT|= BUS_LED_DATA_PIN) : \
                          (BUS_LED_DATA_PORT&= ~BUS_LED_DATA_PIN))
#define _bus_led_data_enable() (BUS_LED_DATA_DDR|= BUS_LED_DATA_PIN)

#define BUS_LED_LOAD_PORT PORTD
#define BUS_LED_LOAD_DDR  DDRD
#define BUS_LED_LOAD_PIN  _BV(7)
#define bus_led_load(x)   (x ? (BUS_LED_LOAD_PORT|= BUS_LED_LOAD_PIN) : \
                          (BUS_LED_LOAD_PORT&= ~BUS_LED_LOAD_PIN))
#define _bus_led_load_enable() (BUS_LED_LOAD_DDR|= BUS_LED_LOAD_PIN)


#define BUS_IR_CLK_PORT   PORTD
#define BUS_IR_CLK_DDR    DDRD
#define BUS_IR_CLK_PIN    _BV(5)
#define bus_ir_clk(x)     (x ? (BUS_IR_CLK_PORT|= BUS_IR_CLK_PIN) : \
                          (BUS_IR_CLK_PORT&= ~BUS_IR_CLK_PIN))
#define _bus_ir_clk_enable() (BUS_IR_CLK_DDR|= BUS_IR_CLK_PIN)

#define BUS_CLK_PORT      PORTD
#define BUS_CLK_DDR       DDRD
#define BUS_CLK_PIN       _BV(6)
#define bus_clk(x)        (x ? (BUS_CLK_PORT|= BUS_CLK_PIN) : \
                          (BUS_CLK_PORT&= ~BUS_CLK_PIN))
#define _bus_clk_enable() (BUS_CLK_DDR|= BUS_CLK_PIN)


#define BUS_RGB_PORT      PORTD
#define BUS_RGB_DDR       DDRD
#define BUS_RGB_PIN       _BV(1)
#define bus_rgb(x)        (x ? (BUS_RGB_PORT&= ~BUS_RGB_PIN) : \
                          (BUS_RGB_PORT|= BUS_RGB_PIN))
#define _bus_rgb_enable() (BUS_RGB_DDR|= BUS_RGB_PIN)

#define RETURN_RGB_PORT   PIND
#define RETURN_RGB_DDR    DDRD
#define RETURN_RGB_PIN    _BV(0)
#define return_rgb(x)     (RETURN_RGB_PORT & RETURN_RGB_PIN)
#define _return_rgb_init() (RETURN_RGB_DDR&= ~RETURN_RGB_PIN)

#define RETURN_BTN_PORT   PORTB
#define RETURN_BTN_DDR    DDRB
#define RETURN_BTN_PIN    _BV(0)
#define return_btn(x)     (x ? (RETURN_BTN_PORT|= RETURN_BTN_PIN) : \
                          (RETURN_BTN_PORT&= ~RETURN_BTN_PIN))
#define _return_btn_enable() (RETURN_BTN_DDR|= RETURN_BTN_PIN)


//**************************<Types and Variables>******************************

// buttons
volatile struct sButtonState ledbox_buttons[LEDBOX_COUNT_MAX];

// rgb leds
volatile struct sRGB ledbox_rgb[LEDBOX_COUNT_MAX];
uint8_t ledbox_rgb_data[LEDBOX_COUNT_MAX * 8];

// ir leds (order is invers!)
volatile uint8_t ledbox_ir[LEDBOX_COUNT_MAX];



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

        _bus_ir_clk_enable();   bus_ir_clk(0);
        _bus_clk_enable();      bus_clk(0);

        _bus_rgb_enable();      bus_rgb(0);
        _return_rgb_init();
        _return_btn_enable();   return_btn(0);

        // uart
        UCSR0A  = _BV(TXC0 ) |   // clear TX-Complete-Flag
                  _BV(U2X0 );    // enable double speed
                  //_BV(MPCM0)   // no multi-processor communication mode
        UCSR0B  = _BV(TXEN0);    // enable transmitter
                  //_BV(RXCIE)   // no RX-Complete-Interrupt
                  //_BV(UDRIE)   // no Date-Register-Empty-Interrupt
                  //_BV(TXCIE)   // no TX-Complete-Interrupt
                  //_BV(RXEN0)   // disable receiver
                  //_BV(UCSZ02)  // 7 bits in UART frame (UCSZ0x = 0x010)
                  //_BV(TXB80)   // 9th data bit for transmission
        UCSR0C  = _BV(UCSZ01);  // 7 bits in UART frame (UCSZ0x = 0x010)
                  //_BV(UMSEL01) // asynchronous mode (UMSEL0x = 0x00)
                  //_BV(UMSEL00) // asynchronous mode (UMSEL0x = 0x00)
                  //_BV(UPM01)   // no parity check (UPM0x = 0x00)
                  //_BV(UPM00)   // no parity check (UPM0x = 0x00)
                  //_BV(USBS0)   // 1 stop bit
                  //_BV(UCSZ00)  // 7 bits in UART frame (UCSZ0x = 0x010)
                  //_BV(UCPOL0)  // clock polarity (not used)

        UBRR0H = 0; // set baudrate to F_CPU
        UBRR0L = 0; // ==> with double speed this will be 2.5 MHz

        DDRD|= _BV(1); // activate strong drivers for TX

    // variables
        // leds
        rgb_clearAll();
        ir_clearAll();

        // buttons
        buttons_reset();
}



//**************************[rgb_set]******************************************
void rgb_set(uint8_t number, enum eColor color) {

    if (number >= LEDBOX_COUNT_MAX) {
        return;
    }

    switch (color) {
        case clBlack : rgb_set2(number,   0,   0,   0); break;
        case clRed   : rgb_set2(number, 255,   0,   0); break;
        case clGreen : rgb_set2(number,   0, 255,   0); break;
        case clYellow: rgb_set2(number, 255, 255,   0); break;
        case clBlue  : rgb_set2(number,   0,   0, 255); break;
        case clPurple: rgb_set2(number, 255,   0, 255); break;
        case clCyan  : rgb_set2(number,   0, 255, 255); break;
        case clWhite : rgb_set2(number, 255, 255, 255); break;
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



//**************************[ir_clearAll]**************************************
void ir_set(uint8_t number, uint8_t x) {

    if (number >= LEDBOX_COUNT_MAX) {
        return;
    }

    // order is invers!
    ledbox_ir[LEDBOX_COUNT_MAX - number - 1] = x;
}

//**************************[ir_clearAll]**************************************
void ir_setAll(uint8_t x) {

    uint8_t i;

    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        ir_set(i, x);
    }
}

//**************************[ir_clearAll]**************************************
void ir_clearAll(void) {

    uint8_t i;

    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        ir_set(i, 0x00);
    }
}



//**************************[buttons_reset]************************************
void buttons_reset(void) {

    uint8_t i;

    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        ledbox_buttons[i].state     = 0;
        ledbox_buttons[i].countdown = LEDBOX_BUTTONS_DEBOUNCE_TIME;
    }

    buttons_clear();
}

//**************************[buttons_clear]************************************
void buttons_clear(void) {

    uint8_t i;

    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        ledbox_buttons[i].flank =  0;
    }
}

//**************************[buttons_get]**************************************
uint8_t buttons_get(uint8_t number) {

    if (number >= LEDBOX_COUNT_MAX) {
        return 0x00;
    }

    uint8_t result = ledbox_buttons[number].flank;
    ledbox_buttons[number].flank = 0;

    return result;
}

//**************************[_ledbox_rgb_update]*******************************
void _ledbox_rgb_update(void) {


    bus_rgb(0);


    uint8_t i, j;

    // init data bytes
    // set all needed "1" (which will be "0" on the bus)
    for (j = 0; j < LEDBOX_COUNT_MAX * 8; j++){
        ledbox_rgb_data[j] = 0x12;
        // S = 0  ==> 1 (high       part of databit 1)
        // 0 = x  ==> !x(adjustable part of databit 1)
        // 1 = 1  ==> 0 (low        part of databit 1)
        // 2 = 0  ==> 1 (high       part of databit 2)
        // 3 = x  ==> !x(adjustable part of databit 2)
        // 4 = 1  ==> 0 (low        part of databit 2)
        // 5 = 0  ==> 1 (high       part of databit 3)
        // 6 = x  ==> !x(adjustable part of databit 3)
        // P = 1  ==> 0 (low        part of databit 3)
    }

    // load needed bits
    for (i = 0, j = 0; i < 1; i++) {
        uint8_t d;

        d = ledbox_rgb[i].g;
        if ((d & 0x80) == 0x00) {ledbox_rgb_data[j]|= 0x01;}
        if ((d & 0x40) == 0x00) {ledbox_rgb_data[j]|= 0x08;}
        if ((d & 0x20) == 0x00) {ledbox_rgb_data[j]|= 0x40;}

        j++;
        if ((d & 0x10) == 0x00) {ledbox_rgb_data[j]|= 0x01;}
        if ((d & 0x08) == 0x00) {ledbox_rgb_data[j]|= 0x08;}
        if ((d & 0x04) == 0x00) {ledbox_rgb_data[j]|= 0x40;}

        j++;
        if ((d & 0x02) == 0x00) {ledbox_rgb_data[j]|= 0x01;}
        if ((d & 0x01) == 0x00) {ledbox_rgb_data[j]|= 0x08;}
        d = ledbox_rgb[i].r;
        if ((d & 0x80) == 0x00) {ledbox_rgb_data[j]|= 0x40;}

        j++;
        if ((d & 0x40) == 0x00) {ledbox_rgb_data[j]|= 0x01;}
        if ((d & 0x20) == 0x00) {ledbox_rgb_data[j]|= 0x08;}
        if ((d & 0x10) == 0x00) {ledbox_rgb_data[j]|= 0x40;}

        j++;
        if ((d & 0x08) == 0x00) {ledbox_rgb_data[j]|= 0x01;}
        if ((d & 0x04) == 0x00) {ledbox_rgb_data[j]|= 0x08;}
        if ((d & 0x02) == 0x00) {ledbox_rgb_data[j]|= 0x40;}

        j++;
        if ((d & 0x01) == 0x00) {ledbox_rgb_data[j]|= 0x01;}
        d = ledbox_rgb[i].b;
        if ((d & 0x80) == 0x00) {ledbox_rgb_data[j]|= 0x08;}
        if ((d & 0x40) == 0x00) {ledbox_rgb_data[j]|= 0x40;}

        j++;
        if ((d & 0x20) == 0x00) {ledbox_rgb_data[j]|= 0x01;}
        if ((d & 0x10) == 0x00) {ledbox_rgb_data[j]|= 0x08;}
        if ((d & 0x08) == 0x00) {ledbox_rgb_data[j]|= 0x40;}

        j++;
        if ((d & 0x04) == 0x00) {ledbox_rgb_data[j]|= 0x01;}
        if ((d & 0x02) == 0x00) {ledbox_rgb_data[j]|= 0x08;}
        if ((d & 0x01) == 0x00) {ledbox_rgb_data[j]|= 0x40;}
    }

    // transmit data
    cli();
    for (j = 0; j < LEDBOX_COUNT_MAX * 8; j++) {
        while ((UCSR0A & _BV(UDRE0)) == 0) {
            nop();
        }
        UDR0 = ledbox_rgb_data[j];
    }
    sei();

}

//**************************[_ledbox_buttons_and_ir_update]********************
void _ledbox_buttons_and_ir_update(void) {

    // load button values
    return_btn(0);

    bus_btn_load(1);
    delay_us(1);

    // debugging - turn on ir-ledbox
    bus_ir_clk(1);

    uint8_t i;
    uint8_t state;

    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {

        // set ir_led
        bus_led_data(ledbox_ir[i]);

        // toggle clock
        toggle_clk();
        // read current state
        state = bus_btn_data();
        bus_btn_load(0);

        // check for changed state
        if (state != ledbox_buttons[i].state) {
            ledbox_buttons[i].state = state;

            // state did change
            if (!state) {
                // button released
                ledbox_buttons[i].countdown = LEDBOX_BUTTONS_DEBOUNCE_TIME;
            } else if (ledbox_buttons[i].countdown == 0x00) {
                // pushed button and no countdown
                ledbox_buttons[i].flank = 1;
            }
            continue;
        }

        if (ledbox_buttons[i].countdown) {
            ledbox_buttons[i].countdown--;
        }
    }
    toggle_led_load();
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
