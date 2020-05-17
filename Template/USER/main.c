#include "includes.h"


/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
static void vTaskTaskInit(void *pvParameters);

static void vTaskLED(void *pvParameters);

static void vTaskKeyGet(void *pvParameters);

static void vTaskSpeechRec(void *pvParameters);

static void vTaskUsartParse(void *pvParameters);

static void vTaskAudioPlay(void *pvParameters);
/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskInit = NULL;

static TaskHandle_t xHandleTaskLED = NULL;

static TaskHandle_t xHandleTaskSpeechRec = NULL;

static TaskHandle_t xHandleTaskKeyGet = NULL;

static TaskHandle_t xHandleTaskUsartParse = NULL;

static TaskHandle_t xHandleTaskAudioPlay = NULL;

//******************************************************************************************************//

SemaphoreHandle_t xKeySemaphoreHandle = NULL;		/*创建按键与中断同步二值信号量*/

SemaphoreHandle_t xSpeechRecSemaphoreHandle = NULL;	/*创建按键与语音识别同步信号量*/

SemaphoreHandle_t xUsartParseSemaphoreHandle = NULL;/*创建二值信号量,专门用于串口空闲中断与串口解析任务同步*/

//QueueHandle_t xDmaModeMutexHandle  = NULL;			/*创建互斥信号量,确保dma的工作模式不会出错*/

QueueHandle_t AudioNoQueueHandle = NULL; 			/*创建队列用于存放音频播放内容*/
/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{ 
	/* 
	  在启动调度前，为了防止初始化STM32外设时有中断服务程序执行，这里禁止全局中断(除了NMI和HardFault)。
	  这样做的好处是：
	  1. 防止执行的中断服务程序中有FreeRTOS的API函数。
	  2. 保证系统正常启动，不受别的中断影响。
	  3. 关于是否关闭全局中断，大家根据自己的实际情况设置即可。
	  在移植文件port.c中的函数prvStartFirstTask中会重新开启全局中断。通过指令cpsie i开启，__set_PRIMASK(1)
	  和cpsie i是等效的。
     */
	//__set_PRIMASK(1);
	
	/*外设初始化*/
	bspInit();
	/* 创建初始任务 */
	 xTaskCreate( vTaskTaskInit,   		/* 任务函数  */
                 "vTaskTaskInit",     	/* 任务名    */
                 128,               	/* 任务栈大小，单位word，也就是4字节 */
                 NULL,              	/* 任务参数  */
                 1,                 	/* 任务优先级*/
                 &xHandleTaskInit );  	/* 任务句柄  */
	
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
	/* 
	  如果系统正常启动是不会运行到这里的，运行到这里极有可能是用于定时器任务或者空闲任务的
	  heap空间不足造成创建失败，此要加大FreeRTOSConfig.h文件中定义的heap大小：
	  #define configTOTAL_HEAP_SIZE	      ( ( size_t ) ( 17 * 1024 ) )
	*/
	while(1);
}

static void vTaskTaskInit(void *pvParameters)

