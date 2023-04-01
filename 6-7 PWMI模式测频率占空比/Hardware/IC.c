#include "stm32f10x.h" // Device header

// 1、RCC开启时钟，把GPIO和TIM的时钟打开
// 2、GPIO初始化，把GPIO配置成输入模式（一般是上拉输入或浮空输入
// 3、配置时基单元，让CNT计数器在内部时钟的驱动下自增运行
// 4、配置输入捕获单元（包括滤波器、极性、直连通道还是交叉通道、分频器
// 5、选择从模式的触发源，触发源选择位TI1FP1，这里调用一个库函数
// 6、选择触发之后执行的操作，执行Reset操作
// 7、调用TIM_Cmd函数，开启定时器
void IC_Init(void)
{
    // 1、RCC开启时钟，把GPIO和TIM的时钟打开
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); // 开启RCC内部时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 2、GPIO初始化，把GPIO配置成输入模式（一般是上拉输入或浮空输入
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    TIM_InternalClockConfig(TIM3); // 选择时基单元的时钟

    // 3、配置时基单元，让CNT计数器在内部时钟的驱动下自增运行
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              // 配置时基单元
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 指定时钟分频率
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式
    TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1;               // ARR自动重装器的值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1;               // PSC预分频器的值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器的值
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

    // 4、配置输入捕获单元（包括滤波器、极性、直连通道还是交叉通道、分频器
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                // 选择通道
    TIM_ICInitStructure.TIM_ICFilter = 0x0F;                        // 配置输入捕获的滤波器
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;     // 边沿检测，极性选择触发方式
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           // 分频器
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 选择触发信号从哪个引脚输入 直连通道
    TIM_ICInit(TIM3, &TIM_ICInitStructure);
    TIM_PWMIConfig(TIM3, &TIM_ICInitStructure); // 函数自动把参数处理相反配置

    // 5、选择从模式的触发源，触发源选择位TI1FP1，这里调用一个库函数
    TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
    // 6、选择触发之后执行的操作，执行Reset操作
    TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // 配置从模式位slave mode

    // 7、调用TIM_Cmd函数，开启定时器
    TIM_Cmd(TIM3, ENABLE);
}

// 读取CCR 进行计算
uint32_t IC_GetFreq(void)
{
    return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

// 获取占空比
uint32_t IC_GetDuty(void)
{
    return (TIM_GetCapture2(TIM3) + 1) * 100 / (TIM_GetCapture1(TIM3) + 1);
}
