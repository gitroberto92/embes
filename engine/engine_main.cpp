
#include "engine_main.h"

#include "Voice.h"
#include "math.h"
#include "ConfigFileManager.h"

#ifdef WIN32
#include "dbg.h"
#endif

short g_outBuff[AUDIO_FRAME_LEN * AUDIO_CHANNELS];

// 최대 active인 voice의 개수
#define MAX_VOICE_COUNT 4
Voice* voices[MAX_VOICE_COUNT];

// config파일 관리클라스 인스턴스
void* g_configFileManager;

// 현재 voice의 개수
int voiceCount;

// 현재 볼륨
float g_fVol = 1;

// 포르타멘토 깊이
double g_portamentoDepth = 0.5;

// 포르타멘토 시간
double g_portamentoTime = 0.5;

// 포르타멘토의 리용에 대한 기발변수
bool g_usePortamento = true;

// ADSR의 Attack 시간
float g_attackTime = 0.05f;

// ADSR의 Release 시간
float g_releaseTime = 0.05f;

// ADSR의 Release준위
float g_releaseLevel = 0.5f;

// 현재 Rec파일등록부의 번호
// 번호는 00-99까지의 두자리수로 표현된다.
int g_currentDirNumber;

// Rec파일등록부를 설정한다.
bool setRecDirectory(char* dir)
{
	return ConfigManager->initRecFile(dir);
}

char* recDirectory()
{
	return ConfigManager->getDir();
}

// 파라메터를 config.txt파일에 보관한다.
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

// 전에 나타났던 voice
Voice* g_enginPrevVoice;

// 초기에 모든 voice를 NULL로 초기화한다.
void initVoices() 
{
	for (int i = 0; i < MAX_VOICE_COUNT; i++)
		voices[i] = NULL;
}

// 끝난 voice들을 delete한다.
void removeEndedVoices() 
{

	for (int i = 0; i < MAX_VOICE_COUNT; i++)
	{
		if (voices[i] != NULL && voices[i]->ended())
		{
			// 만일 delete하여야 할 voice가 전의 voice인 경우에 g_enginPrevVoice를 NULL로 설정
			if (g_enginPrevVoice != NULL && voices[i]->note() == g_enginPrevVoice->note())
				g_enginPrevVoice = NULL;

			delete voices[i];
			voices[i] = NULL;
			--voiceCount;
		}
	}

}

// key에 해당하는 voice를 release상태에로 보낸다.
void releaseVoice(int key) {
	for (int i = 0; i < MAX_VOICE_COUNT; i++)
	{
		if (voices[i] != NULL && voices[i]->note() == key)
			voices[i]->release();
	}
}

// 모든 voice를 release상태에로 보낸다.
void releaseAllVoices()
{
	for (int i = 0; i < MAX_VOICE_COUNT; i++)
	{
		if (voices[i] != NULL)
			voices[i]->release();
	}
}


// A와 B사이에서 로그비률로 ratio인 값을 얻어낸다.
double getLogRatioBetween(double A, double B, double ratio) {
	return A * pow((B/A), ratio);
}

// 새로운 voice를 추가한다.
// 이때 voices중에 빈 자리가 없으면 voice중에서 수준이 최소인 voice를 삭제하고 그 자리에 추가한다.
void addVoice(Voice* voice)
{
	// 시작주파수
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
		if (voices[i] == NULL) // 빈자리가 있으면 거기에 추가 한다.
		{
			voices[i] = voice;
			return;
		}
		float lvl = voices[i]->getLevel();
		if (minLevel > lvl) { // level이 최소인 voice를 선택한다
			minLevel = lvl;
			minIdx = i;
		}

	}
	delete voices[minIdx];
	--voiceCount;
	voices[minIdx] = voice;
}


// framecount만큼의 출력을 buff에 얻어낸다.
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

	// 설정초기화
	g_configFileManager = new ConfigFileManager();
	ConfigManager->initKeySet("keyset.txt");
#ifdef __DEBUG
	printf("init config.txt\r\n");
#endif
	
	if (!ConfigManager->initConfig("config.txt")) // keyset설정을 진행한다음에 호출되여야 한다.
	{
		return false;
	}
#ifdef __DEBUG
	printf("init config.txt success\r\n");
#endif

	
	g_currentDirNumber = atoi(ConfigManager->getDir() + 3); // config.txt파일에서 설정을 불러들인 다음에 호출되여야 한다.

	// voice초기화
	initVoices();
	g_enginPrevVoice = NULL;

	g_buffInit = true;
	return true;
}

void main_finish() {

	// 파라메터 설정을 파일에 보관한다.
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
		// 피리인 경우에 새로운 노트가 발생하면 다른 모든 voice들을 죽인다.
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

