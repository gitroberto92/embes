#ifndef __WAVE_H
#define __WAVE_H
#include "stm32f4xx_hal.h"
#include "fatfs.h"

//RIFF块
typedef __packed struct
{
	uint32_t ChunkID;		   		//RIFF 청크 ID : 0x46464952
	uint32_t ChunkSize ;		  //청크 크기 = 파일총크기 - 8
	uint32_t Format;	   			//WAVE 형식 : 0x45564157
}ChunkRIFF ;
//fmt块
typedef __packed struct
{
	uint32_t ChunkID;		   		//chunk id;여기서는 'fmt'로 고정,0x20746D66
	uint32_t ChunkSize ;		  //자식청크의 크기(ID와 Size는 포함하지 않는다);여기서는 20.
	uint16_t AudioFormat;	  	//음성형식;0x01 : 선형PCM; 0x11 : IMA ADPCM
	uint16_t NumOfChannels;		//통로수;1,모노;2,스테레오;
	uint32_t SampleRate;			//표본화주파수;
	uint32_t ByteRate;				//바이트 전송률; 
	uint16_t BlockAlign;			//블록크기(비트수); 
	uint16_t BitsPerSample;		//단일표본자료의 크기;4비트 ADPCM의 경우 4로 설정
	uint16_t ByteExtraData;		//부가적자료의 바이트수;2바이트; 선형PCM,이 속성값이 없다
}ChunkFMT;	   
//fact청크
typedef __packed struct 
{
	uint32_t ChunkID;		   		//chunk id;여기서는 "fact"로 고정(0x74636166);선형PCM은 이 부분이 없다
	uint32_t ChunkSize ;		  //자식청크 크기(ID와Size는 포함하지 않는다);여기서는 4.
	uint32_t FactSize;	  		//PCM으로 변환했을때의 파일크기
}ChunkFACT;
//LIST청크
typedef __packed struct 
{
	uint32_t ChunkID;		   		//chunk id;"LIST" : 0x74636166;
	uint32_t ChunkSize ;		  //자식청크 크기(ID와Size는 포함하지 않는다);여기서는:4. 
}ChunkLIST;

//data청크
typedef __packed struct 
{
	uint32_t ChunkID;		   		//chunk id;这里固定为"data",即0X5453494C
	uint32_t ChunkSize ;		  //子集合大小(不包括ID和Size) 
}ChunkDATA;

//wav头
typedef __packed struct
{ 
	ChunkRIFF riff;						//riff청크
	ChunkFMT fmt;  						//fmt청크
	ChunkFACT fact;						//fact청크 线性PCM,没有这个结构体	 
	ChunkDATA data;						//data청크		 
}__WaveHeader; 

//wav 播放控制结构体
typedef __packed struct
{ 
	uint16_t audioformat;			//음성형식;0x01,선형PCM;0x11 IMA ADPCM
	uint16_t nchannels;				//통로수;1,모노;2,스테레오
	uint16_t blockalign;			//블록크기(바이트수);  
	uint32_t datasize;				//WAV자료크기
	uint32_t totsec ;					//전체재생시간,단위:초
	uint32_t cursec ;					//현재 재생시간
	uint32_t bitrate;	   			//비트률(비트속도 bps)
	uint32_t samplerate;			//표본화주파수
	uint16_t bps;							//표본화비트수,실례로16bit,24bit,32bit
	uint32_t datastart;				//자료프레임시작위치(파일내 변위)
}wavctrl; 

#define WAVEFILEBUFSIZE		2048
extern uint8_t WaveFileBuf[WAVEFILEBUFSIZE];
extern uint8_t TempBuf[WAVEFILEBUFSIZE/2];
extern wavctrl WaveCtrlData;

uint8_t wave_decode_init(char* fname,wavctrl* wavx);
void ScanWavefiles (char* path);
uint8_t PlayWaveFile(char* path);
uint32_t FillWaveFileBuf(uint8_t *Buf,uint16_t Size);

#endif
