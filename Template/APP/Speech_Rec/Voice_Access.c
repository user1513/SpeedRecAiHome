#include "Voice_Access.h"
#include "audioplay.h"
#include "ff.h"
#include "usart.h"
#include "bsp_wm8978.h"
#include "bsp_i2s.h"
#include "delay.h"
#include "bsp_key.h"
#include "exfuns.h"  
#include "string.h"  
#include "fattester.h"	 
#include "stdlib.h"
#include "base64.h"
#include "FreeRTOS.h"
u8 *i2srecbuf1;
u8 *i2srecbuf2; 

#if config_ENABLE_MODE == SAVE_TYPE_MAV
#endif
FIL* f_rec=0;		//录音文件	
u32 wavsize;		//wav数据大小(字节数,不包括文件头!!)
u8 rec_sta=0;		//录音状态
					//[7]:0,没有开启录音;1,已经开启录音;
					//[6:1]:保留
					//[0]:0,正在录音;1,暂停录音;


//录音 I2S_DMA接收中断服务函数.在中断里面写入数据
void rec_i2s_dma_rx_callback(void) 
{    
	u16 bw;
	u8 res;
	#if config_ENABLE_MODE == SAVE_TYPE_PCM
    unsigned int length = 0;
	unsigned char* cpTmp;
	#endif
	if(rec_sta==0X80)//录音模式
	{  
		if(DMA1_Stream3->CR&(1<<19))
		{	
			#if voiceNumOfChannels == 1
			for(int i=2;i<I2S_RX_DMA_BUF_SIZE/2;i++)
		    {
			 i2srecbuf1[i]=i2srecbuf1[i*2];//i2srecbuf1=i2srecbuf1[i*2];
			 i2srecbuf1[i+1] = i2srecbuf1[i*2+1];//
			  i++;
		     }
                #if config_ENABLE_MODE == SAVE_TYPE_PCM

                cpTmp = base64_array_encoding(I2S_RX_DMA_BUF_SIZE/2, i2srecbuf1, &length);
                
                res=f_write(f_rec,cpTmp,length,(UINT*)&bw);//写入文件
            
                free(cpTmp);

                #else
                
                res=f_write(f_rec,i2srecbuf1,I2S_RX_DMA_BUF_SIZE/2,(UINT*)&bw);//写入文件
                
                #endif
		    #else

			res=f_write(f_rec,i2srecbuf1,I2S_RX_DMA_BUF_SIZE,(UINT*)&bw);//写入文件
		    
            #endif
			if(res)
			{
				printf("write error:%d\r\n",res);
			}
			 
		}else 
		{
			#if voiceNumOfChannels == 1

			for(int i=2;i<I2S_RX_DMA_BUF_SIZE/2;i++)
		    {
			 i2srecbuf2[i]=i2srecbuf2[i*2];//i2srecbuf1=i2srecbuf1[i*2];

			 i2srecbuf2[i+1] = i2srecbuf2[i*2+1];//
			  i++;
		    }
                #if config_ENABLE_MODE == SAVE_TYPE_PCM

                cpTmp = base64_array_encoding(I2S_RX_DMA_BUF_SIZE/2, i2srecbuf2, &length);
                
                res=f_write(f_rec,cpTmp,length,(UINT*)&bw);//写入文件
            
                free(cpTmp);

                #else

                res=f_write(f_rec,i2srecbuf2,I2S_RX_DMA_BUF_SIZE/2,(UINT*)&bw);//写入文件
            
                #endif
            
            #else
			res=f_write(f_rec,i2srecbuf2,I2S_RX_DMA_BUF_SIZE,(UINT*)&bw);//写入文件
		    #endif
			if(res)
			{
				printf("write error:%d\r\n",res);
			}
		}
		#if voiceNumOfChannels == 1
            #if config_ENABLE_MODE == SAVE_TYPE_PCM
                wavsize += length;
            #else
                wavsize += I2S_RX_DMA_BUF_SIZE/2;
            #endif
		#else
		    wavsize+=I2S_RX_DMA_BUF_SIZE;
		#endif
	} 
}  
const u16 i2splaybuf[2]={0X0000,0X0000};//2个16位数据,用于录音时I2S Master发送.循环发送0.
//进入PCM 录音模式 		  
void recoder_enter_rec_mode(void)
{
	WM8978_ADDA_Cfg(0,1);		//开启ADC
	WM8978_Input_Cfg(1,1,0);	//开启输入通道(MIC&LINE IN)
	WM8978_Output_Cfg(0,1);		//开启BYPASS输出 
	WM8978_MIC_Gain(46);		//MIC增益设置 
	
	WM8978_I2S_Cfg(2,0);		//飞利浦标准,16位数据长度
	I2S2_Init(I2S_Standard_Phillips,I2S_Mode_MasterTx,I2S_CPOL_Low,I2S_DataFormat_16b);			//飞利浦标准,主机发送,时钟低电平有效,16位帧长度 
	I2S2ext_Init(I2S_Standard_Phillips,I2S_Mode_SlaveRx,I2S_CPOL_Low,I2S_DataFormat_16b);		//飞利浦标准,从机接收,时钟低电平有效,16位帧长度	
	I2S2_SampleRate_Set(16000);	//设置采样率 
 	I2S2_TX_DMA_Init((u8*)&i2splaybuf[0],(u8*)&i2splaybuf[1],1); 		//配置TX DMA 
	DMA1_Stream4->CR&=~(1<<4);	//关闭传输完成中断(这里不用中断送数据) 
	I2S2ext_RX_DMA_Init(i2srecbuf1,i2srecbuf2,I2S_RX_DMA_BUF_SIZE/2); 	//配置RX DMA
  	i2s_rx_callback=rec_i2s_dma_rx_callback;//回调函数指wav_i2s_dma_callback
 	I2S_Play_Start();	//开始I2S数据发送(主机)
	I2S_Rec_Start(); 	//开始I2S数据接收(从机)
	//recoder_remindmsg_show(0);
}  
//进入PCM 放音模式 		  
void recoder_enter_play_mode(void)
{
	WM8978_ADDA_Cfg(1,0);		//开启DAC 
	WM8978_Input_Cfg(0,0,0);	//关闭输入通道(MIC&LINE IN)
	WM8978_Output_Cfg(1,0);		//开启DAC输出 
	WM8978_MIC_Gain(0);			//MIC增益设置为0 
	I2S_Play_Stop();			//停止时钟发送
	I2S_Rec_Stop(); 			//停止录音
	//recoder_remindmsg_show(1);
}

