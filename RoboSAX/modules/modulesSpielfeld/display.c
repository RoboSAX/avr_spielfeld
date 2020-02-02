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
#include "../modulesInterface/systick.h"


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


#ifdef INVERT_DISPLAY
#define calc_real_seg(segnr) (SEGMENTS_COUNT-1-segnr)
#define calc_real_row(rownr) (7-1-rownr)
#define calc_real_col(colnr) (5-1-colnr)
#else
#define calc_real_seg(segnr) (segnr)
#define calc_real_row(rownr) (rownr)
#define calc_real_col(colnr) (colnr)
#endif

#define display_working_supersegmentbuffer display_supersegmentbuffer[display_current_working_buffer]
#define display_showing_supersegmentbuffer display_supersegmentbuffer[display_current_display_buffer]

//**************************<Types and Variables>******************************

//display
uint16_t display_supersegmentbuffer[2][SUPER_SEGMENTS_COUNT][8];
uint8_t display_double_dot;
uint8_t display_current_row;
uint8_t display_current_display_buffer;
uint8_t display_current_working_buffer;

const uint8_t display_segToSuperSeg[SEGMENTS_COUNT]={0,0,1,1};
const uint8_t display_superSegOffset[SEGMENTS_COUNT]={0,5,0,5};
uint8_t display_superSegWindowStart[SUPER_SEGMENTS_COUNT];


volatile uint8_t needsUpdate; 

//**************************<Methods>******************************************

// clock and load
void toggle_col_sck(void);
void toggle_row_sck(void);
void toggle_rck(void);


void display_fill_col(uint8_t row, uint8_t col, uint8_t seg);
void display_row_reset(void);
void display_row_next(void);
void display_fill_current_row(void);

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
	display_current_display_buffer=0;
	display_current_working_buffer=0;
	needsUpdate=1;

	int8_t i;
	for(i=0;i<SUPER_SEGMENTS_COUNT;i++){
		display_superSegWindowStart[i]=15;
	}
}
//**************************[display]******************************************
//**************************[display::seter]***********************************
void display_setSegment(const uint8_t *pict,uint8_t segmentnumber){
	if ((segmentnumber>=0)&&(segmentnumber<SEGMENTS_COUNT)){
		int8_t i;
		for(i=0;i<7;i++){
			uint8_t superSeg = display_segToSuperSeg[segmentnumber];
			uint8_t posLast = display_superSegWindowStart[superSeg] 
							- display_superSegOffset[segmentnumber] - 4;
			uint16_t mask = ~(((1<<5)-1)<<posLast);
			display_working_supersegmentbuffer[superSeg][i]&=mask;
			display_working_supersegmentbuffer[superSeg][i]|=(pict[i] & 0x1F) << posLast;
		}
		needsUpdate=1;
	}
}

void display_invertSegment(uint8_t segmentnumber){
	if ((segmentnumber>=0)&&(segmentnumber<SEGMENTS_COUNT)){
		int8_t i;
		for(i=0;i<7;i++){
			uint8_t superSeg = display_segToSuperSeg[segmentnumber];
			uint8_t posLast = display_superSegWindowStart[superSeg] 
							- display_superSegOffset[segmentnumber] - 4;
			uint16_t mask = ~(((1<<5)-1)<<posLast);
			display_working_supersegmentbuffer[superSeg][i]=
					(display_working_supersegmentbuffer[superSeg][i] & mask)
					| (~display_working_supersegmentbuffer[superSeg][i] & ~mask);
		}
		needsUpdate=1;
	}
}
/*void display_shiftright(uint8_t number){
	int8_t i;
	for(i=SEGMENTS_COUNT-1;i>=number;i--){
		int8_t j;
		for(j=0;j<7;j++){
			display_segmentbuffer[i][j]=display_segmentbuffer[i-number][j];
		}
	}
	needsUpdate=1;
}
void display_shiftleft(uint8_t number){
	int8_t i;
	for(i=number;i<SEGMENTS_COUNT;i++){
		int8_t j;
		for(j=0;j<7;j++){
			display_segmentbuffer[i-number][j]=display_segmentbuffer[i][j];
		}
	}
	needsUpdate=1;
}
*/
 
