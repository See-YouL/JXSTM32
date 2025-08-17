#include "stm32f10x.h"
#include "Delay.h"
#include "Led.h"

int main(void)
{   
    LED_Init();
    while (1)
    {
        /* 红色LED闪烁 */
        LEDR_ON();
        Delay_ms(500);
        LEDR_OFF();
        Delay_ms(500);

        /* 蓝色LED闪烁 */
        LEDB_ON();
        Delay_ms(500);
        LEDB_OFF();
        Delay_ms(500);

        /* 绿色LED闪烁 */
        LEDG_ON();
        Delay_ms(500);
        LEDG_OFF();
        Delay_ms(500);  
    }
}
