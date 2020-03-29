#define ACTION
#include "freertosinc.h"

/**
  * @brief  控制模式
  * @param   
  * @retval  
  */

void SetInputMode(RC_Ctl_t* RC_Ctl)
{
	switch(RC_Ctl->rc.s2)
	{
		case 1:HeroStatus.CtrMode=MouseKeyMode;break;
		case 2:HeroStatus.CtrMode=CeaseMode;break;
		case 3:HeroStatus.CtrMode=RemoteMode;break;
	}
}

/** @brief  断电模式
   *@param  None
   *@retval None
*/
short ShutdownCnt = 0;
void Powerdown_Process(void)
{
	if(Mouse_Key_Flag != 2)
	{
//		PC_Send.Type = 0x00;
		Mouse_Key_Flag = 2;
	}
	HeroStatus.GimbalMode = Gimbal_CeaseMode;
	HeroStatus.ChassisMode = Chassis_CeaseMode;
	HeroStatus.ShootMode = Shoot_CeaseMode;
//	if((RC_Ctl.rc.ch0 < 400) && (RC_Ctl.rc.ch2 > 1500))
//		ShutdownCnt++;
//	if(ShutdownCnt > 100)
//		PC_Send.Type = 0x30; 
}

/** @brief  遥控器模式
   *@param  None
   *@retval None
*/
void Remote_Process(RC_Ctl_t* RC_Ctl)
{
	if(Mouse_Key_Flag != 3)
	{
		Mouse_Key_Flag = 3;
		//PC_Send.Type = 0x50;
		HeroStatus.GimbalMode  = Gimbal_Act_Mode;
		HeroStatus.ChassisMode = Chassis_Act_Mode;
		HeroStatus.ShootMode = Shoot_Act_Mode;
		
	}
	chassis.SuperCapCmd = 0;
	switch(RC_Ctl->rc.s1)
	{
		case 2:
		   HeroStatus.ShootMode = Shoot_Act_Mode;
		break;
			
		case 1:
			 HeroStatus.ShootMode = Shoot_CeaseMode;
		break;
		default:
		break;
	}
}

/** @brief  键鼠模式
						w,s,a,d					前后左右行进
						shift						电容加速
						ctrl						扭腰
						press_l					射击（单击）
						press_r					辅瞄
						q, e						左旋，右旋
						x								手动补弹
						b								对接（单击）
						g								单轴
						v								紧急解除热量限制
						z								使用自动加速
						r								慢速爬坡
   *@param  None
   *@retval None
*/
void Mouse_Key_Process(RC_Ctl_t* RC_Ctl)
{
	if(Mouse_Key_Flag != 1)
	{
		HeroStatus.GimbalMode = Gimbal_Act_Mode;
		HeroStatus.ChassisMode = Chassis_Act_Mode;
		HeroStatus.ShootMode = Shoot_Act_Mode;
		//PC_Send.Type = 0x50;
		Mouse_Key_Flag = 1;
	}
//	
///**********************************超级电容控制*****************************/
//	if(RC_Ctl->key.shift == 1)
//		chassis.SuperCapCmd = 1;
//	else
//		chassis.SuperCapCmd = 0;
//	
//	key_status.z_rise_flag = RC_Ctl->key.z - key_status.pre_key_z;
//	key_status.pre_key_z = RC_Ctl->key.z;
//	if(key_status.z_rise_flag == 1)
//	{
//		(superCapAutoFlag == 1)?(superCapAutoFlag = 0):(superCapAutoFlag = 1);
//	}
//	
/**********************************慢速爬坡*****************************/
	if(RC_Ctl->key.r == 1)
		slowFac = 0.2;
	else
		slowFac = 1;
/**********************************陀螺控制*****************************/	
	if(RC_Ctl->key.ctrl == 1)
	{
		if(HeroStatus.ChassisMode == Chassis_Act_Mode)
			HeroStatus.ChassisMode = Chassis_SelfProtect_Mode;
	}
	else
	{
		if(HeroStatus.ChassisMode == Chassis_SelfProtect_Mode)
			HeroStatus.ChassisMode = Chassis_Act_Mode;
	}
	
/**********************************发射控制*****************************/	
	switch(RC_Ctl->rc.s1)
	{
		case 2:
		   HeroStatus.ShootMode = Shoot_Act_Mode;
		break;
			
		case 1:
			 HeroStatus.ShootMode = Shoot_CeaseMode;
		break;
		default:
		break;
	}
/**********************************辅瞄控制*****************************/	
	if(RC_Ctl->mouse.press_r == 1)
	{
		if(HeroStatus.GimbalMode == Gimbal_Act_Mode)
		{
			Laser_OFF;
			HeroStatus.GimbalMode = Gimbal_Armor_Mode;
//			PC_Send.Type = 0x10;
		}
	}
	else
	{
		if(HeroStatus.GimbalMode == Gimbal_Armor_Mode)
		{
			Laser_ON;
			HeroStatus.GimbalMode = Gimbal_Act_Mode;
//			PC_Send.Type = 0x50;
		}
	}
/**********************************转头180度补弹*****************************/
	key_status.b_rise_flag = RC_Ctl->key.b - key_status.pre_key_b;
	key_status.pre_key_b = RC_Ctl->key.b;
	if(key_status.b_rise_flag == 1)
	{
		if((HeroStatus.ChassisMode == Chassis_Act_Mode) && (HeroStatus.GimbalMode == Gimbal_Act_Mode))
		{   
			Laser_OFF;
			HeroStatus.ShootMode = Shoot_CeaseMode;
			HeroStatus.ChassisMode = Chassis_Single_Mode;
			HeroStatus.GimbalMode = Gimbal_TurnHeadMode;
		}
		else if((HeroStatus.ChassisMode == Chassis_Single_Mode) && (HeroStatus.GimbalMode == Gimbal_TurnHeadMode))
			turnHeadExitFlag = 1;
	}
	
	/**********************************切换单轴模式*****************************/
	if(RC_Ctl->key.g == 1)
	{
		if((HeroStatus.ChassisMode == Chassis_Act_Mode) && (HeroStatus.GimbalMode == Gimbal_Act_Mode))
		{
			HeroStatus.ChassisMode = Chassis_Single_Mode;
			HeroStatus.GimbalMode = Gimbal_SingleMode;
		}
	}
	else
	{
		if((HeroStatus.ChassisMode == Chassis_Single_Mode) && (HeroStatus.GimbalMode == Gimbal_SingleMode))
		{
			HeroStatus.ChassisMode = Chassis_Act_Mode;
			HeroStatus.GimbalMode = Gimbal_Act_Mode;
		}
	}

}
uint32_t ActionControl_high_water;

void ActionControl_task(void *pvParameters)
{
	 
   while (1) {
    
		SetInputMode(&RC_Ctl);
	  switch(HeroStatus.CtrMode)
					{
						case MouseKeyMode:
							Mouse_Key_Process(&RC_Ctl);
						break;
						case RemoteMode:
							Remote_Process(&RC_Ctl);
						break;
						case CeaseMode:
							Powerdown_Process();
						break;
						default:
							break;
			    }			 
		
    vTaskDelay(2); /* 延时 500 个 tick */
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        ActionControl_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

