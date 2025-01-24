#include "stm32f10x.h"
#include "Delay.h"
#include "Key.h"
#include "OLED.h"

// 定时器初始化函数
void TIM3_Init(void) {
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能 TIM3 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    // TIM3 初始化
    TIM_TimeBaseStructure.TIM_Period = 999; // 自动重装载值
    TIM_TimeBaseStructure.TIM_Prescaler = 71; // 预分频值
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 使能 TIM3 中断
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // 配置 NVIC
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能 TIM3
    TIM_Cmd(TIM3, ENABLE);
}

// 定时器 3 中断服务函数
volatile uint32_t ms_count = 0; // 毫秒计数器
void TIM3_IRQHandler(void) {
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) {
        ms_count++; // 每 1ms 加 1
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
}

// 秒表状态枚举
typedef enum {
    STOPPED,
    RUNNING,
    PAUSED
} StopwatchState;

StopwatchState state = STOPPED; // 秒表初始状态为停止

// 新增标志位，用于记录是否已经进行过清零操作
uint8_t has_been_cleared = 0;

// 记录时间节点的数组，最多记录两个
uint32_t records[2];
// 记录索引，用于标记当前记录的位置
uint8_t record_index = 0,flag=1;

// 秒表函数
void Stopwatch(void) {
    OLED_Init();
    Key_Init();
    TIM3_Init();

    uint32_t last_ms_count = 0; // 上一次的毫秒计数
    uint32_t elapsed_ms = 0;    // 经过的毫秒数
    uint32_t prev_seconds = 0;  // 上一次显示的秒数
    uint32_t prev_milliseconds = 0; // 上一次显示的毫秒数

    // 显示固定部分
    OLED_ShowString(1, 1, "Stopwatch:");
    // 显示初始的时间和冒号
    OLED_ShowNum(2, 1, 0, 3);
    OLED_ShowChar(2, 4, ':');
    OLED_ShowNum(2, 5, 0, 2);

    while (flag) {
        uint8_t key_num = Key_GetNum();

        switch (key_num) {case 3:flag=0;OLED_Clear();break;
            case 2: // 开始/暂停按键
                if (state == STOPPED || state == PAUSED) {
                    state = RUNNING;
                    last_ms_count = ms_count;
                    has_been_cleared = 0; // 开始计时，重置清零标志
                } else if (state == RUNNING) {
                    state = PAUSED;
                }
                break;
            case 1: // 现清零/记录时间节点按键
                if (state == PAUSED) {
                    if (has_been_cleared) {
                        // 若已经清零过，再次按下清零按键退出
                        OLED_Clear();
                        return;
                    }
                    state = STOPPED;
                    ms_count = 0;
                    elapsed_ms = 0;
                    prev_seconds = 0;
                    prev_milliseconds = 0;
                    // 清空记录索引
                    record_index = 0;
                    OLED_ShowNum(2, 1, 0, 3);
                    OLED_ShowChar(2, 4, ':');
                    OLED_ShowNum(2, 5, 0, 2);
                    has_been_cleared = 1; // 标记为已经进行过清零操作
                    // 清空记录显示区域
                    OLED_ShowString(3, 1, "                ");
                    OLED_ShowString(4, 1, "                ");
                } else if (state == RUNNING) {
                    records[record_index] = elapsed_ms;
                    uint32_t sec = records[record_index] / 1000;
                    uint32_t ms = records[record_index] % 1000;
                    // 显示记录，索引为 0 显示在第 3 行，索引为 1 显示在第 4 行
                    OLED_ShowNum(record_index + 3, 1, sec, 3);
                    OLED_ShowChar(record_index + 3, 4, ':');
                    OLED_ShowNum(record_index + 3, 5, ms / 10, 2);                    // 更新记录索引，使用取模运算实现循环覆盖
                    record_index = (record_index + 1) % 2;
                }
                break;
        }

        if (state == RUNNING) {
            elapsed_ms += ms_count - last_ms_count;
            last_ms_count = ms_count;
        }

        // 计算秒和毫秒
        uint32_t seconds = elapsed_ms / 1000;
        uint32_t milliseconds = elapsed_ms % 1000;

        // 只更新有变化的部分
        if (seconds != prev_seconds) {
            OLED_ShowNum(2, 1, seconds, 3);
            prev_seconds = seconds;
        }
        if (milliseconds / 10 != prev_milliseconds / 10) {
            OLED_ShowNum(2, 5, milliseconds / 10, 2);
            prev_milliseconds = milliseconds;
        }

        Delay_ms(10); // 适当延时
		
    }
}


