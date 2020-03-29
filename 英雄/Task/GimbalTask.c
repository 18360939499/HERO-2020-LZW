#define GIMBAL
#include "freertosinc.h"
/**********************************************************************************************************
*�� �� ��: Gimbal_Act_Cal
*����˵��: ��̨�����˶�ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Gimbal_Act_Cal(void)
{
	
	if(GimbalInitFlag != 1)
	{
		Gimbal_LimitCalc();
		GimbalPitPos = Gyro.Pitch;
		GimbalYawPos = Gyro.Yaw;
		bigBulletGetFlag = 0;
		bigBulletGetCnt =0;
		GimbalInitFlag = 1;
	}
	
	if(HeroStatus.CtrMode == RemoteMode)
	{
		GimbalPitPos -= (RC_Ctl.rc.ch3 - 1024) * 0.00005f;
		GimbalYawPos += (RC_Ctl.rc.ch2 - 1024) * 0.00003f;
	}
	if(HeroStatus.CtrMode == MouseKeyMode)
	{
		GimbalPitPos += RC_Ctl.mouse.y * (-0.01f);
		GimbalYawPos += RC_Ctl.mouse.x * 0.01f;
	}
	
	if(RC_Ctl.key.q == 1)
		GimbalYawPos -= 0.2f;
	if(RC_Ctl.key.e == 1)
		GimbalYawPos += 0.2f;
	
	
	//��λ
	GimbalPitPos = LIMIT_MAX_MIN(GimbalPitPos,pitchDegreeMax,pitchDegreeMin);
	//GimbalYawPos = LIMIT_MAX_MIN(GimbalYawPos,yawDegreeMax,yawDegreeMin);
	PidPitchPos.SetPoint = GimbalPitPos;
	PidYawPos.SetPoint   = GimbalYawPos;
}

/**********************************************************************************************************
*�� �� ��: Gimbal_Armor_Cal
*����˵��: ��̨����ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Gimbal_Armor_Cal(void)
{
	float Recent_angle = 0;
	if(GimbalInitFlag != 2)
	{
		GimbalPitPos = ZeroCheck_PitchOutPut();
		GimbalYawPos = Gyro.Yaw;
//		PC_Recv.PCDisconnect = 1;
		GimbalInitFlag = 2;
	}
	
//	Recent_angle = -PC_Recv.RCYaw;
	while(ABS(Gyro.Yaw - Recent_angle)>=180 )
	{	
		if(Gyro.Yaw<0)
		{
			Recent_angle = Recent_angle - 360;  
		}
		else
		{
			Recent_angle = Recent_angle + 360;
		}
	}
//	if(PC_Recv.PCDisconnect != 1)
//	{
//		PC_Recv.PCDisconnect = 1;
//		GimbalPitPos = (GIM_PIT_INIT_SCALE-(-PC_Recv.RCPitch/360.0f)*8192);
//		GimbalYawPos  = Recent_angle;
//		GimbalPitPos = LIMIT_MAX_MIN(GimbalPitPos,GIM_PIT_MAX_SCALE,GIM_PIT_MIN_SCALE);
//	}
	
	PidPitchMotorPos.SetPoint = GimbalPitPos;
	PidYawArmorPos.SetPoint = GimbalYawPos;
}


/**********************************************************************************************************
*�� �� ��: Gimbal_Cease_Cal
*����˵��: ��̨����ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Gimbal_Cease_Cal(void)
{
	if(GimbalInitFlag != 3)
		GimbalInitFlag = 3;
	
	PidPitchSpeed.SetPoint = 0;
	PidYawSpeed.SetPoint = 0;
	PidPitchPos.SetPoint = Gyro.Pitch;
	PidYawPos.SetPoint = Gyro.Yaw;
	PidYawMotorPos.SetPoint = ZeroCheck_YawOutPut();
	PidPitchMotorPos.SetPoint = ZeroCheck_PitchOutPut();
	
	CAN2_GimbalSend(0,0);
}

/**********************************************************************************************************
*�� �� ��: Gimbal_Turnhead_Cal
*����˵��: ��̨תͷ90ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Gimbal_Turnhead_Cal(void)
{
	int YawMotorOutPut,PitMotorOutPut;
	YawMotorOutPut = ZeroCheck_YawOutPut();
	PitMotorOutPut = ZeroCheck_PitchOutPut();
	
	if(GimbalInitFlag != 4)
	{
		GimbalInitFlag = 4;
		GimbalPitPos = PitMotorOutPut;
		GimbalYawPos = YawMotorOutPut;
		turnHeadExitFlag = 0;
		turnHeadCnt = 0;
		turnHeadStep = 1;
	}
	
	if(turnHeadStep == 1)
	{
		
		if(GimbalYawPos > GIM_YAW_TURN_ANGLE)
			GimbalYawPos -= 10;
		if(turnHeadCnt < 1500)
			turnHeadCnt ++;
		else
			GimbalYawPos =  GIM_YAW_TURN_ANGLE;
		
		GimbalPitPos += RC_Ctl.mouse.y * 0.1f;
		GimbalPitPos = LIMIT_MAX_MIN(GimbalPitPos,GIM_PIT_MAX_SCALE,GIM_PIT_MIN_SCALE);
//�˳�����
		if(turnHeadExitFlag == 1)
		{
			turnHeadStep = 2;
			turnHeadCnt = 0;
			turnHeadExitFlag = 0;
		}
	}
/*********************�˳�����**************************************/	
	if(turnHeadStep == 2)
	{
		if(GimbalYawPos < GIM_YAW_INIT_SCALE)
			GimbalYawPos += 20;
		if(turnHeadCnt > 1000)
			turnHeadStep = 3;
		else
			turnHeadCnt ++;
	}
	if(turnHeadStep == 3)
	{
		//Laser_ON;
//		PidBigFrictSpeed[0].SetPoint = BigFrictSpeed;
///		PidBigFrictSpeed[1].SetPoint = -BigFrictSpeed;
		HeroStatus.GimbalMode = Gimbal_Act_Mode;
		HeroStatus.ChassisMode = Chassis_Act_Mode;
		PidYawPos.SetPoint = Gyro.Yaw;
		PidPitchPos.SetPoint = Gyro.Pitch;
	}
	
	PidYawMotorPos.SetPoint = GimbalYawPos;
	PidPitchMotorPos.SetPoint = GimbalPitPos;
}

