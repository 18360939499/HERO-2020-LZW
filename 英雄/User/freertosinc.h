#ifndef FreeRTOSinc_H
#define FreeRTOSinc_H
//	参数配置头文件，减少头文件包含
#include "main.h"
#include "FreeRTOSConfig.h"
#include "FreeRTOS.h"
#include "task.h"
#include "tim.h"
#include "queue.h"
#include "list.h"
#include "semphr.h"
#include "event_groups.h"
#ifdef DEBUG
extern Key_typedef key_status;
extern RC_Ctl_t RC_Ctl;
extern Shoot_typedef BigShoot;
extern Jugement_t Juge;
extern HeroStatus_typedef	HeroStatus;
extern Pid_Typedef PidSmlFrictSpeed[2],PidBigFrictSpeed[2],PidBigPullerPos;
extern short SmlFrictSpeed;
extern Pid_Typedef	PidBigPushPos,PidBigPushSpeed;
extern M2006Receive_Typedef		SmlPuller,BigPushRecv;
extern short BigFrictSpeed;
extern M3508Receive_Typedef		BigFrict[2],SmlFrict[2];
extern ext_client_draw_t	ClientDrawData;
extern PC_Recv_t PC_Recv;

short cnt;
short c = 0;
short DebugInitFlag = 0;
short debug_preRC_s1;
float watch_frict_l,watch_frict_r,watch_diff;
short debugFlag = 0;
float debug_bigPushPos = 0;
short debug_bigShootFlag = 0;
short debug_Priod = 1000;
short debug_cnt = 500;
int 	debug_increasePos = 8191*24;
short debug_setSpeed = 0;
short speed = 5000;
float debug_pitch;
int 	pitch_out;
short debug_pitch_angle;

float yaw_angle;
float pitch_angle;
float pc_yaw;
float pc_pitch;
float now_yaw;
float now_pitch;
#endif
#ifdef MAIN
short Juge_DisConnect = 0;
unsigned char Juge_Flag = 1;
unsigned char RC_Flag = 1;		//0为掉线，1为正常
void System_Init(void);
void System_Configration(void);
extern RC_Ctl_t RC_Ctl;
#endif
#ifdef CLASS
/*--------------------------全局变量----------------------------*/
Pid_Typedef	PidChassisAngle,PidChassisSpeed;
short ChassisInitFlag = 0;
Chassis_typedef chassis;
float slowFac = 1;
short chassisWrigCnt,chassisWrigT = 1000;
unsigned char superCapAutoFlag = 1;
/*--------------------------引用外部变量----------------------------*/
extern  EventGroupHandle_t xCreatedEventGroup ;
extern  SemaphoreHandle_t  xSemClass2 ;
extern HeroStatus_typedef	HeroStatus;
extern RC_Ctl_t RC_Ctl;
extern GM6020Receive_Typedef Gimbal_Motor_Yaw;
#endif
#ifdef GIMBAL
/*--------------------------全局变量----------------------------*/
Pid_Typedef	PidYawPos, PidPitchPos, PidYawSpeed, PidPitchSpeed;
Pid_Typedef	PidYawMotorPos,PidPitchMotorPos,PidYawArmorPos;

short GimbalInitFlag = 0;
float pitchDegreeMax,pitchDegreeMin,yawDegreeMax,yawDegreeMin;
float GimbalPitPos,GimbalYawPos;

unsigned char bigBulletGetFlag = 0;
short bigBulletGetCnt = 0;
short turnHeadCnt = 0;
short turnHeadStep = 0;
short turnHeadExitFlag = 0;
/*--------------------------引用外部变量----------------------------*/
extern Gyro_t Gyro;
//extern short BigFrictSpeed;
extern HeroStatus_typedef	HeroStatus;
extern RC_Ctl_t RC_Ctl;
//extern GM6020Receive_Typedef		Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;
//extern PC_Recv_t PC_Recv;
//extern RC_Ctl_t RC_Ctl;
//extern Key_typedef key_status;
//extern Chassis_typedef chassis;
//extern Pid_Typedef	PidChassisAngle;
//extern Pid_Typedef PidBigFrictSpeed[2];
#endif
#ifdef SHOOT
/*--------------------------全局变量----------------------------*/
Pid_Typedef	PidBigPullerSpeed,PidBigFrictSpeed[2],PidBigPullerPos;


