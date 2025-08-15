#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "oled.h"
#include "adc.h"

uint16_t ADCValueCh0, ADCValueCh1, ADCValueCh2, ADCValueCh3; // 定义ADC通道0-3的转换结果变量

int main(void)
{
    // 1. 模块初始化
	OLED_Init(); //OLED初始化
    ADC1_Init(); //ADC初始化
	
	// 2. OLED显示静态文本
    OLED_ShowString(1, 1, "ADC Value Ch0:"); // 在1行1列显示"ADC Value Ch0:"
    OLED_ShowString(2, 1, "ADC Voltage Ch1:"); // 在2行1列显示"ADC Voltage Ch1:"
    OLED_ShowString(3, 1, "ADC Voltage Ch2:"); // 在3行1列显示"ADC Voltage Ch2:"
    OLED_ShowString(4, 1, "ADC Voltage Ch3:"); // 在4行1列显示"ADC Voltage Ch3:"
	
    // 3. 进入主循环, 持续获取ADC值并显示
	while (1)
	{
        ADCValueCh0 = ADC_GetValue(ADC_Channel_0); // 获取ADC通道0的转换值
        ADCValueCh1 = ADC_GetValue(ADC_Channel_1); // 获取ADC通道1的转换值
        ADCValueCh2 = ADC_GetValue(ADC_Channel_2); // 获取ADC通道2的转换值
        ADCValueCh3 = ADC_GetValue(ADC_Channel_3); // 获取ADC通道3的转换值
		OLED_ShowNum(1, 9, ADCValueCh0, 4); // 在1行9列显示ADC通道0的值，4位宽度
        OLED_ShowNum(2, 9, ADCValueCh1, 4); // 在2行9列显示ADC通道1的值，4位宽度
        OLED_ShowNum(3, 9, ADCValueCh2, 4); // 在3行9列显示ADC通道2的值，4位宽度
        OLED_ShowNum(4, 9, ADCValueCh3, 4); // 在4行9列显示ADC通道3的值，4位宽度

        Delay_ms(100); // 延时100毫秒
	}
}
