#ifndef __MAIN_H__
#define __MAIN_H__

#define ABS(x) ((x)>0? (x):(-(x))) 
#define LIMIT_MAX_MIN(x, max, min)	(((x) <= (min)) ? (min):(((x) >= (max)) ? (max) : (x)))  //????

//Standard Lib
#include <stm32f4xx.h>	 
#include <stm32f4xx_conf.h>
#include <string.h>
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "BSPconfig.h"
/***********************板级支持包**************************/
#include "tim.h"
#include "can1.h"
#include "can2.h"
#include "usart1.h"
#include "usart4.h"
#include "uart5.h"
#include "usart2.h"
#include "iwdg.h"
/************************零件********************************************/
#include "Remote.h"
#include "Gyro.h"
#include "DeBug.h"
#include "BuleTooth.h"
/************************数据传输*************************/
#include "CANSendTask.h"
#include "CANReceiveTask.h"
#include "ZeroCheckTask.h"
/************************总控制*********************************/
#include "ActionControlTask.h"
/************************部位****************************************/
#include "GimbalTask.h"
#include "ShootTask.h"
#include "ChassisTask.h"
#include "Jugement.h"
#include "miniPC.h"
/************************任务****************************************/
#include "Start_Task.h"
/*************************ASSIST************************/
#include "pid.h"
#include "DataScope_DP.h"
#include "algorithmOfCRC.h"


void delay_ms(unsigned int t);
#endif
