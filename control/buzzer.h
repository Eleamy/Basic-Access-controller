/*
 * buzzer.h
 *
 *  Created on: Mar 24, 2024
 *      Author: 7280
 */

#ifndef HAL_BUZZER_H_
#define HAL_BUZZER_H_


#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT  PORTD_ID
#define BUZZER_PIN   PIN7_ID


/*******************************************************************************
 *                                DataType                                     *
 *******************************************************************************/

typedef enum {
	BUZZER_OFF, BUZZER_ON
} buzzerState;


/*******************************************************************************
 *                           Functions Prototypes                               *
 *******************************************************************************/


void buzzerInit(void);


void buzzerToggle(buzzerState state);



#endif /* HAL_BUZZER_H_ */
