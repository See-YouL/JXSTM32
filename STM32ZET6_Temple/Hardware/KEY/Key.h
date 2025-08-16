#ifndef __KEY_H
#define __KEY_H

// 按键宏定义
#define KEY1_GPIO_PORT     GPIOA
#define KEY1_GPIO_PIN      GPIO_Pin_0
#define KEY1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define KEY2_GPIO_PORT     GPIOC
#define KEY2_GPIO_PIN      GPIO_Pin_13
#define KEY2_GPIO_CLK      RCC_APB2Periph_GPIOC

// 按键返回值定义
#define KEY_NONE           0
#define KEY1_PRESSED       1
#define KEY2_PRESSED       2

// 按键触发电平定义
// 如果按下是高电平，设为 Bit_SET
// 如果按下是低电平，设为 Bit_RESET
// Bit_SET 和 Bit_RESET 是 STM32 标准库中定义的宏
#define KEY_DOWN_LEVEL     Bit_SET

// 函数声明
void Key_Init(void); // 按键初始化函数
uint8_t Key_GetNum(void); // 获取按键状态函数

#endif
