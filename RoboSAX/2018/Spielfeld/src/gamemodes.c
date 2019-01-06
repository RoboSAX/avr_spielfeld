/******************************************************************************
* gamemodes.c                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 01.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "gamemodes.h"


//**************************<Macros>*******************************************
#define LED_PER_TEAM LEDBOX_COUNT_MAX/2
#define TEAM1COLOR clGreen
#define TEAM2COLOR clBlue
#define TEAM1NOCOLOR clLBlue
#define TEAM2NOCOLOR clLGreen
#define NEUTRALCOLOR clBlack
#define LED_TEAM1_0_to 4
#define PUNISHERRORTIMER (LEDBOX_BUTTONS_DEBOUNCE_TIME*4)
//**************************<Types and Variables>******************************
struct sTeam {
    uint8_t error_timer;
    uint8_t error_countdown;
    enum eColor error_color;
    uint8_t activate_LEDS[LED_PER_TEAM];
};
struct sTeam team1,team2;
//**************************<Methods>******************************************
uint8_t punishment(struct sTeam *myteam,struct sTeam *enemy,uint8_t mynumber);
void switchLED(struct sTeam *myteam,struct sTeam *enemy,uint8_t mynumber);

void gamemode_init(void){
    gamemode=gmNothing;
    gamemode_reset();
}

void gamemode_reset(void){
    team1.error_countdown=0;
    team2.error_countdown=0;
    uint8_t  i;
    for(i=0;i<LED_PER_TEAM;i++){
        team1.activate_LEDS[i]=0;
        team2.activate_LEDS[i]=0;
    }
}

void setLEDs(void){
    uint8_t i;
    for(i=0;i<LED_PER_TEAM;i++){
        uint8_t number1=(i<(LED_PER_TEAM-LED_TEAM1_0_to))?(i+(LED_PER_TEAM+LED_TEAM1_0_to)):(i-(LED_PER_TEAM-LED_TEAM1_0_to));
        if (team1.error_timer){
            team1.error_timer--;
            rgb_set(number1,team1.error_color);
        }else{
            if (team1.activate_LEDS[i]){
                rgb_set(number1,TEAM1COLOR);
                ir_set(number1,1);
            }else{
                if ((gamemode!=gmNothing)&&(team2.activate_LEDS[i]))rgb_set(number1,TEAM1NOCOLOR);
                else rgb_set(number1,NEUTRALCOLOR);
                ir_set(number1,0);

            }
        }
        uint8_t number2=i+LED_TEAM1_0_to;
        if (team2.error_timer){
            team2.error_timer--;
            rgb_set(number2,team2.error_color);
        }else{
            if (team2.activate_LEDS[i]){
                rgb_set(number2,TEAM2COLOR);
                ir_set(number2,1);
            }else{
                if ((gamemode!=gmNothing)&&(team1.activate_LEDS[i]))rgb_set(number2,TEAM2NOCOLOR);
                else rgb_set(number2,NEUTRALCOLOR);
                ir_set(number2,0);
            }
        }
    }
}

void pushButton(uint8_t number){
    struct sTeam *myteam,*enemy;
    uint8_t mynumber;
    if (number<LED_TEAM1_0_to){
        myteam=&team1;
        enemy=&team2;
        mynumber=number+(LED_PER_TEAM-LED_TEAM1_0_to);
    }else{
        number-=LED_TEAM1_0_to;
        if (number>=LED_PER_TEAM){
            myteam=&team1;
            enemy=&team2;
            mynumber=number-LED_PER_TEAM;
        }else{
            myteam=&team2;
            enemy=&team1;
            mynumber=number;
        }
    }
    if (!punishment(myteam,enemy,mynumber))switchLED(myteam,enemy,mynumber);
}
uint8_t punishment(struct sTeam *myteam,struct sTeam *enemy,uint8_t mynumber){
    if ((gamemode==gmPunishAndEnemy)&&myteam->activate_LEDS[mynumber]){
        myteam->error_countdown++;
        switch (myteam->error_countdown){
            case 1: myteam->error_timer=PUNISHERRORTIMER;
                    myteam->error_color=clYellow;
                break;
            case 2: myteam->error_timer=PUNISHERRORTIMER;
                    myteam->error_color=clPurple;
                break;
            case 3 :myteam->error_timer=PUNISHERRORTIMER;
                    myteam->error_color=clRed;
                    myteam->error_countdown=0;
                    myteam->activate_LEDS[mynumber]=0;
                    enemy->activate_LEDS[mynumber]=1;
                    return 1;
                break;
            default:myteam->error_countdown=0;
                break;
        }
    }
    return 0;
}
void switchLED(struct sTeam *myteam,struct sTeam *enemy,uint8_t mynumber){
    myteam->activate_LEDS[mynumber]=1;
    if ((gamemode==gmPunishAndEnemy)||(gamemode==gmEnemy))enemy->activate_LEDS[mynumber]=0;
}
