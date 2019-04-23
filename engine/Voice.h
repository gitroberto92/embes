#pragma once

#include "MADSR.h"
#include "EGDelay.h"
#include "ConfigFileManager.h"
#include "SDFileIO.h"

#define RINGBUFF_MAX_SIZE 2048  // ���Ͽ��� �о�� ������ ũ��
#define READ_BLOCK_SIZE 1024	// ���Ͽ��� �ѹ��� �о�� ������ ũ��. �ݵ�� 512�� ����� �ǿ��� �Ѵ�

class Voice
{
public:
	Voice(int note);
	~Voice();

	// frame��ŭ�� �������� �������Ѵ�.
	// first�� true�� ��쿡�� ������ buf������ �����ϰ�
	// first�� false�� ��쿡�� ������ buf���� ���Ѵ�.
	void renderOneFrame(int frame, short* buf, bool first);

	// ���� ������ ADSR������ ũ�⸦ ����.
	float getLevel();

	// Voice�� ������ �����°��� �����ִ� ��ߺ���
	bool ended();

	// Voice�� Release���¿��� ������.
	void release();

	// Voice�� Ű���� ����.
	int note();

	// ���� ������ ADSR������ ���¸� ����.
	int stage();

	// ���ۿ� ����Ÿ���並 �߻���Ų��.
	void portamentoAtStart(double nextFrequency, double duration, double delay);

	// Voice�� ���ļ��� ����.
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

