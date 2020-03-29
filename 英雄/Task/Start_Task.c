#include "freertosinc.h"
uint32_t CPU_high_water;

/*任务优先级数值越低，优先级越低*/

#define CHASSIS_TASK_PRIO 20  //任务优先级
#define CHASSIS_STK_SIZE 256 //任务堆栈
static TaskHandle_t CHASSISTask_Handler; //任务句柄

#define GIMBAL_TASK_PRIO 19  //任务优先级
#define GIMBAL_STK_SIZE 256 //任务堆栈
static TaskHandle_t GIMBALTask_Handler; //任务句柄

#define SHOOT_TASK_PRIO 18  //任务优先级
#define SHOOT_STK_SIZE 256 //任务堆栈
static TaskHandle_t SHOOTTask_Handler; //任务句柄

#define CONTROL_TASK_PRIO 17  //任务优先级
#define CONTROL_STK_SIZE 256 //任务堆栈
static TaskHandle_t CONTROLTask_Handler; //任务句柄

#define CPU_TASK_PRIO 2  //任务优先级
#define CPU_STK_SIZE 256 //任务堆栈
static TaskHandle_t CPUTask_Handler; //任务句柄

#define START_TASK_PRIO 1  //任务优先级
#define START_STK_SIZE 128 //任务堆栈
static TaskHandle_t StartTask_Handler; //任务句柄

/**********************************************************************************************************
*函 数 名: start_task
*功能说明: 创建任务函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void start_task(void *pvParameters)
{
//  taskENTER_CRITICAL();
//  xCreatedEventGroup = xEventGroupCreate();/* 创建事件标志组 */
	//xSemaphore = xSemaphoreCreateCounting(5,   5);                     /* 最大计数值为5 */
	                                                     /* 初始计数值为5 */
	xTaskCreate((TaskFunction_t)GIMBAL_task,          //任务函数
              (const char *)"GIMBAL_task",          //任务名称
              (uint16_t)GIMBAL_STK_SIZE,            //任务堆栈大小
              (void *)NULL,                        //传递给任务函数的参数
              (UBaseType_t)GIMBAL_TASK_PRIO,        //任务优先级
              (TaskHandle_t *)&GIMBALTask_Handler); //任务句柄
							
	xTaskCreate((TaskFunction_t)SHOOT_task,          //任务函数
              (const char *)"SHOOT_task",          //任务名称
              (uint16_t)SHOOT_STK_SIZE,            //任务堆栈大小
              (void *)NULL,                        //传递给任务函数的参数
              (UBaseType_t)SHOOT_TASK_PRIO,        //任务优先级
              (TaskHandle_t *)&SHOOTTask_Handler); //任务句柄	
							
  xTaskCreate((TaskFunction_t)CHASSIS_task,          //任务函数
              (const char *)"CHASSIS_task",          //任务名称
              (uint16_t)CHASSIS_STK_SIZE,            //任务堆栈大小
              (void *)NULL,                        //传递给任务函数的参数
              (UBaseType_t)CHASSIS_TASK_PRIO,        //任务优先级
              (TaskHandle_t *)&CHASSISTask_Handler); //任务句柄	
							
  xTaskCreate((TaskFunction_t)ActionControl_task,          //任务函数
              (const char *)"ActionControl_task",          //任务名称
              (uint16_t)CONTROL_STK_SIZE,            //任务堆栈大小
              (void *)NULL,                        //传递给任务函数的参数
              (UBaseType_t)CONTROL_TASK_PRIO,        //任务优先级
              (TaskHandle_t *)&CONTROLTask_Handler); //任务句柄									
	
							
	xTaskCreate((TaskFunction_t)CPU_task,          //任务函数
                (const char *)"CPU_task",          //任务名称
                (uint16_t)CPU_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)CPU_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&CPUTask_Handler); //任务句柄							
							
	vTaskDelete(StartTask_Handler); //删除开始任务
  taskEXIT_CRITICAL();            //退出临界区
}


/**********************************************************************************************************
*函 数 名: CPU_task
*功能说明: 创建任务函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void CPU_task(void *pvParameters)
{
	
	uint8_t CPU_RunInfo1[200]; //保存任务运行时间信息 分别是：任务名 任务状态 优先级 剩余栈 任务序号
  uint8_t CPU_RunInfo2[200]; //保存任务运行时间信息 分别是：任务名 运行计数  使用率
	
   while (1) {
    memset(CPU_RunInfo1,0,200); //信息缓冲区清零
 
    vTaskList((char *)&CPU_RunInfo1); //获取任务运行时间信息
    
    memset(CPU_RunInfo2,0,200); //信息缓冲区清零
 
    vTaskGetRunTimeStats((char *)&CPU_RunInfo2);
		Juge_SendData();
		ClientDrawSend();	
    vTaskDelay(100); /* 延时 500 个 tick */
		 
#if INCLUDE_uxTaskGetStackHighWaterMark
        CPU_high_water = uxTaskGetStackHighWaterMark(NULL);
#endif
    }
}
/**********************************************************************************************************
*函 数 名: startTast
*功能说明: 创建创建任务函数
*形    参: 无
*返 回 值: 无
**********************************************************************************************************/
void startTast(void)
{
    xTaskCreate((TaskFunction_t)start_task,          //任务函数
                (const char *)"start_task",          //任务名称
                (uint16_t)START_STK_SIZE,            //任务堆栈大小
                (void *)NULL,                        //传递给任务函数的参数
                (UBaseType_t)START_TASK_PRIO,        //任务优先级
                (TaskHandle_t *)&StartTask_Handler); //任务句柄
}

