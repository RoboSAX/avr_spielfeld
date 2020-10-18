/******************************************************************************
* display.c                                                                   *
* =========                                                                   *
*                                                                             *
* Version: 1.1.0                                                              *
* Date   : 27.12.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "../modulesInterface/display.h"


//**************************<Types and Variables>******************************
uint8_t display_double_dot;
//**************************<Macros>*******************************************
//times(ms)
//**************************[ledbox_init]**************************************
//**************************[init_display]*************************************
void init_display(void) {
}
//**************************[display]******************************************
//**************************[display::seter]***********************************
void display_setSegment(const uint8_t *pict,uint8_t segmentnumber){
}

void display_invertSegment(uint8_t segmentnumber){
}
 
void display_setSuperSegment(const uint16_t *pict,uint8_t supersegmentnumber){
}

void display_clearSuperSegment(uint8_t supersegmentnumber){
}

void display_invertSuperSegment(uint8_t supersegmentnumber){
}
void display_addLeft(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse){
}

void display_addRight(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse){
}

//**************************[display::shower]**********************************
void display_fill_col(uint8_t row, uint8_t col, uint8_t seg){
}
void display_row_next(void){
}
void display_row_reset(void){
}
void display_fill_current_row(void){
}
void display_show_ms(uint16_t ms){
}

void display_show(){
}

void display_switchBuffer(){
}
