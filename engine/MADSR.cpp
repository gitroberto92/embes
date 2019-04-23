#include "MADSR.h"
#include "math.h"
#include "engine_main.h"

#define make_multiple(x, y) x = y * (int)(x / y + 0.5f)


MADSR::MADSR()
{
	m_preAttackLevel = 0.f;
	m_attackTime = attackTime();
	m_holdAttack = true;
	m_decay1Time = 0.f;
	m_decay2Time = 0.f;
	m_sustain = false;
	m_sustainLevel = 1.f;
	m_releaseTime1 = releaseTime();
	m_releaseLevel1 = 1.f;
	m_releaseTime2 = 0.1f;
	m_releaseLevel2 = releaseLevel();
	m_ended = false;
	modifyParameters();
}


MADSR::~MADSR()
{
}

void MADSR::update(key_event e, int sampleCount)
{
	switch (e)
	{
	case KEY_RELEASE:
		enterRelease1();
		break;
	case KEY_NONE:
		switch (m_stage)
		{
		case MADSR::STAGE_ATTACK:
			m_currentLevel += m_delta * sampleCount;
			if (m_currentLevel > 1)
				enterHold();
			break;
		case MADSR::STAGE_DECAY1:
			m_currentLevel += m_delta * sampleCount;
			if (m_currentLevel < m_sustainLevel + 0.001f)
			{
				if (m_sustain)
					enterSustain();
				else
					enterDecay2();
			}

			break;
		case MADSR::STAGE_DECAY2:
			m_currentLevel += m_delta * sampleCount;
			if (m_currentLevel < m_releaseLevel1 + 0.001f)
				enterRelease1();
			break;
		case MADSR::STAGE_SUSTAIN:
			//Todo
			break;
		case MADSR::STAGE_RELEASE1:
			m_currentLevel += m_delta * sampleCount;
			if (m_currentLevel <= m_releaseLevel2 + 0.001f)
				enterRelease2();
			break;
		case MADSR::STAGE_RELEASE2:
			updateReleaseDelta(sampleCount);
			m_currentLevel += m_delta * sampleCount;
			if (m_currentLevel <= 0.000001f || m_release2CurrentTime >= m_releaseTime2)
				enterEnd();
			break;
		default:
			break;
		}
	}
}

float MADSR::getDelta()
{
	return m_delta;
}

float MADSR::getLevel()
{
	return m_currentLevel;
}

void MADSR::enterAttack()
{
	m_stage = STAGE_ATTACK;
	m_currentLevel = m_preAttackLevel;
	m_delta = (1 - m_currentLevel) / (m_attackTime * AUDIO_SAMPLING_RATE);
}

void MADSR::enterHold()
{
	m_stage = STAGE_HOLD;
	m_currentLevel = 1;
	m_delta = 0;
}

void MADSR::enterDecay1()
{
	if (m_stage == STAGE_DECAY1)
		return;
	m_stage = STAGE_DECAY1;
	m_currentLevel = 1;
	m_delta = (m_sustainLevel - 1) / (m_decay1Time * AUDIO_SAMPLING_RATE);
}

void MADSR::enterDecay2()
{
	if (m_stage == STAGE_DECAY2)
		return;

	m_stage = STAGE_DECAY2;
	m_currentLevel = m_sustainLevel;
	m_delta = (m_releaseLevel1 - m_currentLevel) / (m_decay2Time * AUDIO_SAMPLING_RATE);
}

void MADSR::enterSustain()
{
	if (m_stage == STAGE_SUSTAIN)
		return;

	m_stage = STAGE_SUSTAIN;
	m_currentLevel = m_sustainLevel;
	m_delta = 0;
}

void MADSR::enterRelease1()
{
	if (m_stage == STAGE_RELEASE1)
		return;
	m_stage = STAGE_RELEASE1;
	if (m_sustain)
		m_currentLevel = m_sustainLevel;
	else
		m_currentLevel = m_releaseLevel1;
	m_delta = (0 - m_currentLevel) / (m_releaseTime1 * AUDIO_SAMPLING_RATE);
}

void MADSR::enterRelease2()
{
	m_stage = STAGE_RELEASE2;
	m_releasecoff1 = m_currentLevel;
	m_releasecoff2 = -m_delta / m_releasecoff1;
	m_release2CurrentTime = 0;
	m_currentX = 0;
}

void MADSR::enterEnd()
{
	m_currentLevel = 0;
	m_delta = 0;
	m_ended = true;
}

void MADSR::updateReleaseDelta(int sampleCount)
{
	m_release2CurrentTime += 1.f * sampleCount / AUDIO_SAMPLING_RATE;
	if (m_release2CurrentTime > m_releaseTime2)
		enterEnd();
	m_delta = (- m_releasecoff1 * (exp(-m_currentX) - exp(-(m_currentX + m_releasecoff2 * sampleCount)))) / sampleCount;
	m_currentX += m_releasecoff2 * sampleCount;
}

bool MADSR::isEnded()
{
	return m_ended;
}

int MADSR::stage()
{
	return m_stage;
}

void MADSR::modifyParameters()
{
	float dt = 1.0 * FRAME_SIZE / 44100;
	make_multiple(m_attackTime, dt);
	make_multiple(m_decay1Time, dt);
	make_multiple(m_decay2Time, dt);
	make_multiple(m_releaseTime1, dt);
}

