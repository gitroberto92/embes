#include "Voice.h"
#include "engine_main.h"
#include <math.h>
#include "ConfigFileManager.h"

short readBuff[READ_BLOCK_SIZE];

Voice::Voice(int note)
{
	m_bRelease = false;
	m_lastVolumn = 0;
	m_baseFrequency = ConfigFileManager::getFrequencyFromNote(note);
	m_fileItem = ConfigManager->selectItemWithFrequency(m_baseFrequency);
	m_key = note;
	
	m_fileFrequency = m_fileItem->frequency;
	
	char fn[20];
	sprintf(fn, "%s/%s", ConfigManager->getDir(), m_fileItem->fileName);
	m_file = _fopen_(fn, FT_READ);

	int offset = m_fileItem->wavInfo.DataOffset / 2;

	_fread_(readBuff, 2, READ_BLOCK_SIZE, m_file);
	
	m_writePos = 0;

	for (int i = offset; i < READ_BLOCK_SIZE; i++)
	{
		m_dataBuff[m_writePos] = readBuff[i];
		m_writePos++;
	}
	
	m_filePosition = READ_BLOCK_SIZE - offset;
	
	m_currentPos = 0;

	m_volEG.enterAttack();
}


Voice::~Voice()
{
	_fclose_(m_file);
}

void Voice::renderOneFrame(int nFrame, short* buf, bool first)
{
	float currentFrequency;
	if (usePortamento())
		currentFrequency = m_pitchEG.value();
	else
		currentFrequency = m_baseFrequency;
	m_pitchEG.update(1.0 * nFrame / AUDIO_SAMPLING_RATE);
	m_pitchRatio = currentFrequency / m_fileFrequency;

	float fLevel = m_lastVolumn;
	float fNextVolumn = m_volEG.getLevel() * getVolumn();
	float fDelta = (fNextVolumn - fLevel) / nFrame;

	int totalCount = nFrame * 2;

	if (!first) {
		for (int j = 0; j < totalCount; ) {
			short val = interpolateValue();
			buf[j] += val * fLevel;
			j++;
			buf[j] += val * fLevel;
			j++;
			fLevel += fDelta;
		}

	}
	else 
	{
		for (int j = 0; j < totalCount; ) {
			short val = interpolateValue();
			buf[j] = val * fLevel;
			j++;
			buf[j] = val * fLevel;
			j++;
			fLevel += fDelta;
		}
	}
	m_lastVolumn = fLevel;
	m_volEG.update(MADSR::KEY_NONE, nFrame);
}

float Voice::getLevel()
{
	return m_volEG.getLevel();
}

bool Voice::ended()
{
	return m_volEG.isEnded();
}

void Voice::release()
{
	if (m_bRelease)
		return;
	m_bRelease = true;
	m_volEG.update(MADSR::KEY_RELEASE, 0);
}

int Voice::note()
{
	return m_key;
}

int Voice::stage()
{
	return m_volEG.stage();
}

void Voice::portamentoAtStart(double prevFreqency, double duration, double delay)
{
	m_pitchEG.trigger(prevFreqency, m_baseFrequency, delay, duration);
}

float Voice::frequency()
{
	return m_baseFrequency;
}

inline short Voice::interpolateValue()
{
	int RP = (int)m_currentPos;

	short L = getBuffValue(RP);
	short R = getBuffValue(RP + 1);
	float delta = m_currentPos - RP;
	m_currentPos += m_pitchRatio;
	return (short)(L + delta * (R - L));

}

short Voice::getBuffValue(int pos)
{
	while (pos >= m_writePos) {
		readNewBuff();
	}
	int realpos = pos % RINGBUFF_MAX_SIZE;
	return m_dataBuff[realpos];
}


void Voice::readNewBuff()
{
	int toRead;
	if (m_filePosition + READ_BLOCK_SIZE > m_fileItem->wavInfo.loopEnd)
		toRead = m_fileItem->wavInfo.loopEnd - m_filePosition;
	else
		toRead = READ_BLOCK_SIZE;
	_fread_(readBuff, 2, toRead, m_file);

	for (int i = 0; i < toRead; i++) {
		m_dataBuff[m_writePos % RINGBUFF_MAX_SIZE] = readBuff[i];
		m_writePos++;
	}

	m_filePosition += toRead;

	if (toRead < READ_BLOCK_SIZE)
	{
		int seekpos = m_fileItem->wavInfo.loopStart * 2 + m_fileItem->wavInfo.DataOffset;
		int tmpBufSize = seekpos & 0x1FF;

		// 512의 배수가 되게 seek한다.
		_fseek_(m_file, seekpos - tmpBufSize);

		// 한번에 읽는 크기는 READ_BLOCK_SIZE가 되게 하여 매번 읽는 위치가 512의 배수가 되게 한다.
		_fread_(readBuff, 2, READ_BLOCK_SIZE, m_file);

		for (int i = tmpBufSize / 2; i < READ_BLOCK_SIZE; i++) {
			m_dataBuff[m_writePos % RINGBUFF_MAX_SIZE] = readBuff[i];
			m_writePos++;
		}
		m_filePosition = m_fileItem->wavInfo.loopStart + READ_BLOCK_SIZE - tmpBufSize / 2;
	}
}


