/** 
  
  Copyright (c) 2013 PSJDC.
  
  @file IVoiceCodec.h
  
  @date 2013-4-3
  @author LMC(mizilmc@hotmail.com) 
  
  @brief
  
   실시간 음성코덱 인터페이스
   16000Hz 음성에 최적화 됩니다.
   stereo처리를 하지 않습니다.
*/

#pragma once

#pragma warning(disable : 4996)

#define DEF_CODEC_SAMPLING_RATE		16000
namespace codec
{
	class IEncoder;
	class IDecoder;

// Interface for VocieCode
class IVoiceCodec
{
protected:
	virtual ~IVoiceCodec(){};	// Don't delete. Delete by Terminate
public:
	inline double GetModeBps(int p_mode) { return 1.0 * GetFrameBits(p_mode) * (DEF_CODEC_SAMPLING_RATE / GetFrameSamples()); }

	/**
	  @breif 코덱의 이름을 얻는다.
	*/
	virtual const char* GetCodecName() = 0;

	/**
	  @breif 코덱이 지원하는 방식의 종류개수를 얻는다.
	  
	  방식 번호는 0부터 시작하며 커지는 순으로 음질이 높아지고 bps가 증가한다.
	*/
	virtual int GetModeNum() = 0;

	/**
	  @breif 코덱에서 사용하는 처리프레임의 샘플수를 얻는다.
	  코덱이 여러가지 표본화주파수를 지원한다고 하여도 여기서는 16000 Hz를 처리할 때의 프레임 길이이다.
	*/
	virtual int GetFrameSamples() = 0;

	/**
	  @breif 지적한 방식에 대하여 인코딩된 프레임의 비트수를 얻는다.
	  
	  코덱이 VBR를 지원한다고 하여도 여기서는 CBR인 경우만을 처리하며 
	  한 프레임을 인코딩한 출력 비트수는 항상 동일하다.
	  
	  @param p_mode - 처리 방식 첨수.
	  @return 인코딩된 프레임의 비트수
	  @warning 
	*/
	virtual int GetFrameBits(int p_mode) = 0;

	/**
	  @breif 지적한 방식에 개하여 인코딩된 프레임의 바이트수를 얻는다.
	  
	  코덱이 VBR를 지원한다고 하여도 여기서는 CBR인 경우만을 처리하며 
	  한 프레임을 인코딩한 출력 바이트수는 항상 동일하다. 
	  프레임 인코딩 결과는 바이트 단위로 출력된다.
	  
	  @param p_mode - 처리 방식 첨수.
	  @return 인코딩된 프레임의 바이트수
	  @warning 
	*/
	virtual int GetFrameBytes(int p_mode) = 0;

	/**
	  @breif 인코더를 창조한다.

	  처리방식과 함께 코덱의 JSON형식의 프로필 문자렬을 지적한다.
	  구체적인 프로필구성형식은 코덱마다 다르다.
	  인코더는 창조한 측에서 해방(인스턴스의 Destroy()함수를 호출하여)해야 한다.
	  
	  @param p_mode - 처리방식 첨수
	  @param p_profile - JSON형식의 코덱 프로필 문자렬
	  @return 인코더 인스턴스에 대한 지적자.
	  @warning 
	*/
	virtual IEncoder* CreateEncoder(int p_mode, const char* p_profile = nullptr) = 0;

	/**
	  @breif 디코더를 창조한다.

	  처리방식과 함께 코덱의 JSON형식의 프로필 문자렬을 지적한다.
	  구체적인 프로필구성형식은 코덱마다 다르다.
	  디코더는 창조한 측에서 해방(인스턴스의 Destroy()함수를 호출하여)해야 한다.
	  
	  @param p_mode - 처리방식 첨수
	  @param p_profile - JSON형식의 코덱 프로필 문자렬
	  @return 인코더 인스턴스에 대한 지적자.
	  @warning 
	*/
	virtual IDecoder* CreateDecoder(int p_mode, const char* p_profile = nullptr) = 0;

	/**
	  @breif 객체를 해방
	*/
	virtual void 	Destroy() { delete this; }
};

class IEncoder
{
protected:
	virtual ~IEncoder(){};	// Don't delete. Delete by Destroy
public:
	/**
	  @breif 음성데이터를 인코딩하여 출력 비트스트림 데이터를 얻는다.
	  
	  @param p_inVoice - 입력음성자료.mode에 따른 샘플수 만큼 입력된다고 가정. 한 샘플은 mono 16bit
	  @param p_outBitStream - 프레임을 인코딩한 출력자료. 
	  @param p_FrameNum - 인코딩할 프레임수. 이 프레임수만큼의 음성자료가 입력되였다고 가정.
	  @return 인코딩되여 출력된 바이트수.
	  @warning 
	*/
	virtual int Encode(short* p_inVoice, unsigned char* p_outBitStream, int p_FrameNum = 1) = 0;

	/**
	  @breif 객체를 해방한다.
	*/
	virtual void Destroy() { delete this; }
};

class IDecoder
{
protected:
	virtual ~IDecoder(){};	// Don't delete. Delete by Destroy
public:
	/**
	  @breif 인코딩된 데이터를 디코딩하여 음성 데이터를 출력한다.
	  
	  @param p_inBitStream - 프레임을 인코딩한 데이터.
	  @param p_outVoice - 프레임을 디코딩한 음성자료. mode에 따른 샘플수 만큼 출력된다. 한 샘플은 mono 16bit
	  @param p_FrameNum - 디코딩할 프레임수. 여기서 지적한 만큼 입력자료가 있다고 가정.
	  @return 프레임을 디코딩하여 출력한 샘플수.
	  @warning 
	*/
	virtual int Decode(unsigned char* p_inBitStream, short* p_outVoice, int p_FrameNum = 1) = 0;

	/**
	  @breif 객체를 해방한다.
	*/
	virtual void Destroy() { delete this; }
};



#ifdef WIN32

typedef IVoiceCodec* (_stdcall *LPFNGETPROCESSOR)();

static IVoiceCodec* GetCodecProcessor(HMODULE p_hModuleCodec)
{
	IVoiceCodec* p = NULL;

	if (p_hModuleCodec == NULL)
	{
		return NULL;
	}

	LPFNGETPROCESSOR lpfnGetProcessor = (LPFNGETPROCESSOR)GetProcAddress(p_hModuleCodec, "GetProcessor");

	if (lpfnGetProcessor != NULL)
	{
		p = lpfnGetProcessor();
	}

	return p;
}

#else // android & iOS

#define _stdcall
// DLL API (recommand dynamic using)
IVoiceCodec* _stdcall GetProcessor();

#endif



}