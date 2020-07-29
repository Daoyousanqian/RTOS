#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/2
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	   

//��ʼ��PB5��PE5Ϊ�����.��ʹ���������ڵ�ʱ��		    
//LED IO��ʼ��
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_5);						 //PB.5 �����

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	    		 //LED1-->PE.5 �˿�����, �������
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //������� ��IO���ٶ�Ϊ50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5); 						 //PE.5 ����� 
}
 
void EXTI_GPIO_init(void )
{
		GPIO_InitTypeDef       GPIO_InitStructure;     // define the data structure

   	NVIC_InitTypeDef  NVIC_InitStructure;

		// Initialize the the GPIO pin PA0 to do interrupt
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//�W��?�J
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