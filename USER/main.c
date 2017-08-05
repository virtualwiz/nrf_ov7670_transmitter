#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "led.h"
#include "spi.h"
#include "24l01.h"   
#include "string.h"

#define NUM_OF_BEACONS 3

typedef struct
{
	u8 address;
	u8 status;
}Beacon_InitTypeDef;

void LCD_ShowMenuItem(u8 line,u8 *p){
	LCD_ShowString(5,line,239,24,24,p);
}

void LCD_ClearMenuArea(){
	LCD_Fill(0,20,239,308,WHITE);
}

void LCD_DrawIndex(){
	LCD_Clear(WHITE);
	POINT_COLOR = BLACK;
	LCD_ShowString(5,0,240,12,12,"Copyright(C) 2017 Team DUBHE of WIT");
	LCD_ShowString(5,12,240,24,24,"Beacons Controller");
	LCD_ShowString(5,307,240,12,12,"TARGET: Landzo Beacons System V3");
}

u8 radioTxBuff[32];
u8 radioRxBuff[32];

Beacon_InitTypeDef Beacon[3];


int main(void){
	int cx;
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(9600);	 //串口初始化为9600
	LCD_Init();
	LED_Init();
	LCD_DrawIndex();
	NRF24L01_Init();
	
	for(cx = 0;cx <= 2;cx++)
	{
		Beacon[cx].address = cx + 1;
	}

	while(NRF24L01_Check())	//检查NRF24L01是否在位.	
	{
		LCD_ShowString(60,130,200,16,16,"NRF24L01 Error");
		delay_ms(200);
		LCD_Fill(60,130,239,130+16,WHITE);
 		delay_ms(200);
	}
	
		
	radioTxBuff[0] = 7;
	radioTxBuff[1] = 0xFF;
	radioTxBuff[2] = 1;
	radioTxBuff[3] = 1;
	radioTxBuff[4] = 0;
	radioTxBuff[5] = 0;
	radioTxBuff[6] = 0;
	radioTxBuff[7] = 1;
	
	
	while(1){
//		if(NRF24L01_TxPacket(radioTxBuff) == TX_OK)
//			LED1 = !LED1;
		NRF24L01_TxPacketMode2(WR_TX_PLOAD,radioTxBuff,32);
		delay_ms(1000);
		LED0 = !LED0;
	}
}

