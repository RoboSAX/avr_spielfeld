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
#include "modulesInterface/gamemodes.h"
#include "modulesInterface/random.h"
#include "modulesInterface/master.h"

#include "modulesInterface/global.h"
#include "modulesInterface/ledbox.h"
#include "modulesInterface/menueHelper.h"
//optics:
//blink if pressed correct
//blink red[/color] if error


//**************************<Macros>*******************************************
#define LED_PER_TEAM LEDBOX_COUNT_MAX/3
#define TEAM1COLOR clGreen
#define TEAM1FLASHCOLOR clBGreen
#define TEAM2COLOR clRed
#define TEAM2FLASHCOLOR clBRed
#define TEAMNCOLOR clBlue
#define TEAMNFLASHCOLOR clBBlue
#define NOCOLOR clBlack
#define ERRORCOLOR clPurple
#define POINTSFOROWNCOLOR 3
#define POINTSFORNEUTRAL 1
#define STOREDLASTLEDS 3

#define NUMBEROFBLINKINGS 5
#define BLINKCOUNTDOWNMAX 3
#define BLINKCOUNTMULTI 6

//PHASEX = Y: X LEDs on after Y correct deactivated LEDs
#define PHASE0 19
#define PHASE1 16
#define PHASE2 13
#define PHASE3 10
#define PHASE4 7
#define PHASE5 4
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
    uint8_t countdown : 2;
    uint8_t numberOfBlinks : 3;
};
struct sTeam {
    uint8_t points;
    struct sTeamLED LEDs[LED_PER_TEAM];

    uint8_t disabledLeds;
    uint8_t numberOfActivLeds;
    uint8_t numberOfErrors;
    uint8_t lastPressedLeds[STOREDLASTLEDS];
    enum eColor error_color;
    enum eColor teamColor;
    enum eColor teamFlashColor;

    struct sBlink correct;
    struct sBlink fullBlink;
    struct sBlink error;
};

static volatile struct sTeam team[3];

enum eTeamNumber {
    team1,
    team2,
    teamNeutral
};

struct sGlobalLED{
    uint8_t teamNr : 4;
    uint8_t LEDNr : 4;
};
static struct sGlobalLED LEDToTeam[LEDBOX_COUNT_MAX];

//**************************<Methods>******************************************
static void pushButton(uint8_t number);
static void setLEDs(void);
static void diasbleOneLEDForTeam(volatile struct sTeam *team);
static void setLEDForTeam(volatile struct sTeam *team);

void gamemode_init_2019(void){
	maxGameModes=1;
    random_init();

    gamemode_start(0, omTest, bsSpielfeld);

    uint8_t  i;
    for(i=0;i<LEDBOX_COUNT_MAX;i++){
        LEDToTeam[i].teamNr = 4;
        rgb_set(i,NOCOLOR);
    }

    uint8_t  teamNr=1;
    uint8_t  ledNr=0;
//default
    for(i=0;i<LEDBOX_COUNT_MAX;i++){
        switch (teamNr){
            case team1:
                team[teamNr].teamColor      = TEAM1COLOR;
                team[teamNr].error_color    = ERRORCOLOR;
                team[teamNr].teamFlashColor = TEAM1FLASHCOLOR;
            break;
            case team2:
                team[teamNr].teamColor      = TEAM2COLOR;
                team[teamNr].error_color    = ERRORCOLOR;
                team[teamNr].teamFlashColor = TEAM2FLASHCOLOR;
            break;
            case teamNeutral:
                team[teamNr].teamColor      = TEAMNCOLOR;
                team[teamNr].error_color    = ERRORCOLOR;
                team[teamNr].teamFlashColor = TEAMNFLASHCOLOR;
            break;
        }
        team[teamNr].LEDs[ledNr].globalNumber = i;
        LEDToTeam[i].teamNr = teamNr;
        LEDToTeam[i].LEDNr = ledNr;
        team[teamNr].LEDs[ledNr].activ = 1;

        if (teamNr){
            teamNr--;
        } else {
            teamNr=2;
        }
        if (teamNr==1){
            ledNr++;
        }
    }

//special
/*
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
        while((teamLEDNumber<LED_PER_TEAM)&&!master_button_full3()){

            display_setSegment(alpaI,0);
            display_setSegment(alpaN,1);
            display_setSegment(alpaT,2);
            display_setSegment(numbers[teamNr],3);

            setLEDs();
            uint8_t  j;
            for (j = 0; j < LEDBOX_COUNT_MAX; j++) {
                if (buttons_get(j)) {
                    if(LEDToTeam[j].teamNr >= teamNr){
                        uint8_t  doublicate = LED_PER_TEAM;
                        uint8_t  freeNumber = LED_PER_TEAM;
                        uint8_t  reCheck;
                        for(reCheck=0;reCheck<LED_PER_TEAM;reCheck++){
                            if (team[teamNr].LEDs[reCheck].globalNumber == j) doublicate = reCheck;
                            if (team[teamNr].LEDs[reCheck].globalNumber >= LEDBOX_COUNT_MAX){ freeNumber = reCheck;break;}
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
                            team[teamNr].LEDs[freeNumber].activ = 1;
                            teamLEDNumber++;
                        }
                    }
                }
            }
        }
    }
    */
}

