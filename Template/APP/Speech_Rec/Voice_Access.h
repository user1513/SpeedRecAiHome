#ifndef _VOICE_ACCESS_H
#define _VOICE_ACCESS_H

#include "wavplay.h"
/*注意:
语音识别只能在通道数为1
保存格式为PCM下才可以实现,
PCM内部保存数据为base64编码后的数据
否则就是普通录音文件
*/
#define voiceNumOfChannels 1/*定义录音通道数*/

#define SAVE_TYPE_PCM   0   /*保存格式为PCM*/

#define SAVE_TYPE_WAV   1   /*保存格式为MAV*/

#define config_ENABLE_MODE SAVE_TYPE_PCM    /*使用模式*/

#define I2S_RX_DMA_BUF_SIZE    	3000		//定义RX DMA 数组大小

void rec_i2s_dma_rx_callback(void);
void recoder_enter_rec_mode(void);
void recoder_enter_play_mode(void);
#if config_ENABLE_MODE == SAVE_TYPE_WAV
void recoder_wav_init(__WaveHeader* wavhead);
void recoder_msg_show(u32 tsec,u32 kbps);
void recoder_remindmsg_show(u8 mode);
void recoder_new_pathname(u8 *pname); 
void wav_recorder(void);
#else
u8 voice_Receive(void);
void voice_End_Receive(void);
void voice_info_Send(void);
uint32_t voice_info_size(void);
#endif
#endif

