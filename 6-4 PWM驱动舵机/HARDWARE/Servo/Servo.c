#include "stm32f10x.h"
#include "PWM.h"
#include "Servo.h"


void Servo_Init(void)
{
    PWM_Init(); // 初始化PWM
}

void Servo_SetAngle(float angle)
{
    if (angle > 180) // 限制角度在0~180度之间
    {
        angle = 180;
    }
    
    // 将角度转换为对应的CCR值
    // 0度对应500, 180度对应2500
    uint16_t ccr_value = 500 + (angle * (2500 - 500)) / 180; // 线性映射
    PWM_SetCompare2(ccr_value); // 设置TIM2_CH2的比较值
}
