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
 * @note 该函数用于初始化定时器,包括时钟、配置外部时钟或内部时钟, 并配置中断
 * @param timer 指向定时器的结构体
 * @retval None
 */
void Timer_Init(Timer_Config_t *timer)
{
    // 1. 开启定时器时钟
    if (timer->TIMx == TIM1) 
    {
        RCC_APB2PeriphClockCmd(timer->TIMx_CLK, ENABLE); // 开启TIM1时钟
    }
    else 
    {
        RCC_APB1PeriphClockCmd(timer->TIMx_CLK, ENABLE); // 开启其他TIMx时钟
    }

    // 2. 配置外部时钟或内部时钟
    if (timer->ExternalClockMode == 1 || timer->ExternalClockMode == 2)
    {
        if (timer->ETR_GPIO_Port)
        {
            // 自动开启 GPIO 时钟
            if (timer->ETR_GPIO_Port == GPIOA) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
            else if (timer->ETR_GPIO_Port == GPIOB) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
            else if (timer->ETR_GPIO_Port == GPIOC) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
            else if (timer->ETR_GPIO_Port == GPIOD) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
            else if (timer->ETR_GPIO_Port == GPIOE) RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);

            GPIO_InitTypeDef GPIO_InitStructure;
            GPIO_InitStructure.GPIO_Pin = timer->ETR_GPIO_Pin; // 选择 ETR 引脚
            GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 浮空输入
            GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 50MHz
            GPIO_Init(timer->ETR_GPIO_Port, &GPIO_InitStructure);
        }

        if (timer->ExternalClockMode == 1) // 外部时钟模式1
        {
            TIM_ETRClockMode1Config(timer->TIMx, // 选择 ETR 作为时钟
                                    timer->ExternalClockPrescaler, // 选择 ETR 前分频
                                    timer->ExternalClockPolarity, // 选择 ETR 极性
                                    timer->ExternalClockFilter); // 选择 ETR 滤波器
        }
        else // 外部时钟模式2
        {
            TIM_ETRClockMode2Config(timer->TIMx, // 选择 ETR 作为时钟
                                    timer->ExternalClockPrescaler, // 选择 ETR 前分频
                                    timer->ExternalClockPolarity, // 选择 ETR 极性
                                    timer->ExternalClockFilter); // 选择 ETR 滤波器
        }
    }
    else
    {
        TIM_InternalClockConfig(timer->TIMx); // 内部时钟模式
    }


    // 3. 时基单元初始化
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_ClockDivision = timer->ClockDivision;
    TIM_TimeBaseInitStructure.TIM_CounterMode = timer->CounterMode;
    TIM_TimeBaseInitStructure.TIM_Prescaler = timer->Prescaler;
    TIM_TimeBaseInitStructure.TIM_Period = timer->Period;
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = timer->RepetitionCounter;
    TIM_TimeBaseInit(timer->TIMx, &TIM_TimeBaseInitStructure);

    // 4. 清除标志位
    TIM_ClearFlag(timer->TIMx, TIM_FLAG_Update);

    // 5. 配置中断
    if (timer->EnableInterrupt && timer->Callback) // 如果使能中断并且有中断回调函数
    {
        TIM_ITConfig(timer->TIMx, TIM_IT_Update, ENABLE); // 使能TIMx的更新中断

        NVIC_PriorityGroupConfig(timer->NVIC_PriorityGroup); // 配置NVIC分组
        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel = timer->TIMx_IRQn; // 定时器2中断通道
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; // 使能NVIC通道
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = timer->PreemptionPriority; // 抢占优先级
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = timer->SubPriority; // 响应优先级
        NVIC_Init(&NVIC_InitStructure);
    }

    // 6. 启动定时器
    TIM_Cmd(timer->TIMx, ENABLE);
}


/**
 * @brief 获取定时器计数器的值
 * @param timer 指向定时器的结构体
 * @retval 计数器的值
 */
uint16_t Timer_GetCounter(Timer_Config_t *timer)
{
    return TIM_GetCounter(timer->TIMx);
}

/**
 * @brief 定时器2中断回调函数
 * @param timer 指向定时器的结构体
 * @retval None
 */
void Timer_IRQHandler(Timer_Config_t *timer)
{
    if (TIM_GetITStatus(timer->TIMx, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(timer->TIMx, TIM_IT_Update);
        if (timer->Callback) timer->Callback();
    }
}


// /*
// 硬件连线:(Tim内部时钟测试)
// OLED_SCK -> PB8
// OLED_SDA -> PB9
// */
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
//     .Callback = MyTimer2Task, // 定时器中断回调函数指针，用户中断处理逻辑
//     .ExternalClockMode = 0,       // 1=外部时钟，0=内部时钟
//     .EnableInterrupt = 1, // 使能中断
//     };

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


// /*
// 硬件连接:(Tim外部时钟测试, 不敏感)
// PA0 -> 传感器
// OLED_SCK -> PB8
// OLED_SDA -> PB9
// */
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

//     .Prescaler = 1 - 1, // 1分频
//     .Period = 9 - 1, // 9次触发一次更新中断
//     .RepetitionCounter = 0, // 重复计数器（TIM2无效，可置0）
//     .ClockDivision = TIM_CKD_DIV1, // 时钟分频
//     .CounterMode = TIM_CounterMode_Up, // 向上计数

//     .NVIC_PriorityGroup = NVIC_PriorityGroup_2, // NVIC优先级分组
//     .PreemptionPriority = 2, // 抢占优先级2
//     .SubPriority = 1, // 响应优先级1

//     .Callback = MyTimer2Task, // 定时器中断回调函数指针，用户中断处理逻辑

//     .ExternalClockMode = TIMER_EXTCLK2, // 使用外部时钟模式1
//     .EnableInterrupt = 1, // 使能中断

//     .ExternalClockPrescaler = TIM_ExtTRGPSC_OFF, // 外部时钟不分频
//     .ExternalClockFilter = 0x0,     // 最小滤波，提高响应速度
//     //.ExternalClockPolarity = TIM_ExtTRGPolarity_NonInverted, // 下降沿计数
//     .ExternalClockPolarity = TIM_ExtTRGPolarity_Inverted, // 下降沿计数

//     .ETR_GPIO_Port = GPIOA,
//     .ETR_GPIO_Pin = GPIO_Pin_0, // 红外传感器接PA0
// };

// int main(void)
// {
//     OLED_Init();  
//     Timer_Init(&Timer2Config); // 初始化定时器

//     OLED_Clear();
//     OLED_ShowString(1, 1, "Num = "); // 显示"Num ="
//     OLED_ShowString(10, 1, "CNT = ");

//     while (1)
//     {
//         OLED_ShowNum(1, 5, Num, 5); // 显示Num的值
//         OLED_ShowNum(10, 5, Timer_GetCounter(&Timer2Config), 5);
//         Delay_ms(50);
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
