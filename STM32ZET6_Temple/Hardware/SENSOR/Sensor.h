#ifndef __SENSOR_H
#define __SENSOR_H

#include "stm32f10x.h" // 包含 STM32F10x 设备头文件

// 通用传感器配置结构体
typedef struct
{
    // GPIO
    GPIO_TypeDef* GPIO_Port;      // GPIO 端口
    uint16_t      GPIO_Pin;       // GPIO 引脚
    uint32_t      GPIO_Clk;       // GPIO 时钟
    GPIOMode_TypeDef GPIO_Mode;   // GPIO 模式
    GPIOSpeed_TypeDef GPIO_Speed; // GPIO 速度

    // EXTI
    uint32_t      AFIO_Clk;       // AFIO 时钟
    uint32_t      EXTI_Line;      // EXTI 中断线
    uint8_t       EXTI_PortSource;// EXTI 端口源
    uint8_t       EXTI_PinSource; // EXTI 引脚源
    EXTIMode_TypeDef EXTI_Mode;   // EXTI 模式 (中断/事件)
    EXTITrigger_TypeDef EXTI_Trigger; // EXTI 触发方式 (上升/下降/双边)

    // NVIC
    IRQn_Type     IRQn;           // NVIC 中断通道
    uint8_t       PreemptionPriority; // 抢占优先级
    uint8_t       SubPriority;        // 响应优先级
    uint32_t      NVIC_Group;         // NVIC 分组
} Sensor_Config_t;

// API
void Sensor_Init(Sensor_Config_t* config); // 初始化传感器
uint16_t Sensor_GetState(void); // 获取传感器状态
void Sensor_ClearState(void); // 清除传感器状态
void Sensor_IRQHandler(Sensor_Config_t* config); // 传感器中断处理函数

#endif
