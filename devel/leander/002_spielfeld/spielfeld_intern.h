/******************************************************************************
* spielfeld/tht/spielfeld_intern.h                                              *
* ==============================                                              *
*                                                                             *
* Version: 1.0.2                                                              *
* Date   : 09.02.18                                                           *
* Author : Peter Weissig, Tina Lenk, Leander Herr                             *
*                                                                             *
******************************************************************************/

//done:
//display?

//todo:
//!BUS
//!INTERUPTS


#ifndef __SPIELFELD_INTERN_H
#define __SPIELFELD_INTERN_H

#define segments_count 3
#define bus_count 3
//times(ms)
#define display_offtime 25
#define display_ontime 25

//**************************<Included files>***********************************
#include <avr/io.h>
#include <inttypes.h>
#include "spielfeld.h"

//**************************<PORTDEF>*******************************************
//display
#define disp_row_ser_port PORTC
#define disp_row_ser_ddr DDRC
#define disp_row_ser_pin _BV(0)
#define disp_row_sck_port PORTC
#define disp_row_sck_ddr DDRC
#define disp_row_sck_pin _BV(1)
#define disp_rck_port PORTC
#define disp_rck_ddr DDRC
#define disp_rck_pin _BV(2)
#define disp_enable_port PORTC
#define disp_enable_ddr DDRC
#define disp_enable_pin _BV(3)
#define disp_col_ser_port PORTB
#define disp_col_ser_ddr DDRB
#define disp_col_ser_pin _BV(3)
#define disp_col_sck_port PORTB
#define disp_col_sck_ddr DDRB
#define disp_col_sck_pin _BV(5)

//leds
#define led1_port PORTD
#define led1_ddr DDRD
#define led1_pin _BV(1)
#define led2_port PORTD
#define led2_ddr DDRD
#define led2_pin _BV(4)

//buttons
#define button1_port PORTC
#define button1_ddr DDRC
#define button1_pin _BV(4)
#define button2_port PORTC
#define button2_ddr DDRC
#define button2_pin _BV(5)
#define button3_port PORTB
#define button3_ddr DDRB
#define button3_pin _BV(2)

//bus
#define bus_btn_data_port PORTD
#define bus_btn_data_ddr DDRD
#define bus_btn_data_pin _BV(2)
#define bus_btn_load_port PORTD
#define bus_btn_load_ddr DDRD
#define bus_btn_load_pin _BV(3)

#define bus_led_data_port PORTD
#define bus_led_data_ddr DDRD
#define bus_led_data_pin _BV(4)
#define bus_led_load_port PORTD
#define bus_led_load_ddr DDRD
#define bus_led_load_pin _BV(7)

#define bus_ir_clk_port PORTD
#define bus_ir_clk_ddr DDRD
#define bus_ir_clk_pin _BV(5)
#define bus_clk_port PORTD
#define bus_clk_ddr DDRD
#define bus_clk_pin _BV(6)

#define bus_rgb_port PORTD
#define bus_rgb_ddr DDRD
#define bus_rgb_pin _BV(1)

#define return_rgb_port PORTD
#define return_rgb_ddr DDRD
#define return_rgb_pin _BV(0)
#define return_btn_port PORTB
#define return_btn_ddr DDRB
#define return_btn_pin _BV(0)


//**************************<Macros>*******************************************
#define led1_set(x)   ( x ? (led1_port|= led1_pin) : (led1_port&= ~led1_pin))
#define led2_set(x)   ( x ? (led2_port|= led2_pin) : (led2_port&= ~led2_pin))

#define led1_setRed(x)   ( x ? (PORTB|= _BV(1)) : (PORTB&= ~_BV(1)))
#define led1_setGreen(x) ( x ? (PORTB|= _BV(2)) : (PORTB&= ~_BV(2)))
#define led1_setBlue(x)  ( x ? (PORTD|= _BV(5)) : (PORTD&= ~_BV(5)))

#define led1_readRed()   ((PORTB & _BV(1)) != 0x00)
#define led1_readGreen() ((PORTB & _BV(2)) != 0x00)
#define led1_readBlue()  ((PORTD & _BV(5)) != 0x00)

#define led2_setRed(x)   ( x ? (PORTC|= _BV(0)) : (PORTC&= ~_BV(0)))
#define led2_setGreen(x) ( x ? (PORTC|= _BV(1)) : (PORTC&= ~_BV(1)))
#define led2_setBlue(x)  ( x ? (PORTC|= _BV(2)) : (PORTC&= ~_BV(2)))

#define led2_readRed()   ((PORTC & _BV(0)) != 0x00)
#define led2_readGreen() ((PORTC & _BV(1)) != 0x00)
#define led2_readBlue()  ((PORTC & _BV(2)) != 0x00)

//**************************<Types and Variables>******************************
//bus
struct sButtonState {
    uint8_t state    : 1;

    uint8_t flank    : 1;

    uint8_t countdown   ;
};
volatile struct sButtonState bus_buttons[bus_count],board_buttons[3];

struct sIrLeds {
    uint8_t state    : 1;
};
volatile struct sIrLeds bus_ir_leds[bus_count];

struct sRGBLeds {
    //24bit*3einheinten[=72}
    //72/9bit(7bitmode+start/stop) uart=8byte
//    uint8_t R;
//    uint8_t B;
//    uint8_t G;
    uint8_t bytes[8];
};
volatile struct sRGBLeds bus_rgb_leds[bus_count];

uint8_t bus_button_ir_counter;
uint8_t bus_elements=bus_count;

//display
uint8_t display_segmentbuffer[segments_count][7];
uint8_t display_double_dot;
uint8_t display_current_row;


//**************************<Prototypes>***************************************

void leds_clearAll(void);
void leds_initTest(void);

void led1_setIr(uint8_t state);
void led2_setIr(uint8_t state);

//buttons
void buttons_reset(void);
void buttons_clearAll(void);
uint8_t bus_button_readFlank(uint8_t number);
uint8_t board_button_readFlank(uint8_t number);

void delay_licht(uint16_t mseconds);


void init_hardware(void);

//other

void board_read_buttons(void);

//BUS

void bus_read_buttons(void);
void bus_send_IR(void);
void bus_send_RGB(void);
void bus_activate_ir_buttons(void);
void bus_itteration_ir_buttons(void);

//display
//display_setSegment: pict: 7 columns of 5(last) bits,segmentnumber:0(out),1(out),2(out),3(niu),..
/*bsp:([
1* 00011111,
2* 00010101,
3* 00011111,
4* 00011011,
5* 00001110,
6* 00010101,
7* 00011011],0)
 */
void display_setSegment(uint8_t *pict,uint8_t segmentnumber);
void display_shiftright(uint8_t number);
void display_shiftleft(uint8_t number);

void  display_fill_col(uint8_t row, uint8_t col, uint8_t seg);
void  display_row_reset(void);
void  display_row_next(void);
void display_fill_current_row(void);
void display_show(void);

#endif //#ifndef __SPIELFELD_INTERN_H
