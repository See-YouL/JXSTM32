#include "Key.h"

// 按键实例定义
Key_TypeDef KEY1 = 
{
    GPIOA, // 按键所属的 GPIO 端口
    GPIO_Pin_0, // 按键 GPIO 端口的引脚
    KEY_ACTIVE_HIGH, // 按键按下时为高电平
    0, // 当前按键状态，0=未按下 1=按下
    0, // 按键上次状态
    0, // 消抖计数器
    1 // 按键返回值
};

Key_TypeDef KEY2 = 
{
    GPIOC, // 按键所属的 GPIO 端口
    GPIO_Pin_13, // 按键 GPIO 端口的引脚
    KEY_ACTIVE_HIGH, // 按键按下时为高电平
    0, // 当前按键状态，0=未按下 1=按下
    0, // 按键上次状态
    0, // 消抖计数器
    2 // 按键返回值
};

/**
 * @brief 按键初始化
 * @note 该函数配置按键所使用的 GPIO 作为输入模式，并使能所需的 GPIO 端口时钟
 * @param key: 按键结构体指针
 * @retval None
 */
void Key_Init(Key_TypeDef* key)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 使能对应GPIO时钟
    if (key->GPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if (key->GPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if (key->GPIOx == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if (key->GPIOx == GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if (key->GPIOx == GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

    // 配置输入模式
    if (key->ActiveLevel == KEY_ACTIVE_HIGH) // 按键按下时为高电平
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 上拉输入
    else // 按键按下时为低电平
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; // 下拉输入

    GPIO_InitStructure.GPIO_Pin = key->GPIO_Pin; // 按键 GPIO 端口的引脚
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 配置输出速度50MHz不影响输入模式

    GPIO_Init(key->GPIOx, &GPIO_InitStructure);
}


/**
 * @brief 单个键扫描
 * @note 该函数检测单个按键的状态，并返回按键值
 * @param key: 按键结构体指针
 * @retval 按键值
 */
uint8_t Key_Scan(Key_TypeDef* key)
{
    uint8_t level = GPIO_ReadInputDataBit(key->GPIOx, key->GPIO_Pin); // 读取按键引脚的电平状态

    // 按下检测
    if (level == key->ActiveLevel) // 按键按下
    {
        if (key->LastState != 1) // 按键按下，并且上次状态为未按下
        {
            key->Counter++; // 消抖计数器自增
            if (key->Counter >= KEY_DEBOUNCE_MS) // 消抖时间达到消抖时长
            {
                key->State = 1; // 按键当前状态设置为下
                key->LastState = 1; // 按键上次状态设置按下
                key->Counter = 0; // 消抖计数器清零
                return key->KeyValue; // 返回按键值
            }
        }
        else // 按键按下，并且上次状态为按下
        {
            key->Counter = 0; // 消抖计数器清零
        }
    }
    else // 松开检测
    {
        if (key->LastState != 0) // 按键松开，并且上次状态为按下
        {
            key->Counter++; // 消抖计数器自增
            if (key->Counter >= KEY_DEBOUNCE_MS) // 消抖时间达到消抖时长
            {
                key->State = 0; // 按键当前状态设置为按下
                key->LastState = 0; // 按键上次状态设置为未按下
                key->Counter = 0; // 消抖计数器清零
            }
        }
        else // 按键未按下，并且上次状态为未按下
        {
            key->Counter = 0; // 消抖计数器清零
        }
    }

    return KEY_NONE;
}

/**
 * @brief 所有键扫描
 * @note 该函数检测所有按键的状态，并返回第一个键值
 * @param keys: 按键结构体指针数组
 * @param keyCount: 按键数组的元素个数
 * @retval 第一个按键的值
 */
uint8_t Key_ScanAll(Key_TypeDef* keys[], uint8_t keyCount)
{
    for (uint8_t i = 0; i < keyCount; i++) // 扫描所有按键
    {
        uint8_t val = Key_Scan(keys[i]); // 读取按键值
        if (val != KEY_NONE) // 按键按下
            return val; // 返回第一个按下的按键值
    }
    return KEY_NONE;
}
