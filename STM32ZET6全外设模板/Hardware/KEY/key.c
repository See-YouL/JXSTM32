/**
 * @file key.c
 * @brief 按键初始化和获取按键状态的函数实现
 * @note 该文件包含按键的初始化函数和获取按键状态的函数。(PA0, PC13)
 *       PA0用于按键1，PC13用于按键2。
 *      按键1按下时返回1，按键2按下时返回2，没有按键按下时返回0。
 * @version 1.0
 * @date 2025-08-15
 * @author 含生
 */
#include "stm32f10x.h"                  // Device header
#include "delay.h" // 延时模块
#include "key.h"   // 按键头文件

#define KEY_DEBOUNCE_MS   20  // 消抖时间

typedef enum {
    KEY_RELEASED = 0,
    KEY_PRESSED
} KeyState; // 按键状态

static KeyState key1State = KEY_RELEASED; // 按键1状态
static uint32_t key1Counter = 0; // 按键1计数器

static KeyState key2State = KEY_RELEASED; // 按键2状态
static uint32_t key2Counter = 0; // 按键2计数器


/**
 * @brief 按键初始化函数
 * @note 该函数配置KEY1和KEY2的GPIO引脚为输入模式，并使能相应的时钟。
 * @param None
 * @retval None
 */
void Key_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能时钟
    RCC_APB2PeriphClockCmd(KEY1_GPIO_CLK | KEY2_GPIO_CLK, ENABLE);

    // 根据 KEY_DOWN_LEVEL 自动选择输入模式
    #if (KEY_DOWN_LEVEL == Bit_SET)
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 按下高电平 → 上拉
    #else
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 按下低电平 → 下拉
    #endif

    // Key1
    GPIO_InitStructure.GPIO_Pin = KEY1_GPIO_PIN;
    GPIO_Init(KEY1_GPIO_PORT, &GPIO_InitStructure);

    // Key2
    GPIO_InitStructure.GPIO_Pin = KEY2_GPIO_PIN;
    GPIO_Init(KEY2_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief 扫描按键状态
 * @note 该函数检测按键1和按键2的状态，并返回相应的按键状态。
 *       如果按键1被按下，返回KEY1_PRESSED；如果按键2被按下，返回KEY2_PRESSED；
 *       如果没有按键被按下，返回KEY_NONE。
 *       按键状态使用消抖算法进行处理，避免按键抖动导致的误触发。
 *       按键1和按键2的状态分别使用独立的计数和状态变量进行管理。
 * @param None
 * @retval 返回按键状态：
 *        - KEY1_PRESSED: 按键1被按下
 *       - KEY2_PRESSED: 按键2被按下
 *       - KEY_NONE: 没有按键被按下
 */
uint8_t Key_Scan(void)
{
    uint8_t key1Level = GPIO_ReadInputDataBit(KEY1_GPIO_PORT, KEY1_GPIO_PIN);
    uint8_t key2Level = GPIO_ReadInputDataBit(KEY2_GPIO_PORT, KEY2_GPIO_PIN);

    // 检测Key1
    if (key1State == KEY_RELEASED) // 按键1未按下
    {
        if (key1Level == KEY_DOWN_LEVEL) // 检测到按键1按下
        {
            key1Counter++; // 计数器增加
            if (key1Counter >= KEY_DEBOUNCE_MS) // 如果计数器超过消抖时间
            {
                key1State = KEY_PRESSED; // 设置按键1状态为按下
                key1Counter = 0; // 重置计数器
                return KEY1_PRESSED; // 返回按键1按下
            }
        }
        else
        {
            key1Counter = 0; // 如果按键1仍然未按下，重置计数器
        }
    }
    else if (key1State == KEY_PRESSED) // 按键1已按下
    {
        if (key1Level != KEY_DOWN_LEVEL) // 检测到按键1松开
        {
            key1Counter++; // 计数器增加
            if (key1Counter >= KEY_DEBOUNCE_MS) // 如果计数器超过消抖时间
            {
                key1State = KEY_RELEASED; // 设置按键1状态为未按下
                key1Counter = 0; // 重置计数器
            }
        }
        else
        {
            key1Counter = 0; // 如果按键1仍然按下，重置计数器
        }
    }

    // 检测Key2
    if (key2State == KEY_RELEASED) // 按键2未按下
    {
        if (key2Level == KEY_DOWN_LEVEL) // 检测到按键2按下
        {
            key2Counter++; // 计数器增加
            if (key2Counter >= KEY_DEBOUNCE_MS) // 如果计数器超过消抖时间
            {
                key2State = KEY_PRESSED; // 设置按键2状态为按下
                key2Counter = 0; // 重置计数器
                return KEY2_PRESSED; // 返回按键2按下
            }
        }
        else
        {
            key2Counter = 0; // 如果按键2仍然未按下，重置计数器
        }
    }
    else if (key2State == KEY_PRESSED) // 按键2已按下
    {
        if (key2Level != KEY_DOWN_LEVEL) // 检测到按键2松开
        {
            key2Counter++; // 计数器增加
            if (key2Counter >= KEY_DEBOUNCE_MS) // 如果计数器超过消抖时间
            {
                key2State = KEY_RELEASED; // 设置按键2状态为未按下
                key2Counter = 0; // 重置计数器
            }
        }
        else
        {
            key2Counter = 0; // 如果按键2仍然按下，重置计数器
        }
    }

    return KEY_NONE;
}

/* main函数测试
#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"

int main(void)
{
    LED_Init(); // 初始化LED
    Key_Init(); // 初始化按键

    while (1)
    {
        uint8_t key = Key_Scan();

        if (key == KEY1_PRESSED)
        {
            LEDG_Turn(); // 切换绿色 LED 状态
        }
        else if (key == KEY2_PRESSED)
        {
            LEDR_Turn(); // 切换红色 LED 状态
        }
    }
}
*/
