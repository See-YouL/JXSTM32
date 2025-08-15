# ADC模数转换器笔记

## 理论知识

[ADC模数转换器理论内容视频链接](https://www.bilibili.com/video/BV1th411z7sn/?p=21&share_source=copy_web&vd_source=0db47c15b9f51dbaa4548ec2dc55dea4)

*暂时没时间了，先把视频链接放在这里，等有时间再补充笔记内容。*

## ADC单通道实验

工程文件目录: `7-1 ADC单通道实验`

实验目标: **A**

### ADC相关库函数

在`stm32f10x_rcc.h`文件中，ADC相关的库函数主要有以下几个：

```c
void RCC_ADCCLKConfig(uint32_t RCC_PCLK2); // 配置ADCCLK分频器
```

在`stm32f10x_adc.h`文件中，ADC相关的库函数主要有以下几个：

```c
void ADC_DeInit(ADC_TypeDef* ADCx); // 恢复缺省配置
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct); // 初始化
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct); // ADC_InitTypeDef结构体初始化
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState); // ADC开关控制
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState); // 开启或关闭ADC的DMA功能
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState); // ADC的中断配置
void ADC_ResetCalibration(ADC_TypeDef* ADCx); // ADC复位校准
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx); // 获取ADC复位校准状态
void ADC_StartCalibration(ADC_TypeDef* ADCx); // ADC开始校准
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx); // 获取ADC校准状态
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState); // 软件触发ADC转换
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx); // 获取软件触发ADC转换状态(返回值与转换是否结束无关，一般不用)
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number); // 配置间断模式下DC的规则组转换通道数量
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState); // 开启或关闭ADC的规则组转换通道间断模式
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime); // 配置规则组转换通道
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState); // 开启或关闭ADC的规则组转换外部触发
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx); // 获取规则组转换值
uint32_t ADC_GetDualModeConversionValue(void); // 获取双ADC模式转换值
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv);
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState);
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx);
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime);
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, uint8_t Length);
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset);
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel);
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog); // 开启或关闭ADC的模拟看门狗功能
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold, uint16_t LowThreshold); // 配置ADC模拟看门狗的高低阈值
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel); // 配置ADC模拟看门狗单通道
void ADC_TempSensorVrefintCmd(FunctionalState NewState); // 开启或关闭ADC的温度传感器和内部参考电压
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG); // 获取ADC标志位状态(参数给EOC即可判断转换是否结束)
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG); // 清除ADC标志位
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT); // 获取ADC中断状态
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT); // 清除ADC中断挂起位

```

### ADC初始化函数

在`adc.c`文件中，ADC的初始化函数主要有以下几个步骤：

1. **开启RCC时钟**：开启ADC和GPIO的时钟，以及配置ADCCLK的分频器。
2. **配置GPIO**：将GPIO配置为模拟输入模式，通常使用PA0
3. **配置多路开关**：配置ADC的规则组转换通道,序列位置和采样时间。
4. **配置ADC参数**：设置ADC的工作模式、扫描模式、转换模式、触发方式、数据对齐方式和通道数量等参数。
5. **开启ADC**：使能ADC1。
6. **ADC校准**：复位ADC校准寄存器，等待重置完成后开始校准，等待校准完成。

```c
/**
 * @brief  初始化ADC1为单通道转换模式
 * @note   使用PA0作为ADC1的输入通道，采样时间为55.5个时钟周期
 *         该函数配置ADC1为独立模式，非扫描单通道转换模式，
 *         单次转换模式，外部触发转换禁用，数据右对齐，
 *         规则组转换通道数量为1。
 * @param  None
 * @retval None
 */
void ADC1_Init(void)
{
    // 1. 开启RCC时钟, ADC,GPIO,ADCCLK
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE); // 开启ADC1时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // 配置ADC时钟为PCLK2/6

    // 2. 配置GPIO, 模拟输入模式
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; // 使用PA0作为ADC1输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化PA0->模拟输入

    // 3. 配置多路开关
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); // 配置ADC1的规则组第1个位置写入通道0，采样时间为55.5个时钟周期

    // 4. 配置ADC参数
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 非扫描单通道转换模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 单次转换模式
    // 若使用单通道连续转换模式，可取消注释以下行, 并注释上行
    ////ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; // 连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发转换禁用
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = 1; // 规则组转换通道数量为1
    ADC_Init(ADC1, &ADC_InitStructure);

    // 5. 开启ADC
    ADC_Cmd(ADC1, ENABLE); // 使能ADC1

    // 6. ADC校准
    ADC_ResetCalibration(ADC1); // 复位ADC1校准寄存器
    while (SET == ADC_GetResetCalibrationStatus(ADC1)); // 等待校准寄存器重置完成
    ADC_StartCalibration(ADC1); // 开始ADC1校准
    while (SET == ADC_GetCalibrationStatus(ADC1)); // 等待校准完成

}
```

### ADC获取转换值函数

在`adc.c`文件中，获取ADC转换值的函数主要有以下几个步骤：

1. **使能ADC转换**：使用`ADC_SoftwareStartConvCmd`函数使能ADC1的软件触发转换。
2. **等待转换完成**：使用`ADC_GetFlagStatus`函数检查转换完成标志位`ADC_FLAG_EOC`，直到转换完成。
3. **获取转换结果**：使用`ADC_GetConversionValue`函数获取ADC1

若使用单通道连续转换模式则可以取消注释`ADC_SoftwareStartConvCmd`和`ADC_GetFlagStatus`的相关代码。

```c
/**
 * @brief  获取ADC1转换结果
 * @param  None
 * @retval ADC1转换结果
 */
uint16_t ADC_GetValue(void)
{
    // 若使用单通道连续转换模式, 可取消注释以下两行
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 使能ADC1软件触发转换
    while (RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); // 等待转换完成

    return ADC_GetConversionValue(ADC1); // 返回ADC1转换结果
}
```

### ADC单通道转换实验流程

在`main.c`文件中，ADC单通道转换实验的主要流程如下：

1. **模块初始化**：调用`OLED_Init()`和`ADC_Init()`函数初始化OLED显示屏和ADC模块。
2. **OLED显示静态文本**：使用`OLED_ShowString`函数在OLED
3. **主循环获取ADC值并显示**：在死循环中调用`ADC_GetValue()`函数获取ADC转换结果，并将其转换为电压值。使用`OLED_ShowNum`函数在OLED上显示ADC值和电压值。

```c
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
```
