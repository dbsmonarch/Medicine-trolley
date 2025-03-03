#ifndef __TRACKING_H
#define __TRACKING_H

#define SW_I2C1_PIN_SCL GPIO_Pin_6
#define SW_I2C1_PIN_SDA GPIO_Pin_7

/* 测试模拟数据改成0, 测试开关量数据改成1 */
#define GW_READ_DIGITAL_DATA 1


void sw_i2c_init();
void sda_out(uint8_t bit, void *user_data);
uint8_t sda_in(void *user_data);
void scl_out(uint8_t bit, void *user_data);
uint8_t i2c_scan(sw_i2c_interface_t *i2c_interface, uint8_t *scan_addr);


#endif //TRACKING
