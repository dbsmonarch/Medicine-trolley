#include "stm32f10x.h"
#include "Usart.h"
#include "LED.h"
#include "stdio.h"
#include "OLED.h"

extern int openmv_num,openmv_x,openmv_y,openmv_z;
 
 
void USART1_Init(void)
{
	//USART1_TX:PA 9   
	//USART1_RX:PA10
	GPIO_InitTypeDef GPIO_InitStructure;     //串口端口配置结构体变量
	USART_InitTypeDef USART_InitStructure;   //串口参数配置结构体变量
	NVIC_InitTypeDef NVIC_InitStructure;     //串口中断配置结构体变量
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //打开PA端口时钟
 
    //USART1_TX   PA9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          		 //PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  		 //设定IO口的输出速度为50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		 //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);             	 	 //初始化PA9
    //USART1_RX	  PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //初始化PA10 
 
    //USART1 NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;  //抢占优先级0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //子优先级2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQ通道使能
		NVIC_Init(&NVIC_InitStructure);	                          //根据指定的参数初始化VIC寄存器
 
    //USART 初始化设置
		USART_InitStructure.USART_BaudRate = 9600;                  //串口波特率为115200
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //字长为8位数据格式
		USART_InitStructure.USART_StopBits = USART_StopBits_1;        //一个停止位
		USART_InitStructure.USART_Parity = USART_Parity_No;           //无奇偶校验位
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //无硬件数据流控制
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                  //收发模式
    USART_Init(USART1, &USART_InitStructure);                     //初始化串口1
 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //使能中断
    USART_Cmd(USART1, ENABLE);                     //使能串口1
 
  	//如下语句解决第1个字节无法正确发送出去的问题
	  USART_ClearFlag(USART1, USART_FLAG_TC);        //清串口1发送标志
		
}



 
//USART1 全局中断服务函数
void USART1_IRQHandler(void)			 
{
//	int openmv_data[6];
//	static int i = 0;
//	if(USART_GetFlagStatus(USART1,USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);// 清除标志位
//		openmv_data[i] = USART_ReceiveData(USART1);
//		i++;
//		if(openmv_data[0] != 0xa3)
//		{
//			i = 0;
//		}
//		if((i > 1) && (openmv_data[1] != 0xb3))
//		{
//			i = 0; 
//		}
//		if(i == 6)
//		{
//			if(openmv_data[5] == 0xc3 && openmv_data[1] == 0xb3 && openmv_data[0] == 0xa3)
//			{
//				openmv_num = openmv_data[2];
//				openmv_x = openmv_data[3];
//				openmv_y = openmv_data[4];
//			}
//			i=0;
//		}
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);// 清除标志位
//	}
	
	
	
	
	u8 RxData;
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE) == SET)// 等待数据接收
	{
		RxData = USART_ReceiveData(USART1);// 读取接收到的数据:把数据存储到RxData
//		USART_SendData(USART1, RxData);
		openmv_num = RxData;
		OLED_ShowNum(1,1,openmv_num,5);
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);// 清除标志位
	}
}
