#include "stm32f10x.h"                  // Device header
#include "Delay.h"
uint16_t Key_Count;

void Key_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
    uint8_t KeyNum = 0;
    uint8_t key11State = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
    uint8_t key0State = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);

    // 先检查两个按键是否同时按下
    if (key11State == 0 && key0State == 0)
    {
        Delay_ms(20); // 消抖
        key11State = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11);
        key0State = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0);
        if (key11State == 0 && key0State == 0)
        {
            while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0 && GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);
            Delay_ms(20); // 消抖
            KeyNum = 3; // 定义两个按键一起按返回 3
        }
    }
    else if (key11State == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
        Delay_ms(20);
        KeyNum = 1;
    }
    else if (key0State == 0)
    {
        Delay_ms(20);
        while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0) == 0);
        Delay_ms(20);
        KeyNum = 2;
    }

    return KeyNum;
}