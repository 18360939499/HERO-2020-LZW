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
/***********************�弶֧�ְ�**************************/
#include "tim.h"
#include "can1.h"
#include "can2.h"
#include "usart1.h"
#include "usart4.h"
#include "uart5.h"
#include "usart2.h"
#include "iwdg.h"
/************************���********************************************/
#include "Remote.h"
#include "Gyro.h"
#include "DeBug.h"
#include "BuleTooth.h"
/************************���ݴ���*************************/
#include "CANSendTask.h"
#include "CANReceiveTask.h"
#include "ZeroCheckTask.h"
/************************�ܿ���*********************************/
#include "ActionControlTask.h"
/************************��λ****************************************/
#include "GimbalTask.h"
#include "ShootTask.h"
#include "ChassisTask.h"
#include "Jugement.h"
#include "miniPC.h"
/************************����****************************************/
#include "Start_Task.h"
/*************************ASSIST************************/
#include "pid.h"
#include "DataScope_DP.h"
#include "algorithmOfCRC.h"


void delay_ms(unsigned int t);
#endif
