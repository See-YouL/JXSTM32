#ifndef __PWM_H
#define __PWM_H

#include "stm32f10x.h"

/**
 * @brief PWM配置结构体
 */
typedef struct
{
    TIM_TypeDef *TIMx;           // 定时器
    uint32_t TIMx_CLK;           // 定时器时钟
    uint16_t Prescaler;          // 预分频
    uint16_t Period;             // 自动重装载值
    uint16_t Pulse;              // 初始占空比

    uint8_t Channel;             // 通道号 (1~4)

    GPIO_TypeDef *GPIOx;         // GPIO端口
    uint16_t GPIO_Pin;           // GPIO引脚

    uint16_t ClockDivision;      // 时钟分频
    uint16_t CounterMode;        // 计数模式

    uint16_t OCMode;             // 输出比较模式 (如 TIM_OCMode_PWM1)
    uint16_t OCPolarity;         // 输出极性 (如 TIM_OCPolarity_High/Low)
    uint16_t OutputState;        // 输出使能 (TIM_OutputState_Enable/Disable)
} PWM_Config_t;

void PWM_Init(PWM_Config_t *config); // PWM初始化
void PWM_SetCompare(PWM_Config_t *config, uint16_t Compare); // PWM 设定占空比(CCR)
void PWM_SetPrescaler(PWM_Config_t *config, uint16_t Prescaler); // PWM 设定预分频(PSC)
void PWM_SetPeriod(PWM_Config_t *config, uint16_t Period); // PWM 设定周期(ARR)
void PWM_UpdateFreqDuty(PWM_Config_t *config, uint32_t freq, float duty); // 设定PWM的频率和占空比 
void PWM_GetFreqDuty(PWM_Config_t *config, uint32_t *freq, float *duty); // 读取实际PWM的频率和占空比

#endif
