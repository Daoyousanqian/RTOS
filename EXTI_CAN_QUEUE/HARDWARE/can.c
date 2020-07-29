#include "can.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "key.h"
//////////////////////////////////////////////////////////////////////////////////	 
						  
//////////////////////////////////////////////////////////////////////////////////
//Initialize CAN bus 
//tsjw:synchronization jump width. Scope: CAN_SJW_1tq~ CAN_SJW_4tq
//tbs1:propagation phase + phase 1.   Scope: CAN_BS2_1tq~CAN_BS2_16tq;
//tbs2:Phase2.   Scope: CAN_BS1_1tq ~CAN_BS1_16tq
//brp : prescalar . Scope: 1~1024;  tq=(brp)*tpclk1
//NBR=Fpclk1/((tbs1+1+tbs2+1+1)*brp);
//mode:CAN_Mode_Normal;CAN_Mode_LoopBack;
//Fpclk1 clock frequency is 36M,  CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//the bit frequency :36M/((8+9+1)*4)=500Kbps
//return 0,  initialization OK;
//else failed;

// set  parameter: 
u8 CAN_Mode_Init(u8 tsjw,u8 tbs2,u8 tbs1,u16 brp,u8 mode) 
{

	  GPIO_InitTypeDef       GPIO_InitStructure;     // define the data structure
	  CAN_InitTypeDef        CAN_InitStructure;      // define CAN data
 	  CAN_FilterInitTypeDef  CAN_FilterInitStructure;   // define CAN filter
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif

	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);// Enable PORTA clock;                   											 

  	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);// Enable CAN1 clock;	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//   
    GPIO_Init(GPIOA, &GPIO_InitStructure);		// Initialize IO 
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//ÉÏÀ­ÊäÈë
    GPIO_Init(GPIOA, &GPIO_InitStructure);// initialize the CAN bus GPIO
	  
		
		
		
		
		//CAN parameter seting
		
 	  CAN_InitStructure.CAN_TTCM=DISABLE;						 //Time-Trggered CAN communication disable //
 	  CAN_InitStructure.CAN_ABOM=DISABLE;						 //Busoff mode disable//
  	CAN_InitStructure.CAN_AWUM=DISABLE;						 //sleep mode wake up by SW, disable//
  	CAN_InitStructure.CAN_NART=DISABLE;						 //Disable transmit automotively //     // CHANGED BY WSQ
  	CAN_InitStructure.CAN_RFLM=DISABLE;						 //new message cove old ones// 
  	CAN_InitStructure.CAN_TXFP=DISABLE;						 //CAN priority is decided by identifier //
  	CAN_InitStructure.CAN_Mode= mode;	             // set transmission mode  //
  	//set baud rate
  	CAN_InitStructure.CAN_SJW=tsjw;				//  tsjw + 1,  CAN_SJW_1tq, CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq
  	CAN_InitStructure.CAN_BS1=tbs1; //Tbs1=tbs1+1¸CAN_BS1_1tq ~CAN_BS1_16tq
  	CAN_InitStructure.CAN_BS2=tbs2;//Tbs2=tbs2+1¸öÊ±¼äµ¥Î»CAN_BS2_1tq ~	CAN_BS2_8tq
  	CAN_InitStructure.CAN_Prescaler=brp;            // Prescalar brp+1	//
  	CAN_Init(CAN1, &CAN_InitStructure);            // Initialize CAN1 

 	  CAN_FilterInitStructure.CAN_FilterNumber=0;	  //filter 0
   	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask; // mask 
  	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit; //32 bits 
  	CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;////32 bits ID
  	CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;//32 bit MASK
  	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0;// filter0 and FIFO0
 	  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE; // Enable filter 0

  	CAN_FilterInit(&CAN_FilterInitStructure);// initialize filter mask 
#if CAN_RX0_INT_ENABLE
	
	  CAN_ITConfig(CAN1,CAN_IT_FMP0,ENABLE);//FIFO0 message interrupt .		    
  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);     
		//NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		
  	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn; 		// initialize the external interrupt 
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 15;     //  main priority level is 1
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // ´second is 0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
		
	
		
#endif
	return 0;
}   

/*void EXTI_GPIO_init(void )
{
		GPIO_InitTypeDef       GPIO_InitStructure;     // define the data structure
#if CAN_RX0_INT_ENABLE 
   	NVIC_InitTypeDef  NVIC_InitStructure;
#endif
		// Initialize the the GPIO pin PA0 to do interrupt
		
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;//ÉÏÀ­ÊäÈë
    GPIO_Init(GPIOA, &GPIO_InitStructure);// initialize the CAN bus GPIO
	
		// define the external interrupt of on KEY1
  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn; 		// initialize the external interrupt 
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;     //  main priority level is 1
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // ´second is 0
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
	
	
} */

//*****************************
//****************************
// this is very important
// if you want to use external interrupt. 
// 1. first assign the GPIO pins 
// 2, Second initialize the EXTERNAL Interrupt Config
// 3, Set the External Interrupt priority 
// 4, Interrupt Handler;
void exti_config(void)
{
	EXTI_InitTypeDef exti_init_config;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);	//¥~³¡¤¤?¡A»Ý­n¨Ï¯àAFIO??
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);	//
	
	exti_init_config.EXTI_Line		= EXTI_Line0;
  exti_init_config.EXTI_Mode 		= EXTI_Mode_Interrupt;	
  exti_init_config.EXTI_Trigger 	= EXTI_Trigger_Rising;//¤W¤ÉªuàD?
  exti_init_config.EXTI_LineCmd 	= ENABLE;
  	
	EXTI_Init(&exti_init_config);	 	//®ÚÕuEXTI_InitTypeDef¤¤«ü©wªº??ªì©l¤Æ¥~?EXTI±H¦s¾¹
}





