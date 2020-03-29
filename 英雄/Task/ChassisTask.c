#define CLASS
#include "freertosinc.h"
/**
  * @brief  底盘正常动作
  * @param   
  * @retval  
  */
		int	YawOutPut;
void Chassis_Act_Cal(void)
{

  float theta;
	YawOutPut = ZeroCheck_YawOutPut();
	if(ChassisInitFlag != 1)
	{
		ChassisInitFlag = 1;
		chassis.PowerdownCmd = 0;
		PidChassisAngle.SetPoint = GIM_YAW_INIT_SCALE;
	}
	if(HeroStatus.CtrMode == RemoteMode)
	{
		chassis.carSpeedx = -(RC_Ctl.rc.ch0 - 1024) * RC_Multiple;
		chassis.carSpeedy = (RC_Ctl.rc.ch1 - 1024) * RC_Multiple;
	}
	if(HeroStatus.CtrMode == MouseKeyMode)
	{
		chassis.carSpeedx = (RC_Ctl.key.a - RC_Ctl.key.d) * KEY_Multiple * (-1.5f) * slowFac;			
		chassis.carSpeedy = (RC_Ctl.key.w -	RC_Ctl.key.s) * KEY_Multiple * 1.2f * slowFac;				
	}
		
	PidChassisAngle.ActPoint = ZeroCheck_YawOutPut();
	theta = ((float)((YawOutPut- GIM_YAW_INIT_SCALE) % 8192) / 8192) * (2 * 6.28318f);
		
	//转弯减速，旋转加速
	chassis.carSpeedx = chassis.carSpeedx * expf(-(ABS(theta)/1.0f) * (ABS(theta)/1.0f));
	chassis.carSpeedy = chassis.carSpeedy * expf(-(ABS(theta)/1.0f) * (ABS(theta)/1.0f));
		
	chassis.carSpeedw = PID_Calc(&PidChassisAngle) * RC_Multiple;
	chassis.carSpeedw = LIMIT_MAX_MIN(chassis.carSpeedw,1000,-1000);
}
/**
  * @brief  底盘小陀螺
  * @param   
  * @retval  
  */
void Chassis_SelfProtect_Cal()
{
//	if(HeroStatus.CtrMode == MouseKeyMode)
//	{
//		chassis.carSpeedx = (RC_Ctl.key.a - RC_Ctl.key.d) * KEY_Multiple * (-1.5f) * slowFac;			
//		chassis.carSpeedy = (RC_Ctl.key.w -	RC_Ctl.key.s) * KEY_Multiple * 1.2f * slowFac;		
//		chassis.carSpeedw	= 300;	
//	}
	
}
/**
  * @brief  底盘单轴模式
  * @param   
  * @retval  
  */
void Chassis_Single_Cal(void)
{
	if(ChassisInitFlag != 3)
	{
		ChassisInitFlag = 3;
		chassis.PowerdownCmd = 0;
		PidChassisAngle.SetPoint = ZeroCheck_YawOutPut();
	}
	
	if(HeroStatus.CtrMode == MouseKeyMode)
	{
		if(HeroStatus.GimbalMode == Gimbal_TurnHeadMode)
		{
			chassis.carSpeedx = (RC_Ctl.key.a - RC_Ctl.key.d) * KEY_Multiple ;
			chassis.carSpeedy = (RC_Ctl.key.w -	RC_Ctl.key.s) * KEY_Multiple * (-1.0f);
			chassis.carSpeedw = (RC_Ctl.key.q - RC_Ctl.key.e) * 200 - LIMIT_MAX_MIN(RC_Ctl.mouse.x * 5,200,-200);
		}
		else
		{
			chassis.carSpeedx = (RC_Ctl.key.d - RC_Ctl.key.a) * KEY_Multiple ;
			chassis.carSpeedy = (RC_Ctl.key.s -	RC_Ctl.key.w) * KEY_Multiple * (-1.0f);
			chassis.carSpeedw = (RC_Ctl.key.q - RC_Ctl.key.e) * 200 - LIMIT_MAX_MIN(RC_Ctl.mouse.x * 5,200,-200);
		}
	}
	
	if(HeroStatus.CtrMode == RemoteMode)
	{
		chassis.carSpeedx = -(RC_Ctl.rc.ch0 - 1024) * RC_Multiple;
		chassis.carSpeedy = (RC_Ctl.rc.ch1 - 1024) * RC_Multiple;
		chassis.carSpeedw = (RC_Ctl.rc.ch2 - 1024) * RC_Multiple;
	}
}
/**
  * @brief  底盘掉电模式
  * @param   
  * @retval  
  */
void Chassis_Cease_Cal(void)
{
	if(ChassisInitFlag != 3)
		ChassisInitFlag = 3;
	
	chassis.carSpeedx = 0;
	chassis.carSpeedy = 0;
	chassis.carSpeedw = 0;
	chassis.PowerdownCmd = 1;
	chassis.SuperCapCmd = 0;
	
}

/**
  * @brief  底盘跟随pid初始化 
  * @param   
  * @retval  
  */

void Pid_ChassisAngle(void)
{
	PidChassisAngle.SetPoint = 0;
	PidChassisAngle.P = 0.2;//0.4f;
	PidChassisAngle.I = 0.0001;
	PidChassisAngle.D = 0;
	PidChassisAngle.IMax = 100.0f;
	
	PidChassisSpeed.SetPoint = 0;
	PidChassisSpeed.P = 1;
	PidChassisSpeed.I = 0.0f;
	PidChassisSpeed.D = 0;
	PidChassisSpeed.IMax = 100.0f;
}
/**
  * @brief  底盘数据发送
  * @param   
  * @retval  
  */

void Chassis_Cal(void)
{
//	if(HeroStatus.ChassisMode != Chassis_CeaseMode)
//	{
//		if((superCapAutoFlag == 1) && (HeroStatus.ChassisMode == Chassis_Act_Mode))
//		{
//			if(ABS(chassis.carSpeedw) > 500)
//				chassis.SuperCapCmd = 1;
//			if(ABS(chassis.carSpeedy) > 200)
//				chassis.CapStartCnt ++;
//			else
//				chassis.CapStartCnt = 0;
//			
//			if((chassis.CapStartCnt>100) && (chassis.CapStartCnt<800))
//				chassis.SuperCapCmd = 1;
//			else if(chassis.CapStartCnt >= 800)
//				chassis.CapStartCnt = 800;
//		}
//	  chassis.carSpeedw = 0;
		CAN1_ChassisSend(&chassis,0);//Juge.Power_Heat.Chassis_Remain_Power
//	}
}
uint32_t Chassis_high_water;

void CHASSIS_task(void *pvParameters)
{
   Pid_ChassisAngle();
	 chassis.carSpeedx = 0;
	 chassis.carSpeedy = 0;
	 chassis.carSpeedw = 0;
   while (1) {
    ZeroCheck_cal();
		if(HeroStatus.ChassisMode == Chassis_CeaseMode)
			Chassis_Cease_Cal();
		else if(HeroStatus.ChassisMode == Chassis_Act_Mode)
      Chassis_Act_Cal();
		else if(HeroStatus.ChassisMode == Chassis_Single_Mode)
			Chassis_Single_Cal();
		else if(HeroStatus.ChassisMode == Chassis_SelfProtect_Mode)
			Chassis_SelfProtect_Cal();
		Chassis_Cal();
    vTaskDelay(1); /* 延时 500 个 tick */
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        Chassis_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