uint8_t gamemode_start_2019(uint8_t gameMode, enum eOperationModes operationMode, enum eBaseSystem system){

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
	return 0;
}

void gamemode_update_2019(){
    uint8_t i;
    for (i = 0; i < LEDBOX_COUNT_MAX; i++) {
        if (buttons_get(i)) {
            pushButton(i);
        }
    }
    //code to update RGB and IR LED
    setLEDs();
    team[teamNeutral].points = team[teamNeutral].disabledLeds * POINTSFORNEUTRAL;

    team[team1].points = team[team1].disabledLeds ?
                            team[team1].disabledLeds * POINTSFOROWNCOLOR + team[teamNeutral].points 
                          : 0;
    team[team2].points = team[team2].disabledLeds ?
                            team[team2].disabledLeds * POINTSFOROWNCOLOR + team[teamNeutral].points
                          : 0;
    showPoints(team[team1].points, team[team2].points);
}

struct Points gamemode_points_2019(uint8_t mode){
    //game end

	struct Points points;
    if (mode % 2){
		points.team1=team1;
		points.team2=team2;
		points.type=ptBeide;

		points.maxPoints=(PHASE0 * (POINTSFORNEUTRAL + POINTSFOROWNCOLOR)) - 1U;
    } else {
		points.team1=teamNeutral;
		points.type=ptTeam1;

		points.maxPoints=(PHASE0 * POINTSFORNEUTRAL) - 1U;
    }

    //0 -> no leds
    //76 -> all leds
    //1-75 -> >=one led on, >=1 led off
	points.color1=team[team1].teamColor;
	points.color2=team[team2].teamColor;
	return points;
}

//Game priate

