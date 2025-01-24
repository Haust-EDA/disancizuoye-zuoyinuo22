#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "Key.h"
#include "OLED.h"


#define HISTORY_SIZE 10  // 历史记录的最大数量

uint8_t KeyNum;
float history[HISTORY_SIZE];  // 存储历史记录的数组
uint8_t history_index = 0;    // 当前历史记录的索引
uint8_t is_first_input = 1;   // 标志是否为第一次输入

// 查看历史记录的函数
void ViewHistory() {
    OLED_Clear();
    OLED_ShowString(1, 1, "History:");
    for (int i = 0; i < HISTORY_SIZE; i++) {
        if (history[i] != 0) {
            OLED_ShowFNum(i + 2, 1, history[i], 12, 3);
        }
    }
    while (1) {
        KeyNum = Key_GetNum();
        if (KeyNum == 2) {  // 假设按下按键 2 退出历史记录查看
            OLED_Clear();
            break;
        }
    }
}

void Calculators_on(void)
{   
    OLED_Init();
    Key_Init();
    float num = 0, sum = 0, allsum = 0;
    uint8_t flag = 1, num2 = 1, num3 = 1;
    while (1) {
        if (flag % 2 == 1) {
            OLED_ShowNum(2, 1, sum, 8);
            KeyNum = Key_GetNum();
            if (KeyNum == 1) {
                OLED_Clear();
                num++;
            }
            if (num < 10) {
                OLED_ShowCN(1, 1, 4);
                OLED_ShowCN(1, 2, 5);
                OLED_ShowCN(1, 3, 6);
                OLED_ShowString(1, 9, ":");
                OLED_ShowNum(1, 10, num, 1);
                if (KeyNum == 2) {
                    sum = sum * 10 + num;
                    num = 0;
                }
            }
            if (num == 10) {
                OLED_ShowCN(1, 1, 7);
                OLED_ShowCN(1, 2, 8);
                if (KeyNum == 2) {
                    OLED_Clear();
                    flag++;
                    num2 = 1;
                    if (!is_first_input) {
                        switch (num3) {
                            case 1:
                                allsum = allsum + sum;
                                break;
                            case 2:
                                allsum = allsum - sum;
                                break;
                            case 3:
                                allsum = allsum * sum;
                                break;
                            case 4:
                                allsum = allsum / sum;
                                break;
                        }
                        // 将计算结果添加到历史记录中
                        history[history_index++] = allsum;
                        if (history_index >= HISTORY_SIZE) {
                            history_index = 0;  // 循环覆盖旧的历史记录
                        }
                    } else {
                        allsum = sum;
                        is_first_input = 0;
                    }
                }
            }
            if (num == 11) {
                OLED_ShowCN(1, 1, 30);
                OLED_ShowCN(1, 2, 31);
                OLED_ShowCN(1, 3, 32);
                OLED_ShowCN(1, 4, 33);
                if (KeyNum == 2) {
                    ViewHistory();
                }
            }
            if (num == 12) {
                num = 0;
            }
        }
        if (flag % 2 == 0) {
            KeyNum = Key_GetNum();
            if (KeyNum == 1) {
                OLED_Clear();
                num2++;
            }
            if (num2 == 1) {
                OLED_ShowChar(1, 1, '+');
                num3 = 1;
                if (KeyNum == 2) {
                    OLED_Clear();
                    flag++;
                    num = 1;
                    sum = 0;
                }
            }
            if (num2 == 2) {
                OLED_ShowChar(1, 1, '-');
                num3 = 2;
                if (KeyNum == 2) {
                    OLED_Clear();
                    flag++;
                    num = 1;
                    sum = 0;
                }
            }
            if (num2 == 3) {
                OLED_ShowChar(1, 1, '*');
                num3 = 3;
                if (KeyNum == 2) {
                    OLED_Clear();
                    flag++;
                    num = 1;
                    sum = 0;
                }
            }
            if (num2 == 4) {
                OLED_ShowChar(1, 1, '/');
                num3 = 4;
                if (KeyNum == 2) {
                    OLED_Clear();
                    flag++;
                    num = 1;
                    sum = 0;
                }
            }   // 加减乘除

            if (num2 == 7) {
                num2 = 1;
            }   // 重新开始
            if (num2 == 6) {
                OLED_ShowCN(1, 1, 24);
                OLED_ShowCN(1, 2, 25);
                if (KeyNum == 2) {
                    OLED_Clear();
                    break;
                }
            }   // 退出
            if (num2 == 5) {
                OLED_ShowCN(1, 1, 9);
                OLED_ShowCN(1, 2, 10);
                if (KeyNum == 2) {
                    OLED_Clear();
                    OLED_ShowFNum(2, 1, allsum, 12, 3);
                }
            }   // 等于
        }
    }
}