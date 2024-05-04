

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

#define HALT 0
#define QUARTER_SPEED 64
#define HALF_SPEED 128
#define THREE_QUARTER_SPEED 192
#define FULL_SPEED 255
typedef enum
{
	STOP,CW,A_CW
}DcMotor_State;


void DcMotor_Init(void);
void DcMotor_Rotate(DcMotor_State state,uint8 speed);


#endif /* DC_MOTOR_H_ */
