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
#include "display.h"


//**************************<Macros>*******************************************
//times(ms)
#define DISPLAY_OFFTIME 0
#define DISPLAY_ONTIME 5

//display
#define DISP_ROW_SER_PORT PORTC
#define DISP_ROW_SER_DDR  DDRC
#define DISP_ROW_SER_PIN  _BV(0)
#define disp_row_ser(x)   (x ? (DISP_ROW_SER_PORT|= DISP_ROW_SER_PIN) : \
                          (DISP_ROW_SER_PORT&= ~DISP_ROW_SER_PIN))
#define _disp_row_ser_enable() (DISP_ROW_SER_DDR|= DISP_ROW_SER_PIN)

#define DISP_ROW_SCK_PORT PORTC
#define DISP_ROW_SCK_DDR  DDRC
#define DISP_ROW_SCK_PIN  _BV(1)
#define disp_row_sck(x)   (x ? (DISP_ROW_SCK_PORT|= DISP_ROW_SCK_PIN) : \
                          (DISP_ROW_SCK_PORT&= ~DISP_ROW_SCK_PIN))
#define _disp_row_sck_enable() (DISP_ROW_SCK_DDR|= DISP_ROW_SCK_PIN)

#define DISP_RCK_PORT     PORTC
#define DISP_RCK_DDR      DDRC
#define DISP_RCK_PIN      _BV(2)
#define disp_rck(x)       (x ? (DISP_RCK_PORT|= DISP_RCK_PIN) : \
                          (DISP_RCK_PORT&= ~DISP_RCK_PIN))
#define _disp_rck_enable() (DISP_RCK_DDR|= DISP_RCK_PIN)

#define DISP_ENABLE_PORT  PORTC
#define DISP_ENABLE_DDR   DDRC
#define DISP_ENABLE_PIN   _BV(3)
#define disp_enable(x)    (x ? (DISP_ENABLE_PORT|= DISP_ENABLE_PIN) : \
                          (DISP_ENABLE_PORT&= ~DISP_ENABLE_PIN))
#define _disp_enable_enable() (DISP_ENABLE_DDR|= DISP_ENABLE_PIN)

#define DISP_COL_SER_PORT PORTB
#define DISP_COL_SER_DDR  DDRB
#define DISP_COL_SER_PIN  _BV(3)
#define disp_col_ser(x)   (x ? (DISP_COL_SER_PORT|= DISP_COL_SER_PIN) : \
                          (DISP_COL_SER_PORT&= ~DISP_COL_SER_PIN))
#define _disp_col_ser_enable() (DISP_COL_SER_DDR|= DISP_COL_SER_PIN)

#define DISP_COL_SCK_PORT PORTB
#define DISP_COL_SCK_DDR  DDRB
#define DISP_COL_SCK_PIN  _BV(5)
#define disp_col_sck(x)   (x ? (DISP_COL_SCK_PORT|= DISP_COL_SCK_PIN) : \
                          (DISP_COL_SCK_PORT&= ~DISP_COL_SCK_PIN))
#define _disp_col_sck_enable() (DISP_COL_SCK_DDR|= DISP_COL_SCK_PIN)

//**************************<Types and Variables>******************************

//display
uint8_t display_segmentbuffer[SEGMENTS_COUNT][7];
uint8_t display_double_dot;
uint8_t display_current_row;



//**************************<Methods>******************************************

// clock and load
void toggle_col_sck(void);
void toggle_row_sck(void);
void toggle_rck(void);



//**************************[ledbox_init]**************************************
//**************************[init_display]*************************************
void init_display(void) {
    _disp_row_ser_enable();
    _disp_row_sck_enable();
    _disp_col_ser_enable();
    _disp_col_sck_enable();
    _disp_rck_enable();
    _disp_enable_enable();
    display_double_dot=0;
}
//**************************[display]******************************************
//**************************[display::seter]***********************************
void display_setSegment(const uint8_t *pict,uint8_t segmentnumber){
    if ((segmentnumber>=0)&&(segmentnumber<SEGMENTS_COUNT)){
        int8_t i;
        for(i=0;i<7;i++){
            display_segmentbuffer[segmentnumber][i]=pict[i]<<3;
        }
    }
}
void display_shiftright(uint8_t number){
    int8_t i;
    for(i=SEGMENTS_COUNT-1;i>=number;i--){
        int8_t j;
        for(j=0;j<7;j++){
            display_segmentbuffer[i][j]=display_segmentbuffer[i-number][j];
        }
    }
}
void display_shiftleft(uint8_t number){
    int8_t i;
    for(i=number;i<SEGMENTS_COUNT;i++){
        int8_t j;
        for(j=0;j<7;j++){
            display_segmentbuffer[i-number][j]=display_segmentbuffer[i][j];
        }
    }
}

//**************************[display::shower]**********************************
void display_fill_col(uint8_t row, uint8_t col, uint8_t seg){
    if ((display_segmentbuffer[seg][row]&_BV(col))){
        disp_col_ser(1);
    }else{
        disp_col_ser(0);
    }
    toggle_col_sck();
}
void  display_row_next(void){
    display_current_row++;
    if(display_current_row>=8){
        display_current_row=0;
        disp_row_ser(1);
    }else{
        disp_row_ser(0);
    }
    toggle_row_sck();
}
void display_row_reset(void){
    uint8_t select;
    display_current_row=0;
    for(select=0;select<=7;select++){
        if (select==7){
            disp_row_ser(1);
        } else {
            disp_row_ser(0);
        }
        toggle_row_sck();
    }
}
void display_fill_current_row(void){
    int8_t seg;
    if (display_current_row==7){
        //double dot is out -> dont show it by shift row out, next is anyhow newstart
        if(display_double_dot)toggle_rck();
    } else {
        for(seg=0;seg<SEGMENTS_COUNT;seg++){
            int8_t col;
            for(col=0;col<8;col++){
                display_fill_col(display_current_row,col,seg);
            }
        }
        toggle_rck();
    }
}
void display_show_ms(uint16_t ms){
    for(;ms>0;ms--){
        display_row_next();
        display_fill_current_row();

        delay_ms(1);
    }
}

void display_show(){

    display_row_next();
    display_fill_current_row();
}


//**************************[toggle_col_sck]***********************************
void toggle_col_sck() {

    // 1 MHz
    disp_col_sck(1);
    nop(); nop(); nop(); nop();
    nop(); nop(); nop(); nop(); nop();

    disp_col_sck(0);
    nop(); nop(); nop(); nop();
    nop(); nop(); nop(); nop(); nop();
}

//**************************[toggle_row_sck]***********************************
void toggle_row_sck() {

    // 1 MHz
    disp_row_sck(1);
    nop(); nop(); nop(); nop();
    nop(); nop(); nop(); nop(); nop();

    disp_row_sck(0);
    nop(); nop(); nop(); nop();
    nop(); nop(); nop(); nop(); nop();
}

//**************************[toggle_rck]***************************************
void toggle_rck() {

    // 1 MHz
    disp_rck(1);
    nop(); nop(); nop(); nop();
    nop(); nop(); nop(); nop(); nop();

    disp_rck(0);
    nop(); nop(); nop(); nop();
    nop(); nop(); nop(); nop(); nop();
}
