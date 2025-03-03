#include "stm32f10x.h"                  // Device header

void PWM_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//初始化pwm输出io口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1;		//ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 36 - 1;		//PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	//通道1
	TIM_OCInitTypeDef TIM_OC1InitStructure;
	TIM_OCStructInit(&TIM_OC1InitStructure);
	TIM_OC1InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC1InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC1InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC1InitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC1Init(TIM2, &TIM_OC1InitStructure);
	
//	//通道2
//	TIM_OCInitTypeDef TIM_OC2InitStructure;
//	TIM_OCStructInit(&TIM_OC2InitStructure);
//	TIM_OC2InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OC2InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OC2InitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OC2InitStructure.TIM_Pulse = 0;		//CCR
//	TIM_OC2Init(TIM2, &TIM_OC2InitStructure);
//	
//	//通道3
//	TIM_OCInitTypeDef TIM_OC3InitStructure;
//	TIM_OCStructInit(&TIM_OC3InitStructure);
//	TIM_OC3InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//	TIM_OC3InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
//	TIM_OC3InitStructure.TIM_OutputState = TIM_OutputState_Enable;
//	TIM_OC3InitStructure.TIM_Pulse = 0;		//CCR
//	TIM_OC3Init(TIM2, &TIM_OC3InitStructure);
	
	//通道4
	TIM_OCInitTypeDef TIM_OC4InitStructure;
	TIM_OCStructInit(&TIM_OC4InitStructure);
	TIM_OC4InitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC4InitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC4InitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OC4InitStructure.TIM_Pulse = 0;		//CCR
	TIM_OC4Init(TIM2, &TIM_OC4InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
}

void PWM_SetCompare1(uint16_t Compare)
{
	TIM_SetCompare1(TIM2, Compare);
}

//void PWM_SetCompare2(uint16_t Compare)
//{
//	TIM_SetCompare2(TIM2, Compare);
//}

//void PWM_SetCompare3(uint16_t Compare)
//{
//	TIM_SetCompare3(TIM2, Compare);
//}

void PWM_SetCompare4(uint16_t Compare)
{
	TIM_SetCompare4(TIM2, Compare);
}