void setLEDs(void){

    static uint8_t global_blink_countdown = 0;
    if (global_blink_countdown){global_blink_countdown--;}
    else{
        global_blink_countdown=BLINKCOUNTMULTI;
    }

    uint8_t  teamNr;
    for (teamNr=0;teamNr<3;teamNr++){
        uint8_t i;
        for(i=0;i<LED_PER_TEAM;i++){
            uint8_t number = team[teamNr].LEDs[i].globalNumber;

            if (number < LEDBOX_COUNT_MAX){
                uint8_t useTeamColor = 1;
                if (team[teamNr].fullBlink.numberOfBlinks & 0x01){
                    rgb_set(number,team[teamNr].teamFlashColor);
                    useTeamColor = 0;
                }
                if (team[teamNr].correct.LEDNr == i){
                    if (team[teamNr].correct.numberOfBlinks & 0x01){
                        rgb_set(number,team[teamNr].teamColor);
                        useTeamColor = 0;
                    }
                    if (!global_blink_countdown){
                        if (team[teamNr].correct.countdown) team[teamNr].correct.countdown--;
                        else if(team[teamNr].correct.numberOfBlinks) {
                            team[teamNr].correct.numberOfBlinks--;
                            team[teamNr].correct.countdown = BLINKCOUNTDOWNMAX;
                        }
                    }
                }
                if (team[teamNr].error.LEDNr == i){
                    if (team[teamNr].error.numberOfBlinks & 0x01){
                        rgb_set(number,team[teamNr].error_color);
                        useTeamColor = 0;
                    }
                    if (!global_blink_countdown){
                        if (team[teamNr].error.countdown) team[teamNr].error.countdown--;
                        else if(team[teamNr].error.numberOfBlinks){
                            team[teamNr].error.numberOfBlinks--;
                            team[teamNr].error.countdown = BLINKCOUNTDOWNMAX;
                        }
                    }
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
        if (!global_blink_countdown){
            if (team[teamNr].fullBlink.countdown) team[teamNr].fullBlink.countdown--;
            else if(team[teamNr].fullBlink.numberOfBlinks){
                team[teamNr].fullBlink.numberOfBlinks--;
                team[teamNr].fullBlink.countdown = BLINKCOUNTDOWNMAX;
            }
        }
    }
}

static void calculateLEDNumber(volatile struct sTeam *team){
    if (team->disabledLeds >= PHASE0) team->numberOfActivLeds = 0;
    else if (team->disabledLeds >= PHASE1) team->numberOfActivLeds = 1;
    else if (team->disabledLeds >= PHASE2) team->numberOfActivLeds = 2;
    else if (team->disabledLeds >= PHASE3) team->numberOfActivLeds = 3;
    else if (team->disabledLeds >= PHASE4) team->numberOfActivLeds = 4;
    else if (team->disabledLeds >= PHASE5) team->numberOfActivLeds = 5;
    else if (team->disabledLeds >= PHASE6) team->numberOfActivLeds = 6;
    else if (team->disabledLeds >= PHASE7) team->numberOfActivLeds = 7;
    team->numberOfErrors=0;
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

        team[teamNr].fullBlink.countdown = BLINKCOUNTDOWNMAX;
        team[teamNr].fullBlink.numberOfBlinks = NUMBEROFBLINKINGS;

        setLEDForTeam(&team[teamNr]);
    }
    else{
        //error
        team[teamNr].numberOfErrors++;
        uint8_t i;
        for (i = STOREDLASTLEDS - 1; i > 0; i--){
            team[teamNr].lastPressedLeds[i]=team[teamNr].lastPressedLeds[i-1];
        }
        team[teamNr].lastPressedLeds[0]=LEDNr;

        team[teamNr].error.LEDNr = LEDNr;
        team[teamNr].error.countdown = BLINKCOUNTDOWNMAX;
        team[teamNr].error.numberOfBlinks = NUMBEROFBLINKINGS;
        diasbleOneLEDForTeam(&team[teamNr]);
        //setLEDForTeam(&team[teamNr]);//full random
    }
}

void diasbleOneLEDForTeam(volatile struct sTeam *team){
    int8_t  numActivLEDs = 0;

    uint8_t activLEDs = 0;
    uint8_t i;
    for (i = 0; i < LED_PER_TEAM;i++){
        if ((team->LEDs[i].activ)&&(team->LEDs[i].globalNumber != LEDBOX_COUNT_MAX)){
            activLEDs |= 0x01 << i;
            numActivLEDs++;
        }
    }
    if (numActivLEDs<=1){
        return;
    }

    uint16_t randomNumber= random();
    //deactivate a random led
    uint8_t random = randomNumber % numActivLEDs;
    uint8_t activated=0;
    for (i = 0; i < LED_PER_TEAM;i++){
        if (activLEDs & (0x01 << i)){
            if (activated==random){
                team->LEDs[i].activ = 0;
                team->correct.LEDNr = i;
                team->correct.countdown = BLINKCOUNTDOWNMAX;
                team->correct.numberOfBlinks = NUMBEROFBLINKINGS;
            }
            activated++;
        }
    }
}

void setLEDForTeam(volatile struct sTeam *team){

    uint8_t  i;
    uint8_t  deactivateLeds = 1;
    uint8_t  deactivateLastLed = 0;
    int8_t   ledsToSwitch = LED_PER_TEAM - team->numberOfActivLeds;
    uint8_t  undecidedLEDs = LED_PER_TEAM;

    uint16_t switchLEDs = 0;
    for (i = 0; i < LED_PER_TEAM;i++){
        if (team->LEDs[i].globalNumber == LEDBOX_COUNT_MAX){
            switchLEDs |= 0x01 << i;
            undecidedLEDs--;
        }
    }
    if (ledsToSwitch<=0){
        for(i=0;i<LED_PER_TEAM;i++){
            team->LEDs[i].activ = 1;
        }
        return;
    }
    if (ledsToSwitch>=undecidedLEDs){
        for(i=0;i<LED_PER_TEAM;i++){
            team->LEDs[i].activ = 0;
        }
        return;
    }
    if (ledsToSwitch+team->numberOfErrors >= undecidedLEDs){
        ledsToSwitch=undecidedLEDs-1;
    }
    else{
        ledsToSwitch+=team->numberOfErrors;
    }

    for (i = 0; (i < STOREDLASTLEDS)&&(!deactivateLastLed||(i<ledsToSwitch/2)); ++i){
        if ((ledsToSwitch)&&(team->lastPressedLeds[i] < LED_PER_TEAM))
        {
            switchLEDs |= 0x01 << team->lastPressedLeds[i];
            ledsToSwitch--;
            undecidedLEDs--;
            deactivateLastLed ++;
        }else{
           break;
	}
    }

    if (ledsToSwitch >= team->numberOfActivLeds)
    {
        if (team->numberOfErrors >= team->numberOfActivLeds){
            ledsToSwitch = 1;
        }
        else{
            ledsToSwitch = team->numberOfActivLeds-team->numberOfErrors;
        }
        deactivateLeds = 0;
    }

    uint16_t randomNumber= random();
    while(ledsToSwitch>0){
        //deactivate a random led
        uint8_t random = randomNumber % undecidedLEDs;
        randomNumber /= undecidedLEDs;
        uint8_t deactivateNr=LED_PER_TEAM;
        uint8_t activated=0;
        for (i = 0; i < LED_PER_TEAM;i++){
            if (!(switchLEDs & (0x01 << i))){
                if (activated==random){
                    deactivateNr=i;
                }
                activated++;
            }
        }
        if (activated>random){
            switchLEDs |= 0x01 << deactivateNr;
            ledsToSwitch--;
            undecidedLEDs--;
        }
        else{
            //error all out
            ledsToSwitch=0;
            switchLEDs=0;
            deactivateLeds=0;
        }
    }

    uint16_t activLeds =(deactivateLeds?~switchLEDs:switchLEDs);
    for(i=0;i<LED_PER_TEAM;i++){
        uint8_t notDeactivated=1;
        uint8_t j;
        for (j = 0; (j < deactivateLastLed); ++j){
            if (team->lastPressedLeds[j] == i){
                team->LEDs[team->lastPressedLeds[j]].activ = 0;
                notDeactivated=0;
                break;
            }
        }
        if(notDeactivated)
        {
            team->LEDs[i].activ = (activLeds>>i)&0x01;
        }
    }
}

void gamemode_to_display_2019(uint8_t gameMode, uint8_t const** displayOut1, uint8_t const** displayOut2){
	*displayOut1=numbers[gameMode / 10];
	*displayOut2=numbers[gameMode % 10];
}
