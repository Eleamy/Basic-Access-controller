#include "dc_motor.h"
#include "external_eeprom.h"
#include "buzzer.h"
#include "uart.h"
#include "twi.h"
#include "timer.h"
#include "util/delay.h"


/*TIMER1 initial value*/
#define TIMER1_INTIAL_VALUE 0
/*TIMER1 compare ticks*/
#define TICKS_TO_5_SECOND 39062
/*EEPROM Clear */
#define EEPROM_CLEAR ((uint8)0)
/* UART option select code */
#define RECIEVE_PASSWORD 10
#define CHANGE_PASSWORD '-'
#define OPEN_DOOR '+'
#define ACKNOWLEDGE 1
#define WRONG_PASS ((uint8)0)
/* error codes */
#define TOO_MANY_ATTEMPTS 0x0F
/* NULL pointer */
#define NULL ((void*)0)


uint16 eeprom_address = 0x0001;      /*EEPROM initial byte address*/
UART_ConfigType config_control = {ONE_STOP_BIT, 8, NO_PARITY, 9600};      /*UART config-stuct (NO. of stop bits, number of bits, parity, baud-rate) */
TWI_ConfigType twi_config = {0x01, 400};	/*I2C config-struct (I2C address, Kbps) */
Timer1_ConfigType timer_config = {TIMER1_INTIAL_VALUE, TICKS_TO_5_SECOND, F_CPU_1024, COMPARE};	/*timer1 config-struct (inital value, compare value, pre-scaler, mode) */


/* save password function
 *
 * for saving password in EEPROM
 * PARAM1: uint16 address (contains initial I2C address of EEPROM + byte address)
 * PARAM2: uint16 password (contains password info sent via UART)
 *
 */
void savePassword (uint16 addr, uint16 password)
{
	uint8* ptr_to_password = ((uint8*)&password);

	EEPROM_writeByte(addr, EEPROM_CLEAR);
	_delay_ms(10);
	EEPROM_writeByte(addr, *ptr_to_password);
	_delay_ms(10);
	ptr_to_password++;
	addr++;
	EEPROM_writeByte(addr, EEPROM_CLEAR);
	_delay_ms(10);
	EEPROM_writeByte(addr, *ptr_to_password);
	_delay_ms(10);

}

/* save password function
 *
 * for receiving 2-bytes password bytes via UART password in EEPROM
 * RETURN:uint16 password
 *
 */
uint16 recivePassword (void){
	uint16 password;
	uint8* ptr_to_password =(uint8*) &password;

	*ptr_to_password = UART_recieveByte();
	ptr_to_password++;
	*ptr_to_password = UART_recieveByte();

	return password;
}

/* check password function
 *
 * for reading password from EEPROM and checking if it equals the password sent via UART
 * PARAM1: uint16 address (contains initial I2C address of EEPROM + byte address)
 * PARAM2: uint16 password (contains password info sent via UART)
 * RETURN: uint8 boolean value (TRUE on password match, FALSE on mismatch)
 *
 */
uint8 checkPassword (uint16 addr, uint16 password1){

	uint16 password2;
	uint8* ptr_to_password =(uint8*) &password2;

	EEPROM_readByte(addr, ptr_to_password);
	_delay_ms(10);
	ptr_to_password++;
	addr++;
	EEPROM_readByte(addr, ptr_to_password);
	_delay_ms(10);

	if (password1 == password2){
		return TRUE;
	}
	else{
		return FALSE;
	}
}

/* open door function
 *
 * for controlling DC_Motor sequence to open the door (15 seconds clock-wise, 3 seconds stop, 15 seconds anti_clockwise)
 *
 */
void openDoor (void){
	DcMotor_Rotate(CW, FULL_SPEED);
	_delay_ms(15000);
	DcMotor_Rotate(STOP, HALT);
	_delay_ms(3000);
	DcMotor_Rotate(A_CW, FULL_SPEED);
	_delay_ms(15000);
	DcMotor_Rotate(STOP, HALT);
}

/* error action function
 *
 * runs in case of error
 * toggles the buzzer on for 60 seconds
 *
 */
void errorAction (void){
	buzzerToggle(BUZZER_ON);
	_delay_ms(6000);
	buzzerToggle(BUZZER_OFF);
}


int main (void){
	DcMotor_Init();
	buzzerInit();
	UART_init(&config_control);
	TWI_init(&twi_config);

	uint16 uart_buffer = 0;
	uint8 i =0;

	while (TRUE){
		uart_buffer = UART_recieveByte();
		switch(uart_buffer){
		case RECIEVE_PASSWORD:
			uart_buffer = recivePassword();
			savePassword(eeprom_address, uart_buffer);
		break;

		case OPEN_DOOR:
			for (i =0; i<3; i++){
				uart_buffer = recivePassword();
				if (checkPassword(eeprom_address, uart_buffer)){
					openDoor();
					UART_sendByte(ACKNOWLEDGE);
					break;
				}
				else{
					UART_sendByte(WRONG_PASS);
				}
			}
		break;
		case CHANGE_PASSWORD:
			for (i =0; i<3; i++){
				uart_buffer = recivePassword();
				if (checkPassword(eeprom_address, uart_buffer)){
					UART_sendByte(ACKNOWLEDGE);
					break;
				}
				else{
					UART_sendByte(WRONG_PASS);
				}
			}
		break;

		case TOO_MANY_ATTEMPTS:
			errorAction();
		break;

		}
	}
}


