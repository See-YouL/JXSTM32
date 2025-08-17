/**
 * @file Servo.h
 * @brief 舵机驱动头文件（可移植配置）
 * @version 1.0
 * @date 2025-08-17
 */
#ifndef __SERVO_H
#define __SERVO_H

#include "stm32f10x.h"
#include "Pwm.h"

// 定义舵机使用的PWM配置
#define SERVO_TIM                TIM2
#define SERVO_TIM_CLK            RCC_APB1Periph_TIM2
#define SERVO_CHANNEL            2               // 使用TIMx_CH2
#define SERVO_GPIO_PORT          GPIOA
#define SERVO_GPIO_PIN           GPIO_Pin_1      // TIM2_CH2 默认映射PA1

#define SERVO_PRESCALER          72              // 分频后 1MHz 计数频率
#define SERVO_PERIOD             20000           // ARR = 20000 -> 20ms周期 (50Hz)

#define SERVO_MIN_PULSE          500             // 0度对应的CCR值
#define SERVO_MAX_PULSE          2500            // 180度对应的CCR值

// ===================================================

void Servo_Init(void);
void Servo_SetAngle(float angle);

#endif
