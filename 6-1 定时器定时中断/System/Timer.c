#include "stm32f10x.h"

extern uint16_t Num;

void Timer_Init(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); // 开启RCC内部时钟

    TIM_InternalClockConfig(TIM2); // 选择时基单元的时钟

    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;              // 配置时基单元
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     // 指定时钟分频率
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 计数器模式
    TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1;               // ARR自动重装器的值
    TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1;             // PSC预分频器的值
    TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;            // 重复计数器的值
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);      // 手动清除中断标志位，避免上电立刻进入中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能更新中断

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // NVIC优先级分组

    NVIC_InitTypeDef NVICInitStructure;
    NVICInitStructure.NVIC_IRQChannel = TIM2_IRQn;           // 中断通道
    NVICInitStructure.NVIC_IRQChannelCmd = ENABLE;           //
    NVICInitStructure.NVIC_IRQChannelPreemptionPriority = 2; // 抢占优先级
    NVICInitStructure.NVIC_IRQChannelSubPriority = 1;        // 响应优先级
    NVIC_Init(&NVICInitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

// // 定时器2中断函数
// void TIM2_IRQHandler(void)
// {
//     if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 检查中断标志位
//     {
//         TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清楚标志位
//     }
// }
