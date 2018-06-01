
#include <16F876A.h>
#device ADC=10
#use delay(crystal=16000000)
//#use rs232(baud=9600,parity=N,xmit=PIN_C6,rcv=PIN_C7,bits=8,stream=PORT1)

#bit BUTTON=0x006.0
#bit BUZZER=0x007.5
#bit POWER=0x007.0
#bit FAN1=0x007.1
#bit FAN2=0x007.2
#bit OVERLOAD=0x006.7
#bit BATT_LOW=0x006.6
