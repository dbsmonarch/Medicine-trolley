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
	GPIO_InitTypeDef GPIO_InitStructure;     //���ڶ˿����ýṹ�����
	USART_InitTypeDef USART_InitStructure;   //���ڲ������ýṹ�����
	NVIC_InitTypeDef NVIC_InitStructure;     //�����ж����ýṹ�����
 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);   //��PA�˿�ʱ��
 
    //USART1_TX   PA9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;          		 //PA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  		 //�趨IO�ڵ�����ٶ�Ϊ50MHz
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	   		 //�����������
    GPIO_Init(GPIOA, &GPIO_InitStructure);             	 	 //��ʼ��PA9
    //USART1_RX	  PA10
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;             //PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //��������
    GPIO_Init(GPIOA, &GPIO_InitStructure);                 //��ʼ��PA10 
 
    //USART1 NVIC ����
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;  //��ռ���ȼ�0
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		  //�����ȼ�2
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  //IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	                          //����ָ���Ĳ�����ʼ��VIC�Ĵ���
 
    //USART ��ʼ������
		USART_InitStructure.USART_BaudRate = 9600;                  //���ڲ�����Ϊ115200
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;   //�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;        //һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;           //����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;   //��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	                  //�շ�ģʽ
    USART_Init(USART1, &USART_InitStructure);                     //��ʼ������1
 
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); //ʹ���ж�
    USART_Cmd(USART1, ENABLE);                     //ʹ�ܴ���1
 
  	//�����������1���ֽ��޷���ȷ���ͳ�ȥ������
	  USART_ClearFlag(USART1, USART_FLAG_TC);        //�崮��1���ͱ�־
		
}



 
//USART1 ȫ���жϷ�����
void USART1_IRQHandler(void)			 
{
//	int openmv_data[6];
//	static int i = 0;
//	if(USART_GetFlagStatus(USART1,USART_IT_RXNE) != RESET)
//	{
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);// �����־λ
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
//		USART_ClearITPendingBit(USART1,USART_IT_RXNE);// �����־λ
//	}
	
	
	
	
	u8 RxData;
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE) == SET)// �ȴ����ݽ���
	{
		RxData = USART_ReceiveData(USART1);// ��ȡ���յ�������:�����ݴ洢��RxData
//		USART_SendData(USART1, RxData);
		openmv_num = RxData;
		OLED_ShowNum(1,1,openmv_num,5);
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);// �����־λ
	}
}
