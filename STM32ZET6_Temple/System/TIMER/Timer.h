#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"


// 外部/内部时钟模式宏定义
#define TIMER_INTCLK   0  // 内部时钟
#define TIMER_EXTCLK1  1  // 外部时钟模式1
#define TIMER_EXTCLK2  2  // 外部时钟模式2

// 定时器结构体
typedef struct
{
    TIM_TypeDef *TIMx;                 // 定时器实例
    uint32_t TIMx_CLK;                  // 定时器时钟
    IRQn_Type TIMx_IRQn;                // 中断通道

    uint16_t Prescaler;                 // 预分频
    uint16_t Period;                    // 自动重装载值
    uint16_t RepetitionCounter;         // 重复计数器（高级定时器使用）

    uint16_t ClockDivision;             // 时钟分割
    uint16_t CounterMode;               // 计数器模式

    uint32_t NVIC_PriorityGroup;         // NVIC分组
    uint8_t PreemptionPriority;         // 抢占优先级
    uint8_t SubPriority;                // 响应优先级

    void (*Callback)(void);             // 定时器中断回调函数指针，用户中断处理逻辑

    // 外部时钟参数
    uint8_t ExternalClockMode;       // 2=外部时钟模式2, 1=外部时钟模式1, 0=内部时钟
    uint16_t ExternalClockPrescaler; // 外部时钟预分频
    uint16_t ExternalClockFilter;    // 外部时钟滤波
    uint16_t ExternalClockPolarity;  // 外部时钟极性

    // 外部时钟引脚
    GPIO_TypeDef *ETR_GPIO_Port;       // 外部时钟ETR引脚端口
    uint16_t ETR_GPIO_Pin;             // 外部时钟ETR引脚编号

    uint8_t EnableInterrupt;           // 是否启用定时器中断

} Timer_Config_t;

// API
void Timer_Init(Timer_Config_t *timer); // 通用定时器初始化函数
void Timer_IRQHandler(Timer_Config_t *timer); // 通用定时器中断函数
uint16_t Timer_GetCounter(Timer_Config_t *timer); // 获取定时器计数器值

#endif
