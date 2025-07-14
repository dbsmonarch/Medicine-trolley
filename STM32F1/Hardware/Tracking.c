#include "stm32f10x.h"
#include "sw_i2c.h"
#include "Tracking.h"
#include <stm32f10x_i2c.h>


void delay_us(uint32_t us)
{
	SysTick->LOAD = 72 * us;
	SysTick->VAL = 0x00;
	SysTick->CTRL = 0x00000005;
	while(!(SysTick->CTRL & 0x00010000));
	SysTick->CTRL = 0x00000004;
}

/**
 * ��ʼ��i2c
 */
void sw_i2c_init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Pin = SW_I2C1_PIN_SCL | SW_I2C1_PIN_SDA;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOB, SW_I2C1_PIN_SCL | SW_I2C1_PIN_SDA);
}

/* ����sda������� bit=0Ϊ�͵�ƽ bit=1Ϊ�ߵ�ƽ */
void sda_out(uint8_t bit, void *user_data)
{
	GPIO_WriteBit(GPIOB, SW_I2C1_PIN_SDA, (BitAction)bit);
	
	/* IIC����ӳ� */
	delay_us(10);
}

/* ����sda��ȡ���� bit Ϊ���صĵ�ƽֵ */
uint8_t sda_in(void *user_data)
{
	uint8_t bit;
	bit = (uint8_t)GPIO_ReadInputDataBit(GPIOB, SW_I2C1_PIN_SDA);
	
	/* IIC����ӳ� */
	delay_us(10);
	return bit;
}

/* ����sclʱ��������� bit=0Ϊ�͵�ƽ bit=1Ϊ�ߵ�ƽ */
void scl_out(uint8_t bit, void *user_data)
{
	GPIO_WriteBit(GPIOB, SW_I2C1_PIN_SCL, (BitAction)bit);
	
	/* IIC����ӳ� */
	delay_us(10);
}

/**
 * i2c��ַɨ��
 * @param scan_addr ɨ������ĵ�ַ���,��ֵ��Ϊ0��Ϊɨ�赽�ĵ�ַ��ɨ���ĵ�ַ�ᰤ�������������ǰ��
 * @return ����ɨ�赽���豸����, 0Ϊ���豸����
 */
uint8_t i2c_scan(sw_i2c_interface_t *i2c_interface, uint8_t *scan_addr)
{
	int i;
	uint8_t count = 0;
	uint8_t data;
	int8_t ret;
	
	for (i = 1; i < 127; ++i) {
		ret = sw_i2c_read(i2c_interface, i << 1, &data, 1);
		if (ret == 0) {
			scan_addr[count] = i;
			++count;
		}
	}
	
	return count;
}



