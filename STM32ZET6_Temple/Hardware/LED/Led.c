/**
 * @file Led.c
 * @brief LED控制函数实现
 * @note 本文件实现了LED的初始化和控制函数。
 *       包括绿色、蓝色和红色LED的开关和切换功能。
 * @version 1.0
 * @date 2025-08-15
 * @author 含生
 */
#include "led.h"

/**
 * @brief 初始化LED引脚
 * @note 该函数配置LEDG,LEDB,LEDR引脚为推挽输出模式。
 *       并将所有LED默认设置为关闭状态（高电平）。
 * @param None
 * @return None
 */
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能GPIO时钟
    RCC_APB2PeriphClockCmd(LED_GPIO_CLK, ENABLE);

    // 初始化LED引脚为推挽输出
    GPIO_InitStructure.GPIO_Pin = LEDG_PIN | LEDB_PIN | LEDR_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

    // 默认关闭所有LED
    GPIO_SetBits(LED_GPIO_PORT, LEDG_PIN | LEDB_PIN | LEDR_PIN);
}

// 绿色LED相关函数
void LEDG_ON(void)  { LED_ON(LED_GPIO_PORT, LEDG_PIN); }
void LEDG_OFF(void) { LED_OFF(LED_GPIO_PORT, LEDG_PIN); }
void LEDG_Turn(void) { LED_TOGGLE(LED_GPIO_PORT, LEDG_PIN); }

// 蓝色LED相关函数
void LEDB_ON(void)  { LED_ON(LED_GPIO_PORT, LEDB_PIN); }
void LEDB_OFF(void) { LED_OFF(LED_GPIO_PORT, LEDB_PIN); }
void LEDB_Turn(void) { LED_TOGGLE(LED_GPIO_PORT, LEDB_PIN); }

// 红色LED相关函数
void LEDR_ON(void)  { LED_ON(LED_GPIO_PORT, LEDR_PIN); }
void LEDR_OFF(void) { LED_OFF(LED_GPIO_PORT, LEDR_PIN); }
void LEDR_Turn(void) { LED_TOGGLE(LED_GPIO_PORT, LEDR_PIN); }

/*
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"

int main(void)
{
    LED_Init(); // 初始化LED

    while (1)
    {
        LEDR_ON();   // 打开红色LED
        Delay_ms(500); // 延时500毫秒
        LEDR_OFF();  // 关闭红色LED
        Delay_ms(500); // 延时500毫秒
        LEDB_ON();   // 打开蓝色LED
        Delay_ms(500); // 延时500毫秒
        LEDB_OFF();  // 关闭蓝色LED
        Delay_ms(500); // 延时500毫秒
        LEDG_ON();   // 打开绿色LED
        Delay_ms(500); // 延时500毫秒
        LEDG_OFF();  // 关闭绿色LED
        Delay_ms(500); // 延时500毫秒
    }
}
*/
