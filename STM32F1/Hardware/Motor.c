#include "stm32f10x.h"                  // Device header
#include "PWM.h"
#include "Delay.h"

extern uint8_t gray_sensor[8];

void Motor_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5|GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitTypeDef GPIO_InitStructure1;
	GPIO_InitStructure1.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure1.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1|GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure1.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure1);
	
	PWM_Init();
}

void Motor_SetSpeed1(int8_t Speed)
{
	Speed=-Speed;
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		PWM_SetCompare1(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		PWM_SetCompare1(-Speed);
	}
}

//void Motor_SetSpeed2(int8_t Speed)
//{
//	if (Speed >= 0)
//	{
//		GPIO_SetBits(GPIOA, GPIO_Pin_6);
//		GPIO_ResetBits(GPIOA, GPIO_Pin_7);
//		PWM_SetCompare2(Speed);
//	}
//	else
//	{
//		GPIO_ResetBits(GPIOA, GPIO_Pin_6);
//		GPIO_SetBits(GPIOA, GPIO_Pin_7);
//		PWM_SetCompare2(-Speed);
//	}
//}

//void Motor_SetSpeed3(int8_t Speed)
//{
//	Speed=-Speed;
//	if (Speed >= 0)
//	{
//		GPIO_SetBits(GPIOB, GPIO_Pin_0);
//		GPIO_ResetBits(GPIOB, GPIO_Pin_1);
//		PWM_SetCompare3(Speed);
//	}
//	else
//	{
//		GPIO_ResetBits(GPIOB, GPIO_Pin_0);
//		GPIO_SetBits(GPIOB, GPIO_Pin_1);
//		PWM_SetCompare3(-Speed);
//	}
//}

void Motor_SetSpeed4(int8_t Speed)
{
	if (Speed >= 0)
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);
		PWM_SetCompare4(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_10);
		GPIO_SetBits(GPIOB, GPIO_Pin_11);
		PWM_SetCompare4(-Speed);
	}
}


void Left_turn(void)
{
	Motor_SetSpeed1(-10);
	Motor_SetSpeed4(30);
	Delay_ms(1400);
	Motor_SetSpeed1(0);
	Motor_SetSpeed4(0);
}

void Right_turn(void)
{
	Motor_SetSpeed1(30);
	Motor_SetSpeed4(-10);
	Delay_ms(1400);
	Motor_SetSpeed1(0);
	Motor_SetSpeed1(0);
}

void Turn_round()
{
	Motor_SetSpeed1(30);
	Motor_SetSpeed4(-30);
	Delay_ms(1650);
	Motor_SetSpeed1(20);
	Motor_SetSpeed4(20);
}


