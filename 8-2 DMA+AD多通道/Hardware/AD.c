#include "stm32f10x.h" // Device header

uint16_t AD_Value[4];

void AD_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 开启ADC1的时钟，ADC是APB2上的设备
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 开启GPIOA的时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);    // RCC开启DMA时钟

    RCC_ADCCLKConfig(RCC_PCLK2_Div6); // ADCCLK=72MHz/6=12MHz

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN; // AIN是ADC专属模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_55Cycles5);

    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;                  // ADC模式，独立模式
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;              // 对齐方式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; // 外部触发转换选择，触发控制的触发源,软件触发
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;                  // 连续转换模式，选择连续转换还是单次转换
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;                        // 扫描转换模式，选择扫描模式还是非扫描模式
    ADC_InitStructure.ADC_NbrOfChannel = 4;                             // 通道数目，指定扫描模式下，总共会用到几个通道
    ADC_Init(ADC1, &ADC_InitStructure);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;             // 外设起始地址
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // 外设数据宽度
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            // 是否自增
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)AD_Value;                  // 存储器的起始地址
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         // 存储器的数据宽度
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     // 是否自增
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                          // 传输方向
    DMA_InitStructure.DMA_BufferSize = 4;                                       // 缓存区大小，传输计数器
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // 传输模式，是否自动重装
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                                // 硬件触发还是软件触发
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       // 优先级
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE); // 开启ADC到DMA的输出
    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1) == SET)
        ;
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1) == SET)
        ;
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); // 软件触发
}
