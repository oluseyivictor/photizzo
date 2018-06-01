#include <oscillator.h>




unsigned int16 TBL_POINTER_NEW, TBL_POINTER_OLD, SET_FREQ, TBL_POINTER_SHIFT,step,DUTY_CYCLE;
int1 Direction,start;
                  
const unsigned char sine[64]={0, 5, 10, 15, 20, 24, 29, 34, 38, 43, 47, 51, 56, 60, 63, 67, 71, 74, 77, 80, 83, 86, 88, 90, 92, 94, 96, 97, 98, 99, 100, 100, 100, 100, 100, 99, 98, 97, 96, 94, 92, 90, 88, 86, 83, 80, 77, 74, 71, 67, 63, 60, 56, 51, 47, 43, 38, 34, 29, 24, 20, 15, 10, 5};
#INT_TIMER2
void timer2_isr()
{
 TBL_POINTER_NEW=TBL_POINTER_OLD +128 ;
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

  set_pwm1_duty(sine[DUTY_CYCLE]); 
  set_pwm2_duty(sine[DUTY_CYCLE]);
     TBL_POINTER_OLD=TBL_POINTER_NEW;
     clear_interrupt(INT_TIMER2);
 }


void main()
{
set_tris_b(0x00);
 setup_timer_2(T2_DIV_BY_1,116,1);
 
setup_ccp1(CCP_OFF);
setup_ccp2(CCP_OFF);

 
enable_interrupts(INT_TIMER2);
enable_interrupts(GLOBAL);

 
   while(TRUE)
   {


      //TODO: User Code
   }

}
