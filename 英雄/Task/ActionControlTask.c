#define ACTION
#include "freertosinc.h"

/**
  * @brief  ����ģʽ
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

/** @brief  �ϵ�ģʽ
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

/** @brief  ң����ģʽ
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

/** @brief  ����ģʽ
						w,s,a,d					ǰ�������н�
						shift						���ݼ���
						ctrl						Ť��
						press_l					�����������
						press_r					����
						q, e						����������
						x								�ֶ�����
						b								�Խӣ�������
						g								����
						v								���������������
						z								ʹ���Զ�����
						r								��������
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
///**********************************�������ݿ���*****************************/
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
/**********************************��������*****************************/
	if(RC_Ctl->key.r == 1)
		slowFac = 0.2;
	else
		slowFac = 1;
/**********************************���ݿ���*****************************/	
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
	
/**********************************�������*****************************/	
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
/**********************************�������*****************************/	
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
/**********************************תͷ180�Ȳ���*****************************/
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
	
	/**********************************�л�����ģʽ*****************************/
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
		
    vTaskDelay(2); /* ��ʱ 500 �� tick */
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        ActionControl_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}

