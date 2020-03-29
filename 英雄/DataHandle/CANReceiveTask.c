#include "main.h"

/******************************* 全局变量声明 ************************************/
M3508Receive_Typedef		BigFrict[2],BigPuller;
GM6020Receive_Typedef		Gimbal_Motor_Yaw,Gimbal_Motor_Pitch;

/******************************* 引用外部变量 ************************************/
//extern short Gimbal_DisConnect;
//extern Chassis_typedef chassis;
/**
  * @brief  CAN1接收
  * @param   
  * @retval  
  */
void CAN1_Receive0Task(CanRxMsg rx_message)
{
			switch(rx_message.StdId)
			{ 				
//				 case 0x120:
//					 chassis.CapReaimPower = (rx_message.Data[0]<<8)|(rx_message.Data[1]);
//         break;		
         case 0x202:      // Fric Right;
					 BigFrict[1].Angle 			= (rx_message.Data[0]<<8) | rx_message.Data[1];
					 BigFrict[1].RealSpeed 	= (rx_message.Data[2]<<8) | rx_message.Data[3];	
					 break;
				 case 0x201:     //Frict  Left
					 BigFrict[0].Angle 			= (rx_message.Data[0]<<8) | rx_message.Data[1];
					 BigFrict[0].RealSpeed 	= (rx_message.Data[2]<<8) | rx_message.Data[3];
					 break; 				
				 case 0x204:					//大拨盘
					 BigPuller.Angle 			= (rx_message.Data[0]<<8) | rx_message.Data[1];
					 BigPuller.RealSpeed 	= (rx_message.Data[2]<<8) | rx_message.Data[3];
				 break;
				 
			}	
}
/**
  * @brief  CAN2接收
  * @param   
  * @retval  
  */
void CAN2_Receive0Task(CanRxMsg rx_message)
{
		switch(rx_message.StdId)
		{		

				 case 0x206: 	       //GimYAW			 3				 
					 Gimbal_Motor_Yaw.Angle				= 	(rx_message.Data[0]<<8) | rx_message.Data[1];
					 Gimbal_Motor_Yaw.RealSpeed		=		(rx_message.Data[2]<<8) | rx_message.Data[3];
			   break;
				 case 0x205:         //GimPitch				2		
					 Gimbal_Motor_Pitch.Angle				= 	(rx_message.Data[0]<<8) | rx_message.Data[1];
					 Gimbal_Motor_Pitch.RealSpeed		=		(rx_message.Data[2]<<8) | rx_message.Data[3];
//	 			 Gimbal_DisConnect = 0;
				 break;	 		
			 
		}
}

/**
  * @brief  从板数据接收
  * @param   
  * @retval  接收底盘电容剩余能量
  */
//void Chassis_DataRecv(unsigned char Data[])
//{
//			chassis.CapReaimPower =(uint16_t)(Data[0]<<8|Data[1]);
//}
