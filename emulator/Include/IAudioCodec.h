/** 
  
  Copyright (c) 2012 PSJDC.
  
  @file IAudioEncoder.h
  
  @date 2012-9-18Ŭ����  
  @author LMC(mizilmc@hotmail.com) 
  
  @brief
  
   ����� �ڵ� �������̽�
   ����:
    - mp3���ڵ� ��⳻���� �����̸��� ANSI�� ��ȯ�ǿ� ����ȴ�. 
*/

#ifndef __IAUDIOCODEC_H__
#define __IAUDIOCODEC_H__

#include "ulib_basestrtype.h"

enum
{
	CodecTypeMp3 = 0,
	CodecTypeVorbis,
};


class IAudioEncoder
{
public:
	/**
	  @breif ���ڴ��� �ʱ�ȭ�Ѵ�.
	  
	  @param p_nSamplingRate - ǥ��ȭ ���ļ�. �Ƿʷ� 44.1KHz����̸� 44100
	  @param p_nChannels - �����ڷ��� ä�μ�.
	  @param p_szOutFile - ������� ���
	  @return �����̸� true, �����̸� false
	  @warning 
	*/
	virtual bool Initialize(long p_nSamplingRate, long p_nChannels, const utf16_t* p_szOutFile) = 0;

	/**
	  @breif ����� ���ڴ��� �Ϸ��Ѵ�.
	  �� �Լ����� ��������� �ݴ´�.
	*/
	virtual void Terminate() = 0;

	/**
	  @breif ������ ǥ�������� �����ڷḦ ���ڵ��Ͽ� ������Ͽ� ����Ѵ�.
	  
	  @param p_InData - �����ڷ�(ä�μ��� ��� Ȥ�� ���׷����� ����)
	  @param p_nInSamples - ǥ������
	  @return ��¹���Ʈ��
	  @warning 
	*/
	virtual int Encode(float** p_InData, int p_nInSamples) = 0;
	virtual int Encode(short* p_InData, int p_nInSamples) = 0;

	/**
	  @breif ���ڵ� ��ü�� �����Ѵ�.
	  
	  �� �Լ��� ȣ���ϸ� Ŭ���� �ν��Ͻ��� �ع�ȴ�.(delete this)
	  �Լ�ȣ���� Ŭ���� � ��ɵ� ȣ���Ҽ� ���Եȴ�.
	*/
	virtual void Destroy();

	virtual ~IAudioEncoder() {}
};

class IAudioDecoder
{
public:
	
	/**
	  @breif ���ڴ��� �ʱ�ȭ�Ѵ�.
	  ���ڵ� ����� PCM WAVE���� �������� ��µȴ�.
	  
	  @param p_szInFile - ���ڵ��� ������ ���
	  @param p_szOutFile - ���ڵ� ����� ����� ���� ���
	  @return �����̸� true, �����̸� false
	  @warning 
	*/
	virtual bool Init(const utf16_t* p_szInFile, const utf16_t* p_szOutFile) = 0;

	/**
	  @breif ���ڵ�ó���� �����Ѵ�.

	  @return �����̸� true, �����̸� false
	  @warning 
	*/
	virtual bool DoDecode() = 0;

	/**
	  @breif ���ڵ�ó���� �Ϸ��Ѵ�.
	*/
	virtual void Finish() = 0;

	/**
	  @breif ���� ���ڵ�ó�� ������� �ۼ�Ʈ(0~100%)������ ��´�.
	  
	  �����忡 �����ϴ�.
	  
	  @return ����� �ۼ�Ʈ(0~100%)��
	  @warning 
	*/
	virtual float GetCurrentProgress() = 0;

	/**
	  @breif ���ڵ� ��ü�� �����Ѵ�.
	  
	  �� �Լ��� ȣ���ϸ� Ŭ���� �ν��Ͻ��� �ع�ȴ�.(delete this)
	  �Լ�ȣ���� Ŭ���� � ��ɵ� ȣ���Ҽ� ���Եȴ�.
	*/
	virtual void Destroy();

