#pragma once

#ifndef WIN32
#ifdef __cplusplus
extern "C"
{
#endif
#endif

// 표본화 주파수
#define AUDIO_SAMPLING_RATE		44100

// 한번에 엔진에서 render하는 프레임의 크기
#define AUDIO_FRAME_LEN			1024

// 출력 채널수
#define AUDIO_CHANNELS			2

// 피리의 건반수
#define NUM_KEYS				12

// voice에서 한번에 render하는 프레임의 크기
#define FRAME_SIZE				256

// 출력 메모리
extern short g_outBuff[AUDIO_FRAME_LEN * AUDIO_CHANNELS];

// 출력의 대한 초기화에 대한 기발변수
extern bool g_buffInit;

// 설정파일관리클라스 인스턴스
extern void* g_configFileManager;
#define ConfigManager ((ConfigFileManager*)g_configFileManager)

// 현재 Active되여있는 Voice의 수
extern int voiceCount;

// 기본순환. 이 순환을 호출하면 출력메모리에 AUDIO_FRAME_LEN 만큼의 자료를 계산하여 출력한다.
void main_loop(void);

// 기본순환 초기화
bool main_init(void);

// 기본순환을 끝낸다.
void main_finish(void);

enum KeyEvent{
	NOTE_ON,
	NOTE_OFF,
};

// 건반사건을 발생시킨다.
void raiseKeyEvent(int key, int event);

// 볼륨을 설정한다. volumn 은 0부터 1사이 범위에 있어야 한다.
void setVolumn(float volumn);

// 볼륨을 delta 만큼 증가시킨다.
void increaseVolumn(float delta);

// 현재의 볼륨값을 얻어낸다.
float getVolumn(void);

// 포르타멘토 깊이를 설정한다.
// 포르타멘토는 전의 주파수 와 새로 추가된 주파수의 로그비률이 depth인 위치에서 시작된다.
// depth 는 0부터 1사이의 값이다.
void setPortamentoDepth(double depth);

// 포르타멘토 깊이를 얻어낸다.
double portamentoDepth(void);

// 포르타멘토 시간을 설정한다.  단위는 s(초)이다.
void setPortamentoTime(double time);

// 포르타멘토 시간을 얻어낸다.  단위는 s(초)이다.
double portamentoTime(void);

// 포르타멘토처리를 하겠는가를 설정한다.
void setUsePortamento(bool usePortamento);

// 포르타멘토처리를 진행하는가 하는 변수
bool usePortamento(void);

// ADSR에서 Attack Time을 설정한다.  단위는 s(초)이다.
void setAttackTime(float time);

// ADSR에서 Attack Time을 얻어낸다.  단위는 s(초)이다.
float attackTime(void);

// ADSR에서 Release Time을 설정한다.  단위는 s(초)이다.
void setReleaseTime(float time);

// ADSR에서 Release Time을 얻어낸다.  단위는 s(초)이다.
float releaseTime(void);

// ADSR에서 Release 수준을 설정한다.
void setReleaseLevel(float level);

// ADSR에서 Release 수준을 얻어낸다.
float releaseLevel(void);

// Rec파일 등록부를 설정한다.
bool setRecDirectory(char* dir);

// Rec파일 등록부를 얻어낸다.
char* recDirectory(void);

// 파라메터를 "config.txt"파일에 보관한다.
void saveParams(void);

// Rec파일 등록부를 바꾼다.
void changeDirectory(void);

void changeKeySet(void);


#ifndef WIN32
#ifdef __cplusplus
}
#endif
#endif
