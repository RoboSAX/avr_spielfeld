/******************************************************************************
 * gloabel.h                                                                  *
 * =========                                                                  *
 *                                                                            *
 * Version: 1.2.1                                                             *
 * Date   : 02.06.18                                                          *
 * Author : Peter Weissig, Leander Herr                                       *
 *                                                                            *
 *****************************************************************************/

#ifndef __GLOBAL_H
#define __GLOBAL_H


//**************************<Included files>***********************************
#include <inttypes.h>


//**************************<Types and Variables>******************************

// debounce=debounce[ms](max 63)
#define MASTER_BUTTONS_DEBOUNCE_TIME 63

// debounce=debounce(max 31)*(global+1)[ms]
#define LEDBOX_BUTTONS_DEBOUNCE_TIME 31
#define LEDBOX_BUTTONS_GLOBAL_TIME   8

#endif  // #ifndef __GLOBAL_H
