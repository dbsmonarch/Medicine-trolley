#ifndef _PID_H
#define _PID_H

#include <stdint.h> // ������׼�������͵Ķ���

void update_pid();
float calculate_position_error(uint8_t sensor_values[8]);
int constrain(int value, int min_val, int max_val);
void set_motor_speed(int left_speed, int right_speed);

#endif // _PID_H
