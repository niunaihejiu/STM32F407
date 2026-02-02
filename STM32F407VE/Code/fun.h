#ifndef FUN_H
#define FUN_H

#include "main.h"

/************************ 函数声明 ************************/
// MQ2传感器相关函数
void MQ2_Preheat_Tips(void);          // MQ2预热提示
uint16_t MQ2_Read_AO_Value(void);     // 读取MQ2 AO引脚ADC原始值
float MQ2_Convert_Voltage(uint16_t adc_val); // ADC值转电压（0~3.3V）
uint8_t MQ2_Read_DO_State(void);      // 读取MQ2 DO引脚状态（0/1）

// 蜂鸣器相关函数
void Buzzer_On(void);                 // 蜂鸣器响（低电平触发）
void Buzzer_Off(void);                // 蜂鸣器停（高电平）

// LED控制函数
void LED_On(void);                    // LED亮
void LED_Off(void);                   // LED灭

// 主逻辑函数
void Gas_Detection_System(void);      // 气体检测主系统函数

#endif /* FUN_H */