#if config_ENABLE_MODE == SAVE_TYPE_WAV
//初始化WAV头.
void recoder_wav_init(__WaveHeader* wavhead) //初始化WAV头			   
{
	wavhead->riff.ChunkID=0X46464952;	//"RIFF"
	wavhead->riff.ChunkSize=0;			//还未确定,最后需要计算
	wavhead->riff.Format=0X45564157; 	//"WAVE"
	wavhead->fmt.ChunkID=0X20746D66; 	//"fmt "
	wavhead->fmt.ChunkSize=16; 			//大小为16个字节
	wavhead->fmt.AudioFormat=0X01; 		//0X01,表示PCM;0X01,表示IMA ADPCM
	wavhead->fmt.SampleRate=16000;		//16Khz采样率 采样速率
	#if voiceNumOfChannels == 1
	wavhead->fmt.NumOfChannels=1;		//双声道
	wavhead->fmt.ByteRate=wavhead->fmt.SampleRate*2;//字节速率=采样率*通道数*(ADC位数/8)
 	wavhead->fmt.BlockAlign=2;			//块大小=通道数*(ADC位数/8)
	#else 
	wavhead->fmt.NumOfChannels=2;		//双声道
	wavhead->fmt.ByteRate=wavhead->fmt.SampleRate*4;//字节速率=采样率*通道数*(ADC位数/8)
 	wavhead->fmt.BlockAlign=4;			//块大小=通道数*(ADC位数/8)
	#endif
 	wavhead->fmt.BitsPerSample=16;		//16位PCM
   	wavhead->data.ChunkID=0X61746164;	//"data"
 	wavhead->data.ChunkSize=0;			//数据大小,还需要计算  
} 
	
//通过时间获取文件名
//仅限在SD卡保存,不支持FLASH DISK保存
//组合成:形如"0:RECORDER/REC20120321210633.wav"的文件名
void recoder_new_pathname(u8 *pname)
{	 
	u8 res;					 
	u16 index=0;
	while(index<0XFFFF)
	{
		sprintf((char*)pname,"0:RECORDER/REC%05d.wav",index);
		res=f_open(ftemp,(const TCHAR*)pname,FA_READ);//尝试打开这个文件
		if(res==FR_NO_FILE)break;		//该文件名不存在=正是我们需要的.
		index++;
	}
} 
#endif

