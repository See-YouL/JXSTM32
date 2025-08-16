#include "stm32f10x.h"
#include "Delay.h"
#include "Oled.h"
#include "Timer.h"

uint16_t Num = 0; // 定义一个变量用来计数

void MyTimer2Task(void); // 定时器2中断回调函数声明

Timer_Config_t Timer2Config = { // 定时器2配置结构体
    .TIMx = TIM2, // 定时器2
    .TIMx_CLK = RCC_APB1Periph_TIM2, // 定时器2时钟
    .TIMx_IRQn = TIM2_IRQn, // 定时器2中断号
    .Prescaler = 7200 - 1,           // 10KHz
    .Period = 10000 - 1,             // 1秒周期
    .RepetitionCounter = 0,          // 重复计数器（TIM2无效，可置0）
    .ClockDivision = TIM_CKD_DIV1,   // 时钟分频
    .CounterMode = TIM_CounterMode_Up, // 向上计数
    .NVIC_PriorityGroup = NVIC_PriorityGroup_2, // NVIC优先级分组
    .PreemptionPriority = 2, // 抢占优先级2
    .SubPriority = 1, // 响应优先级1
    .Callback = MyTimer2Task // 定时器中断回调函数指针，用户中断处理逻辑
};

int main(void)
{
    OLED_Init();  
    Timer_Init(&Timer2Config); // 初始化定时器

    OLED_Clear();
    OLED_ShowString(1, 1, "Num = "); // 显示"Num ="

    while (1)
    {
        OLED_ShowNum(1, 5, Num, 5); // 显示Num的值
        Delay_ms(100); // 延时100毫秒
    }
}

/**
 * @brief 定时器2中断处理函数
 * 在定时器2中断发生时调用
 * @note 调用通用中断处理函数
 * @param None
 * @retval None
 */
void TIM2_IRQHandler(void)
{
    Timer_IRQHandler(&Timer2Config); // 调用通用中断处理函数
}

/**
 * @brief 定时器2中断回调函数
 * 在定时器2中断发生时调用
 * @note 执行Num++操作
 * @param None
 * @retval None
 */
void MyTimer2Task(void)
{
    // 在这里处理定时器中断事件，比如翻转LED
    Num++;
}
