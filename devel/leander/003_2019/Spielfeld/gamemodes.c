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
//optics:
//blink if pressed correct
//blink red[/color] if error


//**************************<Macros>*******************************************
#define LED_PER_TEAM LEDBOX_COUNT_MAX/3
#define TEAM1COLOR clGreen
#define TEAM2COLOR clRed
#define TEAMNCOLOR clBlue
#define NOCOLOR clBlack
#define ERRORCOLOR clYellow
#define POINTSFOROWNCOLOR 3
#define POINTSFORNEUTRAL 1
#define STOREDLASTLEDS 3

#define NUMBEROFBLINKINGS 3
#define BLINKCOUNTDOWNMAX 7

//PHASEX = Y: X LEDs on after Y correct deactivated LEDs
#define PHASE0 8
#define PHASE1 7
#define PHASE2 6
#define PHASE3 5
#define PHASE4 4
#define PHASE5 3
#define PHASE6 2
#define PHASE7 1
#define PHASE8 0

//**************************<Types and Variables>******************************
struct sTeamLED{
    uint8_t activ : 1;
    uint8_t globalNumber : 5;
};
struct sBlink{
    uint8_t LEDNr : 3;
    uint8_t countdown : 3;
    uint8_t numberOfBlinks : 2;
};
struct sTeam {
    uint8_t points;
    struct sTeamLED LEDs[LED_PER_TEAM];

    uint8_t disabledLeds;
    uint8_t numberOfActivLeds;
    uint8_t lastPressedLeds[STOREDLASTLEDS];
    enum eColor error_color;
    enum eColor teamColor;
    
    struct sBlink correct;
    struct sBlink error;
};

struct sTeam team[3];

enum eTeamNumber {
    team1,
    team2,
    teamNeutral
};

struct sGlobalLED{
    uint8_t teamNr : 4;
    uint8_t LEDNr : 4;
};
struct sGlobalLED LEDToTeam[LEDBOX_COUNT_MAX];

enum eGamemodes usedGamemode;
//**************************<Methods>******************************************
void pushButton(uint8_t number);
void setLEDs();
void setLEDForTeam(struct sTeam *team);

void gamemode_init(void){
    usedGamemode=gmNothing;
    gamemode_start(usedGamemode);

    uint8_t  i;
    for(i=0;i<LEDBOX_COUNT_MAX;i++){
        LEDToTeam[i].teamNr = 4;
    }
    uint8_t teamNr;
    for (teamNr=0;teamNr<3;teamNr++){
        switch (teamNr){
            case team1:
                team[teamNr].teamColor = TEAM1COLOR;
            break;
            case team2:
                team[teamNr].teamColor = TEAM2COLOR;
            break;
            case teamNeutral:
                team[teamNr].teamColor = TEAMNCOLOR;
            break;
        }
        uint8_t  i;
        for(i=0;i<LED_PER_TEAM;i++){
            team[teamNr].LEDs[i].globalNumber = LEDBOX_COUNT_MAX;
        }
        uint8_t  teamLEDNumber = 0;
        while(teamLEDNumber<LED_PER_TEAM){
            _ledbox_buttons_and_ir_update();
            _ledbox_rgb_update();
            display_show_ms(8);
            uint8_t  j;
            for (j = 0; j < LEDBOX_COUNT_MAX; j++) {
                if (buttons_get(j)) {
                    if(LEDToTeam[j].teamNr >= teamNr){
                        uint8_t  doublicate = LED_PER_TEAM;
                        uint8_t  freeNumber = LED_PER_TEAM;
                        uint8_t  reCheck;
                        for(reCheck=0;reCheck<LED_PER_TEAM;reCheck++){
                            if (team[teamNr].LEDs[reCheck].globalNumber == j) doublicate = reCheck;
                            if (team[teamNr].LEDs[reCheck].globalNumber >= LEDBOX_COUNT_MAX) freeNumber = reCheck;
                        }
                        if (doublicate < LED_PER_TEAM){
                            LEDToTeam[j].teamNr = 4;
                            LEDToTeam[j].LEDNr = LED_PER_TEAM;
                            team[teamNr].LEDs[doublicate].globalNumber = LEDBOX_COUNT_MAX;
                            rgb_set(j,NOCOLOR);
                            teamLEDNumber--;
                        }
                        else{
                            LEDToTeam[j].teamNr = teamNr;
                            LEDToTeam[j].LEDNr = freeNumber;
                            team[teamNr].LEDs[freeNumber].globalNumber = j;
                            rgb_set(j,team[teamNr].teamColor);
                            teamLEDNumber++;
                        }
                    }
                }
            }
        }
    }
}

void gamemode_start(enum eGamemodes gameMode){
    usedGamemode=gameMode;

    //set all LEDs to on
    uint8_t  j;
    for (j=0;j<3;j++){
        team[j].points=0;
        team[j].disabledLeds=0;
        team[j].numberOfActivLeds=8;
        team[j].error.numberOfBlinks=0;
        team[j].correct.numberOfBlinks=0;

        uint8_t  i;
        for(i=0;i<STOREDLASTLEDS;i++){
            team[j].lastPressedLeds[i]=LED_PER_TEAM;
        }

        for(i=0;i<LED_PER_TEAM;i++){
            team[j].LEDs[i].activ=1;
        }
    }
}