void display_setSuperSegment(const uint16_t *pict,uint8_t supersegmentnumber){
	if ((supersegmentnumber>=0)&&(supersegmentnumber<SUPER_SEGMENTS_COUNT)){
		int8_t i;
		uint8_t width=0x00FF & pict[SUPER_SEGMENT_HIGHT];
		uint8_t hight=0xFF00 & pict[SUPER_SEGMENT_HIGHT]>>8;
		if((hight<=SUPER_SEGMENT_HIGHT)&&(width<=SUPER_SEGMENT_WIDTH)){
			display_working_supersegmentbuffer[supersegmentnumber][SUPER_SEGMENT_HIGHT]=pict[SUPER_SEGMENT_HIGHT];
			for(i=0;i<SUPER_SEGMENT_HIGHT;i++){
				display_working_supersegmentbuffer[supersegmentnumber][i]=pict[i]<<(16-width);
			}
		} else {
			for(i=0;i<8;i++){
				display_working_supersegmentbuffer[supersegmentnumber][i]=0x0000;
			}
		}
		needsUpdate=1;
	}
}

void display_clearSuperSegment(uint8_t supersegmentnumber){
	if ((supersegmentnumber>=0)&&(supersegmentnumber<SUPER_SEGMENTS_COUNT)){
		int8_t i;
		for(i=0;i<8;i++){
			display_working_supersegmentbuffer[supersegmentnumber][i]=0x0000;
		}
		needsUpdate=1;
	}
}

void display_invertSuperSegment(uint8_t supersegmentnumber){
	if ((supersegmentnumber>=0)&&(supersegmentnumber<SUPER_SEGMENTS_COUNT)){
		int8_t i;
		for(i=0;i<SUPER_SEGMENT_HIGHT;i++){
			display_working_supersegmentbuffer[supersegmentnumber][i]= ~display_working_supersegmentbuffer[supersegmentnumber][i];
		}
		needsUpdate=1;
	}
}
void display_addLeft(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse){
	if ((supersegmentnumber>=0)&&(supersegmentnumber<SUPER_SEGMENTS_COUNT)){
		int8_t i;
		uint8_t width=0x0F & pict[SUPER_SEGMENT_HIGHT];
		uint8_t hight=0xF0 & pict[SUPER_SEGMENT_HIGHT]>>4;
		uint8_t oldwidth=0x00FF & display_working_supersegmentbuffer[supersegmentnumber][SUPER_SEGMENT_HIGHT];
		uint8_t oldhight=0xFF00 & display_working_supersegmentbuffer[supersegmentnumber][SUPER_SEGMENT_HIGHT]>>8;

		uint8_t movewidth = (sparse && oldwidth && (oldwidth + width + 1 <= SUPER_SEGMENT_WIDTH))?1:0;

		uint8_t newwidth=oldwidth + movewidth;
		uint8_t newhight=(oldhight>hight)?oldhight:hight;

		if((newhight<=SUPER_SEGMENT_HIGHT)&&(newwidth<=SUPER_SEGMENT_WIDTH)){
			display_working_supersegmentbuffer[supersegmentnumber][SUPER_SEGMENT_HIGHT]=(((uint16_t)(newhight))<<8)|(newwidth);
			for(i=0;i<SUPER_SEGMENT_HIGHT;i++){
				display_working_supersegmentbuffer[supersegmentnumber][i]=display_working_supersegmentbuffer[supersegmentnumber][i]>>movewidth;
				display_working_supersegmentbuffer[supersegmentnumber][i]|=((uint16_t)(pict[i]) & ((1<<width)-1))<<(16-width);
			}
		}
		needsUpdate=1;
	}
}

