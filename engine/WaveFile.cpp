/** 
   
   Copyright (c) 2011 PSJDC.
   
   @file WaveFile.cpp
   
   @date 2011-4-1
   
   @author LMC(mizilmc@hotmail.com) 
   
   @brief
   
    음성파일을 처리하기위한 클라스를 구현
*/
#include "SDFileIO.h"
#include "WaveFile.h"

#define MAX_SHORT	(32767)
#define MIN_SHORT	(-32768)
#define SCALE_SHORT	(32768)
#define SCALE_SHORT_INV	(0.000030517578125f)

#define MAX_24Bit	(0xFFFFFF / 2 - 1)
#define MIN_24Bit	(-0xFFFFFF / 2)
#define SCALE_24Bit	(0xFFFFFF / 2)

#define MAX_INT32	(2147483647)
#define MIN_INT32	(-2147483648)
#define SCALE_INT32	(2147483648.0)
#define SCALE_INT32_INV	(0.0000000004656612873077392578125)


inline short float2short(float fValue)
{
	float fVal = fValue * SCALE_SHORT;

	short sVal;

	if (fVal > MAX_SHORT)
	{
		sVal = MAX_SHORT;
	}
	else if (fVal < MIN_SHORT)
	{
		sVal = MIN_SHORT;
	}
	else
	{
		sVal = (short)fVal;
	}

	return sVal;
}

inline void Bit24ToFloat(const unsigned char* pSrc, float* pDst)
{
	int x = pSrc[0] | pSrc[1] << 8 | pSrc[2] << 16;
	int y = x & 0x800000 ? x - 0x1000000 : x;
	pDst[0] = y / float(SCALE_24Bit);
}

inline void FloatToBit24(float* pSrc, unsigned char* pDst)
{
	int x = int(pSrc[0] * SCALE_24Bit);
	if (x < MIN_24Bit)
	{
		x = MIN_24Bit;
	}
	else if(x > MAX_24Bit)
	{
		x = MAX_24Bit;
	}
	x += SCALE_24Bit;

	memcpy(pDst, &x, 3);
}

void Int32DataToFloat(int *p_psData, float **p_ppfData, int p_nSamples, int p_nChannels)
{
	float* pLeft = p_ppfData[0];
	float* pRight = p_ppfData[1];
	int* pData = p_psData;
	if(p_nChannels == 1)
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			*pLeft++ = *pData++ * SCALE_INT32_INV;
		}
	}
	else
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			*pLeft++ = *pData++ * SCALE_INT32_INV;
			*pRight++ = *pData++ * SCALE_INT32_INV;
		}
	}
}


void FloatDataToFloat(float *p_psData, float **p_ppfData, int p_nSamples, int p_nChannels)
{
	float* pLeft = p_ppfData[0];
	float* pRight = p_ppfData[1];
	float* pData = p_psData;
	if(p_nChannels == 1)
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			*pLeft++ = *pData++;
		}
	}
	else
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			*pLeft++ = *pData++;
			*pRight++ = *pData++;
		}
	}
}


void ShortDataToFloat(short *p_psData, float **p_ppfData, int p_nSamples, int p_nChannels, bool isFillStereo)
{
	float* pLeft = p_ppfData[0];
	float* pRight = p_ppfData[1];
	short* pData = p_psData;
	if(p_nChannels == 1)
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			*pLeft++ = *pData++ * SCALE_SHORT_INV;
		}
		if (isFillStereo)
		{
			memcpy(p_ppfData[1], p_ppfData[0], sizeof(float) * p_nSamples);
		}
	}
	else
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			*pLeft++ = *pData++ * SCALE_SHORT_INV;
			*pRight++ = *pData++ * SCALE_SHORT_INV;
		}
	}
}

void FloatDataToShort(float **p_ppfData, short *p_psData, int p_nSamples, int p_nChannels)
{
	if(p_nChannels == 1)
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			p_psData[i] = float2short(p_ppfData[0][i]);
		}
	}
	else
	{
		float* pLeft = p_ppfData[0];
		float* pRight = p_ppfData[1];
		short* pOut = p_psData;
		for (int i = 0;i < p_nSamples;i++)
		{
			*pOut++ = float2short(*pLeft++);
			*pOut++ = float2short(*pRight++);
		}
	}
}

void ByteDataToFloat( unsigned char *p_pbData, float **p_ppfData, int p_nSamples, int p_nChannels )
{
	if(p_nChannels == 1)
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			p_ppfData[0][i] = ((int)(unsigned char)p_pbData[i] - 128) / 128.f;
		}
	}
	else
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			p_ppfData[0][i] = ((int)(unsigned char)p_pbData[i * 2 + 0] - 128) / 128.f;
			p_ppfData[1][i] = ((int)(unsigned char)p_pbData[i * 2 + 1] - 128) / 128.f;
		}
	}
}

