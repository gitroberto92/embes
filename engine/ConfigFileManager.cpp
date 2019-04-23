#include "ConfigFileManager.h"
#include "string.h"
#include "math.h"
#include "stdio.h"
#include "SDFileIO.h"
#include "engine_main.h"
#ifdef WIN32
#include <windows.h>
#else
#include "fatfs.h"
#endif


// 노트의 이름
const char noteNames[12][3] = {
	"C",
	"C#",
	"D",
	"D#",
	"E",
	"F",
	"F#",
	"G",
	"G#",
	"A",
	"A#",
	"B",
};

// 주파수를 계산하기 위한 상수
const float coffFreq = pow(2, 1.f / 12);

ConfigFileManager::ConfigFileManager()
{
	m_recFileHead = NULL;
	for (int i = 0; i < MAX_KEYSET_SIZE; i++)
		m_keySetGroups[i] = NULL;
	m_currentKeySetNumber = 0;
}

ConfigFileManager::~ConfigFileManager()
{
	RecItemList* nextItem;
	for (RecItemList* lstItem = m_recFileHead; lstItem != NULL; lstItem = nextItem) {
		nextItem = (RecItemList*)lstItem->next;
		delete lstItem->item;
		delete lstItem;
	}

	for (int i = 0; i < MAX_KEYSET_SIZE; i++) {
		KeySetList* nxtItem;
		if (m_keySetGroups[i] != NULL)
		{
			for (KeySetList* lstItem = m_keySetGroups[i]->head; lstItem != NULL; lstItem = nxtItem) {
				nxtItem = (KeySetList*)lstItem->next;
				delete lstItem->item;
				delete lstItem;
			}
			delete m_keySetGroups[i];
			m_keySetGroups[i] = NULL;
		}
	}
}

bool ConfigFileManager::initRecFile(char* path)
{
	strcpy(m_dir, path);
#ifdef WIN32
	HANDLE hFind;
	WIN32_FIND_DATAA data;

	char searchStr[20];
	sprintf(searchStr, "%s/*.wav", path);

	hFind = FindFirstFileA(searchStr, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			addNewFile(data.cFileName);
		} while (FindNextFileA(hFind, &data));
		FindClose(hFind);
		return true;
	}
	else {
		return false;
	}
#else
	uint8_t i;
	FRESULT res;
	DIR dir;
	FILINFO fno;
	printf("Rec directory:%s.\r\n", path);
	res = f_opendir(&dir, path);   /* Open the directory */
	if (res != FR_OK)
	{
#ifdef __DEBUG
		printf("Failed to open dir:%s.\r\n", path);
#endif
		return false;
	}
	else
	{
		bool ret = false;
		for (;;)
		{
			res = f_readdir(&dir, &fno);                   					/* Read a directory item */
			if (res != FR_OK || fno.fname[0] == 0)  								/* Break on error or end of dir */
				break;
			if ((fno.fname[0] == '.') || ((fno.fattrib & AM_DIR) != 0))
				continue;																							/* Ignore dot entry and directory*/
			for (i = 0; i < 13; i++)
			{
				if (fno.fname[i] == '.')
					break;
			}
			if (i > 8)
				continue;
			if (((fno.fname[i + 1] == 'w') || (fno.fname[i + 1] == 'W'))
				&& ((fno.fname[i + 2] == 'a') || (fno.fname[i + 2] == 'A'))
				&& ((fno.fname[i + 3] == 'v') || (fno.fname[i + 3] == 'V')))
			{
				addNewFile(fno.fname);
				ret = true;
#ifdef __DEBUG
				printf("add rec file: %s\r\n", fno.fname);
#endif
			}
		}
		f_closedir(&dir);
		return ret;
	}
#endif
}

