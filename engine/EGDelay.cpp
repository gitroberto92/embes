
#include "EGDelay.h"


EGDelay::EGDelay()
{
	m_start = 0;
	m_end = 0;
	m_delay = 0;
	m_duration = 1;
	m_currentTime = 0;
}


EGDelay::~EGDelay()
{
}

void EGDelay::trigger(float start, float end, float delay, float duration)
{
	m_start = start;
	m_end = end;
	m_delay = delay;
	m_duration = duration;
	m_currentTime = 0;
}

void EGDelay::update(float time)
{
	m_currentTime += time;
}

float EGDelay::value()
{
	if (m_currentTime <= m_delay)
		return m_start;
	else if (m_currentTime <= m_delay + m_duration)
		return m_start + (m_end - m_start) * (m_currentTime - m_delay) / m_duration;
	else 
		return m_end;
}
