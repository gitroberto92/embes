#pragma once

// ���� Envelop Generator
class EGDelay
{
public:
	EGDelay();
	~EGDelay();

	// �κ����������� �����Ѵ�.
	void trigger(float start, float end, float delay, float duration);

	// �ð��� �����Ѵ�.
	void update(float time);

	// ������ ���� ����.
	float value();
private:
	float m_start;  // �κ����� ���۰�
	float m_end;	// �κ����� ����
	float m_delay;	// �����ð�
	float m_duration;	// ���� ����/���� �ð�
	float m_currentTime; // ���� �ð�
};

