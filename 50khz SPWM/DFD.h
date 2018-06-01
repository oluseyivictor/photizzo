#include <30F2010.h>
#device ICSP=1
#use delay(crystal=20000000)

#FUSES NOWDT                 	//No Watch Dog Timer
#FUSES CKSFSM                	//Clock Switching is enabled, fail Safe clock monitor is enabled
#FUSES NOBROWNOUT            	//No brownout reset


#define LED PIN_B5
#define DELAY 1000

