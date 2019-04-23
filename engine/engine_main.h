#pragma once

#ifndef WIN32
#ifdef __cplusplus
extern "C"
{
#endif
#endif

// ǥ��ȭ ���ļ�
#define AUDIO_SAMPLING_RATE		44100

// �ѹ��� �������� render�ϴ� �������� ũ��
#define AUDIO_FRAME_LEN			1024

// ��� ä�μ�
#define AUDIO_CHANNELS			2

// �Ǹ��� �ǹݼ�
#define NUM_KEYS				12

// voice���� �ѹ��� render�ϴ� �������� ũ��
#define FRAME_SIZE				256

// ��� �޸�
extern short g_outBuff[AUDIO_FRAME_LEN * AUDIO_CHANNELS];

// ����� ���� �ʱ�ȭ�� ���� ��ߺ���
extern bool g_buffInit;

// �������ϰ���Ŭ�� �ν��Ͻ�
extern void* g_configFileManager;
#define ConfigManager ((ConfigFileManager*)g_configFileManager)

// ���� Active�ǿ��ִ� Voice�� ��
extern int voiceCount;

// �⺻��ȯ. �� ��ȯ�� ȣ���ϸ� ��¸޸𸮿� AUDIO_FRAME_LEN ��ŭ�� �ڷḦ ����Ͽ� ����Ѵ�.
void main_loop(void);

// �⺻��ȯ �ʱ�ȭ
bool main_init(void);

// �⺻��ȯ�� ������.
void main_finish(void);

enum KeyEvent{
	NOTE_ON,
	NOTE_OFF,
};

// �ǹݻ���� �߻���Ų��.
void raiseKeyEvent(int key, int event);

// ������ �����Ѵ�. volumn �� 0���� 1���� ������ �־�� �Ѵ�.
void setVolumn(float volumn);

// ������ delta ��ŭ ������Ų��.
void increaseVolumn(float delta);

// ������ �������� ����.
float getVolumn(void);

// ����Ÿ���� ���̸� �����Ѵ�.
// ����Ÿ����� ���� ���ļ� �� ���� �߰��� ���ļ��� �α׺���� depth�� ��ġ���� ���۵ȴ�.
// depth �� 0���� 1������ ���̴�.
void setPortamentoDepth(double depth);

// ����Ÿ���� ���̸� ����.
double portamentoDepth(void);

// ����Ÿ���� �ð��� �����Ѵ�.  ������ s(��)�̴�.
void setPortamentoTime(double time);

// ����Ÿ���� �ð��� ����.  ������ s(��)�̴�.
double portamentoTime(void);

// ����Ÿ����ó���� �ϰڴ°��� �����Ѵ�.
void setUsePortamento(bool usePortamento);

// ����Ÿ����ó���� �����ϴ°� �ϴ� ����
bool usePortamento(void);

// ADSR���� Attack Time�� �����Ѵ�.  ������ s(��)�̴�.
void setAttackTime(float time);

// ADSR���� Attack Time�� ����.  ������ s(��)�̴�.
float attackTime(void);

// ADSR���� Release Time�� �����Ѵ�.  ������ s(��)�̴�.
void setReleaseTime(float time);

// ADSR���� Release Time�� ����.  ������ s(��)�̴�.
float releaseTime(void);

// ADSR���� Release ������ �����Ѵ�.
void setReleaseLevel(float level);

// ADSR���� Release ������ ����.
float releaseLevel(void);

// Rec���� ��Ϻθ� �����Ѵ�.
bool setRecDirectory(char* dir);

// Rec���� ��Ϻθ� ����.
char* recDirectory(void);

// �Ķ���͸� "config.txt"���Ͽ� �����Ѵ�.
void saveParams(void);

// Rec���� ��Ϻθ� �ٲ۴�.
void changeDirectory(void);

void changeKeySet(void);


#ifndef WIN32
#ifdef __cplusplus
}
#endif
#endif
