#ifndef FUN_H
#define FUN_H

#include "main.h"

// 函数声明
void MQ2_Preheat_Tips(void);                      // MQ2预热提示
uint16_t MQ2_Read_AO_Value(void);                 // 读取MQ2 ADC值
float MQ2_Convert_Voltage(uint16_t adc_val);      // ADC转电压
uint8_t MQ2_Read_DO_State(void);                  // 读取DO状态

void Buzzer_On(void);                             // 蜂鸣器开启
void Buzzer_Off(void);                            // 蜂鸣器关闭

void LED_On(void);                                // LED开启
void LED_Off(void);                               // LED关闭

void Gas_Detection_System(void);                  // 气体检测主函数

// 校准函数
void MQ2_Calibrate(float multimeter_voltage, uint16_t adc_value);  // 校准传感器

#endif /* FUN_H */