{
	taskENTER_CRITICAL();           //进入临界区
	
	/*1. f_mount()注册/取消注册卷的工作区*/
	mf_mount("0:", 1);
	
	/*创建二值信号量,专门用于按键和中断同步*/
	/*首次创建二值信号量,值为0*/
	xKeySemaphoreHandle = xSemaphoreCreateBinary();	
	/*创建二值信号量,专门用于按键和语音识别任务同步*/
	xSpeechRecSemaphoreHandle = xSemaphoreCreateBinary();
	/*创建二值信号量,专门用于串口空闲中断与串口解析任务同步*/
	xUsartParseSemaphoreHandle = xSemaphoreCreateBinary();
	/*创建互斥信号量,确保dma的工作模式不会出错*/
	//xDmaModeMutexHandle = xSemaphoreCreateMutex();
	/*创建队列存放需要播放的音频编号,可以存放5个大小为4个字节的item*/
	AudioNoQueueHandle = xQueueCreate( 5, sizeof(int));

	xTaskCreate( vTaskLED,    				/* 任务函数  */
                 "vTaskLED",  				/* 任务名    */
                 128,         				/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        				/* 任务参数  */
                 5,           				/* 任务优先级*/
                 &xHandleTaskLED ); 		/* 任务句柄  */
	
	
	xTaskCreate( vTaskSpeechRec,    		/* 任务函数  */
                 "vTaskSpeechRec",  		/* 任务名    */
                 256,         				/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        				/* 任务参数  */
                 3,           				/* 任务优先级*/
                 &xHandleTaskSpeechRec ); 	/* 任务句柄  */
	
	xTaskCreate( vTaskKeyGet,    			/* 任务函数  */
                 "vTaskKeyGet",  			/* 任务名    */
                 64,         				/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        				/* 任务参数  */
                 6,           				/* 任务优先级*/
                 &xHandleTaskKeyGet ); 		/* 任务句柄  */
				 
	xTaskCreate( vTaskUsartParse,    		/* 任务函数  */
                 "vTaskUsartParse",  		/* 任务名    */
                 128,         				/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        				/* 任务参数  */
                 7,           				/* 任务优先级*/
                 &xHandleTaskUsartParse ); 	/* 任务句柄  */
				 
	xTaskCreate( vTaskAudioPlay,    		/* 任务函数  */
                 "vTaskAudioPlay",  		/* 任务名    */
                 128,         				/* 任务栈大小，单位word，也就是4字节 */
                 NULL,        				/* 任务参数  */
                 2,           				/* 任务优先级*/
                 &xHandleTaskAudioPlay ); 	/* 任务句柄  */
	
    vTaskDelete(xHandleTaskInit); 			//删除开始任务
	
    taskEXIT_CRITICAL();            		//退出临界区
}

extern uint8_t LastEspStatue ;	/*外部引用*/

uint8_t speech_rec_statue = 0 ;	
/*语音识别任务*/
static void vTaskSpeechRec(void *pvParameters)

{
//	SpeechRecUartPack();
	static uint8_t SpeechRecNum = 0;
	uint8_t queueval = 1;
	while(1)
	{
		xSemaphoreTake(xSpeechRecSemaphoreHandle, portMAX_DELAY);		/*等待信号量*/
		
		printf("go to TaskSpeechRec %d\n", (int)SpeechRecNum);
		
		if(!LastEspStatue)
		{
			speech_rec_statue = 1;
			
			Speech_Handle(SpeechRecNum++ % 2);							/*运行语音识别函数*/
			
			if(speech_rec_statue && (!(SpeechRecNum % 2)))
			{
				speech_rec_statue = 0;
			}
		}
			
		else
		{
			queueval = 1;
			xQueueSend(AudioNoQueueHandle, (void*)&queueval, portMAX_DELAY);/*联网失败*/
		}
	}
}

/*按键信息获取*/
static void vTaskKeyGet(void *pvParameters)

{
	while(1)
	{
		xSemaphoreTake(xKeySemaphoreHandle, portMAX_DELAY);		/*等待按键中断发送信号量*/
		printf("go to vTaskKeyGet \n");
		vTaskDelay(500);

		if((KEY == 0) || (KEY1 == 0))
		{
			xSemaphoreGive(xSpeechRecSemaphoreHandle);			/*启动语音识别任务*/
			printf("go to vTaskKeyGet ---->>> ok\n");
		}
		ENABLE_EXIT_LINE(EXTI_Line15);
		ENABLE_EXIT_LINE(EXTI_Line10);
	}
}

/*串口数据解析*/
static void vTaskUsartParse(void *pvParameters)

{
	while(1)
	{
	xSemaphoreTake(xUsartParseSemaphoreHandle, portMAX_DELAY);		/*等待按键中断发送信号量*/
		
	usart_parse(read_current_finaliy_point());
		
	}
}
//e6 89 93 e5 bc 80 e5 8d a7 e5 ae a4 e7 81 af 2c e5 85 b3 e9 97 ad e5 ae a2 e5 8e 85 e7 a9 ba e8 b0 83

char sprintf_str[30] = {0};	

