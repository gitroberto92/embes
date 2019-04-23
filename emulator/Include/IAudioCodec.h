/** 
  
  Copyright (c) 2012 PSJDC.
  
  @file IAudioEncoder.h
  
  @date 2012-9-18클래스  
  @author LMC(mizilmc@hotmail.com) 
  
  @brief
  
   오디오 코덱 인터페이스
   주의:
    - mp3디코딩 모듈내에서 파일이름이 ANSI로 변환되여 리용된다. 
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
	  @breif 인코더를 초기화한다.
	  
	  @param p_nSamplingRate - 표본화 주파수. 실례로 44.1KHz방식이면 44100
	  @param p_nChannels - 음성자료의 채널수.
	  @param p_szOutFile - 출력파일 경로
	  @return 성공이면 true, 실패이면 false
	  @warning 
	*/
	virtual bool Initialize(long p_nSamplingRate, long p_nChannels, const utf16_t* p_szOutFile) = 0;

	/**
	  @breif 오디오 인코더를 완료한다.
	  이 함수에서 출력파일을 닫는다.
	*/
	virtual void Terminate() = 0;

	/**
	  @breif 지적한 표본점수의 음성자료를 인코딩하여 출력파일에 출력한다.
	  
	  @param p_InData - 음성자료(채널수는 모노 혹은 스테레오를 지원)
	  @param p_nInSamples - 표본점수
	  @return 출력바이트수
	  @warning 
	*/
	virtual int Encode(float** p_InData, int p_nInSamples) = 0;
	virtual int Encode(short* p_InData, int p_nInSamples) = 0;

	/**
	  @breif 인코딩 객체를 삭제한다.
	  
	  이 함수를 호출하면 클라스의 인스턴스가 해방된다.(delete this)
	  함수호출후 클라스의 어떤 기능도 호출할수 없게된다.
	*/
	virtual void Destroy();

	virtual ~IAudioEncoder() {}
};

class IAudioDecoder
{
public:
	
	/**
	  @breif 디코더를 초기화한다.
	  디코딩 결과는 PCM WAVE파일 형식으로 출력된다.
	  
	  @param p_szInFile - 디코딩할 파일의 경로
	  @param p_szOutFile - 디코딩 결과를 출력할 파일 경로
	  @return 성공이면 true, 실패이면 false
	  @warning 
	*/
	virtual bool Init(const utf16_t* p_szInFile, const utf16_t* p_szOutFile) = 0;

	/**
	  @breif 디코딩처리를 시작한다.

	  @return 성공이면 true, 실패이면 false
	  @warning 
	*/
	virtual bool DoDecode() = 0;

	/**
	  @breif 디코딩처리를 완료한다.
	*/
	virtual void Finish() = 0;

	/**
	  @breif 현재 디코딩처리 진행률을 퍼센트(0~100%)값으로 얻는다.
	  
	  스레드에 안전하다.
	  
	  @return 진행률 퍼센트(0~100%)값
	  @warning 
	*/
	virtual float GetCurrentProgress() = 0;

	/**
	  @breif 인코딩 객체를 삭제한다.
	  
	  이 함수를 호출하면 클라스의 인스턴스가 해방된다.(delete this)
	  함수호출후 클라스의 어떤 기능도 호출할수 없게된다.
	*/
	virtual void Destroy();

	virtual ~IAudioDecoder() {}
};

/**
  @breif 코덱타입을 지정하여 인코더 인스턴스를 창조한다.
  인코더는 다중인스턴스창조가 가능하다.
  코덱타입은 mp3, wav가 아닌 경우 vorbis로 가정한다.
  창조된 객체의 삭제는 객체의 Destroy()함수를 호출하여 진행한다.
  
  @param p_CodecType - 코덱타입(CodecTypeMp3/CodecTypeVorbis)
  @return 인코더 인스턴스
  @warning 
*/
IAudioEncoder* createAudioEncoder(int p_CodecType);

/**
  @breif 코덱타입을 지정하여 디코더 인스턴스를 창조한다.
  디코더는 다중인스턴스창조가 가능하다.
  코덱타입은 mp3이 아닌경우 vorbis로 가정한다.
  창조된 객체의 삭제는 객체의 Destroy()함수를 호출하여 진행한다.
  
  @param p_CodecType - 코덱타입(CodecTypeMp3/CodecTypeVorbis)
  @return 디코더 인스턴스
  @warning 
*/
IAudioDecoder* createAudioDecoder(int p_CodecType);

#ifdef WIN32
/************************************************
 * DLL을 동적으로 로드하는 경우 편리하게 사용할수 있는 기능
 ***********************************************/
#include <windows.h>
/**
  @breif DLL 모듈을 지적하여 인코딩을 객체를 얻는다.
  
  DLL을 load/unload하는 것은 호출자의 책임이다.
  
  @param p_hModule - 오디오 코덱 DLL모듈
  @param p_CodecType - 코덱 타입
  @return 인코딩 객체
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
  @breif DLL 모듈을 지적하여 디코딩 객체를 얻는다.
  
  DLL을 load/unload하는 것은 호출자의 책임이다.
  
  @param p_hModule - 오디오 코덱 DLL모듈
  @param p_CodecType - 코덱 타입
  @return 디코딩 객체
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