#if config_ENABLE_MODE == SAVE_TYPE_WAV  
//WAV录音 
void wav_recorder(void)
{ 
	u8 res;
	u8 key;
	u8 rval=0;

 	DIR recdir;	 					//目录  
  	while(f_opendir(&recdir,"0:/RECORDER"))//打开录音文件夹
 	{	 
		delay_ms(200);				       
		delay_ms(200);				  
		f_mkdir("0:/RECORDER");				//创建该目录   
	}   
	i2srecbuf1=malloc(I2S_RX_DMA_BUF_SIZE);//I2S录音内存1申请
	i2srecbuf2=malloc(I2S_RX_DMA_BUF_SIZE);//I2S录音内存2申请  
  	f_rec=(FIL *)malloc(sizeof(FIL));		//开辟FIL字节的内存区域  
    __WaveHeader *wavhead=0; 
 	u8 *pname=0;
 	wavhead=(__WaveHeader*)malloc(sizeof(__WaveHeader));//开辟__WaveHeader字节的内存区域 
    pname=malloc(30);						//申请30个字节内存,类似"0:RECORDER/REC00001.wav" 
	pname[0]=0;					//pname没有任何文件名
    if(!i2srecbuf1||!i2srecbuf2||!f_rec)rval=1; 	if(rval==0)		
	{
		recoder_enter_rec_mode();	//进入录音模式,此时耳机可以听到咪头采集到的音频    
 	   	while(rval==0)	
		{
			key=uasrt_key_get();
			switch(key)
			{		
				case KEY2_PRES:	//STOP&SAVE
					if(rec_sta&0X80)//有录音
					{
						rec_sta=0;	//关闭录音
                            wavhead->riff.ChunkSize=wavsize+36;		//整个文件的大小-8;
                            wavhead->data.ChunkSize=wavsize;		//数据大小
                            f_lseek(f_rec,0);						//偏移到文件头.
                            f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//写入头数据
                        f_close(f_rec);
						wavsize=0;
					}
					rec_sta=0;
						break;	 
				case KEY0_PRES:	//REC/PAUSE
						recoder_new_pathname(pname);			//得到新的名字
				 		recoder_wav_init(wavhead);				//初始化wav数据	
	 					res=f_open(f_rec,(const TCHAR*)pname, FA_CREATE_ALWAYS | FA_WRITE); 
                        if(res)			//文件创建失败
						{
							rec_sta=0;	//创建文件失败,不能录音
							rval=0XFE;	//提示是否存在SD卡
						}else 
						{
							res=f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//写入头数据
 							rec_sta|=0X80;	//开始录音	 
						} 
					break;  
				case WKUP_PRES:	//播放最近一段录音
					if(rec_sta!=0X80)//没有在录音
					{   	 		 				  
						if(pname[0])//如果触摸按键被按下,且pname不为空
						{				 
							recoder_enter_play_mode();	//进入播放模式
							audio_play_song(pname);		//播放pname
							recoder_enter_rec_mode();	//重新进入录音模式 
						}
					}
					break;
			case 5:	//打印文件数据
					mf_open(pname, FA_READ| FA_OPEN_ALWAYS);
			        printf("filesize%d\n", mf_size());
					mf_read_type("%02X ",mf_size());
					printf("filesize%d\n", mf_size());
					mf_read_type("%c",mf_size());
					break;
			} 
			delay_ms(5);

		}		 
	}    
	free(i2srecbuf1);	//释放内存
	free(i2srecbuf2);	//释放内存  
	free(f_rec);		//释放内存
	free(wavhead);		//释放内存  
	free(pname);		//释放内存 
}

#else

//WAV录音 
u8 voice_Receive(void)
{ 

	u8 res;
	u8 rval=0;

	i2srecbuf1=pvPortMalloc(I2S_RX_DMA_BUF_SIZE);//I2S录音内存1申请
	i2srecbuf2=pvPortMalloc(I2S_RX_DMA_BUF_SIZE);//I2S录音内存2申请  
  	f_rec=(FIL *)pvPortMalloc(sizeof(FIL));		//开辟FIL字节的内存区域  
    if(!i2srecbuf1||!i2srecbuf2||!f_rec)rval=1; 	
    if(rval==0)		
	{
		recoder_enter_rec_mode();	//进入录音模式,此时耳机可以听到咪头采集到的音频    

        res=f_open(f_rec,(const TCHAR*)"Speech_Rec.pcm", FA_CREATE_ALWAYS | FA_WRITE);
  
        if(res)			//文件创建失败
        {
            rec_sta=0;	//创建文件失败,不能录音
            rval=0XFE;	//提示是否存在SD卡
        }else 
        {
            #if config_ENABLE_MODE == SAVE_TYPE_WAV 
            res=f_write(f_rec,(const void*)wavhead,sizeof(__WaveHeader),&bw);//写入头数据
            #endif
            rec_sta|=0X80;	//开始录音	 
        } 
				 
	}
    else
    {
		printf("pvPortMalloc fail\n");
        vPortFree(i2srecbuf1);	//释放内存
        vPortFree(i2srecbuf2);	//释放内存  
        vPortFree(f_rec);		//释放内存
    }
    return rval;
}


void voice_End_Receive(void)
{
    if(rec_sta&0X80)//有录音
    {
        rec_sta=0;	//关闭录音
        f_close(f_rec);
        wavsize=0;
    }

    recoder_enter_play_mode();	//关闭接收DMA	
	vPortFree(i2srecbuf1);	        //释放内存
	vPortFree(i2srecbuf2);	        //释放内存  
	vPortFree(f_rec);		        //释放内存
}

void voice_info_Send(void)
{
    mf_open("Speech_Rec.pcm", FA_READ| FA_OPEN_ALWAYS);
    printf("filesize%d\n", mf_size());
    mf_read_type("%c",mf_size());
}

uint32_t voice_info_size(void)
{
	uint32_t size = 0;
	
	mf_open("Speech_Rec.pcm", FA_READ| FA_OPEN_ALWAYS);
	
	size = mf_size();
	
	mf_close();
	
	return size;
}

#endif

