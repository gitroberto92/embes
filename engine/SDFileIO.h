#ifndef SDMANAGER
#define SDMANAGER


#ifndef WIN32
#ifdef __cplusplus
extern "C"
{
#endif

#endif
	
	
enum FOPEN_TYPE {
	FT_READ,
	FT_WRITE,
};

#ifdef WIN32
#include <stdio.h>
#include <windows.h>
typedef FILE* LPFile;
#define _fprintf_ fprintf
#else
#include "fatfs.h"
typedef FIL* LPFile;
#define _fprintf_ f_printf
#endif

LPFile _fopen_(const char *fn, FOPEN_TYPE type);

bool _fseek_(LPFile pf, int pos);
bool _fread_(void* buf, int blockSize, int toRead, LPFile pf);
int _fwrite_(void* buf, int blockSize, int toWrite, LPFile pf);
char _getchar_(LPFile pf);
char* _fgets_(char* buf, int len, LPFile pf);
void _fclose_(LPFile pf);
bool isDir(char* path);


#ifndef WIN32
#ifdef __cplusplus
}
#endif
#endif

#endif
