#include <30F2010.h>
#device ICSP=1
#use delay(clock=8000000,crystal=8000000)

#FUSES NOWDT                    //No Watch Dog Timer
#FUSES CKSFSM                   //Clock Switching is enabled, fail Safe clock monitor is enabled
#FUSES NOBROWNOUT               //No brownout reset



