#pragma once

#include "engine_main.h"

class MADSR
{
public:
	// ADSR�� �ܰ�
	enum adsr_stage{

		STAGE_ATTACK,
		STAGE_HOLD,
		STAGE_DECAY1,
		STAGE_DECAY2,
		STAGE_SUSTAIN,
		STAGE_RELEASE1,
		STAGE_RELEASE2,
	};

	// �ǹ��� ���
	enum key_event {
		KEY_ATTACK,
		KEY_RELEASE,
		KEY_NONE,
	};

	

	MADSR();
	~MADSR();

	// frame����ŭ ����
	void update(key_event e, int frame);

	// �� �����ӳ����� ó���ǿ����� ������ ǥ���������� ������ ����
	float getDelta();

	// ���� ������ ũ��
	float getLevel();

	// Attack ���·� 
	void enterAttack();

	// hold ���·� 
	void enterHold();

	// decay1 ���·� 
	void enterDecay1();

	// decay2 ���·� 
	void enterDecay2();

	// sustain ���·� 
	void enterSustain();

	// release ù���·� 
	// �� ��쿡 �������� �����Ѵ�.
	void enterRelease1();

	// release �ι�°���·�. 
	// �� ��쿡 �����Լ������� �����Ѵ�.
	void enterRelease2();

	// ������ ���� ���·�. 
	void enterEnd();

	// Release �ι�° ���¿��� �Ź� Delta�� �����Ҷ� ȣ��Ǵ� �Լ�.
	void updateReleaseDelta(int sampleCount);

	// ������ ���� ���¿� ���� ��ߺ���
	bool isEnded();

	// ���� �ܰ�
	int stage();

private:

	void modifyParameters();

	adsr_stage m_stage;

	

	float m_currentLevel;
	float m_delta;

	bool m_ended;
	
	float m_preAttackLevel;
	float m_attackTime;
	bool m_holdAttack;
	float m_decay1Time;
	float m_decay2Time;
	bool m_sustain;
	float m_sustainLevel;
	float m_releaseTime1;
	float m_releaseLevel1;
	float m_releaseTime2;
	float m_releaseLevel2;

	float m_releasecoff1;
	float m_releasecoff2;
	float m_currentX;
	float m_release2CurrentTime;
};

