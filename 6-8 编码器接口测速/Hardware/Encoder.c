#include "stm32f10x.h" // Device header

// 1、RCC开启时钟，开启GPIO和定时器的时钟
// 2、配置GPIO，这里需要PA6和PA7配置成输入模式
// 3、配置时基单元，预分频器选择不分频
// 4、配置输入捕获单元
// 5、配置编码器接口模式
// 6、调用TIM_Cmd，启动定时器

void Encoder_Init(void)
{
    // 1、RCC开启时钟，把GPIO和TIM的时钟打开
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 开启RCC内部时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2、GPIO初始化，把GPIO配置成输入模式（一般是上拉输入或浮空输入
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // 3、配置时基单元，让CNT计数器在内部时钟的驱动下自增运行
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              // 配置时基单元
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 指定时钟分频率
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               // ARR自动重装器的值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;                // PSC预分频器的值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器的值
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    // 4、配置输入捕获单元（包括滤波器、极性、直连通道还是交叉通道、分频器
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICStructInit(&TIM_ICInitStructure);          // 给结构体赋初始值
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1; // 选择通道
    TIM_ICInitStructure.TIM_ICFilter = 0x0F;         // 配置输入捕获的滤波器
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2; // 选择通道
    TIM_ICInitStructure.TIM_ICFilter = 0x0F;         // 配置输入捕获的滤波器
    TIM_ICInit(TIM3, &TIM_ICInitStructure);

    TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_Cmd(TIM3, ENABLE);
}

int16_t Encoder_Get(void)
{
    int16_t Temp;
    Temp = TIM_GetCounter(TIM3);
    TIM_SetCounter(TIM3, 0);
    return Temp;
}
