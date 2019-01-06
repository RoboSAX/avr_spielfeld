/******************************************************************************
* spielfeld/tht/spielfeld_intern.c                                              *
* ==============================                                              *
*                                                                             *
* Version: 1.0.2                                                              *
* Date   : 09.02.18                                                           *
* Author : Peter Weissig, Tina Lenk, Leander Herr                             *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "spielfeld_intern.h"


//**************************<Macros>*******************************************
#define bus_leds_setIr(x)    ( x ? (bus_led_data_port|= bus_led_data_pin) : (bus_led_data_port&= ~bus_led_data_pin))

#define buttons_getBtnbus() ((bus_btn_data_port & bus_btn_data_pin) == 0x00)
#define buttons_getBtn1()    ((button1_port & button1_pin) == 0x00)
#define buttons_getBtn2()    ((button2_port & button2_pin) == 0x00)
#define buttons_getBtn3()    ((button3_port & button3_pin) == 0x00)



//**************************<Methods>******************************************

//**************************[delay_licht]**************************************
void delay_licht(uint16_t mseconds) {

    uint8_t state;

    mseconds/= 10;
    do {
        // button1
        state = buttons_getBtn1();
        if (state != buttons.state1) {
            buttons.state1 = state;
            // state did change
            if (!state) {
                // released button
                buttons.countdown1 = 50;
            } else if (!buttons.countdown1) {
                // pushed button and no countdown
                buttons.flank1 = 1;
            }
        }
        if (buttons.countdown1) {
            buttons.countdown1--;
        }

        // button2
        state = buttons_getBtn2();
        if (state != buttons.state2) {
            buttons.state2 = state;
            // state did change
            if (!state) {
                // released button
                buttons.countdown2 = 50;
            } else if (!buttons.countdown2) {
                // pushed button and no countdown
                buttons.flank2 = 1;
            }
        }
        if (buttons.countdown2) {
            buttons.countdown2--;
        }

        // buttonMode
        state = buttons_getBtnMode();
        if (state != buttons.stateMode) {
            buttons.stateMode = state;
            // state did change
            if (!state) {
                // released button
                buttons.countdownMode = 20;
            } else if (!buttons.countdownMode) {
                // pushed button and no countdown
                buttons.flankMode = 1;
            }
        }
        if (buttons.countdownMode) {
            buttons.countdownMode--;
        }

        // wait 10ms
        delay_ms(10);
    } while (mseconds--);
}

//**************************[delay_licht]**************************************
ISR(TIMER2_COMPA_vect) {

    if (ir_leds.counter) {
        ir_leds.counter = 0;
        leds_setIr1(ir_leds.state1);
        leds_setIr2(ir_leds.state2);
    } else {
        ir_leds.counter = 1;
        leds_setIr1(0);
        leds_setIr2(0);
    }
}

//**************************[leds_clearAll]************************************
void leds_clearAll(void) {

    // schaltet alle LEDs aus
    // LED von beiden Teams starten in jedem Modus im ausgeschalteten Zustand
    led1_setRed(0);
    led1_setGreen(0);
    led1_setBlue(0);
    led1_setIr(0);

    led2_setRed(0);
    led2_setGreen(0);
    led2_setBlue(0);
    led2_setIr(0);
}

//**************************[leds_initTest]************************************
void leds_initTest(void) {

    leds_clearAll();


    // RGB 1 & IR1
    led1_setIr   (1);

    led1_setRed  (1);                   delay_licht(500);
    led1_setGreen(1); led1_setRed  (0); delay_licht(500);
    led1_setBlue (1); led1_setGreen(0); delay_licht(500);
                      led1_setBlue (0);

                      led1_setIr   (0);


    // RGB 2 & IR2
    led2_setIr   (1);

    led2_setRed  (1);                   delay_licht(500);
    led2_setGreen(1); led2_setRed  (0); delay_licht(500);
    led2_setBlue (1); led2_setGreen(0); delay_licht(500);
                      led2_setBlue (0);

                      led2_setIr   (0);

    leds_clearAll();
}

//**************************[led1_setIr]***************************************
void led1_setIr(uint8_t state) {

    ir_leds.state1 = state;
}

//**************************[led2_setIr]***************************************
void led2_setIr(uint8_t state) {

    ir_leds.state2 = state;
}


//**************************[init_hardware]************************************
void init_hardware(void) {

    // set output
    DDRB = 0;
    DDRC = 0;
    DDRD = 0;
    //disp
    disp_col_ser_ddr|=disp_col_ser_pin;
    disp_col_sck_ddr|=disp_col_sck_pin;
    disp_row_ser_ddr|=disp_row_ser_pin;
    disp_row_sck_ddr|=disp_row_sck_pin;
    disp_rck_ddr|=disp_rck_pin;
    disp_enable_ddr|=disp_enable_pin;
    //led
    led1_ddr|=led1_pin;
    led2_ddr|=led2_pin;
    //bus
    bus_btn_load_ddr|=bus_btn_load_pin;
    bus_led_load_ddr|=bus_led_load_pin;
    bus_led_data_ddr|=bus_led_data_pin;
    bus_clk_ddr|=bus_clk_pin;
    bus_ir_clk_ddr|=bus_ir_clk_pin;
    bus_rgb_ddr|=bus_rgb_pin;



    // set buttons to input (disable pullups)
    //button
    button1_ddr&=~button1_pin;
    button2_ddr&=~button2_pin;
    button3_ddr&=~button3_pin;
    //bus
    bus_btn_data_ddr&=~bus_btn_data_pin;
    return_btn_ddr&=~return_btn_pin;
    return_rgb_ddr&=~return_rgb_pin;

    // Timer 2 (IR-led)
/*    TCCR2A = _BV(WGM21);  // Output-Mode: none
                          // Operation-Mode: CTC til OCRA
    TCCR2B = _BV(CS20);   // Operation-Mode: CTC til OCRA
                          // Prescaler: 1
    OCR2A = 210;          // Top: 210 ==> 76KHz
    TIMSK2 = _BV(OCIE2A); // enable compare match interrupt
*/
    // startup time
    delay_ms(50);

    // initialize variables
    buttons_reset();

    // enable interrupts
    sei();
}
//**************************[buttons_reset]************************************
void buttons_reset(void) {

    uint8_t i;
    for (i=0;i<3;i++){
        board_buttons[number].state    = 0;
        board_buttons[number].flank    = 0;
        board_buttons[number].countdown    = 0;
    }
    for (i=0;i<bus_count;i++){
        board_buttons[number].state    = 0;
        board_buttons[number].flank    = 0;
        board_buttons[number].countdown    = 0;
    }
}

