#define GYRO
#include "freertosinc.h"

void NewGyro_DataHandle(CanRxMsg rx_message1)
{
//	Gyro_DisConnect = 0;
	switch(rx_message1.StdId)
	{ 
		 case 0x100://PITCH
		 {
			 if(Gyro.recvd[0]>5)
			 {
				 pitch[last]=pitch[now];
				 memcpy(&pitch[now], rx_message1.Data, 4);
				 memcpy(&gy, &rx_message1.Data[4], 8);				 
			   
				  Gyro.GY=gy;
				  dPitch=pitch[now]-pitch[last];
          Gyro.Pitch+=dPitch;
			 }
			 else
			 {
				 Gyro.recvd[0]++;
				 memcpy(&pitch[now], rx_message1.Data, 4);
			 }
		 }
		 break;
		 case 0x101:
		 {
			 if(Gyro.recvd[1]>5)
			 {
					 yaw[last]=yaw[now];
					 memcpy(&yaw[now], &rx_message1.Data, 4);
					 memcpy(&gz, &rx_message1.Data[4], 8);		

				   Gyro.GZ=gz;
					 dYaw=yaw[last]-yaw[now];
					 Gyro.Yaw+=dYaw;					 
			 }
			 else
			 {
				 Gyro.recvd[1]++;
				 memcpy(&yaw[now], &rx_message1.Data, 4);
			 }
		 }
	 }	
}

