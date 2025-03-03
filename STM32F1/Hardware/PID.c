#include "PID.h"
#include "Motor.h"
#include "OLED.h"



// 假设有一个名为gray_sensor的全局变量存储传感器数据
extern uint8_t gray_sensor[8];

// PID参数
static const float Kp = 2.0; // 比例系数
static const float Ki = 0.0; // 积分系数
static const float Kd = 0.0; // 微分系数

// PID变量
static float previous_error = 0.0;
static float integral = 0.f;

void update_pid() 
{
    // 这个函数会计算并更新电机速度来实现转向

    // 利用传感器数组计算位置偏差
    float error = calculate_position_error(gray_sensor);

    // 计算PID
    integral += error;
    float derivative = error - previous_error;
    previous_error = error;
    float output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // 计算电机速度，这里的"base_speed"应该是一个可以让机器人前进的速度
    int base_speed = 20; // 基础速度
    int max_speed = 100;  // 最大速度限制，保证机器人不会运行得过快
    int left_motor_speed = constrain(base_speed + output, 0, max_speed);
    int right_motor_speed = constrain(base_speed - output, 0, max_speed);
		OLED_ShowNum(3,1,base_speed + output,5);

    // 设置电机速度
    set_motor_speed(left_motor_speed, right_motor_speed);
}

float calculate_position_error(uint8_t sensor_values[8]) 
	{
    // 为每个传感器分配权重，中间传感器为0，向外侧权重逐渐增大
    const int sensor_weights[8] = {-4, -3, -2, -1, 1, 2, 3, 4};
    float position_error = 0.0f;
    int line_detected = 0;

    // 计算偏差值
    for (int i = 0; i < 8; i++) 
	{
        // 如果传感器检测到线（假设0表示检测到线），增加偏差
        if (sensor_values[i] == 0) 
		{
            position_error += sensor_weights[i];
            line_detected += 1;
        }
    }

    
	if (line_detected > 0) 
	{
        position_error /= line_detected;
    } 
	else 
	{
        position_error = 0; // 一个特殊值，表示没有检测到线
    }
	return position_error;
}


int constrain(int value, int min_val, int max_val) {
    // 辅助函数，用于限制值的范围
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

void set_motor_speed(int left_speed, int right_speed) 
{
    Motor_SetSpeed1(left_speed);
	Motor_SetSpeed4(right_speed);
}
