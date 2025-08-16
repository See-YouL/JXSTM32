/**
 * @file FireSensor.c
 * @brief 火焰传感器驱动代码
 * 该文件实现了火焰传感器的初始化、状态获取和中断处理功能
 * @version 1.0
 * @date 2025-08-16
 * @author Eric
 */

#include "stm32f10x.h"                  // Device header
#include "Sensor.h"
#include "FireSensor.h"

/*
 * 火焰传感器:
 * 传感器链接: https://telesky.yuque.com/bdys8w/01/qyw1o22t6g5e278p
 * 传感器型号: 3针版
 * 注意事项: 当火焰强度超过阈值时,输出低电平,否则输出高电平
 * 传感器连接方法: DO 接 PB14
 */
/**
 * @brief 火焰传感器配置
 * 该结构体定义了火焰传感器的GPIO、EXTI和NVIC配置
 * 包括GPIO端口、引脚、时钟、模式、速度等参数
 * EXTI配置包括中断线、端口源、引脚源、触发方式等
 * NVIC配置包括中断通道、优先级等
 */
static Sensor_Config_t FireSensorConfig = 
{
    .GPIO_Port = GPIOB, // GPIO端口B
    .GPIO_Pin = GPIO_Pin_14, // GPIO引脚14
    .GPIO_Clk = RCC_APB2Periph_GPIOB, // GPIOB时钟
    .GPIO_Mode = GPIO_Mode_IPU, // GPIO模式: 上拉输入
    .GPIO_Speed = GPIO_Speed_50MHz, // GPIO速度: 50MHz

    .AFIO_Clk = RCC_APB2Periph_AFIO, // AFIO时钟
    .EXTI_Line = EXTI_Line14, // EXTI中断线: 14
    .EXTI_PortSource = GPIO_PortSourceGPIOB, // EXTI端口源: GPIOB
    .EXTI_PinSource = GPIO_PinSource14, // EXTI引脚源: 14
    .EXTI_Mode = EXTI_Mode_Interrupt, // EXTI模式: 中断模式
    .EXTI_Trigger = EXTI_Trigger_Falling, // EXTI触发方式: 下降沿触发

    .IRQn = EXTI15_10_IRQn, // NVIC中断通道: EXTI15_10
    .PreemptionPriority = 1, // 抢占优先级: 1
    .SubPriority = 1, // 响应优先级: 1
    .NVIC_Group = NVIC_PriorityGroup_2, // NVIC分组: 2组
};

/**
 * @brief 火焰传感器初始化
 * @param None
 * @retval None
 */
void Fire_Init(void)
{
    Sensor_Init(&FireSensorConfig);
}

/**
 * @brief 获取火焰传感器状态
 * @note 该函数用于获取当前火焰传感器的状态
 * @retval 传感器状态: 0 无触发, 1 有触发
 */
uint16_t Fire_GetState(void)
{
    return Sensor_GetState();
}

/**
 * @brief 清除火焰传感器状态
 * @note 该函数用于清除当前火焰传感器的状态
 * 在处理完传感器触发事件后调用
 * @retval None
 */
void Fire_ClearState(void)
{
    Sensor_ClearState();
}

/**
 * @brief 火焰传感器中断处理函数
 * @note 该函数在火焰传感器触发时调用
 * 处理传感器状态更新和中断清除
 * @retval None
 */
void EXTI15_10_IRQHandler(void)
{
    Sensor_IRQHandler(&FireSensorConfig);
}

/* 测试函数
#include "stm32f10x.h"
#include "Delay.h"
#include "Oled.h"
#include "Sensor.h"
#include "FireSensor.h"


int main(void)
{
    OLED_Init();  
    OLED_Clear();
    Fire_Init(); // 初始化火焰传感器

    while (1)
    {
        // 判断火焰传感器状态
        if (Fire_GetState() == 1)
        {
            OLED_Clear();
            OLED_ShowString(1, 1, "Fire Warning");

            // 状态显示一次后清零
            Fire_ClearState();
        }
        else
        {
            OLED_Clear();
            OLED_ShowString(1, 1, "StandBy");
        }
        Delay_ms(500);
    }
}

*/