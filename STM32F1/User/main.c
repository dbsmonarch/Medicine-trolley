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

//openmv数据
int openmv_num=-1,openmv_x,openmv_y,openmv_z;
//读取的开关量数据
uint8_t gray_sensor[8];
uint8_t digital_data;
//判断去或回
int regression=0;
//判断停车
int stop =1;





//设置软件IIC驱动
sw_i2c_interface_t i2c_interface = 
{
	.sda_in = sda_in,
	.scl_out = scl_out,
	.sda_out = sda_out,
	.user_data = 0, //用户数据，可在输入输出函数里得到
};



//读取开关量数据
void Tracking_Data(void);
//屏幕显示
void OLED_reveal();





int main(void)
{
	//初始化
	OLED_Init();
	PWM_Init();
	USART1_Init();
	Motor_Init();
	sw_i2c_init();

	
	
	
	
	/* 存放扫描到的地址 */
	uint8_t scan_addr[128] = {0};
	volatile uint8_t count;
	uint8_t ping_response;
	

	//ps: 软件IIC初始化的时候会出发一次IIC start，会导致第一次IIC通讯会失败
	//第一次IIC通讯会失败（因为软件IIC触发了start），手动发个stop也能消除
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);
	//后面IIC通讯是正常的
	sw_i2c_mem_read(&i2c_interface, 0x4C << 1, GW_GRAY_PING, &ping_response, 1);
	
	//扫描开始
	count = i2c_scan(&i2c_interface, scan_addr);

	
	//打开开关量数据模式
	sw_i2c_write_byte(&i2c_interface, 0x4C << 1, GW_GRAY_DIGITAL_MODE);
	
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data 有1~8号探头开关数据
	
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
	// 读取开关量数据 
	sw_i2c_read_byte(&i2c_interface, 0x4C << 1, &digital_data); // digital_data 有1~8号探头开关数据
				
	// 把字节里的8个开关量存到八个变量里，为gray_sensor[0] ~ gray_sensor[7], 
	SEP_ALL_BIT8(digital_data, 
		gray_sensor[0], //探头1
		gray_sensor[1], //探头2
		gray_sensor[2], //探头3
		gray_sensor[3], //探头4
		gray_sensor[4], //探头5
		gray_sensor[5], //探头6
		gray_sensor[6], //探头7
		gray_sensor[7]  //探头8
	);
		

		
}
