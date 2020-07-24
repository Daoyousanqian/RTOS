#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
//#include "FreeRTOS.h"
#include "queue.h"
/************************************************

************************************************/

static void vSenderTask1( void *pvParameters );
static void vReceiverTask( void *pvParameters );
static void vSenderTask2( void *pvParameters );

// delacare one xQueueHandle variables. 
xQueueHandle xQueue;



int main(void)
{	
	// initialization
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//nested vecor interrupt control	 
	delay_init();	    				//clock tick initialization	  
	uart_init(115200);					//uart_init 
	LED_Init();		  					//init the LED
	LCD_Init();								// init the LCD
	
	POINT_COLOR = RED;
	LCD_ShowString(30,10,200,16,16,"Create 1 queues"); 
	LCD_ShowString(30,30,200,16,16,"Create two send task");
	LCD_ShowString(30,50,200,16,16,"Create 1 receive task");
	
	// here create one queue that is with 5 numbers with type long
	xQueue = xQueueCreate( 5, sizeof( long ) );
	// if create queue successfully then return non NULL handler
	if(xQueue != NULL)
	{
		// create two tasks that write data to the queue 
		xTaskCreate( vSenderTask1, "Sender1", 1000, ( void * ) 100, 2, NULL );
		xTaskCreate( vSenderTask2, "Sender2", 1000, ( void * ) 200, 2, NULL );
		// create the the task that receive data from the queue
		xTaskCreate( vReceiverTask, "Receiver", 1000, NULL, 3, NULL );
		// start the scheduler and the tasks;
		vTaskStartScheduler();
		
	}
	else
	{
		
	}
	// it will not reach here 
	for(;;);
	
	
}

static void vSenderTask1( void *pvParameters )
{
		// declare the value that will be sent to the tasks; 
		long lValueToSend;
		portBASE_TYPE xStatus;
		// covert the argument to long type;
		lValueToSend = ( long ) pvParameters;
		for(;;)
		{
			xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0 );
			if( xStatus != pdPASS )
			{
				//vPrintString( "Could not send to the queue.\r\n" );
				printf("Sender Task1 could not send to the queue\r\n");
				//LCD_ShowString(30,70,200,16,16,"COULD not send to queue");
				
			}
			else
				printf("Sender Task1 send 100 successfully\r\n");
			vTaskDelay(50);
			// inform the schecduler to switch the tasks;
			taskYIELD();
			
		}
	
}

static void vSenderTask2( void *pvParameters )
{
		// declare the value that will be sent to the tasks; 
		long lValueToSend;
		portBASE_TYPE xStatus;
		// covert the argument to long type;
		lValueToSend = ( long ) pvParameters;
		for(;;)
		{
			xStatus = xQueueSendToBack( xQueue, &lValueToSend, 0 );
			if( xStatus != pdPASS )
			{
				//vPrintString( "Could not send to the queue.\r\n" );
				printf("Sender Task2 could not send to the queue\r\n");
				//LCD_ShowString(30,150,200,16,16,"COULD not send to queue");
			}
			else
				printf("Sender Task2 send 200 successfully\r\n");
			vTaskDelay(50);
			// inform the schecduler to switch the tasks;
			taskYIELD();
			
		}
	
}

static void vReceiverTask( void *pvParameters )
{
		// declare the receiver variables 
		long lReceivedValue;
		portBASE_TYPE xStatus;
		const portTickType xTicksToWait = 100 / portTICK_RATE_MS;
		for(;;)
		{
			if( uxQueueMessagesWaiting( xQueue ) == 0 )
				{
				//vPrintString( "Queue should have been empty!\r\n" );
					printf("The queue has been empty\r\n");
					//LCD_ShowString(30,90,200,16,16,"Queue has been empty");
				}
			xStatus = xQueueReceive( xQueue, &lReceivedValue, xTicksToWait );
			if( xStatus == pdPASS )
			{
				// if the status is pass then display the data 
				//vPrintStringAndNumber( "Received = ", lReceivedValue );
				printf("the received value is %ld \r\n",lReceivedValue);
				//LCD_ShowString(30,110,200,16,16,"Received data is:");
				//LCD_ShowNum(30,130,lReceivedValue,16,16);
				
			}
			else
			{
				// wait 100m then receive nothing
				//LCD_ShowString(30,170,200,16,16,"Could not receive data ");
			}
		}
	
}