/**********************************************************************************************************
*�� �� ��: Gimbal_Single_Cal
*����˵��: ��̨����ģʽ
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Gimbal_Single_Cal(void)
{
	int PitMotorOutPut;
	PitMotorOutPut = ZeroCheck_PitchOutPut();
	if(GimbalInitFlag != 5)
	{
		GimbalInitFlag = 5;
		GimbalPitPos = PitMotorOutPut;
		GimbalYawPos = GIM_YAW_INIT_SCALE;
	}
	if(HeroStatus.CtrMode == RemoteMode)
		GimbalPitPos += (RC_Ctl.rc.ch3 - 1024) * (-0.01f);
	if(HeroStatus.CtrMode == MouseKeyMode)
		GimbalPitPos += RC_Ctl.mouse.y * 0.1f;
	
	GimbalPitPos = LIMIT_MAX_MIN(GimbalPitPos,GIM_PIT_MAX_SCALE,GIM_PIT_MIN_SCALE);
	PidPitchMotorPos.SetPoint = GimbalPitPos;
	PidYawMotorPos.SetPoint = GimbalYawPos;
	
}
/**********************************************************************************************************
*�� �� ��: GimbalCur_Pid_Cal
*����˵��: ��̨��������
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
short PitchCur,YawCur;
void GimbalCur_Pid_Cal(void)
{
	if(HeroStatus.GimbalMode != Gimbal_CeaseMode)
	{
		if(HeroStatus.GimbalMode == Gimbal_Act_Mode)
		{
			PidPitchPos.ActPoint = Gyro.Pitch;
			PidPitchSpeed.ActPoint = Gyro.GY;
			PidPitchSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidPitchPos),7,-7);
			PitchCur = -(short)LIMIT_MAX_MIN(PID_Calc(&PidPitchSpeed),30000,-30000);
			
			PidYawPos.ActPoint = Gyro.Yaw;
			PidYawSpeed.ActPoint = -Gyro.GZ;
			PidYawSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidYawPos),7,-7);
			YawCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidYawSpeed),30000,-30000);
		}
		
		if(HeroStatus.GimbalMode == Gimbal_Armor_Mode)
		{
			PidPitchMotorPos.ActPoint = ZeroCheck_PitchOutPut();
			PidPitchSpeed.ActPoint = Gyro.GY;
			PidPitchSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidPitchMotorPos),7,-7);
			PitchCur = -(short)LIMIT_MAX_MIN(PID_Calc(&PidPitchSpeed),30000,-30000);
			
			PidYawArmorPos.ActPoint = Gyro.Yaw;
			PidYawSpeed.ActPoint = Gyro.GZ;
			PidYawSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidYawArmorPos),7,-7);
			YawCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidYawSpeed),30000,-30000);			
		}
		
		if(HeroStatus.GimbalMode == Gimbal_TurnHeadMode)
		{
			PidPitchMotorPos.ActPoint = ZeroCheck_PitchOutPut();
			PidPitchSpeed.ActPoint = Gyro.GY;
			PidPitchSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidPitchMotorPos),7,-7);
			PitchCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidPitchSpeed),30000,-30000);
			
			PidYawMotorPos.ActPoint = ZeroCheck_YawOutPut();
			PidYawSpeed.ActPoint = Gyro.GZ;
			PidYawSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidYawMotorPos),7,-7);
			YawCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidYawSpeed),30000,-30000);
		}
		
		if(HeroStatus.GimbalMode == Gimbal_SingleMode)
		{
			PidPitchMotorPos.ActPoint = ZeroCheck_PitchOutPut();
			PidPitchSpeed.ActPoint = Gyro.GY;
			PidPitchSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidPitchMotorPos),7,-7);
			PitchCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidPitchSpeed),30000,-30000);
			
			PidYawMotorPos.ActPoint = ZeroCheck_YawOutPut();
			PidYawSpeed.ActPoint = Gyro.GZ;
			PidYawSpeed.SetPoint = LIMIT_MAX_MIN(PID_Calc(&PidYawMotorPos),7,-7);
			YawCur = (short)LIMIT_MAX_MIN(PID_Calc(&PidYawSpeed),30000,-30000);
		}
		
		CAN2_GimbalSend(PitchCur,YawCur);
	}
	else
	{
		PitchCur = 0;
		YawCur = 0;
		Gimbal_Cease_Cal();
	}
}
/**********************************************************************************************************
*�� �� ��: Gimbal_LimitCalc
*����˵��: ��̨��λ����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/

void Gimbal_LimitCalc(void)
{
//		float pitchDegree,yawDegree;
//		int PitchOutPut,YawOutPut;
//		PitchOutPut = ZeroCheck_PitchOutPut();
//		YawOutPut 	= ZeroCheck_YawOutPut();
	
	  //pitchDegree    = ((PitchOutPut-GIM_PIT_INIT_SCALE)%8192)/8192.0f*360;
		pitchDegreeMax = GIM_PIT_MAX_DEGREE + Gyro.Pitch;				//pit�������෴
	  pitchDegreeMin = GIM_PIT_MIN_DEGREE + Gyro.Pitch;	
	
		//yawDegree      = ((YawOutPut-GIM_YAW_INIT_SCALE)%8192)/8192.0*(-360);
	  yawDegreeMax 	 = GIM_YAW_MAX_DEGREE + Gyro.Yaw;
	  yawDegreeMin 	 = GIM_YAW_MIN_DEGREE + Gyro.Yaw;	
}
/**********************************************************************************************************
*�� �� ��: Pid_YawGyroPosSpeed
*����˵��: Yaw�������ǣ��ٶȻ���λ�û�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_YawGyroPosSpeed(void)
{
	PidYawPos.SetPoint = 0.0f;
	PidYawPos.P = 0.6;//-0.35f;								
	PidYawPos.I = 0;					
	PidYawPos.D = 20;
	PidYawPos.IMax = 50;
	
	PidYawSpeed.SetPoint = 0.0f;
	PidYawSpeed.P = 13000; 						
	PidYawSpeed.I = 0.05;										
	PidYawSpeed.D = 1;
	PidYawSpeed.IMax = 200;
	
}
/**********************************************************************************************************
*�� �� ��: Pid_PitchGyroPosSpeed
*����˵��: Pitch�������ǣ��ٶȻ���λ�û�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_PitchGyroPosSpeed(void)
{
	PidPitchPos.SetPoint = 0.0f;
	PidPitchPos.P = 0.5f;		//-0.4
	PidPitchPos.I = 0.0008f;	//-0.002		
	PidPitchPos.D = 0;
	PidPitchPos.IMax = 50;
	
	PidPitchSpeed.SetPoint = 0.0f;
	PidPitchSpeed.P = -8000; //10000
	PidPitchSpeed.I = 0;		//1.5
	PidPitchSpeed.D = 0;
	PidPitchSpeed.IMax = 100;
}
/**********************************************************************************************************
*�� �� ��: Pid_PitchGyroPosSpeed
*����˵��: ����ģʽYaw��Pitchλ�û�
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void Pid_Pitch_MotorPos_Yaw_GyroPos(void)
{
	PidPitchMotorPos.SetPoint = 0.0f;
	PidPitchMotorPos.P = 0.02f;				//0.01
	PidPitchMotorPos.I = 0.001f;
	PidPitchMotorPos.D = 0;
	PidPitchMotorPos.IMax = 100;
	
	PidYawArmorPos.SetPoint = 0.0f;
	PidYawArmorPos.P = -0.3f;
	PidYawArmorPos.I = -0.002f;
	PidYawArmorPos.D = 0;
	PidYawArmorPos.IMax = 100.0f;
}
/**
  * @brief  yaw�������,pit��������������ģʽ
  * @param   
  * @retval  
  */
