#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
 //////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//CAN驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/11
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//Enable the interrupt of CAN1
#define CAN_RX0_INT_ENABLE	1		//0, .								    
										 							 				    
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode);//initialization
 
u8 Can_Send_Msg(u8* msg,u8 len);						// send Msg

u8 Can_Receive_Msg(u8 *buf);							// receive the message

u8 Can_Rx_Msg(u8 *buf, uint32_t *Rx_ID);

u8 Transmit_Msg(u8* msg,u8 len, uint32_t identifier);

u32 Priority2ID( uint32_t priority, u8 FixID );

u32 B2O(u8 array[], u8 length);

void exti_config(void);
void EXTI_GPIO_init(void);
#endif

















