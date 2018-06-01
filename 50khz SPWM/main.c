#include <main.h>


void main()
{

	setup_timer2(TMR_INTERNAL | TMR_DIV_BY_1, 13);
	setup_timer3(TMR_INTERNAL | TMR_DIV_BY_1, 87);

	setup_compare(1, COMPARE_PWM | COMPARE_TIMER3);
	set_pwm_duty(1, 43);


	while(TRUE)
	{
		//TODO: User Code
	}

}
