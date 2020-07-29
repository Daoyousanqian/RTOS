#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//Here is used for setting the keys function							  
//////////////////////////////////////////////////////////////////////////////////   	 


//#define KEY0 PEin(4)   	//PE4
//#define KEY1 PEin(3)	//PE3 
//#define WK_UP PAin(0)	//PA0  WK_UP

#define KEY0  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_4)// define key0 on GPIO_Pin_
#define KEY1  GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_3)// define key1 on GPIO_Pin_3
#define WK_UP   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)//define Key_Up on GPIO_Pin_0

 

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	  2	//KEY1����
#define WKUP_PRES   3	//KEY_UP����(��WK_UP/KEY_UP)


void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan(u8);  	//����ɨ�躯��					    
#endif
