#pragma once

// 선형 Envelop Generator
class EGDelay
{
public:
	EGDelay();
	~EGDelay();

	// 인벨로프생성을 시작한다.
	void trigger(float start, float end, float delay, float duration);

	// 시간을 갱신한다.
	void update(float time);

	// 현재의 값을 얻어낸다.
	float value();
private:
	float m_start;  // 인벨로프 시작값
	float m_end;	// 인벨로프 끝값
	float m_delay;	// 지연시간
	float m_duration;	// 선형 감소/증가 시간
	float m_currentTime; // 현재 시간
};