	virtual ~IAudioDecoder() {}
};

/**
  @breif �ڵ�Ÿ���� �����Ͽ� ���ڴ� �ν��Ͻ��� â���Ѵ�.
  ���ڴ��� �����ν��Ͻ�â���� �����ϴ�.
  �ڵ�Ÿ���� mp3, wav�� �ƴ� ��� vorbis�� �����Ѵ�.
  â���� ��ü�� ������ ��ü�� Destroy()�Լ��� ȣ���Ͽ� �����Ѵ�.
  
  @param p_CodecType - �ڵ�Ÿ��(CodecTypeMp3/CodecTypeVorbis)
  @return ���ڴ� �ν��Ͻ�
  @warning 
*/
IAudioEncoder* createAudioEncoder(int p_CodecType);

/**
  @breif �ڵ�Ÿ���� �����Ͽ� ���ڴ� �ν��Ͻ��� â���Ѵ�.
  ���ڴ��� �����ν��Ͻ�â���� �����ϴ�.
  �ڵ�Ÿ���� mp3�� �ƴѰ�� vorbis�� �����Ѵ�.
  â���� ��ü�� ������ ��ü�� Destroy()�Լ��� ȣ���Ͽ� �����Ѵ�.
  
  @param p_CodecType - �ڵ�Ÿ��(CodecTypeMp3/CodecTypeVorbis)
  @return ���ڴ� �ν��Ͻ�
  @warning 
*/
IAudioDecoder* createAudioDecoder(int p_CodecType);

#ifdef WIN32
/************************************************
 * DLL�� �������� �ε��ϴ� ��� ���ϰ� ����Ҽ� �ִ� ���
 ***********************************************/
#include <windows.h>
/**
  @breif DLL ����� �����Ͽ� ���ڵ��� ��ü�� ��´�.
  
  DLL�� load/unload�ϴ� ���� ȣ������ å���̴�.
  
  @param p_hModule - ����� �ڵ� DLL���
  @param p_CodecType - �ڵ� Ÿ��
  @return ���ڵ� ��ü
  @warning 
*/
static IAudioEncoder* GetEncodeProcessor(HMODULE p_hModule, int p_CodecType)
{
	IAudioEncoder* p = NULL;

	if (p_hModule == NULL)
	{
		return NULL;
	}

	typedef IAudioEncoder* (*LPFN_CREATE_PROCESSOR)(int p_CodecType);

	LPFN_CREATE_PROCESSOR lpfnCreateInstance = (LPFN_CREATE_PROCESSOR)GetProcAddress(p_hModule, "createAudioEncoder");

	if (lpfnCreateInstance != NULL)
	{
		p = lpfnCreateInstance(p_CodecType);
	}

	return p;
}

/**
  @breif DLL ����� �����Ͽ� ���ڵ� ��ü�� ��´�.
  
  DLL�� load/unload�ϴ� ���� ȣ������ å���̴�.
  
  @param p_hModule - ����� �ڵ� DLL���
  @param p_CodecType - �ڵ� Ÿ��
  @return ���ڵ� ��ü
  @warning 
*/
static IAudioDecoder* GetDecodeProcessor(HMODULE p_hModule, int p_CodecType)
{
	IAudioDecoder* p = NULL;

	if (p_hModule == NULL)
	{
		return NULL;
	}

	typedef IAudioDecoder* (*LPFN_CREATE_PROCESSOR)(int p_CodecType);

	LPFN_CREATE_PROCESSOR lpfnCreateInstance = (LPFN_CREATE_PROCESSOR)GetProcAddress(p_hModule, "createAudioDecoder");

	if (lpfnCreateInstance != NULL)
	{
		p = lpfnCreateInstance(p_CodecType);
	}

	return p;
}
#endif // WIN32

#endif