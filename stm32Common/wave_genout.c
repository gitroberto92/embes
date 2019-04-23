/* WM8978 모듈은 http://51maxgear.taobao.com 에서 얻을수 있다，
본 소프트의 저작권은 http://51maxgear.taobao.com 에속하며，수정, 배포는 허락하지만 상기 정보를 남겨주십시오.*/
#include "wave_genout.h"
#include "string.h"
#include "wm8978.h"
#include "math.h"
#include "stdbool.h"
#include "engine_main.h"
#include "mpr121.h"

#include "main.h"

#include "fatfs.h"


//#define WAVEFILEBUFSIZE (AUDIO_FRAME_LEN * AUDIO_CHANNELS * 4)
#define WAVEFILEBUFSIZE 8192
__align(8) uint8_t WaveFileBuf[WAVEFILEBUFSIZE];
__IO uint8_t FillBufFlag;								//0:fill first half buf;1:fill second half buf;0xff do nothing



#define SAMPLE_BITS 16
#define SAMPLE_RATE I2S_AUDIOFREQ_44K
#define CHANNELS    2


#define MIN_VOLUMN_PRESSURE 700
#define MAX_VOLUMN_PRESSURE 4000

uint32_t FillWaveDataBuf(void *Buf);

uint8_t settingMode = 0;

uint32_t FillWaveDataBuf(void *Buf)
{
	main_loop();
	memcpy(Buf, g_outBuff, WAVEFILEBUFSIZE / 2);
	return WAVEFILEBUFSIZE;
}

void handlePressureEvent(){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 100);
	int ret = HAL_ADC_GetValue(&hadc1);
	float vol = 1.f * (ret - MIN_VOLUMN_PRESSURE) / (MAX_VOLUMN_PRESSURE - MIN_VOLUMN_PRESSURE);
	setVolumn(vol);
}

void PlayWaveData()
{

#ifdef __DEBUG
	printf("start play\r\n");
#endif
	// 초기화
	if(main_init() == false){
#ifdef __DEBUG
		printf("main init failed\r\n");
#endif
		initSuccess  = 1;
	}
	
	FillWaveDataBuf(WaveFileBuf);
	FillWaveDataBuf(&WaveFileBuf[WAVEFILEBUFSIZE/2]);
	HAL_I2S_Transmit_DMA(&hi2s2, (uint16_t *)WaveFileBuf, WAVEFILEBUFSIZE / 2);//size는 표본수이며，바이트수가 아니다，DMA표본은 16비트
#ifdef __DEBUG
	printf("init finish\r\n");
#endif
	
	// 초기화가 제대로 진행되였는지를 표시한다.
	showInitStatus();
	
	// 기본순환
	while(1)
	{
		handleKeyEvent();
		handlePressureEvent();
		
		if (FillBufFlag == 0)
		{
			FillWaveDataBuf(WaveFileBuf);
			FillBufFlag = 0xFF;
		}
		else if (FillBufFlag == 1)
		{
			FillWaveDataBuf(&WaveFileBuf[WAVEFILEBUFSIZE / 2]);
			FillBufFlag = 0xFF;
		}
	}

}

void HAL_I2S_TxHalfCpltCallback(I2S_HandleTypeDef *hi2s)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hi2s);
	/* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_TxHalfCpltCallback could be implemented in the user file
	*/
	FillBufFlag=0;

}

void HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	/* Prevent unused argument(s) compilation warning */
	UNUSED(hi2s);
	/* NOTE : This function Should not be modified, when the callback is needed,
            the HAL_I2S_TxCpltCallback could be implemented in the user file
	*/
	FillBufFlag=1;
}

