#include "stm32f10x.h"                  // Device header
#include "Delay.h" 
#include "Key.h"
#include "OLED.h"
#include "Calculators.h"
#include "waterled.h"
#include "clock.h"
uint8_t KeyNum2;
int main(void)
{   Key_Init();OLED_Init ();
	uint8_t Flag=0;
	while(1){OLED_ShowCN(1,3,14);OLED_ShowCN(1,4,15);
		KeyNum2 = Key_GetNum();
	if(KeyNum2==1){
		OLED_Clear ();
		Flag++;}
	if(Flag==3){Flag=0;}
	
	
	if(Flag ==0){
	OLED_ShowCN(4,3,16);OLED_ShowCN(4,4,17);OLED_ShowCN(4,5,18);
		
	if(KeyNum2==2){Calculators_on ();}
	}
	
	
	if(Flag ==1){
	OLED_ShowCN(4,3,19);OLED_ShowCN(4,4,20);OLED_ShowCN(4,5,21);
		
	if(KeyNum2==2){waterled_on ();}
	}
	
	
	if(Flag ==2){
	OLED_ShowCN(4,3,22);OLED_ShowCN(4,4,23);
	

	if(KeyNum2==2){Stopwatch();}
	}
}
}