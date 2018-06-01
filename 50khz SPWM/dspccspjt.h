#include <30F2010.h>
#device ICSP=1
#use delay(internal=8000000)

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES CKSFSM                	//Clock Switching is enabled, fail Safe clock monitor is enabled
#FUSES NOBROWNOUT            	//No brownout reset


#define LED PIN_B0
#define DELAY 1000

