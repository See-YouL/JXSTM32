/**
 * @file Servo.c
 * @brief 舵机驱动实现文件（基于PWM配置）
 * @version 1.0
 * @date 2025-08-17
 */

#include "Servo.h"

// 舵机对应的PWM配置结构体
static PWM_Config_t servo_pwm_config = {
    .TIMx = SERVO_TIM,
    .TIMx_CLK = SERVO_TIM_CLK,
    .Prescaler = SERVO_PRESCALER,
    .Period = SERVO_PERIOD,
    .Pulse = SERVO_MIN_PULSE,  // 初始占空比：0度
    .Channel = SERVO_CHANNEL,
    .GPIOx = SERVO_GPIO_PORT,
    .GPIO_Pin = SERVO_GPIO_PIN,
    .ClockDivision = TIM_CKD_DIV1,
    .CounterMode = TIM_CounterMode_Up,

    .OCMode = TIM_OCMode_PWM1,
    .OCPolarity = TIM_OCPolarity_High,
    .OutputState = TIM_OutputState_Enable
};

/**
 * @brief 舵机初始化函数
 * @param None
 * @return None
 */
void Servo_Init(void)
{
    PWM_Init(&servo_pwm_config); // 使用配置初始化PWM
}

/**
 * @brief 舵机设置角度函数
 * @param angle 舵机机需要转到的角度(0-180度)
 * @retval None
 */
void Servo_SetAngle(float angle)
{
    if (angle > 180) angle = 180;
    if (angle < 0)   angle = 0;

    // 线性映射：角度 -> CCR值
    uint16_t ccr_value = SERVO_MIN_PULSE + 
                         (uint16_t)(angle * (SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180.0f);

    PWM_SetCompare(&servo_pwm_config, ccr_value);
}

// 舵机驱动测试函数
// #include "stm32f10x.h"
// #include "Delay.h"
// #include "Key.h"
// #include "Oled.h"
// #include "Servo.h"

// Key_TypeDef* KeyArray[] = { &KEY1, &KEY2 }; // 按键数组
// #define KEY_COUNT  (sizeof(KeyArray)/sizeof(KeyArray[0])) // 按键数组中的按键数量
// uint8_t key = 0; // 按键值
// float Angle = 0; // 舵机角度变量

// int main(void)
// {
//     OLED_Init(); // OLED 初始化
//     Servo_Init(); // 初始化舵机驱动

//     OLED_Clear(); // OLED 清屏
//     OLED_ShowString(1, 1, "Angle:"); // 在(1, 1) 位置显示"Angle:"

    
//     for(uint8_t i = 0; i < KEY_COUNT; i++)
//     {
//         Key_Init(KeyArray[i]); // 初始化每个按键
//     }

//     while (1)
//     {
//         // 扫描所有按键
//         key = Key_ScanAll(KeyArray, KEY_COUNT);

//         switch(key)
//         {
//             case 1: // KEY1
//             {
//                 Angle += 10; // 按键1按下，舵机角度增加10度
//                 if (Angle > 180) // 舵机角度超出 180 度
//                 {
//                     Angle = 0; // 舵机角度归零
//                 }
//                 Servo_SetAngle(Angle); // 设置舵机角度
//                 OLED_ShowNum(1, 7, Angle, 3); // 在(1, 5) 位置显示舵机角度
//                 break;
//             }

//             case 2: // KEY2
//             {
//                 Angle -= 10; // 按键1按下，舵机角度增加10度
//                 if (Angle < 0) // 舵机角度超出 180 度
//                 {
//                     Angle = 180; // 舵机角度归零
//                 }
//                 Servo_SetAngle(Angle); // 设置舵机角度
//                 OLED_ShowNum(1, 7, Angle, 3); // 在(1, 5) 位置显示舵机角度
//                 break;
//             }

//             default:
//                 break;
//         }
//     }
// }
