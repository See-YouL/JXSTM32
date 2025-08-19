#ifndef __IC_H
#define __IC_H

#include "stm32f10x.h"

/**
 * @brief 输入捕获配置结构体
 */
typedef struct
{
    TIM_TypeDef *TIMx;           // 定时器
    uint32_t TIMx_CLK;           // 定时器时钟

    GPIO_TypeDef *GPIOx;         // GPIO端口
    uint16_t GPIO_Pin;           // GPIO引脚

    uint8_t Channel;             // 捕获通道 (1~4)

    uint16_t Prescaler;          // 预分频
    uint16_t Period;             // 自动重装载值
    uint16_t ClockDivision;      // 时钟分频
    uint16_t CounterMode;        // 计数模式
    uint8_t  RepetitionCounter;  // 重复计数器 (高级定时器用)

    uint16_t ICPolarity;         // 捕获极性 (上升沿/下降沿)
    uint16_t ICSelection;        // 捕获输入选择
    uint16_t ICPrescaler;        // 捕获分频
    uint16_t ICFilter;           // 输入滤波

    uint16_t SlaveMode;          // 从模式选择 (TIM_SlaveMode_Reset/Trigger/External1 等)
    uint16_t TriggerSource;      // 触发源选择 (TIM_TS_TI1FP1/TIM_TS_TI2FP2/ETRF 等)

} IC_Config_t;

void IC_Init(IC_Config_t *config);   // 输入捕获初始化
uint32_t IC_GetFreq(IC_Config_t *config); // 获取输入频率

#endif
