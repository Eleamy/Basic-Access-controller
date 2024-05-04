#include "gpio.h"
#include "buzzer.h"



void buzzerInit (void){
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);
}

void buzzerToggle (buzzerState state){
	switch (state){
	case(BUZZER_ON):
			GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
			break;
	case(BUZZER_OFF):
			GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
			break;
	default:
			GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
	}
}
