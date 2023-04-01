#include "stm32f10x.h" // Device header

void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 开启ADC1的时钟，ADC是APB2上的设备
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟

    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADCCLK=72MHz/6=12MHz

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // AIN是ADC专属模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // ADC模式，独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 对齐方式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发转换选择，触发控制的触发源,软件触发
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;                 // 连续转换模式，选择连续转换还是单次转换
    ADC_InitStructure.ADC_ScanConvMode = DISABLE;                       // 扫描转换模式，选择扫描模式还是非扫描模式
    ADC_InitStructure.ADC_NbrOfChannel = 1;                             // 通道数目，指定扫描模式下，总共会用到几个通道
    ADC_Init(ADC1, &ADC_InitStructure);

    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET);
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET);
}

// 启动转换，获取结果
uint16_t AD_GetValue(void)
{
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 软件触发
    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET); // 获取标志位状态函数,等待标志位
    return ADC_GetConversionValue(ADC1); // 读取DR寄存器会自动清楚EOC标志位
}
