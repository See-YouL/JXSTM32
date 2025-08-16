/**
 * @file Sensor.c
 * @brief 通用传感器驱动代码
 * 该文件实现了传感器的初始化、状态获取和中断处理功能
 * @version 1.0
 * @date 2025-08-16
 * @author 含生
 */
#include "Sensor.h"
#include "stm32f10x.h"

static volatile uint16_t SensorState = 0;   // 状态标志: 0 无触发, 1 有触发
static Sensor_Config_t SensorConfig;        // 保存当前传感器配置

/**
 * @brief 初始化传感器
 * @note 该函数用于初始化传感器的 GPIO、EXTI 和 NVIC 配置
 * @param config 传感器配置结构体指针
 * @retval None
 */
void Sensor_Init(Sensor_Config_t* config)
{
    // 保存配置
    SensorConfig = *config;

    // 开启时钟
    RCC_APB2PeriphClockCmd(SensorConfig.GPIO_Clk, ENABLE);
    RCC_APB2PeriphClockCmd(SensorConfig.AFIO_Clk, ENABLE);
    
    // GPIO 初始化
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = SensorConfig.GPIO_Mode; // GPIO 模式
    GPIO_InitStructure.GPIO_Pin = SensorConfig.GPIO_Pin; // GPIO 引脚
    GPIO_InitStructure.GPIO_Speed = SensorConfig.GPIO_Speed; // GPIO 速度
    GPIO_Init(SensorConfig.GPIO_Port, &GPIO_InitStructure);

    // EXTI 端口配置
    GPIO_EXTILineConfig(SensorConfig.EXTI_PortSource, SensorConfig.EXTI_PinSource); // 配置 EXTI 端口源和引脚源

    // EXTI 初始化
    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = SensorConfig.EXTI_Line; // 配置 EXTI 中断线
    EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 使能 EXTI 中断线
    EXTI_InitStructure.EXTI_Mode = SensorConfig.EXTI_Mode; // 配置 EXTI 模式
    EXTI_InitStructure.EXTI_Trigger = SensorConfig.EXTI_Trigger; // 配置 EXTI 触发方式
    EXTI_Init(&EXTI_InitStructure);

    // NVIC 配置
    NVIC_PriorityGroupConfig(SensorConfig.NVIC_Group); // 设置 NVIC 分组
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = SensorConfig.IRQn; // 配置 NVIC 中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能 NVIC 中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = SensorConfig.PreemptionPriority; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SensorConfig.SubPriority; // 响应优先级
    NVIC_Init(&NVIC_InitStructure);
}

/**
 * @brief 获取传感器状态
 * @note 该函数用于获取当前传感器的状态
 * @retval 传感器状态: 0 无触发, 1 有触发
 */
uint16_t Sensor_GetState(void)
{
    return SensorState; // 返回当前传感器状态
}

/**
 * @brief 清除传感器状态
 * @note 该函数用于清除传感器状态标志
 * @retval None
 */
void Sensor_ClearState(void)
{
    SensorState = 0; // 清除传感器标志
}

/**
 * @brief 传感器中断处理函数
 * @note 该函数用于处理传感器的 EXTI 中断
 * @param config 传感器配置结构体指针
 * @retval None
 */
void Sensor_IRQHandler(Sensor_Config_t* config)
{
    if (EXTI_GetITStatus(config->EXTI_Line) == SET) // 检查 EXTI 中断挂起位是否挂起
    {
        SensorState = 1; // 设置状态标志为有触发
        EXTI_ClearITPendingBit(config->EXTI_Line); // 清除 EXTI 中断挂起位
    }
}