float ConfigFileManager::getFrequencyFromFileName(const char* fileName)
{
	int len;
	if (fileName[1] == '_')
		len = 1;
	else
		len = 2;

	if (fileName[len] != '_')
		return 0;

	int baseKey = -1;
	for (int i = 0; i < 12; i++) {
		if (strncmp(noteNames[i], fileName, len) == 0) {
			baseKey = i;
			break;
		}
	}
	if (baseKey < 0)
		return 0;
	int octave = fileName[len + 1] - '0';
	int key = baseKey + (octave - 4) * 12;
	return pow(coffFreq, key) * 440.f;
}

float ConfigFileManager::getFrequencyFromNote(int note)
{
	return pow(coffFreq, note - 60) * 440.f; // 60 = C4
}

int ConfigFileManager::noteNumberFromStr(const char* noteName)
{
	int len;
	if (noteName[1] == '_')
		len = 1;
	else
		len = 2;

	if (noteName[len] != '_')
		return 0;

	int baseKey = -1;
	for (int i = 0; i < 12; i++) {
		if (strncmp(noteNames[i], noteName, len) == 0) {
			baseKey = i;
			break;
		}
	}
	if (baseKey < 0)
		return 0;
	int octave = noteName[len + 1] - '0';
	return baseKey + (octave + 1) * 12;
}

void ConfigFileManager::noteStrFromNumber(int noteNumber, char* outStr)
{
	sprintf(outStr, "%s_%d", noteNames[noteNumber % 12], noteNumber / 12 - 1);
}

RecItem* ConfigFileManager::selectItemWithFrequency(float frequency)
{
	RecItem* ret = NULL;
	float minDiff = 1e8;
	for (RecItemList* lstItem = m_recFileHead; lstItem != NULL; lstItem = (RecItemList*)lstItem->next)
	{
		float diff = fabs(lstItem->item->frequency - frequency);
		if (diff < minDiff) {
			minDiff = diff;
			ret = lstItem->item;
		}
	}
	return ret;
}



RecItem* ConfigFileManager::selectItemWithNote(const char* Note)
{
	float freq = getFrequencyFromFileName(Note);
	return selectItemWithFrequency(freq);
}

char* ConfigFileManager::getDir()
{
	return m_dir;
}

bool ConfigFileManager::changeKeySet()
{
	m_currentKeySetGroup = NULL;
	for(int i = 0; i < MAX_KEYSET_SIZE; i++)
	{
		m_currentKeySetNumber = (m_currentKeySetNumber + 1) % MAX_KEYSET_SIZE;
		if (m_keySetGroups[m_currentKeySetNumber] != NULL) {
			m_currentKeySetGroup = m_keySetGroups[m_currentKeySetNumber];
			break;
		}
	}

	if (m_currentKeySetGroup == NULL)
		return false;
	return true;
	
}

int ConfigFileManager::currentKeySetNumber()
{
	return m_currentKeySetNumber;
}

int ConfigFileManager::noteFromState(int noteState)
{
	for (KeySetList* lstItem = m_currentKeySetGroup->head; lstItem != NULL; lstItem = (KeySetList*)lstItem->next)
	{
		if (lstItem->item->keyState == noteState)
			return lstItem->item->note;
	}
	return -1;
}

KeySetGroup** ConfigFileManager::keySetGroups()
{
	return m_keySetGroups;
}

KeySetGroup* ConfigFileManager::currentKeySet()
{
	return m_currentKeySetGroup;
}

void ConfigFileManager::setCurrentKeySet(int idx)
{
	if (idx < 0 || idx > MAX_KEYSET_SIZE)
		return;
	m_currentKeySetGroup = m_keySetGroups[idx];
	m_currentKeySetNumber = idx;
}

bool ConfigFileManager::addNewFile(char *fn)
{
	float frequency = getFrequencyFromFileName(fn);
	if (frequency == 0)
		return false;
	RecItem* item = new RecItem;
	strcpy(item->fileName, fn);
	item->frequency = frequency;
	RecItemList* newItem = new RecItemList;
	newItem->item = item;
	newItem->next = m_recFileHead;
	m_recFileHead = newItem;

	char rfn[20];
	sprintf(rfn, "%s/%s", ConfigManager->getDir(), fn);
	if (ReadPcmWaveFileInfo(rfn, &item->wavInfo) != WF_ERR_NOERROR) {
		return false;
	}
	return true;
}

