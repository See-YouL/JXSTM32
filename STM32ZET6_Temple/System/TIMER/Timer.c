/**
 * @file Timer.c
 * @brief 定时器模块实现文件
 * @version 1.0
 * @date 2025-08-16
 * @author 含生
 */
#include "stm32f10x.h"
#include "Timer.h"

/**
 * @brief 定时器初始化函数
 * @note 该函数用于初始化定时器，包括时钟配置、时基单元初始化、NVIC配置等。
 *       需要在具体的 main 函数中调用，例如 Timer_Init(&Timer2Config);
 *       定时器中断回调函数需要在 Timer_Config_t 结构体中设置。
 *       该函数会使能定时器的更新中断，并在中断发生时调用用户定义的回调函数。
 *       注意：在使用定时器之前，需要先配置好 NVIC 的优先级分组。
 * @param timer: 指向Timer_Config_t结构体，包含定时器配置参数
 * @return None
 */
void Timer_Init(Timer_Config_t *timer)
{
    // 1. 开启定时器时钟
    RCC_APB1PeriphClockCmd(timer->TIMx_CLK, ENABLE);

    // 2. 配置内部时钟
    TIM_InternalClockConfig(timer->TIMx);

    // 3. 时基单元初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = timer->ClockDivision; // 时钟分割
    TIM_TimeBaseInitStructure.TIM_CounterMode = timer->CounterMode; // 计数器模式
    TIM_TimeBaseInitStructure.TIM_Prescaler = timer->Prescaler; // 预分频
    TIM_TimeBaseInitStructure.TIM_Period = timer->Period; // 自动重装载值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = timer->RepetitionCounter; // 重复计数器（高级定时器使用）
    TIM_TimeBaseInit(timer->TIMx, &TIM_TimeBaseInitStructure);

    // 4. 清除标志位并使能更新中断
    TIM_ClearFlag(timer->TIMx, TIM_FLAG_Update); // 清除更新标志位
    TIM_ITConfig(timer->TIMx, TIM_IT_Update, ENABLE); // 使能更新中断

    // 5. NVIC分组配置
    NVIC_PriorityGroupConfig(timer->NVIC_PriorityGroup); // 设置NVIC分组

    // 6. NVIC初始化
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = timer->TIMx_IRQn; // 中断通道
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = timer->PreemptionPriority; // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = timer->SubPriority; // 响应优先级
    NVIC_Init(&NVIC_InitStructure);

    // 7. 启动定时器
    TIM_Cmd(timer->TIMx, ENABLE);
}

/**
 * @brief 定时器中断处理函数
 * @note 该函数用于处理定时器中断，当定时器发生更新中断时调用。
 *       在具体的中断处理函数中，需要调用此函数来处理定时器中断。
 *       该函数会检查定时器的更新中断标志位，如果标志位被设置，则清除标志位并调用用户定义的回调函数。
 *       用户需要在 Timer_Config_t 结构体中设置回调函数指针。
 * @param timer: 指向Timer_Config_t结构体，包含定时器配置参数
 * @return None
 */
void Timer_IRQHandler(Timer_Config_t *timer)
{
    if (TIM_GetITStatus(timer->TIMx, TIM_IT_Update) != RESET) // 检查更新中断标志位
    {
        TIM_ClearITPendingBit(timer->TIMx, TIM_IT_Update); // 清除中断标志位

        if (timer->Callback) // 检查回调函数指针是否有效
        {
            timer->Callback(); // 调用用户回调函数
        }
    }
}

// 测试程序
// #include "stm32f10x.h"
// #include "Delay.h"
// #include "Oled.h"
// #include "Timer.h"

// uint16_t Num = 0; // 定义一个变量用来计数

// void MyTimer2Task(void); // 定时器2中断回调函数声明

// Timer_Config_t Timer2Config = { // 定时器2配置结构体
//     .TIMx = TIM2, // 定时器2
//     .TIMx_CLK = RCC_APB1Periph_TIM2, // 定时器2时钟
//     .TIMx_IRQn = TIM2_IRQn, // 定时器2中断号
//     .Prescaler = 7200 - 1,           // 10KHz
//     .Period = 10000 - 1,             // 1秒周期
//     .RepetitionCounter = 0,          // 重复计数器（TIM2无效，可置0）
//     .ClockDivision = TIM_CKD_DIV1,   // 时钟分频
//     .CounterMode = TIM_CounterMode_Up, // 向上计数
//     .NVIC_PriorityGroup = NVIC_PriorityGroup_2, // NVIC优先级分组
//     .PreemptionPriority = 2, // 抢占优先级2
//     .SubPriority = 1, // 响应优先级1
//     .Callback = MyTimer2Task // 定时器中断回调函数指针，用户中断处理逻辑
// };

// int main(void)
// {
//     OLED_Init();  
//     Timer_Init(&Timer2Config); // 初始化定时器

//     OLED_Clear();
//     OLED_ShowString(1, 1, "Num = "); // 显示"Num ="

//     while (1)
//     {
//         OLED_ShowNum(1, 5, Num, 5); // 显示Num的值
//         Delay_ms(100); // 延时100毫秒
//     }
// }

// /**
//  * @brief 定时器2中断处理函数
//  * 在定时器2中断发生时调用
//  * @note 调用通用中断处理函数
//  * @param None
//  * @retval None
//  */
// void TIM2_IRQHandler(void)
// {
//     Timer_IRQHandler(&Timer2Config); // 调用通用中断处理函数
// }

// /**
//  * @brief 定时器2中断回调函数
//  * 在定时器2中断发生时调用
//  * @note 执行Num++操作
//  * @param None
//  * @retval None
//  */
// void MyTimer2Task(void)
// {
//     // 在这里处理定时器中断事件，比如翻转LED
//     Num++;
// }
