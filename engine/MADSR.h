#pragma once

#include "engine_main.h"

class MADSR
{
public:
	// ADSR의 단계
	enum adsr_stage{

		STAGE_ATTACK,
		STAGE_HOLD,
		STAGE_DECAY1,
		STAGE_DECAY2,
		STAGE_SUSTAIN,
		STAGE_RELEASE1,
		STAGE_RELEASE2,
	};

	// 건반의 사건
	enum key_event {
		KEY_ATTACK,
		KEY_RELEASE,
		KEY_NONE,
	};

	

	MADSR();
	~MADSR();

	// frame수만큼 갱신
	void update(key_event e, int frame);

	// 한 프레임내에서 처리되여야할 련이은 표본점사이의 볼륨의 차이
	float getDelta();

	// 현재 볼륨의 크기
	float getLevel();

	// Attack 상태로 
	void enterAttack();

	// hold 상태로 
	void enterHold();

	// decay1 상태로 
	void enterDecay1();

	// decay2 상태로 
	void enterDecay2();

	// sustain 상태로 
	void enterSustain();

	// release 첫상태로 
	// 이 경우에 선형으로 감소한다.
	void enterRelease1();

	// release 두번째상태로. 
	// 이 경우에 지수함수적으로 감소한다.
	void enterRelease2();

	// 완전히 끝난 상태로. 
	void enterEnd();

	// Release 두번째 상태에서 매번 Delta를 갱신할때 호출되는 함수.
	void updateReleaseDelta(int sampleCount);

	// 완전히 끝난 상태에 대한 기발변수
	bool isEnded();

	// 현재 단계
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

