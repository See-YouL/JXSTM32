#include "stm32f10x.h"
#include "Delay.h"
#include "Oled.h"
#include "Sensor.h"
#include "FireSensor.h"


int main(void)
{
    OLED_Init();  
    OLED_Clear();
    Fire_Init(); // 初始化火焰传感器

    while (1)
    {
        // 判断火焰传感器状态
        if (Fire_GetState() == 1)
        {
            OLED_Clear();
            OLED_ShowString(1, 1, "Fire Warning");

            // 状态显示一次后清零
            Fire_ClearState();
        }
        else
        {
            OLED_Clear();
            OLED_ShowString(1, 1, "StandBy");
        }
        Delay_ms(500);
    }
}
