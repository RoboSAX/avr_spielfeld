/******************************************************************************
* random.h                                                                    *
* ========                                                                    *
*                                                                             *
* Version: 1.0.0                                                              *
* Date   : 21.03.18                                                           *
* Author : Peter Weissig                                                      *
*                                                                             *
* Source code was taken from:                                                 *
*   https://github.com/RoboAG/projekt_gluecksrad                              *
*                                                                             *
* See also:                                                                   *
*   https://github.com/RoboSAX/avr_spielfeld                                  *
******************************************************************************/

#ifndef _random_h_
#define _random_h_

//*******************************<Included files>******************************
#include <avr/io.h>
#include <inttypes.h>



//*********************************<Types>*************************************



//*********************************<Constants>*********************************



//*********************************<Variables>*********************************



//*********************************<Macros>************************************



//**********************************<Prototypes>*******************************
void random_init(void);

uint16_t random(void);


#endif // #ifndef _random_h_
