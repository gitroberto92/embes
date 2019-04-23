#pragma once

#include "WaveFile.h"

#define MAX_KEYSET_SIZE 10
#define MAX_KEY_GROUP_NAME_SIZE 20

// rec파일에 대한 정보를 기억하는 구조체
struct RecItem {
	char fileName[10];	// 파일이름
	float frequency;	// 파일의 주파수(주파수는 파일이름에 반영된다.)
	WAVE_FILE_INFO wavInfo; // wav파일정보
};

// RecItem 구조체를 list형태로 저장하기 위한 구조체
struct RecItemList{
	RecItem* item;
	void* next;
};

// 건반상태에 대응되는 노트를 저장하기 위한 구조체
struct KeySet {
	short	keyState;	// 상태
	char	note;		// 상태에 대응되는 노트번호
};

// KeySet 구조체를 list형태로 저장하기 위한 구조체
struct KeySetList {
	KeySet*	item;
	void* next;
};

struct KeySetGroup {
	char name[MAX_KEY_GROUP_NAME_SIZE];
	KeySetList* head;
};


// rec파일과 설정파일을 관리하기 위한 클라스
class ConfigFileManager
{
public:
	ConfigFileManager();
	~ConfigFileManager();

	//rec파일을 dir로부터 읽어들여 초기화를 진행한다.
	bool initRecFile(char* dir);

	// configFile로부터 설정을 읽어들인다.(포르타멘토 설정과 ADSR설정)
	bool initConfig(char* configFile);	


	// keySetFileName 로부터 keyset를 읽어들여서 m_keysethead에 보관한다.
	bool initKeySet(char* keySetFileName);

	// 파일이름으로부터 주파수를 얻어낸다.
	static float getFrequencyFromFileName(const char* fileName); 

	// 노트로부터 해당한 주파수를 얻어낸다.
	static float getFrequencyFromNote(int note);

	// 노트이름으로부터 노트번호를 얻어낸다.
	static int noteNumberFromStr(const char* noteName);

	// 노트번호로부터 이름문자렬을 얻는다.
	static void noteStrFromNumber(int noteNumber, char* outStr);

	// rec파일중에서 frequency가 제일 가까운 설정파일을 선택한다.
	RecItem* selectItemWithFrequency(float frequency);

	// rec파일중에서 노트이름과 frequency가 제일 가까운 설정파일을 선택한다.
	RecItem* selectItemWithNote(const char* Note);

	// 현재 rec파일등록부를 돌려준다.
	char* getDir();

	//현재 음조를 다음 음조로 바꾼다.
	bool changeKeySet();

	// 현재 keyset의 번호를 돌려준다.
	int currentKeySetNumber();

	// 현재 keyset에서 상태로부터 해당한 노트를 돌려준다.
	int noteFromState(int noteState);

	//모든 keysetgroup을 얻는다
	KeySetGroup** keySetGroups();

	//현재 keysetgroup을 얻는다.
	KeySetGroup* currentKeySet();

	//keySetGroup을 현재 keysetgroup으로 한다.
	void setCurrentKeySet(int idx);

	
private:
	// 새로운 rec파일을 추가한다.
	bool addNewFile(char *fn);

	// keyset를 읽어들인다.
	bool readOneKeySet(LPFile f, KeySetGroup** group);
	
	RecItemList* m_recFileHead;
	KeySetGroup* m_keySetGroups[MAX_KEYSET_SIZE];
	KeySetGroup* m_currentKeySetGroup;
	int m_currentKeySetNumber;
	int m_keySetCount;
	char m_dir[10];
};

