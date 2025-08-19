#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f10x.h"
#include "PWM.h"

// 电机方向定义
#define MOTOR_DIR_FORWARD    1
#define MOTOR_DIR_BACKWARD   0

// 电机结构体
typedef struct
{
    GPIO_TypeDef* DirPortA;      // 方向控制A端口
    uint16_t DirPinA;            // 方向控制A引脚
    GPIO_TypeDef* DirPortB;      // 方向控制B端口
    uint16_t DirPinB;            // 方向控制B引脚
    PWM_Config_t* PWMConfig;     // 电机对应PWM配置结构体指针
} Motor_TypeDef;

// API
void Motor_Init(Motor_TypeDef* motor); // 电机初始化函数
void Motor_SetSpeed(Motor_TypeDef* motor, int16_t speed); // 设置电机速度函数

#endif