//**************************[buttons_clearAll]*********************************
void buttons_clearAll(void) {

    uint8_t i;
    for (i=0;i<3;i++){
        board_buttons[number].flank    = 0;
    }
    for (i=0;i<bus_count;i++){
        board_buttons[number].flank    = 0;
    }
}

//**************************[buttons_readFlank]********************************
uint8_t board_button_readFlank(uint8_t number) {

    if (number>2) return 0;
    uint8_t result = board_buttons[number].flank;
    board_buttons[number].flank = 0;

    return result;
}
uint8_t bus_button_readFlank(uint8_t number) {

    if (number>=bus_count) return 0;
    uint8_t result = bus_buttons[number].flank;
    bus_buttons[number].flank = 0;

    return result;
}

//**************************[BUSBUSBUS]**************************************
void bus_read_buttons(void){
    //1bit shift in
    uint8_t state;
    state = buttons_getBtnbus();
    if (state != bus_buttons[bus_button_ir_counter].state) {
        bus_buttons[bus_button_ir_counter].state = state;
        // state did change
        if (!state) {
            // released button
            bus_buttons[bus_button_ir_counter].countdown = 50;
        } else if (!bus_buttons[bus_button_ir_counter].countdown) {
            // pushed button and no countdown
            bus_buttons[bus_button_ir_counter].flank = 1;
        }
    }
    if (bus_buttons[bus_button_ir_counter].countdown) {
        bus_buttons[bus_button_ir_counter].countdown--;
    }
}
void bus_send_IR(void){
    //1bit shift out
    bus_leds_setIr(bus_ir_leds[bus_button_ir_counter]);
}
void bus_send_RGB(void){
    //send uart

}
void bus_activate_ir_buttons(void){

    //set last IR round
    //get new Button round
    bus_button_ir_counter=0;
}
void bus_itteration_ir_buttons(void){
    bus_read_buttons();
    bus_send_IR();
    bus_button_ir_counter++;
    if (bus_button_ir_counter==bus_elements){
        bus_activate_ir_buttons();
    }
}
//**************************[display]**************************************
//**************************[display::seter]************************************
void display_setSegment(uint8_t *pict,uint8_t segmentnumber){
    if ((segmentnumber>=0)&&(segmentnumber<segments_count)){
        int8_t i;
        for(i=0;i<7;i++){
            display_segmentbuffer[segmentnumber][i]=pict[i]<<3;
        }
    }
}
void display_shiftright(uint8_t number){
    int8_t i;
    for(i=segments_count-1;i>=number;i--){
        int8_t j;
        for(j=0;j<7;j++){
            display_segmentbuffer[i][j]=display_segmentbuffer[i-number][j];
        }
    }
}
void display_shiftleft(uint8_t number){
    int8_t i;
    for(i=number;i<segments_count;i++){
        int8_t j;
        for(j=0;j<7;j++){
            display_segmentbuffer[i-number][j]=display_segmentbuffer[i][j];
        }
    }
}

