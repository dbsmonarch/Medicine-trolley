#include "PID.h"
#include "Motor.h"
#include "OLED.h"



// ������һ����Ϊgray_sensor��ȫ�ֱ����洢����������
extern uint8_t gray_sensor[8];

// PID����
static const float Kp = 2.0; // ����ϵ��
static const float Ki = 0.0; // ����ϵ��
static const float Kd = 0.0; // ΢��ϵ��

// PID����
static float previous_error = 0.0;
static float integral = 0.f;

void update_pid() 
{
    // �����������㲢���µ���ٶ���ʵ��ת��

    // ���ô������������λ��ƫ��
    float error = calculate_position_error(gray_sensor);

    // ����PID
    integral += error;
    float derivative = error - previous_error;
    previous_error = error;
    float output = (Kp * error) + (Ki * integral) + (Kd * derivative);

    // �������ٶȣ������"base_speed"Ӧ����һ�������û�����ǰ�����ٶ�
    int base_speed = 20; // �����ٶ�
    int max_speed = 100;  // ����ٶ����ƣ���֤�����˲������еù���
    int left_motor_speed = constrain(base_speed + output, 0, max_speed);
    int right_motor_speed = constrain(base_speed - output, 0, max_speed);
		OLED_ShowNum(3,1,base_speed + output,5);

    // ���õ���ٶ�
    set_motor_speed(left_motor_speed, right_motor_speed);
}

float calculate_position_error(uint8_t sensor_values[8]) 
	{
    // Ϊÿ������������Ȩ�أ��м䴫����Ϊ0�������Ȩ��������
    const int sensor_weights[8] = {-4, -3, -2, -1, 1, 2, 3, 4};
    float position_error = 0.0f;
    int line_detected = 0;

    // ����ƫ��ֵ
    for (int i = 0; i < 8; i++) 
	{
        // �����������⵽�ߣ�����0��ʾ��⵽�ߣ�������ƫ��
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
        position_error = 0; // һ������ֵ����ʾû�м�⵽��
    }
	return position_error;
}


int constrain(int value, int min_val, int max_val) {
    // ������������������ֵ�ķ�Χ
    if (value < min_val) return min_val;
    if (value > max_val) return max_val;
    return value;
}

void set_motor_speed(int left_speed, int right_speed) 
{
    Motor_SetSpeed1(left_speed);
	Motor_SetSpeed4(right_speed);
}