/*
#if CAN_RX0_INT_ENABLE	//enable RX0 interrupt
//interrupt function			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	CanRxMsg RxMessage;
	u8 canbuf[8];
	int i=0;
	for( i = 0; i <8; i++)
		canbuf[i] = i+10;
  
	
  CAN_Receive(CAN1, 0, &RxMessage);
	if(RxMessage.StdId == 0x11)
		Transmit_Msg(canbuf, 8 ,238);
	if(RxMessage.StdId != 0x11)
			{
					//LED0=!LED0;
					CAN_CancelTransmit(CAN1, 0);
			}
	printf("the id is %d\r\n", RxMessage.StdId);
	//for(i=0;i<8;i++)
	//printf("rxbuf[%d]:%d\r\n",i,RxMessage.Data[i]);
}
#endif

*/


//CAN transmit data 
//len: up to 8 bytes			     
//msg: the data array, at most 8 bytes.
//return:0, succeed;
//else, fail;
u8 Can_Send_Msg(u8* msg,u8 len)
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x12;					 // standard identifier 
  TxMessage.ExtId=0x13A4C2;				   // extended identifier
  TxMessage.IDE=CAN_Id_Extended;
	//TxMessage.IDE=CAN_Id_Standard; // 
  TxMessage.RTR=CAN_RTR_Data;		 // Here is delare this is data frame, not remote trasmission requirement¡
  TxMessage.DLC=len;						//  data length 
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];			          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
  while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//
  if(i>=0XFFF)return 1;
  return 0;		

}

//CAN received messages
//buf: 	 
//if no message then return;
u8 Can_Receive_Msg(u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//if there is no data then return
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);// read data
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
	return RxMessage.DLC;	
}
// CAN transimit function
// The ID need to be update 
u8 Transmit_Msg(u8* msg,u8 len, uint32_t identifier) 
{	
  u8 mbox;
  u16 i=0;
  CanTxMsg TxMessage;
  TxMessage.StdId=0x145;					 // standard identifier 
  TxMessage.ExtId=identifier;				   // extended identifier
  TxMessage.IDE=CAN_Id_Standard;
	//TxMessage.IDE=CAN_Id_Extended;  // 
  TxMessage.RTR=CAN_RTR_Data;		 // Here is to data frame, not remote trasmission requirement¡
  TxMessage.DLC=len;						//  data length 
  for(i=0;i<len;i++)
  TxMessage.Data[i]=msg[i];			          
  mbox= CAN_Transmit(CAN1, &TxMessage);   
  i=0;
	return 0;
  //while((CAN_TransmitStatus(CAN1, mbox)==CAN_TxStatus_Failed)&&(i<0XFFF))i++;	//
  //if(i>=0XFFF)return 1;
  //return 0;		

}


u8 Can_Rx_Msg(u8 *buf, uint32_t *Rx_ID)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;
    if( CAN_MessagePending(CAN1,CAN_FIFO0)==0)return 0;		//if there is no data then return
    CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);// read data
    for(i=0;i<8;i++)
    buf[i]=RxMessage.Data[i];  
		*Rx_ID = RxMessage.StdId;
	  //*Rx_ID = RxMessage.ExtId;
	return RxMessage.DLC;	
}

u32 Priority2ID( uint32_t priority, u8 FixID )
{
	// also avoid for bit stuffing in CAN bus;
	// the identifier format is 
	// *** 1 1 bit     **** ***** 18 bits
	// 0***#####  1xxx 01 xxxx0 ***11*** 0 xxxx 10 xxxx ####last for the fixed ####  10 xxxx 1 ####    
	u8 temp_Fix_id; 
	u8 length;
	u8 temp;
	u8 a[15] = {0};
		// some of bits are already fixed to avoid bit stuffing
	u8 Dynamic_ID[22] ={1,2,2,2,0,1,2,2,2,2,0,0,2,2,2,2,1,0,2,2,2,2};
	u32 Dynamic_num = 0;
	u32 Ext_ID;
	u8 i;
	int k, j = 0;
	temp_Fix_id = (FixID << 1) + 65  ;
	
	length = 15;
	
	i =0;
	
	while(priority>0)
		{
			a[i]=priority%2;
			priority=priority/2;
			i++;
		}
	
	// reverse the sequence of array 	
	for(i = 0 ; i <length/2; i++ ){
		
		temp = a[i];
		a[i] = a[length - i -1];
		a[length - i -1] = temp;
		
	}		
	
	
	// put the number in array to b 
	for(i = 0; i < 22; i++)
	{
		if(Dynamic_ID[i] == 2 )
			{
				Dynamic_ID[i] = a[j];
				j++;
			}
	}
	

	Dynamic_num = B2O(Dynamic_ID , 22);
	
//	j = 1;
//	for( k = 21 ; k>=0; k--)
//		{
//			 Dynamic_num = Dynamic_num  + Dynamic_ID[k]*j;
//			 j=j*2;
//			//array++;
//		}
	
	Dynamic_num = Dynamic_num<<7;
	
	Ext_ID = Dynamic_num + temp_Fix_id;
	
	return Ext_ID;
	
}


u32 B2O(u8 array[], u8 length)
	{
		u32 j=1;
		u32 sum=0;
		int i = length-1;
		for( ; i>=0; i--)
		{
			sum = sum + array[i]*j;
			j=j*2;
			//array++;
		}
		
		return sum;
	} 