bool ConfigFileManager::readOneKeySet(LPFile f, KeySetGroup** group)
{
	if (*group == NULL) {
		*group = new KeySetGroup;
		(*group)->head = NULL;
	}

	char buf[100];
	char keyS[NUM_KEYS + 1];
	char noteStr[5];

	while (1) {
		buf[0] = 1;
		_fgets_(buf, 100, f);
		if (buf[0] == 1) {
			*group = NULL;
			delete (*group);
			return false;
		}
		if (strlen(buf) > 2) break;
	}

	strcpy((*group)->name, buf);

	while (1) {
		buf[0] = 0;
		_fgets_(buf, 100, f);
		if (strlen(buf) <= 2)
			break;
#ifdef __DEBUG
		//printf("Read buff:%s\r\n", buf);
#endif

		sscanf(buf, "%s %s", keyS, noteStr);
		KeySet* newKeySet = new KeySet;
		newKeySet->keyState = 0;
		for (int i = 0; i < NUM_KEYS && keyS[i] != 0; i++)
		{
			if (keyS[i] == '1')
				newKeySet->keyState |= (1 << i);
		}
		newKeySet->note = noteNumberFromStr(noteStr);
		KeySetList* newKeyList = new KeySetList;
		newKeyList->item = newKeySet;
		newKeyList->next = (*group)->head;
		(*group)->head = newKeyList;
	}
	
#ifdef __DEBUG
	printf("Read one keyset\r\n");
#endif
	return true;
}

bool ConfigFileManager::initConfig(char* configFile)
{
	LPFile f = _fopen_(configFile, FT_READ);
	if (f == NULL)
		return false;
	char buf[40];
	char confName[20];
	char value[10];
	float v;

	_fgets_(buf, 40, f);
	sscanf(buf, "%s = %s", confName, value);
	if (strcmp(confName, "RecDirectory") != 0)
		return false;
	setRecDirectory(value);

	_fgets_(buf, 40, f);
	sscanf(buf, "%s = %f", confName, &v);
	if (strcmp(confName, "PortamentoDepth") != 0)
		return false;
	setPortamentoDepth(v);

	_fgets_(buf, 40, f);
	sscanf(buf, "%s = %f", confName, &v);
	if (strcmp(confName, "PortamentoTime") != 0)
		return false;
	setPortamentoTime(v);

	_fgets_(buf, 40, f);
	sscanf(buf, "%s = %s", confName, value);
	if (strcmp(confName, "PortamentoFlag") != 0)
		return false;
	setUsePortamento(value[0] == 't');

	_fgets_(buf, 40, f);
	sscanf(buf, "%s = %f", confName, &v);
	if (strcmp(confName, "AttackTime") != 0)
		return false;
	setAttackTime(v);

	_fgets_(buf, 40, f);
	sscanf(buf, "%s = %f", confName, &v);
	if (strcmp(confName, "ReleaseTime") != 0)
		return false;
	setReleaseTime(v);

	_fgets_(buf, 40, f);
	sscanf(buf, "%s = %f", confName, &v);
	if (strcmp(confName, "ReleaseLevel") != 0)
		return false;
	setReleaseLevel(v);

	int n;
	_fgets_(buf, 40, f);
	sscanf(buf, "%s = %d", confName, &n);
	if (strcmp(confName, "CurrentKeySet") != 0)
		return false;

	m_currentKeySetNumber = n;
	m_currentKeySetGroup = m_keySetGroups[n];

	return true;
}

bool ConfigFileManager::initKeySet(char* keySetFileName)
{
	LPFile f = _fopen_(keySetFileName, FT_READ);
	if (f == NULL)
		return false;
	m_keySetCount = 0;
	for(int i = 0; i < 2; i++){
		readOneKeySet(f, &m_keySetGroups[m_keySetCount]);
		m_keySetCount++;
	}
	return true;
}