void Pid_Yaw_MotorPos(void)
{
	
	PidYawMotorPos.SetPoint = 0.0f;
	PidYawMotorPos.P = 0.02f;
	PidYawMotorPos.I = 0;//0.01f;
	PidYawMotorPos.D = 0;
	PidYawMotorPos.IMax = 0;
}
/**********************************************************************************************************
*�� �� ��: GIMBAL_task
*����˵��: ��̨����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
uint32_t GIMBAL_high_water;

void GIMBAL_task(void *pvParameters)
{
	 Pid_YawGyroPosSpeed();
   Pid_PitchGyroPosSpeed();
	 
   while (1) {
    ZeroCheck_cal();
		if(HeroStatus.GimbalMode == Gimbal_Act_Mode) 
     Gimbal_Act_Cal();
		else if(HeroStatus.GimbalMode == Gimbal_CeaseMode)
		 Gimbal_Cease_Cal();
		else if(HeroStatus.GimbalMode == Gimbal_TurnHeadMode)
		 Gimbal_Turnhead_Cal();
		else if(HeroStatus.GimbalMode == Gimbal_SingleMode)
		 Gimbal_Single_Cal();
    GimbalCur_Pid_Cal();
    vTaskDelay(1); /* ��ʱ 500 �� tick */
		// xEventGroupSetBits(xCreatedEventGroup, 0x01);
