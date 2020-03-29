#ifndef _GIMBAL_TASK_H
#define _GIMBAL_TASK_H

#include "main.h"

#define GIM_YAW_MAX_SCALE    	5703
#define GIM_YAW_INIT_SCALE   	7061
#define GIM_YAW_MIN_SCALE   	5303

#define GIM_PIT_MAX_SCALE    	9562
#define GIM_PIT_INIT_SCALE   	3036
#define GIM_PIT_MIN_SCALE     7962


#define GIM_YAW_MAX_DEGREE    30
#define GIM_YAW_MIN_DEGREE    -30

#define GIM_PIT_MAX_DEGREE    33
#define GIM_PIT_MIN_DEGREE    0

#define GIM_YAW_TURN_ANGLE		40

void Gimbal_Act_Cal(void);
void Gimbal_Armor_Cal(void);
void Gimbal_Cease_Cal(void);
void Gimbal_Turnhead_Cal(void);
void Gimbal_Single_Cal(void);

void Pid_YawGyroPosSpeed(void);
void Pid_PitchGyroPosSpeed(void);
void Pid_Pitch_MotorPos_Yaw_GyroPos(void);
void Pid_Yaw_MotorPos(void);
void Gimbal_LimitCalc(void);
void Gimbal_Single_Cal(void);
void GimbalCur_Pid_Cal(void);

void GIMBAL_task(void *pvParameters);
#endif