void FloatDataToByte( float **p_ppfData, unsigned char *p_pbData, int p_nSamples, int p_nChannels )
{
	float fVal;
	if(p_nChannels == 1)
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			fVal = p_ppfData[0][i] * 128;
			fVal = fVal > 127 ? 127 : fVal;
			fVal = fVal < -128? -128 : fVal;
			p_pbData[i] = (unsigned char)(fVal + 128);
		}
	}
	else
	{
		for (int i = 0;i < p_nSamples;i++)
		{
			fVal = p_ppfData[0][i] * 128;
			fVal = fVal > 127 ? 127 : fVal;
			fVal = fVal < -128 ? -128 : fVal;
			p_pbData[i * 2 + 0] = (unsigned char)(fVal + 128);

			fVal = p_ppfData[1][i];
			fVal = fVal > 127 ? 127 : fVal;
			fVal = fVal < -128 ? -128 : fVal;
			p_pbData[i * 2 + 1] = (unsigned char)(fVal + 128);
		}
	}
}

void Bit24DataToFloat( unsigned char *p_pbData, float **p_ppfData, int p_nSamples, int p_nChannels )
{
	if(p_nChannels == 1)
	{
		float *pf = p_ppfData[0];
		for (int i = 0;i < p_nSamples;i++)
		{
			Bit24ToFloat(p_pbData, pf);
			p_pbData += 3;
			pf++;
		}
	}
	else
	{
		float *pf0 = p_ppfData[0];
		float *pf1 = p_ppfData[1];
		for (int i = 0;i < p_nSamples;i++)
		{
			Bit24ToFloat(p_pbData, pf0);
			p_pbData += 3;
			pf0++;

			Bit24ToFloat(p_pbData, pf1);
			p_pbData += 3;
			pf1++;
		}
	}
}

void FloatDataToBit24( float **p_ppfData, unsigned char *p_pbData, int p_nSamples, int p_nChannels )
{
	if(p_nChannels == 1)
	{
		float *pf = p_ppfData[0];
		for (int i = 0;i < p_nSamples;i++)
		{
			FloatToBit24(pf, p_pbData);
			p_pbData += 3;
			pf++;
		}
	}
	else
	{
		float *pf0 = p_ppfData[0];
		float *pf1 = p_ppfData[1];
		for (int i = 0;i < p_nSamples;i++)
		{
			FloatToBit24(pf0, p_pbData);
			p_pbData += 3;
			pf0++;

			FloatToBit24(pf1, p_pbData);
			p_pbData += 3;
			pf1++;
		}
	}
}

WaveFileWriter::WaveFileWriter( void )
{
	m_fp = NULL;
}

WaveFileWriter::~WaveFileWriter( void )
{
	Finish();
	if(m_fp) _fclose_(m_fp);
}

bool WaveFileWriter::_Create(LPFile fp, int p_nSamplingFreq, int p_nBits, int p_nChannels)
{
	if (fp == NULL) return false;

	WaveHead wavHead(0, p_nSamplingFreq, p_nBits, p_nChannels);
	_fwrite_(&wavHead, sizeof(WaveHead), 1, fp);

	m_wavHead = wavHead;
	m_nDataSize = 0;

	return true;
}

bool WaveFileWriter::Create(const char* p_szFileName, int p_nSamplingFreq, int p_nBits, int p_nChannels )
{
	if(m_fp) _fclose_(m_fp);

	m_ErrorCode = WF_ERR_NOERROR;

	m_fp = _fopen_(p_szFileName, FT_WRITE);
	if(m_fp == NULL)
	{
		m_ErrorCode = WF_ERR_FILE_OPEN;
		return false;
	}
	return _Create(m_fp, p_nSamplingFreq, p_nBits, p_nChannels);
}

bool WaveFileWriter::AppendData( void* p_pData, int p_nBytes )
{
	m_ErrorCode = WF_ERR_NOERROR;
	if(!m_fp) 
	{
		m_ErrorCode = WF_ERR_FILE_OPEN;
		return false;
	}
	
	size_t nWritten = _fwrite_(p_pData, 1, p_nBytes, m_fp);
	m_nDataSize += nWritten;
	if(nWritten != p_nBytes)
	{
		m_ErrorCode = WF_ERR_WRITE;
		return false;
	}
	return true;
}

