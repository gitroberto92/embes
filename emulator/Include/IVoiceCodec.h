/** 
  
  Copyright (c) 2013 PSJDC.
  
  @file IVoiceCodec.h
  
  @date 2013-4-3
  @author LMC(mizilmc@hotmail.com) 
  
  @brief
  
   �ǽð� �����ڵ� �������̽�
   16000Hz ������ ����ȭ �˴ϴ�.
   stereoó���� ���� �ʽ��ϴ�.
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
	  @breif �ڵ��� �̸��� ��´�.
	*/
	virtual const char* GetCodecName() = 0;

	/**
	  @breif �ڵ��� �����ϴ� ����� ���������� ��´�.
	  
	  ��� ��ȣ�� 0���� �����ϸ� Ŀ���� ������ ������ �������� bps�� �����Ѵ�.
	*/
	virtual int GetModeNum() = 0;

	/**
	  @breif �ڵ����� ����ϴ� ó���������� ���ü��� ��´�.
	  �ڵ��� �������� ǥ��ȭ���ļ��� �����Ѵٰ� �Ͽ��� ���⼭�� 16000 Hz�� ó���� ���� ������ �����̴�.
	*/
	virtual int GetFrameSamples() = 0;

	/**
	  @breif ������ ��Ŀ� ���Ͽ� ���ڵ��� �������� ��Ʈ���� ��´�.
	  
	  �ڵ��� VBR�� �����Ѵٰ� �Ͽ��� ���⼭�� CBR�� ��츸�� ó���ϸ� 
	  �� �������� ���ڵ��� ��� ��Ʈ���� �׻� �����ϴ�.
	  
	  @param p_mode - ó�� ��� ÷��.
	  @return ���ڵ��� �������� ��Ʈ��
	  @warning 
	*/
	virtual int GetFrameBits(int p_mode) = 0;

	/**
	  @breif ������ ��Ŀ� ���Ͽ� ���ڵ��� �������� ����Ʈ���� ��´�.
	  
	  �ڵ��� VBR�� �����Ѵٰ� �Ͽ��� ���⼭�� CBR�� ��츸�� ó���ϸ� 
	  �� �������� ���ڵ��� ��� ����Ʈ���� �׻� �����ϴ�. 
	  ������ ���ڵ� ����� ����Ʈ ������ ��µȴ�.
	  
	  @param p_mode - ó�� ��� ÷��.
	  @return ���ڵ��� �������� ����Ʈ��
	  @warning 
	*/
	virtual int GetFrameBytes(int p_mode) = 0;

	/**
	  @breif ���ڴ��� â���Ѵ�.

	  ó����İ� �Բ� �ڵ��� JSON������ ������ ���ڷ��� �����Ѵ�.
	  ��ü���� �����ʱ��������� �ڵ����� �ٸ���.
	  ���ڴ��� â���� ������ �ع�(�ν��Ͻ��� Destroy()�Լ��� ȣ���Ͽ�)�ؾ� �Ѵ�.
	  
	  @param p_mode - ó����� ÷��
	  @param p_profile - JSON������ �ڵ� ������ ���ڷ�
	  @return ���ڴ� �ν��Ͻ��� ���� ������.
	  @warning 
	*/
	virtual IEncoder* CreateEncoder(int p_mode, const char* p_profile = nullptr) = 0;

	/**
	  @breif ���ڴ��� â���Ѵ�.

	  ó����İ� �Բ� �ڵ��� JSON������ ������ ���ڷ��� �����Ѵ�.
	  ��ü���� �����ʱ��������� �ڵ����� �ٸ���.
	  ���ڴ��� â���� ������ �ع�(�ν��Ͻ��� Destroy()�Լ��� ȣ���Ͽ�)�ؾ� �Ѵ�.
	  
	  @param p_mode - ó����� ÷��
	  @param p_profile - JSON������ �ڵ� ������ ���ڷ�
	  @return ���ڴ� �ν��Ͻ��� ���� ������.
	  @warning 
	*/
	virtual IDecoder* CreateDecoder(int p_mode, const char* p_profile = nullptr) = 0;

	/**
	  @breif ��ü�� �ع�
	*/
	virtual void 	Destroy() { delete this; }
};

class IEncoder
{
protected:
	virtual ~IEncoder(){};	// Don't delete. Delete by Destroy
public:
	/**
	  @breif ���������͸� ���ڵ��Ͽ� ��� ��Ʈ��Ʈ�� �����͸� ��´�.
	  
	  @param p_inVoice - �Է������ڷ�.mode�� ���� ���ü� ��ŭ �Էµȴٰ� ����. �� ������ mono 16bit
	  @param p_outBitStream - �������� ���ڵ��� ����ڷ�. 
	  @param p_FrameNum - ���ڵ��� �����Ӽ�. �� �����Ӽ���ŭ�� �����ڷᰡ �Էµǿ��ٰ� ����.
	  @return ���ڵ��ǿ� ��µ� ����Ʈ��.
	  @warning 
	*/
	virtual int Encode(short* p_inVoice, unsigned char* p_outBitStream, int p_FrameNum = 1) = 0;

	/**
	  @breif ��ü�� �ع��Ѵ�.
	*/
	virtual void Destroy() { delete this; }
};

class IDecoder
{
protected:
	virtual ~IDecoder(){};	// Don't delete. Delete by Destroy
public:
	/**
	  @breif ���ڵ��� �����͸� ���ڵ��Ͽ� ���� �����͸� ����Ѵ�.
	  
	  @param p_inBitStream - �������� ���ڵ��� ������.
	  @param p_outVoice - �������� ���ڵ��� �����ڷ�. mode�� ���� ���ü� ��ŭ ��µȴ�. �� ������ mono 16bit
	  @param p_FrameNum - ���ڵ��� �����Ӽ�. ���⼭ ������ ��ŭ �Է��ڷᰡ �ִٰ� ����.
	  @return �������� ���ڵ��Ͽ� ����� ���ü�.
	  @warning 
	*/
	virtual int Decode(unsigned char* p_inBitStream, short* p_outVoice, int p_FrameNum = 1) = 0;

	/**
	  @breif ��ü�� �ع��Ѵ�.
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