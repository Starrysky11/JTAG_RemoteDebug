#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "string.h"
#include "stm32f10x_gpio.h"
#include "misc.h"
#include "stm32f10x_usart.h"
 
/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#define START_TASK_PRIO		1	 	//任务优先级
#define START_STK_SIZE 		128  	//任务堆栈大小
TaskHandle_t StartTask_Handler;		//任务句柄
void start_task(void *pvParameters);	//任务函数

#define UART2_TASK_PRIO	2	 	//任务优先级
#define UART2_STK_SIZE 	50  	//任务堆栈大小
TaskHandle_t Uart2Task_Handler;		//任务句柄
void uart2_task(void *pvParameters);	//任务函数

/************************************************
 ALIENTEK战舰STM32开发板实验4
 串口实验 
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/
u8 jtag_state = 0;
u8 jtag_power_state = 0;
u8 pcb1_power_state = 0;
u8 pcb2_power_state = 0;
u8 pcb3_power_state = 0;
u8 pcb4_power_state = 0;
u8 pcb5_power_state = 0;

void Print_Port_State(char *str, uint16_t GPIO_Pin)
{
	if (GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin) == Bit_SET) {
		uartPrintf("%s Power State is ON\r\n", str);
	} else {
		uartPrintf("%s Power State is OFF\r\n", str);
	}
}
void Uart_Process(u16 size)
{
	if (size == strlen("JTAG_ON") && strncmp(USART_RX_BUF, "JTAG_ON", strlen("JTAG_ON")) == 0) {
		GPIO_SetBits(GPIOB, GPIO_Pin_3);						 //PB.3 输出高, 开始供电
	}
	if (size == strlen("JTAG_OFF") && strncmp(USART_RX_BUF, "JTAG_OFF", strlen("JTAG_OFF")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_3);						 //PB.3 输出低, 停止供电
	}
	if (size == strlen("JTAG_POWER_ON") && strncmp(USART_RX_BUF, "JTAG_POWER_ON", strlen("JTAG_POWER_ON")) == 0) {
		GPIO_SetBits(GPIOB, GPIO_Pin_4);					 	 //PB.4 输出高, 开始供电
	}
	if (size == strlen("JTAG_POWER_OFF") && strncmp(USART_RX_BUF, "JTAG_POWER_OFF", strlen("JTAG_POWER_OFF")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_4);						 //PB.4 输出低, 停止供电
	}

	if (size == strlen("PCB1_POWER_ON") && strncmp(USART_RX_BUF, "PCB1_POWER_ON", strlen("PCB1_POWER_ON")) == 0) {
		GPIO_SetBits(GPIOB, GPIO_Pin_5);					 	 //PB.5 输出高, 开始供电
	}
	if (size == strlen("PCB1_POWER_OFF") && strncmp(USART_RX_BUF, "PCB1_POWER_OFF", strlen("PCB1_POWER_OFF")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);						 //PB.5 输出低, 停止供电
	}

	if (size == strlen("PCB2_POWER_ON") && strncmp(USART_RX_BUF, "PCB2_POWER_ON", strlen("PCB2_POWER_ON")) == 0) {
		GPIO_SetBits(GPIOB, GPIO_Pin_6);					 	 //PB.6 输出高, 开始供电
		
	}
	if (size == strlen("PCB2_POWER_OFF") && strncmp(USART_RX_BUF, "PCB2_POWER_OFF", strlen("PCB2_POWER_OFF")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);						 //PB.6 输出低, 停止供电
	}

	if (size == strlen("PCB3_POWER_ON") && strncmp(USART_RX_BUF, "PCB3_POWER_ON", strlen("PCB3_POWER_ON")) == 0) {
		GPIO_SetBits(GPIOB, GPIO_Pin_7);					 	 //PB.7 输出高, 开始供电		pcb3_power_state = 1;
	}
	if (size == strlen("PCB3_POWER_OFF") && strncmp(USART_RX_BUF, "PCB3_POWER_OFF", strlen("PCB3_POWER_OFF")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);						 //PB.7 输出低, 停止供电
	}
	
	if (size == strlen("PCB4_POWER_ON") && strncmp(USART_RX_BUF, "PCB4_POWER_ON", strlen("PCB4_POWER_ON")) == 0) {
		GPIO_SetBits(GPIOB, GPIO_Pin_8);					 	 //PB.8 输出高, 开始供电
	}
	if (size == strlen("PCB4_POWER_OFF") && strncmp(USART_RX_BUF, "PCB4_POWER_OFF", strlen("PCB4_POWER_OFF")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);						 //PB.8 输出低, 停止供电
	}	

	if (size == strlen("PCB5_POWER_ON") && strncmp(USART_RX_BUF, "PCB5_POWER_ON", strlen("PCB5_POWER_ON")) == 0) {
		GPIO_SetBits(GPIOB, GPIO_Pin_9);					 	 //PB.9 输出高, 开始供电
	}
	if (size == strlen("PCB5_POWER_OFF") && strncmp(USART_RX_BUF, "PCB5_POWER_OFF", strlen("PCB5_POWER_OFF")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);						 //PB.9 输出低, 停止供电
	}
	// 仅将PCB1与JTAG连接，支持调试PCB1
	if (size == strlen("JTAG_ONLY_CONNECT_PCB1") && strncmp(USART_RX_BUF, "JTAG_ONLY_CONNECT_PCB1", strlen("JTAG_ONLY_CONNECT_PCB1")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);						 //PB.6 输出低, 停止供电, PCB2_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);						 //PB.7 输出低, 停止供电, PCB3_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);						 //PB.8 输出低, 停止供电, PCB4_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);						 //PB.9 输出低, 停止供电, PCB5_POWER_OFF
		delay_ms(500);											 //延迟500ms
		GPIO_SetBits(GPIOB, GPIO_Pin_4);						 //PB.4 输出高, 开始供电, JTAG_POWER_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_3);					 	 //PB.3 输出高, 开始供电, JTAG_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_5);					 	 //PB.5 输出高, 开始供电, PCB1_POWER_ON
	}
	// 仅将PCB2与JTAG连接，支持调试PCB2
	if (size == strlen("JTAG_ONLY_CONNECT_PCB2") && strncmp(USART_RX_BUF, "JTAG_ONLY_CONNECT_PCB2", strlen("JTAG_ONLY_CONNECT_PCB2")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);						 //PB.5 输出低, 停止供电, PCB1_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);						 //PB.7 输出低, 停止供电, PCB3_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);						 //PB.8 输出低, 停止供电, PCB4_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);						 //PB.9 输出低, 停止供电, PCB5_POWER_OFF
		delay_ms(500);											 //延迟500ms
		GPIO_SetBits(GPIOB, GPIO_Pin_4);						 //PB.4 输出高, 开始供电, JTAG_POWER_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_3);						 //PB.3 输出高, 开始供电, JTAG_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_6);					 	 //PB.6 输出高, 开始供电, PCB2_POWER_ON
	}
	// 仅将PCB3与JTAG连接，支持调试PCB3
	if (size == strlen("JTAG_ONLY_CONNECT_PCB3") && strncmp(USART_RX_BUF, "JTAG_ONLY_CONNECT_PCB3", strlen("JTAG_ONLY_CONNECT_PCB3")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);						 //PB.5 输出低, 停止供电, PCB1_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);						 //PB.6 输出低, 停止供电, PCB2_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);						 //PB.8 输出低, 停止供电, PCB4_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);						 //PB.9 输出低, 停止供电, PCB5_POWER_OFF
		delay_ms(500);											 //延迟500ms
		GPIO_SetBits(GPIOB, GPIO_Pin_4);						 //PB.4 输出高, 开始供电, JTAG_POWER_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_3);						 //PB.3 输出高, 开始供电, JTAG_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_7);					 	 //PB.7 输出高, 开始供电, PCB3_POWER_ON
	}
	// 仅将PCB4与JTAG连接，支持调试PCB4
	if (size == strlen("JTAG_ONLY_CONNECT_PCB4") && strncmp(USART_RX_BUF, "JTAG_ONLY_CONNECT_PCB4", strlen("JTAG_ONLY_CONNECT_PCB4")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);						 //PB.5 输出低, 停止供电, PCB1_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);						 //PB.6 输出低, 停止供电, PCB2_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);						 //PB.7 输出低, 停止供电, PCB3_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_9);						 //PB.9 输出低, 停止供电, PCB5_POWER_OFF
		delay_ms(500);											 //延迟500ms
		GPIO_SetBits(GPIOB, GPIO_Pin_4);						 //PB.4 输出高, 开始供电, JTAG_POWER_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_3);						 //PB.3 输出高, 开始供电, JTAG_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_8);					 	 //PB.8 输出高, 开始供电, PCB4_POWER_ON
	}
	// 仅将PCB5与JTAG连接，支持调试PCB5
	if (size == strlen("JTAG_ONLY_CONNECT_PCB5") && strncmp(USART_RX_BUF, "JTAG_ONLY_CONNECT_PCB5", strlen("JTAG_ONLY_CONNECT_PCB5")) == 0) {
		GPIO_ResetBits(GPIOB, GPIO_Pin_5);						 //PB.5 输出低, 停止供电, PCB1_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_6);						 //PB.6 输出低, 停止供电, PCB2_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_7);						 //PB.7 输出低, 停止供电, PCB3_POWER_OFF
		GPIO_ResetBits(GPIOB, GPIO_Pin_8);						 //PB.8 输出低, 停止供电, PCB4_POWER_OFF
		delay_ms(500);											 //延迟500ms
		GPIO_SetBits(GPIOB, GPIO_Pin_4);						 //PB.4 输出高, 开始供电, JTAG_POWER_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_3);					 	 //PB.3 输出高, 开始供电, JTAG_ON
		GPIO_SetBits(GPIOB, GPIO_Pin_9);					 	 //PB.9 输出高, 开始供电, PCB5_POWER_ON
	}
	if (size == strlen("JTAG_CONNECT_LIST") && strncmp(USART_RX_BUF, "JTAG_CONNECT_LIST", strlen("JTAG_CONNECT_LIST")) == 0) {
		Print_Port_State("JTAG", GPIO_Pin_3);
		Print_Port_State("JTAG POWER", GPIO_Pin_4);
		Print_Port_State("PCB1", GPIO_Pin_5);
		Print_Port_State("PCB2", GPIO_Pin_6);
		Print_Port_State("PCB3", GPIO_Pin_7);
		Print_Port_State("PCB4", GPIO_Pin_8);
		Print_Port_State("PCB5", GPIO_Pin_9);
	}
}

void start_task(void *pvParameters)	//任务函数
{
	taskENTER_CRITICAL();				//进入临界区
	Uart2Task_Handler = NULL;			//任务句柄初始化
	xTaskCreate((TaskFunction_t )uart2_task,	//任务函数
				  (const char*    )"uart2_task",//任务名称
				  (uint16_t       )UART2_STK_SIZE,//任务堆栈大小
				  (void*          )NULL,      	//传递给任务函数的参数
				  (UBaseType_t    )UART2_TASK_PRIO,//任务优先级
				  (TaskHandle_t*  )&Uart2Task_Handler //任务句柄
				 );
	vTaskDelete(StartTask_Handler);	//删除开始任务
	taskEXIT_CRITICAL();				//退出临界区
}

void uart2_task(void *pvParameters)	//任务函数
{
 	u16 t;
	u16 len;
	u16 times=0;
	while(1)
	{
		if(USART_RX_STA&0x8000)
		{					   
			len=USART_RX_STA&0x3fff;//得到此次接收到的数据长度
			uartPrintf("\r\nStar_WalkIn has received the instruction->\r\n");
			Uart_Process(len);
			delay_ms(10);
			for(t=0;t<len;t++)
			{
				USART_SendData(USART2, USART_RX_BUF[t]);//向串口2发送数据
				while(USART_GetFlagStatus(USART2,USART_FLAG_TC)!=SET);//等待发送结束
			}
			uartPrintf("\r\n");//插入换行
			USART_RX_STA=0;
		}else
		{
			times++;
			if(times%500==0)
			{
				uartPrintf("\r\nStar_WalkIn Is_Living\r\n");
				LED1=~LED1;
			}
			delay_ms(10);   
		}
	}
}

 int main(void)
 {
	delay_init();	    							//延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 							//串口初始化为115200
 	LED_Init();			    			 			//LED端口初始化
	KEY_Init();          							//初始化与按键连接的硬件接口

	xTaskCreate((TaskFunction_t )start_task,     	//任务函数
				  (const char*    )"start_task",   	//任务名称
				  (uint16_t       )START_STK_SIZE, 	//任务堆栈大小
				  (void*          )NULL,           	//传递给任务函数的参数
				  (UBaseType_t    )START_TASK_PRIO,	//任务优先级
				  (TaskHandle_t*  )&StartTask_Handler //任务句柄
				 );
	vTaskStartScheduler(); 						//开启任务调度~
	while(1);
	return 0;
 }

