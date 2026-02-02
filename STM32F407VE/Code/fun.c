#include "fun.h"
#include "gpio.h"
#include "usart.h"
#include "adc.h"
#include <string.h>
#include <stdio.h>

/************************ 蜂鸣器控制 ************************/
// 蜂鸣器响（低电平）
void Buzzer_On(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);  // PB0低电平
}

// 蜂鸣器停（高电平）
void Buzzer_Off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);    // PB0高电平
}

/************************ LED控制 ************************/
void LED_On(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_RESET);  // PB2低电平，LED亮
}

void LED_Off(void)
{
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_2, GPIO_PIN_SET);    // PB2高电平，LED灭
}

/************************ MQ2传感器驱动 ************************/
// MQ2预热提示（串口打印）
void MQ2_Preheat_Tips(void)
{
    printf("MQ2 sensor preheating... Please wait 2-3 minutes for stable reading!\r\n");
    HAL_Delay(1000);
}

// 读取MQ2 AO引脚ADC原始值（10次采样平均，提高精度）
uint16_t MQ2_Read_AO_Value(void)
{
    uint16_t adc_val = 0;
    uint8_t sample_cnt = 10;

    // 启动ADC转换
    HAL_ADC_Start(&hadc1);

    // 多次采样取平均
    for(uint8_t i = 0; i < sample_cnt; i++)
    {
        if(HAL_ADC_PollForConversion(&hadc1, 100) == HAL_OK)
        {
            adc_val += HAL_ADC_GetValue(&hadc1);
        }
        HAL_Delay(10);
    }

    HAL_ADC_Stop(&hadc1);
    return adc_val / sample_cnt;
}

// ADC值转换为电压（0~4095 → 0~3.3V）
float MQ2_Convert_Voltage(uint16_t adc_val)
{
    return (float)adc_val / 4095.0f * 3.3f;
}

// 读取MQ2 DO引脚状态（适配你的硬件：0=可燃气体，1=正常空气）
uint8_t MQ2_Read_DO_State(void)
{
    return HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1);
}

/************************ 气体检测系统主逻辑 ************************/
void Gas_Detection_System(void)
{
    static uint8_t init_flag = 0;
    uint16_t mq2_adc;
    float mq2_volt;
    uint8_t mq2_do;

    // 首次调用：初始化
    if(init_flag == 0)
    {
        MQ2_Preheat_Tips(); // MQ2预热提示
        Buzzer_Off();       // 确保蜂鸣器关闭
        LED_Off();          // 确保LED关闭
        init_flag = 1;
    }

    // 第1阶段：LED关闭，读取传感器
    LED_Off();
    
    mq2_adc = MQ2_Read_AO_Value();
    mq2_volt = MQ2_Convert_Voltage(mq2_adc);
    mq2_do = MQ2_Read_DO_State();

    // 根据DO状态控制蜂鸣器
    if(mq2_do == 0)  // DO=0：检测到气体
    {
        Buzzer_On();
        printf("LED_OFF | 气体检测！ADC:%4d | 电压:%.2fV | DO:%d | 蜂鸣器:ON\r\n", 
               mq2_adc, mq2_volt, mq2_do);
    }
    else  // DO=1：无气体
    {
        Buzzer_Off();
        printf("LED_OFF | 正常状态。ADC:%4d | 电压:%.2fV | DO:%d | 蜂鸣器:OFF\r\n", 
               mq2_adc, mq2_volt, mq2_do);
    }
    HAL_Delay(1000);

    // 第2阶段：LED开启，再次读取传感器
    LED_On();
    
    mq2_adc = MQ2_Read_AO_Value();
    mq2_volt = MQ2_Convert_Voltage(mq2_adc);
    mq2_do = MQ2_Read_DO_State();

    // 根据DO状态控制蜂鸣器
    if(mq2_do == 0)  // DO=0：检测到气体
    {
        Buzzer_On();
        printf("LED_ON  | 气体检测！ADC:%4d | 电压:%.2fV | DO:%d | 蜂鸣器:ON\r\n", 
               mq2_adc, mq2_volt, mq2_do);
    }
    else  // DO=1：无气体
    {
        Buzzer_Off();
        printf("LED_ON  | 正常状态。ADC:%4d | 电压:%.2fV | DO:%d | 蜂鸣器:OFF\r\n", 
               mq2_adc, mq2_volt, mq2_do);
    }
    HAL_Delay(1000);
}