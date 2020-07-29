#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//LED驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/2
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB5和PE5为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 输出高 
}
 
void EXTI_GPIO_init(void )
{
		GPIO_InitTypeDef       GPIO_InitStructure;     // define the data structure

   	NVIC_InitTypeDef  NVIC_InitStructure;

		// Initialize the the GPIO pin PA0 to do interrupt
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//┰?
    GPIO_Init(GPIOA, &GPIO_InitStructure);// initialize the CAN bus GPIO
	
		// define the external interrupt of on KEY1
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 		// initialize the external interrupt 
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 10;     //  main priority level is 1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // ?econd is 0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
	
}


void EXTI0_Config(void)
{
	EXTI_InitTypeDef exti_init_config; 		// struct of external interrupt config 
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	// set the clock 
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	// External interrupt line enabled 
	
	exti_init_config.EXTI_Line		= EXTI_Line0;
  exti_init_config.EXTI_Mode 		= EXTI_Mode_Interrupt;	
  exti_init_config.EXTI_Trigger 	= EXTI_Trigger_Rising;// rising edge is the trigger
  exti_init_config.EXTI_LineCmd 	= ENABLE;
  	
	EXTI_Init(&exti_init_config);	 	// 
}