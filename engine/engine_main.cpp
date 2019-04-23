
#include "engine_main.h"

#include "Voice.h"
#include "math.h"
#include "ConfigFileManager.h"

#ifdef WIN32
#include "dbg.h"
#endif

short g_outBuff[AUDIO_FRAME_LEN * AUDIO_CHANNELS];

// �ִ� active�� voice�� ����
#define MAX_VOICE_COUNT 4
Voice* voices[MAX_VOICE_COUNT];

// config���� ����Ŭ�� �ν��Ͻ�
void* g_configFileManager;

// ���� voice�� ����
int voiceCount;

// ���� ����
float g_fVol = 1;

// ����Ÿ���� ����
double g_portamentoDepth = 0.5;

// ����Ÿ���� �ð�
double g_portamentoTime = 0.5;

// ����Ÿ������ ���뿡 ���� ��ߺ���
bool g_usePortamento = true;

// ADSR�� Attack �ð�
float g_attackTime = 0.05f;

// ADSR�� Release �ð�
float g_releaseTime = 0.05f;

// ADSR�� Release����
float g_releaseLevel = 0.5f;

// ���� Rec���ϵ�Ϻ��� ��ȣ
// ��ȣ�� 00-99������ ���ڸ����� ǥ���ȴ�.
int g_currentDirNumber;

// Rec���ϵ�Ϻθ� �����Ѵ�.
bool setRecDirectory(char* dir)
{
	return ConfigManager->initRecFile(dir);
}

char* recDirectory()
{
	return ConfigManager->getDir();
}

// �Ķ���͸� config.txt���Ͽ� �����Ѵ�.
void saveParams()
{
	LPFile f = _fopen_("config.txt", FT_WRITE);
	_fprintf_(f, "RecDirectory = %s\n", recDirectory());
	_fprintf_(f, "PortamentoDepth = %.2lf\n", portamentoDepth());
	_fprintf_(f, "PortamentoTime = %.2lf\n", portamentoTime());
	_fprintf_(f, "PortamentoFlag = %s\n", usePortamento() ? "true" : "false");
	_fprintf_(f, "AttackTime = %.2lf\n", attackTime());
	_fprintf_(f, "ReleaseTime = %.2lf\n", releaseTime());
	_fprintf_(f, "ReleaseLevel = %.2lf\n", releaseLevel());
	_fprintf_(f, "CurrentKeySet = %d\n", ConfigManager->currentKeySetNumber());
	_fclose_(f);
}

void changeDirectory(void)
{
	while (1) {
		g_currentDirNumber = (g_currentDirNumber + 1) % 100;
		char newDir[7];
		sprintf(newDir, "rec%02d", g_currentDirNumber);
		if (isDir(newDir))
		{
			strcpy(ConfigManager->getDir(), newDir);
			saveParams();
			break;
		}
	}
}

void changeKeySet()
{
	ConfigManager->changeKeySet();
}

void setAttackTime(float time)
{
	g_attackTime = time;
}

float attackTime()
{
	return g_attackTime;
}


void setReleaseTime(float time)
{
	g_releaseTime = time;
}


float releaseTime()
{
	return g_releaseTime;
}



void setReleaseLevel(float level)
{
	g_releaseLevel = level;
}

float releaseLevel()
{
	return g_releaseLevel;
}


void setUsePortamento(bool usePortamento)
{
	g_usePortamento = usePortamento;
}

bool usePortamento()
{
	return g_usePortamento;
}

// ���� ��Ÿ���� voice
Voice* g_enginPrevVoice;

// �ʱ⿡ ��� voice�� NULL�� �ʱ�ȭ�Ѵ�.
void initVoices() 
{
	for (int i = 0; i < MAX_VOICE_COUNT; i++)
		voices[i] = NULL;
}

// ���� voice���� delete�Ѵ�.
void removeEndedVoices() 
{

	for (int i = 0; i < MAX_VOICE_COUNT; i++)
	{
		if (voices[i] != NULL && voices[i]->ended())
		{
			// ���� delete�Ͽ��� �� voice�� ���� voice�� ��쿡 g_enginPrevVoice�� NULL�� ����
			if (g_enginPrevVoice != NULL && voices[i]->note() == g_enginPrevVoice->note())
				g_enginPrevVoice = NULL;

			delete voices[i];
			voices[i] = NULL;
			--voiceCount;
		}
	}

}

// key�� �ش��ϴ� voice�� release���¿��� ������.
void releaseVoice(int key) {
	for (int i = 0; i < MAX_VOICE_COUNT; i++)
	{
		if (voices[i] != NULL && voices[i]->note() == key)
			voices[i]->release();
	}
}

// ��� voice�� release���¿��� ������.
void releaseAllVoices()
{
	for (int i = 0; i < MAX_VOICE_COUNT; i++)
	{
		if (voices[i] != NULL)
			voices[i]->release();
	}
}


// A�� B���̿��� �α׺���� ratio�� ���� ����.
double getLogRatioBetween(double A, double B, double ratio) {
	return A * pow((B/A), ratio);
}