/*
//任务优先级

#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

// define the priority of LED1
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//LED1 Handler 
TaskHandle_t LED1Task_Handler;
//
void led1_task(void *pvParameters);

// define the priority of LCD which could control the LED 
//and the background colors of LCD
#define LCD1_TASK_PRIO		3
#define LCD1_STK_SIZE		128
TaskHandle_t LCD1Task_Handler;
void lcd1Task(void *pvParameter);

// this task is used to control the LED1 and update the 
// LCD background colors, THIS TASK WILL be delete by vTaskDelete() 
#define LCD2_TASK_PRIO		4
#define LCD2_STK_SIZE		128
TaskHandle_t LCD2Task_Handler;
void lcd2Task(void *pvParameter);

void vApplicationIdleHook( void );

unsigned long ulIdleCycleCount = 0UL;

int lcd_discolor[14]={ WHITE, BLACK, BLUE,
											BRED, GRED, GBLUE, RED,
											MAGENTA, GREEN, CYAN, 
											YELLOW,BROWN, BRRED, GRAY };

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//nested vecor interrupt control	 
	delay_init();	    				//clock tick initialization	  
	uart_init(115200);					//uart_init 
	LED_Init();		  					//init the LED
	LCD_Init();								// init the LCD
	 
	POINT_COLOR = RED; 
	LCD_ShowString(30,10,200,16,16,"Create Task"); 
	LCD_ShowString(30,30,200,16,16,"Delete Tasks"); 
	LCD_ShowString(30,50,200,16,16,"Task Creat and Del"); 
	LCD_ShowString(30,70,200,16,16,"ATOM@ALIENTEK"); 
	LCD_ShowString(30,90,200,16,16,"2020/07/25");
	//start to create the start task 
	xTaskCreate((TaskFunction_t )start_task,            // create a new task and the taskcode is : start_task
                (const char*    )"start_task",          // only one name of the task and is useful to debug
                (uint16_t       )START_STK_SIZE,        // declare the stack size of the task
                (void*          )NULL,                  // arguement to the task function
                (UBaseType_t    )START_TASK_PRIO,       // define the priority of the task 
                (TaskHandle_t*  )&StartTask_Handler);   //Handler function             
  vTaskStartScheduler();          //
	
	// if all well in main(), it will never reach here
	for(;;);
}

//start task is used to create two other two tasks
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //enter the critical 
    // excute the start task to create other tasks
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //create led1 task1
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler);         
   
								
		xTaskCreate((TaskFunction_t )lcd1Task,     
                (const char*    )"lcd1_task",   
                (uint16_t       )LCD1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LCD1_TASK_PRIO,
                (TaskHandle_t*  )&LCD1Task_Handler);  
		
							
		xTaskCreate((TaskFunction_t )lcd2Task,     
                (const char*    )"lcd2_task",   
                (uint16_t       )LCD2_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LCD2_TASK_PRIO,
                (TaskHandle_t*  )&LCD2Task_Handler); 


								
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//LED0_task is used to control the 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED0=~LED0;
        vTaskDelay(500);
    }
}   

//LED1任务函数
void led1_task(void *pvParameters)
{
    u8 led1_number = 0;
		while(1)
    {	
				led1_number++;
				LCD_ShowNum(30,70,ulIdleCycleCount,16,16); 
				//LCD_ShowString(30,50,200,16,16,pcTaskGetName); 
        LED1=0;
        vTaskDelay(200);
        LED1=1;
        vTaskDelay(800);
    }
}

void lcd1Task(void *pvParameters)
{
	u8 task1_num=0; 
	POINT_COLOR = BLACK; LCD_DrawRectangle(5,110,115,314); // SET THE NUMBER 
	
	// uxPriority is used to check current Priority;
	unsigned portBASE_TYPE uxPriority;
	uxPriority = uxTaskPriorityGet(NULL);
	
	LCD_DrawLine(5,130,115,130); //??
	POINT_COLOR = BLUE; 
	LCD_ShowString(6,111,110,16,16,"Task1 Run 000"); 
	while(1) 
		{ task1_num++; //COUNT THE NUMBER UNTIL 5
			LED0=!LED0;
			printf("the number is %d\r\n",task1_num); 
			if(task1_num==100) { 
				vTaskDelete(LCD2Task_Handler);//IF COUNT UP TO 5 THEN DELETE LCD2 Handler 
				printf("Task 2 Has been deleted\r\n"); } 
			LCD_Fill(6,131,114,313,lcd_discolor[task1_num%14]); //恶?办
			LCD_ShowxNum(86,111,task1_num,3,16,0x80); //show the number of the task1_num
			
			// set the priority of current task
			vTaskPrioritySet( NULL, ( uxPriority - 2 ) );
			
			vTaskDelay(1000); // CYCLE IS 1 SECOND
			//for(;;);
	}
}

void lcd2Task(void *pvParameters)
{
	u8 task2_num=0; POINT_COLOR = BLACK; 
	// uxPriority is used to check current Priority;
	unsigned portBASE_TYPE uxPriority;
	LCD_DrawRectangle(125,110,234,314); // To draw a rectangle 
	LCD_DrawLine(125,130,234,130); //??
	POINT_COLOR = BLUE; 
	LCD_ShowString(126,111,110,16,16,"Task2 Run:000");
	portTickType  LastWakeTime;
	LastWakeTime = xTaskGetTickCount();
	
	// check the current priroty
	uxPriority = uxTaskPriorityGet(NULL);
	
	while(1) 
		{ task2_num++; //count the number for task 2, when it is up to 255 and will restart from 0;
			
			if(task2_num == 50)
				vTaskPrioritySet(LCD1Task_Handler, (uxPriority+2)); // set the priority of LCD1_Task
			
			LED1=!LED1; 
			
			if(task2_num == 15)
				vTaskDelete(LCD1Task_Handler);
			
			printf("the number of task2 %d \r\n",task2_num); 
			LCD_ShowxNum(206,111,task2_num,3,16,0x80); //display the number 
			LCD_Fill(126,131,233,313,lcd_discolor[13-task2_num%14]); //恶?办
			//for(;;); // if use this delay, and this task has the highest priority then it will stay here forever;
			vTaskDelay(1000); //┑? 1s碞琌 1000????╃
			//vTaskDelayUntil(&LastWakeTime, 1000);
		}
	
}

*/