void display_addRight(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse){
	if ((supersegmentnumber>=0)&&(supersegmentnumber<SUPER_SEGMENTS_COUNT)){
		int8_t i;
		uint8_t width=0x0F & pict[SUPER_SEGMENT_HIGHT];
		uint8_t hight=0xF0 & pict[SUPER_SEGMENT_HIGHT]>>4;
		uint8_t oldwidth=0x00FF & display_working_supersegmentbuffer[supersegmentnumber][SUPER_SEGMENT_HIGHT];
		uint8_t oldhight=0xFF00 & display_working_supersegmentbuffer[supersegmentnumber][SUPER_SEGMENT_HIGHT]>>8;

		oldwidth += (sparse && oldwidth && (oldwidth + width + 1 <= SUPER_SEGMENT_WIDTH))?1:0;

		uint8_t newwidth=oldwidth + width;
		uint8_t newhight=(oldhight>hight)?oldhight:hight;

		if((newhight<=SUPER_SEGMENT_HIGHT)&&(newwidth<=SUPER_SEGMENT_WIDTH)){
			display_working_supersegmentbuffer[supersegmentnumber][SUPER_SEGMENT_HIGHT]=(((uint16_t)(newhight))<<8)|(newwidth);
			for(i=0;i<SUPER_SEGMENT_HIGHT;i++){
				display_working_supersegmentbuffer[supersegmentnumber][i]|=(uint16_t)(pict[i] & ((1<<width)-1))<<(16-oldwidth-width);
			}
		}
		needsUpdate=1;
	}
}

//void display_addAbove(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse){}
//void display_addBelow(const uint8_t *pict,uint8_t supersegmentnumber,uint8_t sparse){}
//void display_windowShiftright(uint8_t number);
//void display_windowShiftleft (uint8_t number);

//**************************[display::shower]**********************************
void display_fill_col(uint8_t row, uint8_t col, uint8_t seg){
	if(col>=16){
		disp_col_ser(0);
	}else if ((display_showing_supersegmentbuffer[seg][row]&_BV(col))){
		disp_col_ser(1);
	}else{
		disp_col_ser(0);
	}
	toggle_col_sck();
}
void display_row_next(void){
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
	for(select=0;select<=SUPER_SEGMENT_HIGHT;select++){
		if (select==SUPER_SEGMENT_HIGHT){
			disp_row_ser(1);
		} else {
			disp_row_ser(0);
		}
		toggle_row_sck();
	}
}
void display_fill_current_row(void){
	if (display_current_row==SUPER_SEGMENT_HIGHT){
		//double dot is out -> dont show it by shift row out, next is anyhow newstart
		if(display_double_dot)toggle_rck();
	} else {
		int8_t seg;
		for(seg=0;seg<SEGMENTS_COUNT;seg++){
			int8_t col;
			for(col=SUPER_SEGMENT_HIGHT;col>4;col--){
				//empty
				display_fill_col(99,99,99);
			}
			for(col=4;col>=0;col--){
				display_fill_col(
								calc_real_row(display_current_row),
								display_superSegWindowStart[display_segToSuperSeg[calc_real_seg(seg)]]
								   - display_superSegOffset[calc_real_seg(seg)]
								   - calc_real_col(col),
								display_segToSuperSeg[calc_real_seg(seg)]);
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

void display_switchBuffer(){

	if (needsUpdate) if(systick_freezUpdate(update_Display)){
		display_current_display_buffer=0;
		display_current_working_buffer=1;

		uint8_t i;
		uint8_t j;
		for(i=0;i<SUPER_SEGMENTS_COUNT;i++)
			for(j=0;j<8;j++)
				display_showing_supersegmentbuffer[i][j]=display_working_supersegmentbuffer[i][j];

		needsUpdate=0;
		systick_unFreezUpdate(update_buttons);
	}
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
