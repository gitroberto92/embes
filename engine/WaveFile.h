
/** 
   
   Copyright (c) 2011 PSJDC.
   
   @file WaveFile.h
   
   @date 2011-4-1
   
   @author LMC(mizilmc@hotmail.com) 
   
   @brief
   
    음성파일을 처리하기위한 클라스를 선언한다.
*/

#pragma once
#include <string.h>
#ifdef WIN32
#include <memory.h>
#else
#include <memory>
#endif
#include "SDFileIO.h"

typedef unsigned short _WORD;
typedef unsigned int _UINT;

#ifndef MAX_PATH
#define MAX_PATH 100
#endif 

enum WF_ErrorCode
{
	WF_ERR_NOERROR = 0,
	WF_ERR_FILE_OPEN,
	WF_ERR_FORMAT,
	WF_ERR_DATA_CHUNK,
	WF_ERR_UNSUPPORT_FORMAT,
	WF_ERR_MAPPING,
	WF_ERR_WRITE,
	WF_ERR_UNKNOWN = -1
};


typedef struct 
{
	int FormatTag;
	int SamplingFreq;
	int Bits;
	int Channels;
	int DataOffset;
	int DataBytes;
	int BytesOneSample;
	int loopStart;
	int loopEnd;
}WAVE_FILE_INFO, *PWAVE_FILE_INFO;


struct WaveHead
{
	WaveHead()
	{
		memset(this, 0, sizeof(WaveHead));
	}

	WaveHead(unsigned int p_nBytes,  int p_nSamplingFreq, int p_nBits, int p_nChannels)
	{
		memset(this, 0, sizeof(WaveHead));
		memcpy(RIFF, "RIFF", 4);
		nRIFF = p_nBytes + 36;
		memcpy(WAVE, "WAVE", 4);
		memcpy(fmt, "fmt ", 4);
		nfmt = 16;
		wFormatTag = 1;
		channels = (_WORD)p_nChannels;
		nSamplesPerSec = p_nSamplingFreq;
		nAvgBytesPerSec = p_nSamplingFreq * p_nBits / 8 * p_nChannels;
		wBlockAlign = p_nBits / 8 * p_nChannels;
		wBitsPerSample = p_nBits;
		memcpy(data, "data", 4);
		size = p_nBytes;
	}

	char	RIFF[4];		//'RIFF'
	int		nRIFF;			//RIFF chunk 의 바이트수
	char	WAVE[4];		//'WAVE'
	char	fmt[4];			//'fmt '
	int		nfmt;			//size fmt chunk
	_WORD	wFormatTag;		//Wave format tag
	_WORD	channels;		//channels
	int		nSamplesPerSec;	//nSamplesPerSec
	int		nAvgBytesPerSec;//nAvgBytesPerSec
	_WORD	wBlockAlign;	//wBlockAlign (channel*bitspersample/8)
	_WORD	wBitsPerSample;	//wBitsPerSample
	char	data[4];		//'data'chunk
	_UINT	size;			//bytes of wave data
};

// CuePoint: each individual 'marker' in a wave file is represented by a cue point.
typedef struct {
	_UINT id;			// a unique ID for the Cue Point.
	_UINT playOrderPosition;	// Unsigned 4-byte little endian int: If a Playlist chunk is present in the Wave file, this the sample number at which this cue point will occur during playback of the entire play list as defined by the play list's order.  **Otherwise set to same as sample offset??***  Set to 0 when there is no playlist.
	_UINT dataChunkID;		// Unsigned 4-byte little endian int: The ID of the chunk containing the sample data that corresponds to this cue point.  If there is no playlist, this should be 'data'.
	_UINT chunkStart;			// Unsigned 4-byte little endian int: The byte offset into the Wave List Chunk of the chunk containing the sample that corresponds to this cue point. This is the same chunk described by the Data Chunk ID value. If no Wave List Chunk exists in the Wave file, this value is 0.
	_UINT blockStart;			// Unsigned 4-byte little endian int: The byte offset into the "data" or "slnt" Chunk to the start of the block containing the sample. The start of a block is defined as the first byte in uncompressed PCM wave data or the last byte in compressed wave data where decompression can begin to find the value of the corresponding sample value.
	_UINT frameOffset;		// Unsigned 4-byte little endian int: The offset into the block (specified by Block Start) for the sample that corresponds to the cue point.
} CuePoint;


class WaveFileWriter
{

public:
	WaveFileWriter(void);
	~WaveFileWriter(void);

	// 지적한 포맷의 음성파일 창조
	bool Create(const char* p_szFileName, int p_nSamplingFreq, int p_nBits, int p_nChannels);
	
	// 음성자료 추가
	bool AppendData(void* p_pData, int p_nBytes);

	// 음성파일 쓰기처리 끝
	bool Finish();

	bool IsOpened(){return m_fp != NULL;}

	int GetChannels()	{return (int)m_wavHead.channels;}
	int GetSamplingFreq()	{return m_wavHead.nSamplesPerSec;}
	int GetBits()	{return (int)m_wavHead.wBitsPerSample;}

private:
	bool _Create(LPFile fp, int p_nSamplingFreq, int p_nBits, int p_nChannels);

	LPFile		 m_fp;
	unsigned int m_nDataSize;
	WaveHead     m_wavHead;
	WF_ErrorCode m_ErrorCode;
	char		 m_strPath[MAX_PATH];
};

void ShortDataToFloat(short *p_psData, float **p_pfData, int p_nSamples, int p_nChannels, bool isFillStereo = false);
void FloatDataToShort(float **p_pfData, short *p_psData, int p_nSamples, int p_nChannels);
void ByteDataToFloat(unsigned char *p_pbData, float **p_pfData, int p_nSamples, int p_nChannels);
void FloatDataToByte(float **p_pfData, unsigned char *p_pbData, int p_nSamples, int p_nChannels);
void Int32DataToFloat( int *p_pbData, float **p_ppfData, int p_nSamples, int p_nChannels );
void FloatDataToFloat( float *p_pbData, float **p_ppfData, int p_nSamples, int p_nChannels );
void Bit24DataToFloat( unsigned char *p_pbData, float **p_ppfData, int p_nSamples, int p_nChannels );
void FloatDataToBit24( float **p_ppfData, unsigned char *p_pbData, int p_nSamples, int p_nChannels );


WF_ErrorCode ReadPcmWaveFileInfo(const char* p_fileName, PWAVE_FILE_INFO p_pWaveInfo);
