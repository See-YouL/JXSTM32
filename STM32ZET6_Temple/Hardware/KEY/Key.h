#ifndef __KEY_H
#define __KEY_H

#include "stm32f10x.h"
#include <stdint.h>

// 按键触发宏定义
#define KEY_ACTIVE_HIGH   Bit_SET   // 按下为高电平
#define KEY_ACTIVE_LOW    Bit_RESET // 按下为低电平

// 按键返回值定义
#define KEY_NONE          0

// 按键软件消抖时间(ms)
#define KEY_DEBOUNCE_MS   20

// 按键结构体
typedef struct
{
    GPIO_TypeDef* GPIOx;    // GPIO端口
    uint16_t GPIO_Pin;      // GPIO引脚
    uint8_t ActiveLevel;    // 按下电平（KEY_ACTIVE_HIGH / KEY_ACTIVE_LOW）
    uint8_t State;          // 当前按键状态 0=未按下 1=按下
    uint8_t LastState;      // 上次状态
    uint16_t Counter;       // 消抖计数
    uint8_t KeyValue;       // 按键返回值
} Key_TypeDef;

// 按键声明
// 示例按键定义，移植时可添加更多按键
extern Key_TypeDef KEY1;
extern Key_TypeDef KEY2;

// API
void Key_Init(Key_TypeDef* key);          // 初始化按键
uint8_t Key_Scan(Key_TypeDef* key);       // 扫描单个按键状态
uint8_t Key_ScanAll(Key_TypeDef* keys[], uint8_t keyCount); // 扫描多个按键状态

#endif
