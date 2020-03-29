#include "main.h"

/**********************************引用外部全局变量*****************************/

extern float AD_Power;

extern short qyro_cnt[2];
extern Gyro_t Gyro;
/**
  * @brief  蓝牙发送
  * @param   
  * @retval  
  */
void BuleToothThread(int SendPeriod)
{
	  static short TimeCnt=0;
	  if(TimeCnt<SendPeriod) TimeCnt++;
	  else
		{
			TimeCnt=0;			
			short i,Send_Count;
			{
					DataScope_Get_Channel_Data(0,1);      //底盘功率
					DataScope_Get_Channel_Data(0,2);
					DataScope_Get_Channel_Data(0,3);
					DataScope_Get_Channel_Data(0,4);
					DataScope_Get_Channel_Data(0,5);
					DataScope_Get_Channel_Data(0,6);
					DataScope_Get_Channel_Data(0,7);
					DataScope_Get_Channel_Data(0,8);
					DataScope_Get_Channel_Data(0,9);
					DataScope_Get_Channel_Data(0, 10);             // 
			}
			Send_Count=DataScope_Data_Generate(10);	
			for(i=0;i<Send_Count;i++)
			{
				USART_SendData(USART3,DataScope_OutPut_Buffer[i]);
				while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=SET);
			}			
		}
}

