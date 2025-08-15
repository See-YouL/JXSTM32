#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "oled.h"
#include "adc.h"

uint16_t ADCValue; // ADC转换结果
float ADCVoltage; // ADC电压值

int main(void)
{
    // 1. 模块初始化
	OLED_Init(); //OLED初始化
    ADC1_Init(); //ADC初始化
	
	// 2. OLED显示静态文本
	OLED_ShowString(1, 1, "ADCValue:"); // 在1行1列显示文本"ADCValue:"
    OLED_ShowString(2, 1, "ADCVoltage:"); // 在2行1列显示文本"ADCVoltage:"
	
    // 3. 进入主循环, 持续获取ADC值并显示
	while (1)
	{
        ADCValue = ADC_GetValue();		// 获取ADC转换结果
        ADCVoltage = (float)ADCValue / 4095 * 3.3; // 将ADC值转换为电压值，假设参考电压为3.3V
		OLED_ShowNum(1, 9, ADCValue, 4); // 在1行9列显示ADC值，4位宽度
        OLED_ShowNum(2, 9, ADCVoltage, 1); // 在2行9列显示ADC电压值，1位宽度
        OLED_ShowNum(2, 11, (uint16_t)(ADCVoltage * 100) % 100, 2); // 在2行11列显示ADC电压值的小数部分，2位宽度

        Delay_ms(100); // 延时100毫秒
	}
}
