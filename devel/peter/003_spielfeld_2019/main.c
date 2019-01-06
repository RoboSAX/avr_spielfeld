/******************************************************************************
* main.c                                                                      *
* ======                                                                      *
*                                                                             *
* Version: 1.0.1                                                              *
* Date   : 27.12.18                                                           *
* Author : Leander Herr, Peter Weissig                                        *
*                                                                             *
******************************************************************************/
//ms per round
#define ROUNDTIME ((uint32_t)(3UL*60UL*1000UL))

//**************************<Included files>***********************************
#include "master.h"
#include "ledbox.h"
#include "gamemodes.h"
#include "display.h"
#include "systick.h"
#include "display_chars.h"

#include <avr/io.h>
#include <inttypes.h>


//**************************<Prototypes>***************************************
int main(void);
void init(void);
uint8_t *numbers[10]={num0,num1,num2,num3,num4,num5,num6,num7,num8,num9};

//**************************<Methods>******************************************
void showtime (uint16_t time, uint8_t dot) {

    if(dot) {
        display_double_dot=1;
        display_setSegment(numbers[time%10],3);
        time/=10;
        display_setSegment(numbers[time% 6],2);
        time/=6;
        display_setSegment(numbers[time%10],1);
        display_setSegment(numbers[0]      ,0);
    } else {
        display_double_dot=0;
        display_setSegment(numbers[time%10],3);
        time/=10;
        display_setSegment(numbers[time%10],2);
        time/=10;
        display_setSegment(numbers[time%10],1);
        display_setSegment(numbers[0]      ,0);
    }
}



//**************************[init]*********************************************
void init () {

}

//**************************[main]*********************************************
int main () {

    // initialization
    master_init();
    ledbox_init();
    init_display();
    systick_init();

    gamemode_init();

    init();



    display_double_dot=0;
    display_setSegment(Robol,0);
    display_setSegment(Robor,1);
    display_setSegment(space,2);
    display_setSegment(Smile,3);
    rgb_setAll(clRain);



    uint8_t i;
    uint8_t menuemode=0;
    uint32_t starttime=0;
    while (1) {
        if(menuemode<50){

            if (master_button1()) {
                switch (menuemode){
                    case 1:
                        menuemode=99;
                        systick_reset();
                        starttime=systick_get();
                    break;
                    default:
                    case 0:
                        switch (gamemode){
                            case gmNothing:
                                gamemode=gmEnemy;
                                rgb_setAll(clYellow);
                                ir_clearAll();
                            break;
                            case gmEnemy:
                                gamemode=gmPunishAndEnemy;
                                rgb_setAll(clRed);
                                ir_clearAll();
                            break;
                            case gmPunishAndEnemy:
                                gamemode=gmNothing;
                                rgb_setAll(clGreen);
                                ir_clearAll();
                            break;
                            default:
                                gamemode=gmNothing;
                                rgb_setAll(clRain);
                                ir_clearAll();
                            break;
                        }
                        _ledbox_buttons_and_ir_update();
                        _ledbox_rgb_update();
                        display_show_ms(800);
                        gamemode_reset();
                    break;
                }
            }
            if (master_button2()) {
                menuemode++;
                switch (menuemode){
                    case 1:
                        display_double_dot=0;
                        display_setSegment(Robol,0);
                        display_setSegment(Robor,1);
                        display_setSegment(space,2);
                        display_setSegment(alpaS,3);
                    break;
                    case 2:
                    default:
                        display_double_dot=0;
                        display_setSegment(Robol,0);
                        display_setSegment(Robor,1);
                        display_setSegment(space,2);
                        display_setSegment(alpaT,3);
                        menuemode=0;
                    break;

                }
            }
        }
        if(menuemode==99){
            rgb_setAll(clRain);
            while (systick_get()<(9990+starttime)){
                showtime((9990+starttime-systick_get())/10,0);
                display_show_ms(8);
            }
            gamemode_reset();
            starttime=systick_get();
            menuemode=100;
        }
        if(menuemode==100){
            showtime((ROUNDTIME+starttime-systick_get())/(1000UL),1);
            if(ROUNDTIME+starttime<systick_get()){
                menuemode=45;
                display_double_dot=0;
                display_setSegment(Robol,0);
                display_setSegment(Robor,1);
                display_setSegment(space,2);
                display_setSegment(alpaW,3);
            }
        }

        if (master_button1()) {
            debug_led1(1);

            rgb_setAll(clYellow);
        }/*
        if (master_button2()) {
            debug_led2(1);

            rgb_setAll(clBlue);
        }*/
        if (master_button3()) {
            debug_led1(0);
            debug_led2(0);

            rgb_setAll(clGreen);
            ir_clearAll();
        }


        if(menuemode!=45){


            for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
                if (buttons_get(i)) {
                    pushButton(i);
                }
            }

            setLEDs();
            _ledbox_buttons_and_ir_update();
            _ledbox_rgb_update();
        }

        _master_buttons_update();
        display_show_ms(8);
    }

    return (0);
}
