/******************************************************************************
* gloabel.h                                                                   *
* =========                                                                   *
*                                                                             *
* Version: 1.2.1                                                              *
* Date   : 02.06.18                                                           *
* Author : Peter Weissig, Leander Herr                                        *
*                                                                             *
******************************************************************************/

#ifndef __GLOBAL_H
#define __GLOBAL_H


//**************************<Included files>***********************************
#include <inttypes.h>


//**************************<Types and Variables>******************************

//debounce=debounce*(global+1)*(update+1)[ms]
#define MASTER_BUTTONS_DEBOUNCE_TIME 15
#define MASTER_BUTTONS_GLOBAL_TIME 2

//debounce=debounce*(global+1)*(update+1)[ms]
#define LEDBOX_BUTTONS_DEBOUNCE_TIME 15
#define LEDBOX_BUTTONS_GLOBAL_TIME 9

#endif //#ifndef __GLOBAL_H
