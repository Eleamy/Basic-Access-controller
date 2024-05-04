/*
 * timer.h
 *
 *  Created on: Mar 11, 2024
 *      Author: 7280
 */

#ifndef MCAL_TIMER_H_
#define MCAL_TIMER_H_


#include "std_types.h"


/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum {
	F_CPU_1, F_CPU_8, F_CPU_64, F_CPU_256, F_CPU_1024
}Timer1_Prescaler;


typedef enum {
	OVER_FLOW, COMPARE
}Timer1_Mode;

typedef struct {
 uint16 initial_value;
 uint16 compare_value; // it will be used in compare mode only.
 Timer1_Prescaler prescaler;
 Timer1_Mode mode;
} Timer1_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Intialize timer, setup mode and prescaler vlau
 */
void Timer1_init (const Timer1_ConfigType * Config_Ptr);


void Timer1_deInit(void);


void Timer1_setCallBack(void(*a_ptr)(void));




#endif /* MCAL_TIMER_H_ */
