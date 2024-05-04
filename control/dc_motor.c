#include "dc_motor.h"
#include"GPIO.h"
#include "pwm.h"



void DcMotor_Init(void)
{

	GPIO_setupPinDirection( PORTB_ID,PIN1_ID, PIN_OUTPUT);
	GPIO_setupPinDirection( PORTB_ID,PIN0_ID, PIN_OUTPUT);


	GPIO_writePin (PORTB_ID, PIN0_ID, LOGIC_LOW);
	GPIO_writePin (PORTB_ID, PIN1_ID, LOGIC_LOW);
	GPIO_writePin (PORTB_ID, PIN3_ID, LOGIC_LOW);
}

void DcMotor_Rotate(DcMotor_State state,uint8 speed)
{
	switch (state)
	{
		case (STOP):
				GPIO_writePin (PORTB_ID, PIN0_ID, LOGIC_LOW);
				GPIO_writePin (PORTB_ID, PIN1_ID, LOGIC_LOW);
				break;
		case (CW):
				GPIO_writePin (PORTB_ID, PIN0_ID, LOGIC_HIGH);
				GPIO_writePin (PORTB_ID, PIN1_ID, LOGIC_LOW);
				break;
		case (A_CW):
				GPIO_writePin (PORTB_ID, PIN0_ID, LOGIC_LOW);
				GPIO_writePin (PORTB_ID, PIN1_ID, LOGIC_HIGH);
				break;
		default:
				GPIO_writePin (PORTB_ID, PIN0_ID, LOGIC_LOW);
				GPIO_writePin (PORTB_ID, PIN1_ID, LOGIC_LOW);
	}


	Timer0_PWM_Init(speed);
}