// ���ο� voice�� �߰��Ѵ�.
// �̶� voices�߿� �� �ڸ��� ������ voice�߿��� ������ �ּ��� voice�� �����ϰ� �� �ڸ��� �߰��Ѵ�.
void addVoice(Voice* voice)
{
	// �������ļ�
	double start;

	if (usePortamento())
	{
		if (g_enginPrevVoice != NULL)
		{
			start = getLogRatioBetween(voice->frequency(), g_enginPrevVoice->frequency(), g_portamentoDepth);
		}
		else
		{
			start = voice->frequency();
		}
		voice->portamentoAtStart(start, g_portamentoTime, 0);
	}
	
	g_enginPrevVoice = voice;
	++voiceCount;
	float minLevel = 1;
	int minIdx;
	for (int i = 0; i < MAX_VOICE_COUNT; i++)
	{
		if (voices[i] == NULL) // ���ڸ��� ������ �ű⿡ �߰� �Ѵ�.
		{
			voices[i] = voice;
			return;
		}
		float lvl = voices[i]->getLevel();
		if (minLevel > lvl) { // level�� �ּ��� voice�� �����Ѵ�
			minLevel = lvl;
			minIdx = i;
		}

	}
	delete voices[minIdx];
	--voiceCount;
	voices[minIdx] = voice;
}


// framecount��ŭ�� ����� buff�� ����.
void renderVoices(int frameCount, short* buff)
{
	if (voiceCount == 0) {
		for (int i = 0; i < frameCount * AUDIO_CHANNELS; i++) {
			buff[i] = 0;
		}
	}
	else {
		bool first = true;
		for (int i = 0; i < MAX_VOICE_COUNT; i++)
		{
			if (voices[i] != NULL)
			{
				voices[i]->renderOneFrame(frameCount, buff, first);
				first = false;
			}
		}
	}
}


bool g_buffInit = false;

bool  main_init() {

	// �����ʱ�ȭ
	g_configFileManager = new ConfigFileManager();
	ConfigManager->initKeySet("keyset.txt");
#ifdef __DEBUG
	printf("init config.txt\r\n");
#endif
	
	if (!ConfigManager->initConfig("config.txt")) // keyset������ �����Ѵ����� ȣ��ǿ��� �Ѵ�.
	{
		return false;
	}
#ifdef __DEBUG
	printf("init config.txt success\r\n");
#endif

	
	g_currentDirNumber = atoi(ConfigManager->getDir() + 3); // config.txt���Ͽ��� ������ �ҷ����� ������ ȣ��ǿ��� �Ѵ�.

	// voice�ʱ�ȭ
	initVoices();
	g_enginPrevVoice = NULL;

	g_buffInit = true;
	return true;
}

void main_finish() {

	// �Ķ���� ������ ���Ͽ� �����Ѵ�.
	saveParams();
	

	delete (ConfigFileManager*)g_configFileManager;

	for (int i = 0; i < MAX_VOICE_COUNT; i++)
	{
		if (voices[i] != NULL)
		{
			delete voices[i];
			voices[i] = NULL;
		}
	}
	voiceCount = 0;
}

void raiseKeyEvent(int state, int event)
{
	if (event == NOTE_ON) {
		// �Ǹ��� ��쿡 ���ο� ��Ʈ�� �߻��ϸ� �ٸ� ��� voice���� ���δ�.
		releaseAllVoices();
		if(state >= 0)
		{
			int note = ConfigManager->noteFromState(state);
			if (note >= 0)
			{
				Voice * voice = new Voice(note);
				addVoice(voice);
			}
		}
	}
	else if(event == NOTE_OFF)
	{
		if (state >= 0)
		{
			int note = ConfigManager->noteFromState(state);
			releaseVoice(note);
		}
	}
}

void setVolumn(float volumn)
{
	g_fVol = volumn;
	if (g_fVol < 0) g_fVol = 0;
	if (g_fVol > 1) g_fVol = 1;
}

void increaseVolumn(float delta)
{
	g_fVol = g_fVol + delta;
	if (g_fVol < 0) g_fVol = 0;
	if (g_fVol > 1) g_fVol = 1;
}

float getVolumn()
{
	return g_fVol;
}



void setPortamentoDepth(double depth)
{
	g_portamentoDepth = depth;
}


double portamentoDepth()
{
	return g_portamentoDepth;
}

void setPortamentoTime(double time)
{
	g_portamentoTime = time;
}


double portamentoTime()
{
	return g_portamentoTime;
}

void main_loop() 
{
	removeEndedVoices();
	for (int i = 0; i < AUDIO_FRAME_LEN; i += FRAME_SIZE)
	{
		int frameCount = i + FRAME_SIZE > AUDIO_FRAME_LEN ? AUDIO_FRAME_LEN - i : FRAME_SIZE;
		renderVoices(frameCount, &g_outBuff[i * AUDIO_CHANNELS]);
	}
}

