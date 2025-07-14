#include "stm32f10x.h"
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Usart.h"
#include "PWM.h"
#include "sw_i2c.h"
#include "gw_grayscale_sensor.h"
#include "Tracking.h"
#include "PID.h"
#include <math.h>

int8_t Speed=0;

//openmv����
int openmv_num=-1,openmv_x,openmv_y,openmv_z;
//��ȡ�Ŀ���������
uint8_t gray_sensor[8];
uint8_t digital_data;
//�ж�ȥ���
int regression=0;
//�ж�ͣ��
int stop =1;





//�������IIC����
sw_i2c_interface_t i2c_interface = 
{
	.sda_in = sda_in,
	.scl_out = scl_out,
	.sda_out = sda_out,
	.user_data = 0, //�û����ݣ������������������õ�
};



//��ȡ����������
void Tracking_Data(void);
//��Ļ��ʾ
void OLED_reveal();





int main(void)
{
	//��ʼ��
	OLED_Init();
	PWM_Init();
	USART1_Init();
	Motor_Init();
	sw_i2c_init();

	
	
	
	
	/* ���ɨ�赽�ĵ�ַ */
	uint8_t scan_addr[128] = {0};
	volatile uint8_t count;
	uint8_t ping_response;
	

	//ps: ���IIC��ʼ����ʱ������һ��IIC start���ᵼ�µ�һ��IICͨѶ��ʧ��
	//��һ��IICͨѶ��ʧ�ܣ���Ϊ���IIC������start�����ֶ�����stopҲ������
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);
	//����IICͨѶ��������
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);
	
	//ɨ�迪ʼ
	count = i2c_scan(&i2c_interface, scan_addr);

	
	//�򿪿���������ģʽ
	sw_i2c_write_byte(&i2c_interface, 0x4C << 1, GW_GRAY_DIGITAL_MODE);
	
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data ��1~8��̽ͷ��������
	
	while(1)
	{
		Motor_SetSpeed1(100);
		Motor_SetSpeed4(100);
		OLED_reveal();
//		OLED_reveal();
//		Tracking_Data();
//		while(openmv_num>'0'&&openmv_num<'9')
//		{
//			if(openmv_num=='1')
//			{
//				while(1)
//				{
//					OLED_reveal();
//					Tracking_Data();
//					update_pid();
//					if(digital_data==0)
//					{
//						if(stop==1&&regression==1)
//						{
//							Motor_SetSpeed1(0);
//							Motor_SetSpeed4(0);
//							return 0;
//						}
//						if(regression==0)
//						{
//							Left_turn();
//							stop++;
//						}
//						if(regression==1)
//						{
//							Right_turn();
//							stop--;
//						}
//					}
//					Tracking_Data();
//					if(digital_data==255&&regression==0)
//					{
//						Turn_round();
//						regression=1;

//					}
//				}
//			}
//			if(openmv_num=='2')
//			{
//				while(1)
//				{
//					OLED_reveal();
//					Tracking_Data();
//					update_pid();
//					if(digital_data==0)
//					{
//						if(stop==1&&regression==1)
//						{
//							Motor_SetSpeed1(0);
//							Motor_SetSpeed4(0);
//							return 0;
//						}
//						if(regression==0)
//						{
//							Right_turn();
//							stop++;
//						}
//						if(regression==1)
//						{
//							Left_turn();
//							stop--;
//						}
//					}
//					Tracking_Data();
//					if(digital_data==255&&regression==0)
//					{
//						Turn_round();
//						regression=1;

//					}
//				}
//			}
//			if(openmv_num>'2'&&openmv_num<'9')
//			{
//				while(1)
//				{
//					OLED_reveal();
//					Tracking_Data();
//					update_pid();
//					if(digital_data==0)
//					{
//						if(stop==1&&regression==1)
//						{
//							Motor_SetSpeed1(0);
//							Motor_SetSpeed4(0);
//							return 0;
//						}
//						if(regression==0&&stop==2)
//						{
//							Motor_SetSpeed1(0);
//							Motor_SetSpeed4(0);
//							USART_SendData(USART1, '1');
//							Delay_ms(2500);
//							if(openmv_num=='1')
//							{
//								Left_turn();
//							}
//							if(openmv_num=='2')
//							{
//								Right_turn();
//							}
//							stop++;
//						}
//						if(regression==0&&stop==1)
//						{
//							Delay_ms(1000);
//							stop++;
//						}
//						if(regression==1&&stop==2)
//						{
//							Delay_ms(1000);
//							stop--;
//						}
//						if(regression==1&&stop==3)
//						{
//							if(openmv_num=='1')
//							{
//								Right_turn();
//							}
//							if(openmv_num=='2')
//							{
//								Left_turn();
//							}
//							stop--;
//						}
//					}
//					Tracking_Data();
//					if(digital_data==255&&regression==0)
//					{
//						Turn_round();
//						regression=1;

//					}
//				}
//			}
//		}
////		update_pid();
////		Left_turn(1500);
////		Right_turn(1500);
////		Turn_round(2000);
	}
	
}


void OLED_reveal()
{
	OLED_ShowNum(1,1,openmv_num,5);
	OLED_ShowNum(2,1,digital_data,5);
	OLED_ShowNum(4,4,gray_sensor[0],1);
	OLED_ShowNum(4,5,gray_sensor[1],1);
	OLED_ShowNum(4,6,gray_sensor[2],1);
	OLED_ShowNum(4,7,gray_sensor[3],1);
	OLED_ShowNum(4,8,gray_sensor[4],1);
	OLED_ShowNum(4,9,gray_sensor[5],1);
	OLED_ShowNum(4,10,gray_sensor[6],1);
	OLED_ShowNum(4,11,gray_sensor[7],1);
}

void Tracking_Data()
{
	// ��ȡ���������� 
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data ��1~8��̽ͷ��������
				
	// ���ֽ����8���������浽�˸������Ϊgray_sensor[0] ~ gray_sensor[7], 
	SEP_ALL_BIT8(digital_data, 
		gray_sensor[0], //̽ͷ1
		gray_sensor[1], //̽ͷ2
		gray_sensor[2], //̽ͷ3
		gray_sensor[3], //̽ͷ4
		gray_sensor[4], //̽ͷ5
		gray_sensor[5], //̽ͷ6
		gray_sensor[6], //̽ͷ7
		gray_sensor[7]  //̽ͷ8
	);
		

		
}