static void vTaskAudioPlay(void *pvParameters)
{
	uint32_t ulMessage = 0;
	uint8_t ucTmp = 0;
	while(1)
	{
		xQueueReceive(AudioNoQueueHandle,&ulMessage, portMAX_DELAY);/*获取音频播放队列*/
		

		ucTmp = ulMessage/1000;
		if((ulMessage - (ucTmp * 1000)) < 10 && ulMessage > 10)							/*判断是否是从MQTT接收的指令*/
		{
			if(ulMessage % 2)
				bsp_pcf8974x_bit_set(0, ucTmp + 3,0);
			else
				bsp_pcf8974x_bit_set(0, ucTmp + 3,1);
		}
		else
		{
			if(ulMessage == 1011)/*卧室灯*/
				bsp_pcf8974x_bit_set(0, 4 + 3,0);
			if(ulMessage == 1012)/*卧室灯*/
				bsp_pcf8974x_bit_set(0, 4 + 3,1);
			if(ulMessage == 2011)/*客厅灯*/
				bsp_pcf8974x_bit_set(0, 2 + 3,0);
			if(ulMessage == 2012)/*客厅灯*/
				bsp_pcf8974x_bit_set(0, 2 + 3,1);
			if(ulMessage == 4011)/*书房灯*/
				bsp_pcf8974x_bit_set(0, 3 + 3,0);
			if(ulMessage == 4012)/*书房灯*/
				bsp_pcf8974x_bit_set(0, 3 + 3,1);
			if(ulMessage == 5011)/*厨房灯*/
				bsp_pcf8974x_bit_set(0, 1 + 3,1);
			if(ulMessage == 5012)/*厨房灯*/
				bsp_pcf8974x_bit_set(0, 1 + 3,1);
			
			sprintf(sprintf_str,"0:AudioPlay/%04d.wav",ulMessage);		/*将接收到的音频名进行整合*/
	
			recoder_enter_play_mode();								/*进入播放模式*/
			
			wav_play_song((u8*)sprintf_str);						/*播放音频*/
			
			vTaskDelay(500);
		}
		
	}
}



/*
*********************************************************************************************************
*	函 数 名: vTaskLED
*	功能说明: LED闪烁	
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
uint8_t timeout = 0;
uint8_t count = 0;
double DHTxx_Tab[2];
static void vTaskLED(void *pvParameters)

{
    while(1)
    {
		bspLedToggle();

		uint8_t pcf8974x_state = bsp_pcf8974x_receive(0);
		
		bsp_pcf8974x_test(0, (pcf8974x_state&0xf0) + (count & 0x0f));

		count++;
		
		if((!(speech_rec_statue)) && (!LastEspStatue))
		{
			if(timeout++ > 10)				/*每5s上传一次数据*/
			{
				clear_screen();				/*oled清屏*/
				
				DHTxx_Get_Data(DHTxx_Tab);	/*获取dhtxx数据*/
				
				uint16_t bh1750fvi_val = bh1750fvi_send_measure(BH1750FVI_H_RESOLUTION);/*获取bh1750fvi数据*/
				
				char str[30];
				
				lx_Gb2312g_Str("  [传感器INFO]  ", 1, 0, 1);
				
				sprintf(str,"温度:%2.1f℃",DHTxx_Tab[1]);
				
				lx_Gb2312g_Str(str, 3, 0, 0);
				
				sprintf(str,"湿度:%2.1f%%",DHTxx_Tab[0]);
				
				lx_Gb2312g_Str(str, 5, 0, 0);
				
				sprintf(str,"光照强度:%dlx",bh1750fvi_val);
				
				lx_Gb2312g_Str(str, 7, 0, 0);
				
				sprintf(&str[7], "%2.1f,%2.1f,%d,%d",DHTxx_Tab[1] , DHTxx_Tab[0] , bh1750fvi_val, count + 100);
				
				printf("%s\n", &str[7]);
				
				uint8_t length = strlen(&str[7]);
				
				UartDataPacking(str, 6, length, 0);
				
				usartSendStart((uint8_t*)str, 9 + length);/*数据长度+数据包结构*/
				
				timeout = 0;
				//GetEspInfo();
				
			}
		}
		else
		{
			timeout = 0;
		}
        vTaskDelay(500);		
    }
}






