#pragma once

#include "WaveFile.h"

#define MAX_KEYSET_SIZE 10
#define MAX_KEY_GROUP_NAME_SIZE 20

// rec���Ͽ� ���� ������ ����ϴ� ����ü
struct RecItem {
	char fileName[10];	// �����̸�
	float frequency;	// ������ ���ļ�(���ļ��� �����̸��� �ݿ��ȴ�.)
	WAVE_FILE_INFO wavInfo; // wav��������
};

// RecItem ����ü�� list���·� �����ϱ� ���� ����ü
struct RecItemList{
	RecItem* item;
	void* next;
};

// �ǹݻ��¿� �����Ǵ� ��Ʈ�� �����ϱ� ���� ����ü
struct KeySet {
	short	keyState;	// ����
	char	note;		// ���¿� �����Ǵ� ��Ʈ��ȣ
};

// KeySet ����ü�� list���·� �����ϱ� ���� ����ü
struct KeySetList {
	KeySet*	item;
	void* next;
};

struct KeySetGroup {
	char name[MAX_KEY_GROUP_NAME_SIZE];
	KeySetList* head;
};


// rec���ϰ� ���������� �����ϱ� ���� Ŭ��
class ConfigFileManager
{
public:
	ConfigFileManager();
	~ConfigFileManager();

	//rec������ dir�κ��� �о�鿩 �ʱ�ȭ�� �����Ѵ�.
	bool initRecFile(char* dir);

	// configFile�κ��� ������ �о���δ�.(����Ÿ���� ������ ADSR����)
	bool initConfig(char* configFile);	


	// keySetFileName �κ��� keyset�� �о�鿩�� m_keysethead�� �����Ѵ�.
	bool initKeySet(char* keySetFileName);

	// �����̸����κ��� ���ļ��� ����.
	static float getFrequencyFromFileName(const char* fileName); 

	// ��Ʈ�κ��� �ش��� ���ļ��� ����.
	static float getFrequencyFromNote(int note);

	// ��Ʈ�̸����κ��� ��Ʈ��ȣ�� ����.
	static int noteNumberFromStr(const char* noteName);

	// ��Ʈ��ȣ�κ��� �̸����ڷ��� ��´�.
	static void noteStrFromNumber(int noteNumber, char* outStr);

	// rec�����߿��� frequency�� ���� ����� ���������� �����Ѵ�.
	RecItem* selectItemWithFrequency(float frequency);

	// rec�����߿��� ��Ʈ�̸��� frequency�� ���� ����� ���������� �����Ѵ�.
	RecItem* selectItemWithNote(const char* Note);

	// ���� rec���ϵ�Ϻθ� �����ش�.
	char* getDir();

	//���� ������ ���� ������ �ٲ۴�.
	bool changeKeySet();

	// ���� keyset�� ��ȣ�� �����ش�.
	int currentKeySetNumber();

	// ���� keyset���� ���·κ��� �ش��� ��Ʈ�� �����ش�.
	int noteFromState(int noteState);

	//��� keysetgroup�� ��´�
	KeySetGroup** keySetGroups();

	//���� keysetgroup�� ��´�.
	KeySetGroup* currentKeySet();

	//keySetGroup�� ���� keysetgroup���� �Ѵ�.
	void setCurrentKeySet(int idx);

	
private:
	// ���ο� rec������ �߰��Ѵ�.
	bool addNewFile(char *fn);

	// keyset�� �о���δ�.
	bool readOneKeySet(LPFile f, KeySetGroup** group);
	
	RecItemList* m_recFileHead;
	KeySetGroup* m_keySetGroups[MAX_KEYSET_SIZE];
	KeySetGroup* m_currentKeySetGroup;
	int m_currentKeySetNumber;
	int m_keySetCount;
	char m_dir[10];
};

