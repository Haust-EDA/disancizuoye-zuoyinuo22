#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "Key.h"
#include "OLED.h"
#include "PWM.h"

uint8_t KeyNum3, p;

// 封装PWM渐变函数
void PWM_Fade(uint8_t channel) {
    for (p = 0; p <= 100; p++) {
        switch (channel) {
            case 1: PWM_SetCompare1(p); break;
            case 2: PWM_SetCompare2(p); break;
            case 3: PWM_SetCompare3(p); break;
            case 4: PWM_SetCompare4(p); break;
        }
        Delay_ms(10);
        KeyNum3 = Key_GetNum();  // 检测按键状态
        if (KeyNum3 == 1) {
            // 退出时将PWM占空比置为0
            switch (channel) {
                case 1: PWM_SetCompare1(0); break;
                case 2: PWM_SetCompare2(0); break;
                case 3: PWM_SetCompare3(0); break;
                case 4: PWM_SetCompare4(0); break;
            }
            return; // 若按键1按下，退出函数
        }
    }
    for (p = 0; p <= 100; p++) {
        switch (channel) {
            case 1: PWM_SetCompare1(100 - p); break;
            case 2: PWM_SetCompare2(100 - p); break;
            case 3: PWM_SetCompare3(100 - p); break;
            case 4: PWM_SetCompare4(100 - p); break;
        }
        Delay_ms(10);
        KeyNum3 = Key_GetNum();  // 检测按键状态
        if (KeyNum3 == 1) {
            // 退出时将PWM占空比置为0
            switch (channel) {
                case 1: PWM_SetCompare1(0); break;
                case 2: PWM_SetCompare2(0); break;
                case 3: PWM_SetCompare3(0); break;
                case 4: PWM_SetCompare4(0); break;
            }
            return; // 若按键1按下，退出函数
        }
    }
}

void waterled_on(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    Key_Init();
    OLED_Init();
    PWM_Init();

    uint8_t Flag = 0, led = 0;
    while (1) {
        KeyNum3 = Key_GetNum();
        if (KeyNum3 == 1) {
            OLED_Clear();
            Flag++;
            // 退出时将所有PWM占空比置为0
            PWM_SetCompare1(0);
            PWM_SetCompare2(0);
            PWM_SetCompare3(0);
            PWM_SetCompare4(0);
        }
        if (Flag == 3) {
            Flag = 0;
        }
        if (Flag == 2) {
            OLED_ShowCN(1, 1, 24);
            OLED_ShowCN(1, 2, 25);
            if (KeyNum3 == 2) {
                // 退出时将所有PWM占空比置为0
                PWM_SetCompare1(0);
                PWM_SetCompare2(0);
                PWM_SetCompare3(0);
                PWM_SetCompare4(0);
                OLED_Clear();
                break;
            }
        }
        if (Flag == 0) {
            OLED_ShowCN(4, 3, 26);
            OLED_ShowCN(4, 4, 27);
            if (KeyNum3 == 2) {
                led = 1;
            }
        }
        if (Flag == 1) {
            OLED_ShowCN(4, 3, 28);
            OLED_ShowCN(4, 4, 29);
            if (KeyNum3 == 2) {
                led = 2;
            }
        }


        while (led == 1) {
            KeyNum3 = Key_GetNum();
            PWM_SetCompare1(100);
            Delay_ms(100);
            PWM_SetCompare1(0);
            Delay_ms(100);
            KeyNum3 = Key_GetNum();  // 检测按键状态
            if (KeyNum3 == 1) {
                // 退出时将所有PWM占空比置为0
                PWM_SetCompare1(0);
                PWM_SetCompare2(0);
                PWM_SetCompare3(0);
                PWM_SetCompare4(0);
                OLED_Clear();
                Flag++;
                led = 0;
                break;
            }

            PWM_SetCompare2(100);
            Delay_ms(100);
            PWM_SetCompare2(0);
            Delay_ms(100);
            KeyNum3 = Key_GetNum();  // 检测按键状态
            if (KeyNum3 == 1) {
                // 退出时将所有PWM占空比置为0
                PWM_SetCompare1(0);
                PWM_SetCompare2(0);
                PWM_SetCompare3(0);
                PWM_SetCompare4(0);
                OLED_Clear();
                Flag++;
                led = 0;
                break;
            }

            PWM_SetCompare3(100);
            Delay_ms(100);
            PWM_SetCompare3(0);
            Delay_ms(100);
            KeyNum3 = Key_GetNum();  // 检测按键状态
            if (KeyNum3 == 1) {
                // 退出时将所有PWM占空比置为0
                PWM_SetCompare1(0);
                PWM_SetCompare2(0);
                PWM_SetCompare3(0);
                PWM_SetCompare4(0);
                OLED_Clear();
                Flag++;
                led = 0;
                break;
            }

            PWM_SetCompare4(100);
            Delay_ms(100);
            PWM_SetCompare4(0);
            Delay_ms(100);
            KeyNum3 = Key_GetNum();  // 检测按键状态
            if (KeyNum3 == 1) {
                // 退出时将所有PWM占空比置为0
                PWM_SetCompare1(0);
                PWM_SetCompare2(0);
                PWM_SetCompare3(0);
                PWM_SetCompare4(0);
                OLED_Clear();
                Flag++;
                led = 0;
                break;
            }
        }

        while (led == 2) {
            for (uint8_t i = 1; i <= 4; i++) {
                PWM_Fade(i);
                if (KeyNum3 == 1) {
                    // 退出时将所有PWM占空比置为0
                    PWM_SetCompare1(0);
                    PWM_SetCompare2(0);
                    PWM_SetCompare3(0);
                    PWM_SetCompare4(0);
                    OLED_Clear();
                    Flag++;
                    led = 0;
                    break;
                }
            }
        }
    }
}