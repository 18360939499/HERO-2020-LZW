#include "main.h"	

/*--------------------------全局变量----------------------------*/

/*--------------------------引用外部变量----------------------------*/

/**
  * @brief  CAN2数据发送
  * @param   
  * @retval  
  */
void CAN1_FrictSend(short a,short b,short c,short d)
{
	  CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x200;
	  a=LIMIT_MAX_MIN(a,16000,-16000);
	  b=LIMIT_MAX_MIN(b,16000,-16000);
	  c=LIMIT_MAX_MIN(c,16000,-16000);
	  d=LIMIT_MAX_MIN(d,16000,-16000);
    tx_message.Data[0] = (unsigned char)((a>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(a&0xff);  
    tx_message.Data[2] = (unsigned char)((b>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(b&0xff);
    tx_message.Data[4] = (unsigned char)((c>>8)&0xff);
    tx_message.Data[5] = (unsigned char)(c&0xff);
    tx_message.Data[6] = (unsigned char)((d>>8)&0xff);
    tx_message.Data[7] = (unsigned char)(d&0xff);
    CAN_Transmit(CAN1,&tx_message);
}

/**
  * @brief  大拨盘数据发送
  * @param   
  * @retval  
  */

void CAN1_BigPullerSend(short	a)
{
		 CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x1FF;
	  a	=	LIMIT_MAX_MIN(a,16000,-16000);
	
    tx_message.Data[0] = (unsigned char)((a>>8) & 0xff);
    tx_message.Data[1] = (unsigned char)(a & 0xff);  
    tx_message.Data[2] = 0;
    tx_message.Data[3] = 0;
    tx_message.Data[4] = 0;
    tx_message.Data[5] = 0;
    tx_message.Data[6] = 0;
    tx_message.Data[7] = 0;
    CAN_Transmit(CAN1,&tx_message);
}
/**
  * @brief  底盘速度分量发送发送
  * @param   
  * @retval  
  */
void CAN1_ChassisSend(Chassis_typedef* Chassis,short ChassisRemainPower)
{
		CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08; 	
	  tx_message.StdId = 0x100;
		
    tx_message.Data[0]=(unsigned char)( ((short)Chassis->carSpeedx>>8)&0xff);	
    tx_message.Data[1]= (unsigned char)( (short)Chassis->carSpeedx&0xff);	
    tx_message.Data[2]=(unsigned char)( ((short)Chassis->carSpeedy>>8)&0xff);	
    tx_message.Data[3]= (unsigned char)( (short)Chassis->carSpeedy&0xff);
    tx_message.Data[4]=(unsigned char)( ((short)Chassis->carSpeedw>>8)&0xff);	
    tx_message.Data[5]= (unsigned char)( (short)Chassis->carSpeedw&0xff);
    tx_message.Data[6]=(unsigned char) ChassisRemainPower;
		
    tx_message.Data[7]= (unsigned char)	(Chassis->SuperCapCmd<<1|Chassis->PowerdownCmd);

	  CAN_Transmit(CAN1,&tx_message);
}

/**
  * @brief   云台电流发送
  * @param   
  * @retval  
  */
void CAN2_GimbalSend(short pitch,short yaw)
{
		 CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x1FF;
	  pitch	=	LIMIT_MAX_MIN(pitch,30000,-30000);
	  yaw		=	LIMIT_MAX_MIN(yaw,30000,-30000);
	
    tx_message.Data[0] = (unsigned char)((pitch>>8)&0xff);
    tx_message.Data[1] = (unsigned char)(pitch&0xff);
    tx_message.Data[2] = (unsigned char)((yaw>>8)&0xff);
    tx_message.Data[3] = (unsigned char)(yaw&0xff);
    CAN_Transmit(CAN2,&tx_message);
}
/**
  * @brief  小拨盘数据发送
  * @param   
  * @retval  
  */

void CAN2_SmlPullerSend(short	a)
{
		CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x1FF;
	  a	=	LIMIT_MAX_MIN(a,10000,-10000);
	
    tx_message.Data[0] = 0;
    tx_message.Data[1] = 0;  
    tx_message.Data[2] = 0;
    tx_message.Data[3] = 0;
    tx_message.Data[4] = 0;
    tx_message.Data[5] = 0;
    tx_message.Data[6] = (unsigned char)((a>>8) & 0xff);
    tx_message.Data[7] = (unsigned char)(a & 0xff);
    CAN_Transmit(CAN2,&tx_message);
}

/**
  * @brief  大发射结构数据发送
  * @param   
  * @retval  
  */
void CAN2_BigPushSend(short a)
{
		CanTxMsg tx_message;
    tx_message.IDE = CAN_ID_STD;    
    tx_message.RTR = CAN_RTR_DATA; 
    tx_message.DLC = 0x08;    
    tx_message.StdId = 0x1FF;
	  a	=	LIMIT_MAX_MIN(a,10000,-10000);
	
    tx_message.Data[0] = 0;
    tx_message.Data[1] = 0;  
    tx_message.Data[2] = 0;
    tx_message.Data[3] = 0;
    tx_message.Data[4] = 0;
    tx_message.Data[5] = 0;
    tx_message.Data[6] = (unsigned char)((a>>8) & 0xff);
    tx_message.Data[7] = (unsigned char)(a & 0xff);
    CAN_Transmit(CAN2,&tx_message);
}
