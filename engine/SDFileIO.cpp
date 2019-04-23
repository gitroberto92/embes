#include "SDFileIO.h"
#include <stdio.h>

LPFile _fopen_(const char *fn, FOPEN_TYPE type)
{
	if (type == FT_READ) {
#ifdef WIN32
		return fopen(fn, "rb");
#else
		FIL* fil = new FIL;
		//return SD.open(fn);
		
		FRESULT ret = f_open(fil,(const TCHAR*)fn,FA_READ | FA_OPEN_EXISTING);
		if(ret != FR_OK)
		{
			delete fil;
#ifdef __DEBUG
			printf("File open for read error: %s, %d\r\n", fn, ret);
#endif
			return NULL;
		}
		return fil;
#endif
	}
	else {
#ifdef WIN32
		return fopen(fn, "wb");
#else
		FIL* fil = new FIL;
		FRESULT ret = f_open(fil,(const TCHAR*)fn,FA_WRITE | FA_CREATE_ALWAYS);
		if(ret != FR_OK)
		{
			delete fil;
#ifdef __DEBUG
			printf("File open for write error: %s, %d\r\n", fn, ret);
#endif
			return NULL;
		}
		return fil;
#endif
	}
}

bool _fread_(void* buf, int blockSize, int toRead, LPFile pf)
{
#ifdef WIN32
	return fread(buf, blockSize, toRead, pf) == toRead;
#else
	UINT read;
	FRESULT ret = f_read(pf, buf, blockSize * toRead, &read);
	if(ret != FR_OK || read != blockSize * toRead){
#ifdef __DEBUG
		printf("File read error: %d\r\n", ret);
#endif
		return false;
	}
	return read == toRead;
#endif
}

int _fwrite_(void* buf, int blockSize, int toWrite, LPFile pf)
{
#ifdef WIN32
	return fwrite(buf, blockSize, toWrite, pf);
#else
	UINT write;
	FRESULT ret = f_write(pf, buf, blockSize * toWrite, &write);
	if(ret != FR_OK || write != blockSize * toWrite){
#ifdef __DEBUG
		printf("File write error: %d\r\n", ret);
#endif
		return -1;
	}

	return write;

#endif
}

char _getchar_(LPFile pf)
{
#ifdef WIN32
	return fgetc(pf);
#else
 char c;
	UINT read;
	FRESULT ret = f_read(pf, &c, 1, &read);
	if(ret != FR_OK || read != 1){
#ifdef __DEBUG
		printf("File read char error: %d\r\n", ret);
#endif
		return 0;
	}
 return c;
#endif
}

char* _fgets_(char* buf, int len, LPFile pf)
{
#ifdef WIN32
	return fgets(buf, len, pf);
#else
	TCHAR* ret = f_gets((TCHAR*)buf, len, pf);
	return ret;
#endif

}

void _fclose_(LPFile pf)
{
#ifdef WIN32
	fclose(pf);
#else
	FRESULT ret = f_close(pf);
    if(ret != FR_OK){
#ifdef __DEBUG
		printf("File close: %d\r\n", ret);
#endif
	}
	delete pf;

#endif
}

bool isDir(char* path)
{
#ifdef WIN32
	HANDLE hFind;
	WIN32_FIND_DATAA data;

	char searchStr[20];
	sprintf(searchStr, "%s/*.wav", path);

	hFind = FindFirstFileA(searchStr, &data);
	if (hFind != INVALID_HANDLE_VALUE) {
		return true;
	}
	else {
		return false;
	}
#else
	FRESULT res;
	DIR dir;
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
		return true;
	}
#endif
}

bool _fseek_(LPFile pf, int pos)
{
#ifdef WIN32
	return fseek(pf, pos, SEEK_SET) == 0;
#else
	//return pf.seek(pos);
	FRESULT ret = f_lseek(pf, pos);
	//printf("seek position:%d\r\n", pos);
	if(ret != FR_OK){
#ifdef __DEBUG
		printf("File seek error: %d\r\n", ret);
#endif
	}
  return true;
#endif
}




