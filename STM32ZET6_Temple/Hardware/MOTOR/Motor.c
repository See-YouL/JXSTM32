/**
 * @file   Motor.c
 * @brief  Motor 驱动函数
 * @note 该函数包含了电机驱动的初始化函数
 *      包括方向控制GPIO和PWM的初始化
 *     并根据所给的 PWM参数来初始化PWM
 * @version 1.0
 * @date 2025-08-17
 * @author 含生
 */
#include "Motor.h"
#include <stddef.h>   // NULL定义

/**
 * @brief 自动使能GPIO时钟
 * @note  该函数根据所给的GPIO端口来使能GPIO时钟
 *        使能GPIOA~GPIOE的时钟
 * @param GPIOx 指向GPIO的结构体
 * @retval None
 */
static void RCC_EnableGPIO(GPIO_TypeDef* GPIOx)
{
    if(GPIOx == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    else if(GPIOx == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    else if(GPIOx == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    else if(GPIOx == GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    else if(GPIOx == GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
}

/**
 * @brief 电机驱动初始化
 * @note 该函数根据所给的电机参数来初始化电机驱动
 *       包括方向控制GPIO和PWM的初始化
 *      并根据所给的 PWM参数来初始化PWM
 * @param motor 指向电机的结构体
 * @retval None
 */
void Motor_Init(Motor_TypeDef* motor)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 初始化方向控制GPIO A 并置为低电平
    if(motor->DirPortA != NULL) // 如果有方向控制GPIO A相
    {
        RCC_EnableGPIO(motor->DirPortA); // 使能方向控制GPIO A相时钟
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
        GPIO_InitStructure.GPIO_Pin = motor->DirPinA; // 选择GPIOA相引脚
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 速度50MHz
        GPIO_Init(motor->DirPortA, &GPIO_InitStructure);
        GPIO_ResetBits(motor->DirPortA, motor->DirPinA); // 置为低电平
    }

    // 初始化方向控制GPIO B 并置为低电平
    if(motor->DirPortB != NULL) // 如果有方向控制GPIO B相
    {
        RCC_EnableGPIO(motor->DirPortB); // 使能方向控制GPIO B相时钟
        GPIO_InitStructure.GPIO_Pin = motor->DirPinB; // 选择GPIOB相引脚
        GPIO_Init(motor->DirPortB, &GPIO_InitStructure);
        GPIO_ResetBits(motor->DirPortB, motor->DirPinB); // 置为低电平
    }

    // 初始化PWM
    if(motor->PWMConfig != NULL) // 如果有 PWM参数
    {
        PWM_Init(motor->PWMConfig); // 使能PWM
    }
}

/**
 * @brief 电机转速设置
 * @note  该函数根据所给的电机参数和转速来设置电机转速
 *     并根据所给的 PWM参数来设置 PWM占空比
 * @param motor 指向电机的结构体
 * @param speed 转速
 * @retval None
 */
void Motor_SetSpeed(Motor_TypeDef* motor, int16_t speed)
{
    if(motor->PWMConfig == NULL) return; // 如果PWM未配置直接返回

    if(speed >= 0) // 如果转速为正
    {
        if(motor->DirPortA) GPIO_SetBits(motor->DirPortA, motor->DirPinA); // A相打开
        if(motor->DirPortB) GPIO_ResetBits(motor->DirPortB, motor->DirPinB); // B相关闭
        PWM_SetCompare(motor->PWMConfig, speed); // 设置转速
    }
    else // 如果转速为负
    {
        if(motor->DirPortA) GPIO_ResetBits(motor->DirPortA, motor->DirPinA); // A相关闭
        if(motor->DirPortB) GPIO_SetBits(motor->DirPortB, motor->DirPinB); // B相打开
        PWM_SetCompare(motor->PWMConfig, -speed); // 设置转速
    }
}

// /*
// 硬件连接：(电机驱动130马达+TB6612FNG电机驱动板)
// VM -> 5V
// VCC -> 3.3V
// GND -> GND(需要与STM32的GND共地)
// AO1 -> 电机端口1
// AO2 -> 电机端口2(不分正反)
// PWMA -> TIM2_CH3引脚(PA2)
// AIN2 -> PA5
// AIN1 -> PA4
// STBY -> 3.3V
// OLED_SCK -> PB8
// OLED_SDA -> PB9
// Key1 -> PA0
// Key2 -> PC13
//  */
// #include "stm32f10x.h"
// #include "Delay.h"
// #include "Key.h"
// #include "Oled.h"
// #include "Motor.h"

// uint8_t KeyNum = 0;   // 按键编号变量
// int16_t Speed = 0;    // 电机速度变量，扩展为int16_t以支持负数
// Key_TypeDef* KeyArray[] = { &KEY1, &KEY2 }; // 按键数组
// #define KEY_COUNT  (sizeof(KeyArray)/sizeof(KeyArray[0])) // 按键数组中的按键数量

// // 电机PWM配置
// PWM_Config_t MotorPWM = {
//     .TIMx = TIM2,
//     .TIMx_CLK = RCC_APB1Periph_TIM2,
//     .Prescaler = 720-1,          // 假设72MHz主频
//     .Period = 100-1,           // PWM周期
//     .Pulse = 50,                  // 初始占空比
//     .Channel = 3,                // TIM2 CH3
//     .GPIOx = GPIOA,
//     .GPIO_Pin = GPIO_Pin_2,      // 对应TIM2_CH3引脚
//     .ClockDivision = TIM_CKD_DIV1, // 时钟分频系数, 不分频
//     .CounterMode = TIM_CounterMode_Up, // 向上计数模式
//     .OCMode = TIM_OCMode_PWM1, // PWM1模式
//     .OCPolarity = TIM_OCPolarity_High, // 极性为高
//     .OutputState = TIM_OutputState_Enable // 使能输出比较
// };

// // 电机结构体
// Motor_TypeDef Motor = {
//     .DirPortA = GPIOA, // A相输出端口
//     .DirPinA = GPIO_Pin_4, // A相输出引脚
//     .DirPortB = GPIOA, // B相输出端口
//     .DirPinB = GPIO_Pin_5, // B相输出引脚
//     .PWMConfig = &MotorPWM // 电机对应PWM配置结构体指针
// };

// int main(void)
// {   
//     OLED_Init(); // OLED初始化
//     for (uint8_t i = 0; i < KEY_COUNT; i++)
//     {
//         Key_Init(KeyArray[i]); // 初始化每个按键
//     }
//     Motor_Init(&Motor); // 初始化电机

//     OLED_Clear();  
//     OLED_ShowString(1, 1, "Speed:"); // 显示静态文字

//     Speed = 0;
//     Motor_SetSpeed(&Motor, Speed); // 设置电机初始速度为0
//     OLED_ShowChar(1, 7, (Speed < 0) ? '-' : ' ');   // 显示符号
//     OLED_ShowNum(1, 8, (Speed < 0) ? -Speed : Speed, 3); // 显示绝对值

//     while (1)
//     {
//         KeyNum = Key_ScanAll(KeyArray, KEY_COUNT); // 扫描所有按键

//         switch(KeyNum) // 按键按下情况
//         {
//             case 1: // Key1按下
//             {
//                 Speed += 20;       // 增加速度
//                 if (Speed > 100)   // 限制速度范围
//                 {
//                     Speed = 0;  // 超过上限则清零
//                 }
//                 break;
//             }
//             case 2: // Key2按下
//             {
//                 Speed -= 20;       // 减少速度
//                 if (Speed < -100)   // 限制速度范围
//                 {
//                     Speed = 0;  // 超过下限则清零
//                 }
//                 break;
//             }
//         }
//         Motor_SetSpeed(&Motor, Speed); // 设置电机速度
//         OLED_ShowChar(1, 7, (Speed < 0) ? '-' : ' ');   // 显示符号
//         OLED_ShowNum(1, 8, (Speed < 0) ? -Speed : Speed, 3); // 显示绝对值
//     }
// }
