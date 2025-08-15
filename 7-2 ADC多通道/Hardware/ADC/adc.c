#include "stm32f10x.h"

/**
 * @brief ADC1初始化函数
 * @note 该函数配置ADC1为单通道转换模式，使用PA0-PA3作为输入通道
 *       扫描模式和连续转换模式被禁用，ADC时钟为PCLK2/6。
 * @param None
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
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; // 使用PA0123作为ADC1输入
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // 模拟输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // 设置GPIO速度
    GPIO_Init(GPIOA, &GPIO_InitStructure); // 初始化PA0->模拟输入

    // 3. 配置多路开关(在获取ADC值时指定通道)
    ////ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5); // 配置ADC1的规则组第1个位置写入通道0，采样时间为55.5个时钟周期

    // 4. 配置ADC参数
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; // 独立模式
    ADC_InitStructure.ADC_ScanConvMode = DISABLE; // 非扫描单通道转换模式
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; // 单次转换模式
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

/**
 * @brief 获取指定ADC通道的转换值
 * @param ADC_ChannelN: ADC通道编号 (0-3)
 * @retval 返回指定通道的ADC转换结果
 * @note 该函数配置ADC1的规则组第1个位置为指定通道N，并启动软件触发转换。
 *       等待转换完成后返回转换结果。
 */
uint16_t ADC_GetValue(uint8_t ADC_ChannelN)
{
    ADC_RegularChannelConfig(ADC1, ADC_ChannelN, 1, ADC_SampleTime_55Cycles5); // 配置ADC1的规则组第1个位置写入通道N，采样时间为55.5个时钟周期
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 使能ADC1软件触发转换
    while (RESET == ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); // 等待转换完成

    return ADC_GetConversionValue(ADC1); // 返回ADC1转换结果
}
