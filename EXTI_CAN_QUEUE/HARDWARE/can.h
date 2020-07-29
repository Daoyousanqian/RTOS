#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
 //////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//CAN���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/11
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
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

















