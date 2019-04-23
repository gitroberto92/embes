#pragma once

#include "MADSR.h"
#include "EGDelay.h"
#include "ConfigFileManager.h"
#include "SDFileIO.h"

#define RINGBUFF_MAX_SIZE 2048  // 파일에서 읽어낼때 바퍼의 크기
#define READ_BLOCK_SIZE 1024	// 파일에서 한번에 읽어내는 바퍼의 크기. 반드시 512의 배수가 되여야 한다

class Voice
{
public:
	Voice(int note);
	~Voice();

	// frame만큼의 쌤플점을 렌더링한다.
	// first가 true인 경우에는 얻어낸값을 buf값으로 설정하고
	// first가 false인 경우에는 얻어낸값을 buf값에 더한다.
	void renderOneFrame(int frame, short* buf, bool first);

	// 현재 볼륨의 ADSR에서의 크기를 얻어낸다.
	float getLevel();

	// Voice가 완전히 끝났는가를 돌려주는 기발변수
	bool ended();

	// Voice를 Release상태에로 보낸다.
	void release();

	// Voice의 키값을 얻어낸다.
	int note();

	// 현재 볼륨의 ADSR에서의 상태를 얻어낸다.
	int stage();

	// 시작에 포르타멘토를 발생시킨다.
	void portamentoAtStart(double nextFrequency, double duration, double delay);

	// Voice의 주파수를 얻어낸다.
	float frequency();
private:
	
	inline short interpolateValue();
	void readNewBuff();
	inline short getBuffValue(int pos);

	MADSR m_volEG;
	EGDelay m_pitchEG;
	bool m_bRelease;
	int m_key;
	float m_pitchRatio;
	float m_baseFrequency;
	float m_fileFrequency;
	float m_lastVolumn;
	short m_dataBuff[RINGBUFF_MAX_SIZE];
	float m_currentPos;
	int m_writePos;

	int m_filePosition;

	RecItem* m_fileItem;
	LPFile m_file;
};

