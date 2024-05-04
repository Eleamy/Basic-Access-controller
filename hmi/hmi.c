#include "keypad.h"
#include "uart.h"
#include "lcd.h"
#include "util\delay.h"

/* keypad enter key */
#define ENTER_KEY 13
/* number of times password is entered */
#define FIRST_TIME 0
#define SECOND_TIME 1
/* error codes */
#define TOO_MANY_ATTEMPTS 0x0F
/* uart option select code */
#define SEND_PASSWORD 10
#define ACKNOWLEDGE 1
#define WRONG_PASS 0
#define OPEN_DOOR '+'
#define CHANGE_PASSWORD '-'
/* NULL pointer */
#define NULL ((void*)0)

/** Intializing UART driver configuration **/
UART_ConfigType config_hmi = {ONE_STOP_BIT, 8, NO_PARITY, 9600};


/* Positive power function
 *
 * for calculating positive integer powers
 * takes base integer and power
 * returns base raised to power
 *
 */
uint32 positve_pow (uint16 base, uint8 power)
{
	uint32 result = 1, i;
	if (power == 0)
		return 1;
	for (i=0; i<power; i++)
	{
		result *= base;
	}
	return result;
}

/* enter password function
 *
 * for entering password steps and displaying on LCD
 * takes pointer to password and entry time
 *
 */

void enterPassowrd (uint16* u16_ptr, uint8 query)
{
	sint8 i;
	uint8 key_stroke;

	if (query == FIRST_TIME){
		LCD_displayStringRowColumn(0, 0, "plz enter pass");
		LCD_moveCursor(1,0);
	}

	else{
		LCD_displayStringRowColumn(0, 0, "plz re-enter the ");
		LCD_displayStringRowColumn(1, 0, "same pass: ");
	}


	for (i =4; i>=0; i--)
	{
		key_stroke = KEYPAD_getPressedKey_onRelease();
		if (key_stroke != ENTER_KEY){
			LCD_displayCharacter('*');
		}
		else
			break;
		*u16_ptr += positve_pow(10, (i)) * key_stroke;
	}

}

/* send password
 *
 * for sending password to control_Uc via UART
 * takes uint16 password
 *
 */

void sendPassword (uint16 password)
{
	uint8* ptr_to_password = NULL;

	ptr_to_password = ((uint8*)&password);
	UART_sendByte(*ptr_to_password);
	_delay_ms(10);
	ptr_to_password++;
	UART_sendByte(*ptr_to_password);
}

/* set password function
 *
 * for setting or changing passwords for a specific user, displaying it on the LCD module and sending it via sendPassword function
 *
 */
void setPassword(void){
	uint16 password1 = 0;
	uint16 password2 = 0;

	while (TRUE){
		LCD_clearScreen();
		enterPassowrd(&password1, FIRST_TIME);
		LCD_clearScreen();
		enterPassowrd(&password2, SECOND_TIME);

		if (password1 == password2){
			UART_sendByte(SEND_PASSWORD);
			sendPassword(password1);
			break;
		}
		password1 = password2 = 0;
		LCD_clearScreen();
	}
}


/* options menu function
 *
 * for displaying options menu on LCD and sending options via UART to control_Uc
 *
 */
void optionsMenu (void){
	uint16 password1 = 0;
	uint8  option, uart_confirmation = 0, password_flag = 0;

	while (TRUE){

		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"+ : Open Door");
		LCD_displayStringRowColumn(1,0,"- : Change Pass");
		option = KEYPAD_getPressedKey_onRelease();

		switch (option){

		case '+':
			UART_sendByte(OPEN_DOOR);
			for (password_flag=0; password_flag < 3; password_flag++){
				LCD_clearScreen();
				enterPassowrd(&password1, FIRST_TIME);
				sendPassword(password1);
				password1 = 0;
				uart_confirmation = UART_recieveByte();
//				uart_confirmation = 0;

				if (uart_confirmation){
					break;
				}

			}

			if (password_flag>=3){
				LCD_clearScreen();
				LCD_displayString("TOO MANY WRONG");
				LCD_displayStringRowColumn(1, 0, "ATTEMPTS!!!");
				UART_sendByte(TOO_MANY_ATTEMPTS);
				_delay_ms(6000);
			}
			break;

		case '-':
			UART_sendByte(CHANGE_PASSWORD);
			for (password_flag=0; password_flag < 3; password_flag++){
				LCD_clearScreen();
				enterPassowrd(&password1, FIRST_TIME);
				sendPassword(password1);
				password1 = 0;
				uart_confirmation = UART_recieveByte();

				if (uart_confirmation){
					setPassword();
					break;
				}

			}

			if (password_flag>=3){
				LCD_clearScreen();
				LCD_displayString("TOO MANY WRONG");
				LCD_displayStringRowColumn(1, 0, "ATTEMPTS!!!");
				_delay_ms(6000);
				UART_sendByte(TOO_MANY_ATTEMPTS);
			}
			break;
		}

	}
}


int main (void){
	LCD_init();   /* initializing  LCD module*/
	UART_init(&config_hmi);  /* initializing  UART communication module*/

	setPassword();

	optionsMenu();
}