bool WaveFileWriter::Finish()
{
	m_ErrorCode = WF_ERR_NOERROR;

	if(m_fp == NULL) 
	{
		m_ErrorCode = WF_ERR_FILE_OPEN;
		return false;
	}
	m_wavHead.nRIFF = 36 + m_nDataSize;
	m_wavHead.size = m_nDataSize;

	_fseek_(m_fp, 0);
	_fwrite_(&m_wavHead, sizeof(WaveHead), 1, m_fp);
	_fclose_(m_fp);
	m_fp = NULL;

	return true;
}

WF_ErrorCode ReadPcmWaveFileInfo(LPFile pFile, PWAVE_FILE_INFO p_pWaveInfo)
{
	if (pFile == NULL)
		return WF_ERR_FILE_OPEN;
	WF_ErrorCode ret = WF_ERR_FORMAT;
	int	nSize, nOffset;
	WaveHead wh;
	int loopStart = 0xFFFFFFF;
	int loopEnd = 0x0000000;
	int dataOffset;	

	_fseek_(pFile, 0);
	_fread_(&wh, 1, 44, pFile);

	if (wh.nRIFF < 0x30)
	{
		
		goto ERROR_HANDLE;
	}

	if (strncmp(wh.WAVE, "WAVEfmt ", 8))
	{
		goto ERROR_HANDLE;
	}

	if (wh.nfmt < 16)
	{
		goto ERROR_HANDLE;
	}

	if (wh.channels < 1)
	{
		goto ERROR_HANDLE;
	}

	// wave파일의 0x14위치에 seek
	// 이 위치는 fmt chunk (포맷자료 chunk)의 자료부분이 시작되는 위치이다.
	// 여기까지는 모든  wave 파일에서 고정이라고 본다.
	_fseek_(pFile, 0x14);
	nOffset = 0x14;

	nOffset += wh.nfmt;

	if (!_fseek_(pFile, nOffset))
	{
		goto ERROR_HANDLE;
	}

	ret = WF_ERR_DATA_CHUNK;




	// 'data' chunk를 찾는다.
	do
	{
		if(!_fread_(wh.data, 1, 4, pFile))
			break;
		nOffset += 4;

		if(!_fread_(&wh.size, 1, 4, pFile))
			break;
		nOffset += 4;

		if (strncmp(wh.data, "data", 4) == 0)
		{
			// data chunk 를 찾았다.
			nSize = wh.size;
			dataOffset = nOffset;
		}

		if (strncmp(wh.data, "cue ", 4) == 0)
		{
			// cue point 를 찾았다.
			_UINT nCuePoint;
			_fread_(&nCuePoint, 4, 1, pFile);
			CuePoint point;

			for (int i = 0; i < nCuePoint; i++)
			{
				_fread_(&point, sizeof(CuePoint), 1, pFile);
				if (loopStart > point.frameOffset)
					loopStart = point.frameOffset;
				if (loopEnd < point.frameOffset)
					loopEnd = point.frameOffset;
			}
			break;
		}

		nOffset += wh.size;
	} while (_fseek_(pFile, nOffset));

	ret = WF_ERR_NOERROR;

	if (loopStart == 0xFFFFFFF)
	{
		loopStart = 0;
	}

	if (loopEnd == 0x0000000)
	{
		loopEnd = nSize;
	}

	if (loopStart == loopEnd)
	{
		loopStart = 0;
	}

	//음성정보 설정
	p_pWaveInfo->FormatTag = wh.wFormatTag;
	p_pWaveInfo->SamplingFreq = wh.nSamplesPerSec;
	p_pWaveInfo->Bits = wh.wBitsPerSample;
	p_pWaveInfo->Channels = wh.channels;
	p_pWaveInfo->SamplingFreq = wh.nSamplesPerSec;
	p_pWaveInfo->BytesOneSample = (p_pWaveInfo->Bits / 8) * p_pWaveInfo->Channels;

	p_pWaveInfo->DataOffset = dataOffset;
	p_pWaveInfo->DataBytes = nSize;
	p_pWaveInfo->loopStart = loopStart;
	p_pWaveInfo->loopEnd = loopEnd;


ERROR_HANDLE:
	return ret;
}


WF_ErrorCode ReadPcmWaveFileInfo(const char* p_fileName, PWAVE_FILE_INFO p_pWaveInfo)
{
	LPFile fp = _fopen_(p_fileName, FT_READ);
	
	WF_ErrorCode ret = ReadPcmWaveFileInfo(fp, p_pWaveInfo);
#ifdef __DEBUG
	printf("read wav info:%s %d\r\n", p_fileName, ret);
	printf("loop start:%d, loop end: %d\r\n", p_pWaveInfo->loopStart, p_pWaveInfo->loopEnd);
#endif
	_fclose_(fp);
	return ret;
}

