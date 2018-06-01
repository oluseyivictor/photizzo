#include <still_on_it.h>





unsigned int16 TBL_POINTER_NEW, TBL_POINTER_OLD, SET_FREQ, TBL_POINTER_SHIFT,step,DUTY_CYCLE;
int1 Direction,start;
                  
const unsigned int16 sin_table[64]={0, 25, 49, 73, 98, 121, 145, 168, 191, 214, 236, 257, 278, 298, 317, 336, 354, 370, 
387, 402, 416, 429, 441, 452, 462, 471, 478, 485, 490, 495, 498, 499, 500, 499, 498, 495, 490, 485, 478, 471, 462, 452,
441, 429, 416, 402, 387, 370, 354, 336, 317, 298, 278, 257, 236, 214, 191, 168, 145, 121, 98, 73, 49, 25};


#INT_TIMER2
void timer2_isr()
{
 TBL_POINTER_NEW=TBL_POINTER_OLD +410 ;
 if (TBL_POINTER_NEW < TBL_POINTER_OLD){


           if (Direction == 0){

          
setup_ccp1(CCP_OFF);

setup_ccp2(CCP_PWM);
               
              Direction = 1;

           }

           else{
          
                
             
setup_ccp2(CCP_OFF);

setup_ccp1(CCP_PWM);
                Direction = 0;

           }
 }
 // TBL_POINTER_SHIFT= TBL_POINTER_NEW >> 11;//200     same as divide by 2048 hence 65536/2048 = 32 repeat 5 times
    TBL_POINTER_SHIFT= TBL_POINTER_NEW >> 10;                    //  65536/16
     DUTY_CYCLE= TBL_POINTER_SHIFT;

  set_pwm1_duty(sin_table[DUTY_CYCLE]); 
  set_pwm2_duty(sin_table[DUTY_CYCLE]);
     TBL_POINTER_OLD=TBL_POINTER_NEW;
     clear_interrupt(INT_TIMER2);
 }


void main()
{
set_tris_b(0x00);
set_tris_C(0x00);
 setup_timer_2(T2_DIV_BY_1,124,1);
 
setup_ccp1(CCP_OFF);
setup_ccp2(CCP_OFF);

 
enable_interrupts(INT_TIMER2);
enable_interrupts(GLOBAL);

 
   while(TRUE)
   {


      //TODO: User Code
   }

}