void gamemode_update(){
    uint8_t i;
    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        if (buttons_get(i)) {
            pushButton(i);
        }
    }
    //code to update RGB and IR LED
    setLEDs();
    team[team1].points = team[team1].disabledLeds * POINTSFOROWNCOLOR + team[teamNeutral].disabledLeds * POINTSFORNEUTRAL;
    team[team2].points = team[team2].disabledLeds * POINTSFOROWNCOLOR + team[teamNeutral].disabledLeds * POINTSFORNEUTRAL;
    showPoints(team[team1].points, team[team2].points);
}

void gamemode_finalize(){
    //game end
    showPoints(team[team1].points, team[team2].points);
}

//Game priate

void setLEDs(void){
    uint8_t  teamNr;
    for (teamNr=0;teamNr<3;teamNr++){
        uint8_t i;
        for(i=0;i<LED_PER_TEAM;i++){
            uint8_t number = team[teamNr].LEDs[i].globalNumber;

            uint8_t useTeamColor = 1;
            if (team[teamNr].correct.LEDNr == i){
                if (team[teamNr].correct.numberOfBlinks & 0x01){
                    rgb_set(number,team[teamNr].teamColor);
                    useTeamColor = 0;
                }
                if (team[teamNr].correct.countdown) team[teamNr].correct.countdown--;
                else if(!team[teamNr].correct.numberOfBlinks) team[teamNr].correct.numberOfBlinks--;
            }
            else if (team[teamNr].error.LEDNr == i){
                if (team[teamNr].error.numberOfBlinks & 0x01){
                    rgb_set(number,ERRORCOLOR);
                    useTeamColor = 0;
                }
                if (team[teamNr].error.countdown) team[teamNr].error.countdown--;
                else if(!team[teamNr].error.numberOfBlinks) team[teamNr].error.numberOfBlinks--;
            }

            if (team[teamNr].LEDs[i].activ){
                if (useTeamColor){
                    rgb_set(number,team[teamNr].teamColor);
                }
                ir_set(number,1);
            }else{
                if (useTeamColor){
                    rgb_set(number,NOCOLOR);
                }
                ir_set(number,0);
            }
        }
    }
}

void calculateLEDNumber(struct sTeam *team){
    if (team->disabledLeds >= PHASE0) team->numberOfActivLeds = 0;
    else if (team->disabledLeds >= PHASE1) team->numberOfActivLeds = 1;
    else if (team->disabledLeds >= PHASE2) team->numberOfActivLeds = 2;
    else if (team->disabledLeds >= PHASE3) team->numberOfActivLeds = 3;
    else if (team->disabledLeds >= PHASE4) team->numberOfActivLeds = 4;
    else if (team->disabledLeds >= PHASE5) team->numberOfActivLeds = 5;
    else if (team->disabledLeds >= PHASE6) team->numberOfActivLeds = 6;
    else if (team->disabledLeds >= PHASE7) team->numberOfActivLeds = 7;
}

void pushButton(uint8_t number){
    uint8_t teamNr = LEDToTeam[number].teamNr;
    uint8_t LEDNr = LEDToTeam[number].LEDNr;

    if (team[teamNr].LEDs[LEDNr].activ){
        team[teamNr].disabledLeds += 1;
        uint8_t i;
        for (i = STOREDLASTLEDS - 1; i > 0; i--){
            team[teamNr].lastPressedLeds[i]=team[teamNr].lastPressedLeds[i-1];
        }
        team[teamNr].lastPressedLeds[0]=LEDNr;
        calculateLEDNumber(&team[teamNr]);

        team[teamNr].correct.LEDNr = LEDNr;
        team[teamNr].correct.countdown = BLINKCOUNTDOWNMAX;
        team[teamNr].correct.numberOfBlinks = NUMBEROFBLINKINGS;
    }
    else{
        //error
        if (team[teamNr].numberOfActivLeds > 1) team[teamNr].numberOfActivLeds--;
        uint8_t i;
        for (i = STOREDLASTLEDS; i > 0; i--){
            if (team[teamNr].lastPressedLeds[i - 1] < LED_PER_TEAM){
                team[teamNr].lastPressedLeds[i-1] = LED_PER_TEAM;
                break;
            }
        }

        team[teamNr].error.LEDNr = LEDNr;
        team[teamNr].error.countdown = BLINKCOUNTDOWNMAX;
        team[teamNr].error.numberOfBlinks = NUMBEROFBLINKINGS;
    }
    setLEDForTeam(&team[teamNr]);
}

void setLEDForTeam(struct sTeam *team){

    uint8_t  i;
    for(i=0;i<LED_PER_TEAM;i++){
        team->LEDs[i].activ=1;
    }
    uint8_t  ledsToDeactivate = LED_PER_TEAM - team->numberOfActivLeds;

    if ((ledsToDeactivate)&&(team->lastPressedLeds[0] < LED_PER_TEAM)){
        team->LEDs[team->lastPressedLeds[0]].activ = 0;
        ledsToDeactivate--;
    }

    while(ledsToDeactivate>0){
        //deactivate a random led
        ledsToDeactivate--;
    }
}
