#include "bsp.h"

void bspInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4
	
    delay_init(168);		        /*初始化延时函数*/

	bspLedInit();		            /*	*/

	bspKeyInit();					/*初始化按键端口*/
	
	bspExitInit();					/*初始化按键对于的中断*/
	
	/*使用EventRecorder的printf需要屏蔽usart.c中宏定义*/
//	EventRecorderInitialize(EventRecordAll, 1U);/*事件记录器初始化*/

//	EventRecorderStart();           /*事件记录器启动*/

//    TIM3_Int_Init(9,8399);          /*timer3 = 84Mhz, 设定溢出时间为1ms*/

	uart_init(460800);				/*串口0初始化*/
	
    vDhtxx_Init();                	/*DHT22初始化函数*/
	
	//OLED_Init();					/*OLED初始化函数*/

	//bspBuzzerInit();				/*蜂鸣器初始化*/

	//bspBuzzerFreq(50, 1500);		/*蜂鸣器跳变周期100ms,总响应时间为5000ms*/

	//bsp_sgp30_io_init();			/*sgp30传感器外设初始化*/

	//bsp_adc_init();				/*adc1通道0初始化*/

	//TIM2_PWM_Init(999, 839);		/*PWM周期初始化*/

	//TIM_SetCompare2(TIM2, 32);	/*PWM占空比初始化*/

	//bsp_pcf8974x_io_init();		/*pcf8974外设初始化*/

	usmart_init(84);				/*初始化usmart*/
	
	WM8978_Init();				//初始化WM8978
	WM8978_HPvol_Set(63,63);	//耳机音量设置
	WM8978_SPKvol_Set(63);		//喇叭音量设置

	exfuns_init();					/*fatfs变量声明*/
	
	while(SD_Init())//检测不到SD卡	/*SD卡初始化*/
	{
		printf("SD Card Error!\n");
		delay_ms(500);					
		printf("Please Check!\n");
		delay_ms(500);
	}
	
	bspUsartInit(460800);			/*串口初始化*/
	
	bspDmaUsartInit();				/*DMA初始化*/

	//bsp_encoder_init();			/*定时器编码器模式配置*/
	
	bsp_pcf8974x_io_init();			/*pcf8974io初始化*/
	
	bsp_bh1750fvi_io_init();		/*光照度传感器初始化*/
	
	SPI1_Init();					/*SPI1初始化*/
	
	initial_lcd();					/*LCD 模块初始化*/
	
	bsp_pcf8974x_test(0,0xff);		/*设定led全灭*/
	
	clear_screen();					/*清屏*/
	
	lx_Gb2312g_Str("<基于语音控制的>", 1, 0, 0);
	
	lx_Gb2312g_Str("智能家居系统设计", 3, 0, 0);
	
	lx_Gb2312g_Str("等待设备联网成功", 5, 0, 0);
	
	lx_Gb2312g_Str("    制作者：小袁", 7, 0, 0);
	
	printf("程序开始!!!\n");

}

