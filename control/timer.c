#include <avr\io.h>
#include "common_macros.h"
#include <avr/interrupt.h>
#include "timer.h"


static volatile void (*g_callBackPtr)(void) = NULL_PTR;

ISR (TIMER1_OVF_vect)
{
	if (g_callBackPtr != NULL_PTR)
		g_callBackPtr();
}

ISR(TIMER1_COMPA_vect)
{
	if (g_callBackPtr != NULL_PTR)
		g_callBackPtr();
}



void Timer1_init (const Timer1_ConfigType * Config_Ptr)
{

	TCNT1 = Config_Ptr -> initial_value;  /*** SET Timer initial value ***/
	switch (Config_Ptr -> mode)
	{
		case(OVER_FLOW):

			SET_BIT(TIMSK, TOIE1);        /*** Enable overflow interrupt ***/
			break;
		case(COMPARE):
			SET_BIT(TIMSK, OCIE1A);       /*** Enable compare interrupt ***/
			SET_BIT(TCCR1B, WGM12);		  /*** SET WaveForm generation mode to CTC (clear timer on compare match) ***/
			OCR1A = Config_Ptr -> compare_value;   /*** SET Timer compare value ***/
			break;

	}
	switch (Config_Ptr -> prescaler)		/*** PRESCALER-SELECT (FCPU, FCPU/8, FCPU/64, FCPU/256, FCPU/1024) ***/
	{
		case(F_CPU_1):
			SET_BIT(TCCR1B, CS10);
			CLEAR_BIT(TCCR1B, CS11);
			CLEAR_BIT(TCCR1B, CS12);
			break;
		case (F_CPU_8):
			SET_BIT(TCCR1B, CS10);
			CLEAR_BIT(TCCR1B, CS11);
			CLEAR_BIT(TCCR1B, CS12);
			break;
		case (F_CPU_64):
			SET_BIT(TCCR1B, CS10);
			SET_BIT(TCCR1B, CS11);
			CLEAR_BIT(TCCR1B, CS12);
			break;
		case (F_CPU_256):
			SET_BIT(TCCR1B, CS10);
			CLEAR_BIT(TCCR1B, CS11);
			CLEAR_BIT(TCCR1B, CS12);
			break;
		case (F_CPU_1024):
			SET_BIT(TCCR1B, CS10);
			CLEAR_BIT(TCCR1B, CS11);
			SET_BIT(TCCR1B, CS12);
			break;



	}
}

void Timer1_deInit(void)
{
	TIMSK = 0;			/*** Clear Timer interrupts ***/
	OCR1A = 0;			/*** clear compare register ***/
	TCNT1 = 0;			/*** Clear Timer initial value ***/
	CLEAR_BIT(TCCR1B, WGM12);	/*** Clear waveform generation mode ***/
}

void Timer1_setCallBack(void(*a_ptr)(void))
{
	g_callBackPtr = a_ptr; 			/*** Setup call-back pointer ***/
}