//**************************[display::shower]***********************************
void display_fill_col(uint8_t row, uint8_t col, uint8_t seg){
        if ((col<5)&&(display_segmentbuffer[seg][row]&_BV(col+3))){
            disp_col_ser_port|=disp_col_ser_pin;
        }else{
            disp_col_ser_port&=~disp_col_ser_pin;
        }
        disp_col_sck_port|=disp_col_sck_pin;
    nop();
    nop();
        disp_col_sck_port&=~disp_col_sck_pin;
    nop();
    nop();
}
void  display_row_next(void){
    display_current_row++;
    if(display_current_row>=8){
        display_current_row=0;
        disp_row_ser_port|=disp_row_ser_pin;
    }else{
        disp_row_ser_port&=~disp_row_ser_pin;
    }
    disp_row_sck_port|=disp_row_sck_pin;
    nop();
    nop();
    disp_row_sck_port&=~disp_row_sck_pin;
    nop();
    nop();
}
void display_row_reset(void){
    for(select=0;select<7;select++){
        if (select==6){
            disp_row_ser_port|=disp_row_ser_pin;
        }else{
            disp_row_ser_port&=~disp_row_ser_pin;
        }
        disp_row_sck_port|=disp_row_sck_pin;
        nop();
        nop();
        disp_row_sck_port&=~disp_row_sck_pin;
        nop();
        nop();
    }
}
void display_fill_current_row(void){
    int8_t seg;
    if ((display_current_row==7)&&!display_double_dot){
        //double dot is out-> dont show it by shift row out,next is anyhow newstart
        disp_row_sck_port|=disp_row_sck_pin;
        nop();
        nop();
        disp_row_sck_port&=~disp_row_sck_pin;
        nop();
        nop();
    } else{
        for(seg=0;seg<segments_count;seg++){
            int8_t col;
            for(col=0;col<8;col++){
                display_fill_col(row,col,seg);
            }
        }
    }
}
void display_show(){
    int8_t row;
    display_row_reset(row);
    for(row=0;row<7;row++){
        display_row_next(row);
        display_fill_current_row();

        disp_rck_port|=disp_rck_pin;
        //delay_on
        delay_ms(display_ontime);
        disp_rck_port&=~disp_rck_pin;
        delay_ms(display_offtime);
        //delay_off
    }
}
/*
 INTERUPT METHODE:
    disp_rck_port&=~disp_rck_pin;
    nop();
    nop();
    display_row_next(row);
    display_fill_current_row();
    disp_rck_port|=disp_rck_pin;
 */