Shoot_typedef BigShoot;
short ShootInitFlag;
short BigPullerTimeCnt;
short BigPullerDir = 3;
short BigFrictSpeed = 5000;
short BigPullerSpeed = 500;
short BigPuller_PosAngle = 8191;
short TrigFlag = 0;
unsigned char BigFrictFlag = 0;
/*--------------------------引用外部变量----------------------------*/
//extern Jugement_t Juge;
extern HeroStatus_typedef			HeroStatus;
extern M3508Receive_Typedef		BigFrict[2],BigPuller;
extern GM6020Receive_Typedef	Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;
extern RC_Ctl_t RC_Ctl;

#endif

#ifdef ACTION
/*--------------------------全局变量----------------------------*/
HeroStatus_typedef	HeroStatus;
//short last_rc_s1;
short Mouse_Key_Flag = 0;

Key_typedef key_status;

/*--------------------------引用外部变量----------------------------*/
//extern Shoot_typedef 		BigShoot;
//extern Pid_Typedef			PidBigPullerSpeed;
//extern Pid_Typedef			PidBigFrictSpeed[2],PidSmlFrictSpeed[2];
//extern short 						BigFrictSpeed,SmlFrictSpeed;
extern Chassis_typedef 	chassis;
//extern unsigned char bigBulletGetFlag;
//extern short bigBulletGetCnt;
//extern float GimbalPitPos,GimbalYawPos;
extern short turnHeadExitFlag;
//extern PC_Send_t PC_Send;
extern RC_Ctl_t	RC_Ctl;
//extern M3508Receive_Typedef		BigFrict[2];
extern float slowFac;
//extern unsigned char superCapAutoFlag;
#endif

#ifdef JUDGE
/**********************************全局变量*************************************/
Jugement_t Juge;
CustomData_t CustomData;
ext_client_draw_t	ClientDrawData;

/**********************************引用外部全局变量*****************************/
extern short Juge_DisConnect;
extern unsigned char  usart5_tx_buff[70];
extern Chassis_typedef chassis;
extern Shoot_typedef BigShoot;
extern unsigned char superCapAutoFlag;
extern unsigned char BigFrictFlag;
extern unsigned char Gyro_Flag;
extern unsigned char Gimbal_Flag;
extern unsigned char Juge_Flag;
extern unsigned char RC_Flag;

/**********************************内部静态变量**********************************/

//测试数据
float Test_Bullet42Spd_Last=0;
short Test_Update_Bullet42=0;
float Log_Bullet42Spd[50]={0};

float Test_Bullet17Spd_Last=0;
short Test_Update_Bullet17=0;
float Log_Bullet17Spd[50]={0};
//
unsigned char SaveBuffer[60];
uint16_t DataLen;
uint16_t cmd_id;
short test_cntt=0;

#endif

#ifdef ZERO
/*--------------------------内部变量----------------------------*/
ZeroCheck_Typedef ZeroCheck_Pitch,ZeroCheck_Yaw,ZeroCheck_Pitch_speed,ZeroCheck_Yaw_speed; 
ZeroCheck_Typedef ZeroCheck_SmlPullerPos,ZeroCheck_SmlPullerSpeed;
ZeroCheck_Typedef ZeroCheck_GyroYaw,ZeroCheck_GyroPitch; 
ZeroCheck_Typedef	ZeroCheck_BigPushPos,ZeroCheck_BigPullerPos;

int Gimbal_Pitch,Gimbal_Yaw,Gimbal_Pitch_Speed,Gimbal_Yaw_Speed;                                                                          
int SmlPuller_Pos,SmlPuller_Speed;
float Gyro_Yaw,Gyro_Pitch;
int BigPush_Pos,BigPuller_Pos;
/*--------------------------外部引用变量----------------------------*/
extern M3508Receive_Typedef			BigFrict[2],SmlFrict[2],BigPuller;
extern GM6020Receive_Typedef		Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;
//extern Gyro_t 									Gyro;
extern M3508Receive_Typedef			BigPuller;
#endif

#ifdef GYRO
/*--------------------------引用外部变量----------------------------*/
extern short Gyro_DisConnect;
/**
  * @brief 新陀螺仪  
  * @param   新陀螺仪不需要过零检测
  * @retval  
  */
Gyro_t Gyro={0,0,0,0,0,0,1};
float pitch[2],yaw[2],dPitch=0,dYaw=0,gy=0,gz=0;	

#endif
#endif