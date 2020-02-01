/******************************************************************************
* master.c                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.1                                                              *
* Date   : 31.05.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/


//**************************<Included files>***********************************
#include "../modulesInterface/master.h"


//**************************<Macros>*******************************************
// buttons
#define MASTER_BUTTONS_COUNT 3

#define button1()       ()
#define _button1_init() ()

#define button2()       ()
#define _button2_init() ()

#define MASTER_BUTTON3_PORT PIND
#define MASTER_BUTTON3_DDR  DDRD
#define MASTER_BUTTON3_PIN  _BV(4)
#define button3()       ((MASTER_BUTTON3_PORT & MASTER_BUTTON3_PIN) == 0x00)
#define _button3_init() (MASTER_BUTTON3_DDR&= ~MASTER_BUTTON3_PIN)