#if INCLUDE_uxTaskGetStackHighWaterMark
        GIMBAL_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
//ϵͳ��ʶ����
/*���Է�Χ+50�ȵ�-50��*/
//int Gimbal_System_start_flag;
//float	Gimbal_direct;
//void Gimbal_System_identification(float Gimbal_x,float Gimbal_y)
//{
//	if(Gimbal_System_start_flag == 1)
//	{
//		if(Gimbal_x != 0&&Gimbal_y == 0)
//		{
//			Gimbal_x = Gimbal_direct*Gimbal_x;
//		}
//		else if(Gimbal_x == 0&&Gimbal_y != 0)
//		{
//			Gimbal_y = Gimbal_direct*Gimbal_y;
//		}
//	}
//}
//int T;//����
//int T_cnt = 0;//����
//int T_Time_cnt = 0;//���ڴ�������
//int F_cnt = 0,F_cnt_last;//ָ��F��ָ��
//int F = 1;
//void get_F(void)
//{
//	if(F < 22)
//	{
//		F = F + 0.5;
//	}
//	else if(F == 22)
//	{
//		F = 24;
//	}
//	else if(F >= 24&&F < 40)
//	{
//		F = F + 2;
//	}
//	else if(F == 40)
//	{
//		F = 50;
//	}
//	else if(F >= 50&&F < 120)
//	{
//		F = F + 10;
//	}
//	else if(F == 120)
//	{
//		F = 200;
//	}
//	else if(F == 250)
//	{
//		F = 333;
//	}
//	else if(F == 333)
//	{
//		F = 500;
//	}
//}

//void T_change(void)
//{
//	T = round(1000/F);
//	if(Gimbal_System_start_flag == 1)
//	{
//    Gimbal_direct = sin(2*3.14*T_cnt/T);
//		if(T_cnt/T >= 1)
//		{
//			T_cnt = 0;
//			T_Time_cnt++;
//		}
//		T_cnt++;
//		F_cnt_last = F_cnt;
//		if(T_Time_cnt >= 40)
//		{
//			T_Time_cnt = 0;
//			F_cnt++; 
//		}
//		if(F_cnt != F_cnt_last)
//		{
//			get_F();
//		}
//	}
//	else if(Gimbal_System_start_flag == 0)
//	{
//		F = 1;
//	}
//}
