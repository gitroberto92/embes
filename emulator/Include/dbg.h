//////////////////////////////////////////////////////////////////////
// StringSafe.h: Safe String Code.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifdef __cplusplus 

#include <stdarg.h>
#include <stdio.h>

#if defined(WIN32)
#include "dbg_string.h"
namespace dbg
{
	const int  MAX_BUF_LEN = 1023;

	static void print(LPCTSTR fmt, ...)
	{
		TCHAR buf[MAX_BUF_LEN + 3];

		va_list args;
		va_start(args, fmt);

        int len = _vsctprintf(fmt, args);

		bool isAllocMem = (len > MAX_BUF_LEN);
		TCHAR *p = nullptr;
		int maxLen = 0;
		if (isAllocMem)
		{
			p = new TCHAR[len + 3];
			maxLen = len;
		}
		else
		{
			p = buf;
			maxLen = MAX_BUF_LEN;
		}

        _vstprintf_s(p, maxLen + 1, fmt, args);
		va_end(args);
		
		OutputDebugString(p);

		if (isAllocMem)
		{
			delete [] p;
		}
	}

	static void printa(LPCSTR fmt, ...)
	{
		CHAR buf[MAX_BUF_LEN + 3];

		va_list args;
		va_start(args, fmt);

		int len = _vscprintf(fmt, args);

		bool isAllocMem = (len > MAX_BUF_LEN);
		CHAR *p = nullptr;
		int maxLen = 0;
		if (isAllocMem)
		{
			p = new CHAR[len + 3];
			maxLen = len;
		}
		else
		{
			p = buf;
			maxLen = MAX_BUF_LEN;
		}

		vsprintf_s(p, maxLen + 1, fmt, args);
		va_end(args);

		strcat(p, "\r\n");
		OutputDebugStringA(p);

		if (isAllocMem)
		{
			delete [] p;
		}
	}

	static void printw(LPCWSTR fmt, ...)
	{
		WCHAR buf[MAX_BUF_LEN + 3];

		va_list args;
		va_start(args, fmt);

		int len = _vscwprintf(fmt, args);

		bool isAllocMem = (len > MAX_BUF_LEN);
		WCHAR *p = nullptr;
		int maxLen = 0;
		if (isAllocMem)
		{
			p = new WCHAR[len + 3];
			maxLen = len;
		}
		else
		{
			p = buf;
			maxLen = MAX_BUF_LEN;
		}

		vswprintf_s(p, maxLen + 1, fmt, args);
		va_end(args);

		wcscat(p, L"\r\n");
		OutputDebugStringW(p);

		if (isAllocMem)
		{
			delete [] p;
		}
	}

	static void fprinta(LPCSTR p_lpFormat, ...)
	{
		va_list w_pVars;

		va_start(w_pVars, p_lpFormat);

		CHAR w_szBuffer[MAX_BUF_LEN];
		ZeroMemory(w_szBuffer, MAX_BUF_LEN * sizeof(CHAR));

		String::vsprintf_x(w_szBuffer, MAX_BUF_LEN, p_lpFormat, w_pVars);
		String::strcat_x(w_szBuffer, MAX_BUF_LEN, "\r\n");

		String::WriteString(w_szBuffer);

		va_end(w_pVars);	
	}

	static void fprintw(LPCWSTR p_lpFormat, ...)
	{
		va_list w_pVars;

		va_start(w_pVars, p_lpFormat);

		WCHAR w_szBuffer[MAX_BUF_LEN];
		CHAR w_szBuffer2[MAX_BUF_LEN*2];
		ZeroMemory(w_szBuffer, MAX_BUF_LEN * sizeof(WCHAR));

		String::vswprintf_x(w_szBuffer, MAX_BUF_LEN, p_lpFormat, w_pVars);
		sprintf(w_szBuffer2, "%S\r\n", w_szBuffer);

		String::WriteString(w_szBuffer2);

		va_end(w_pVars);	
	}
}
#elif defined(ANDROID)
#include <android/log.h>


namespace dbg
{
	const int  MAX_BUF_LEN = 1024;

#define LOGV(...)	__android_log_print(ANDROID_LOG_VERBOSE, "native", __VA_ARGS__)
#define LOGD(...)	__android_log_print(ANDROID_LOG_DEBUG, "native", __VA_ARGS__)
#define LOGI(...)	__android_log_print(ANDROID_LOG_INFO, "native", __VA_ARGS__)
#define LOGW(...)	__android_log_print(ANDROID_LOG_WARN, "native", __VA_ARGS__)
#define LOGE(...)	__android_log_print(ANDROID_LOG_ERROR, "native", __VA_ARGS__)

	static void printa(const char* p_lpFormat, ...)
	{
		char buffer[MAX_BUF_LEN];
		va_list ap;

		va_start(ap, p_lpFormat);
		vsnprintf(buffer, sizeof(buffer), p_lpFormat, (__va_list)ap);
		va_end(ap);

		LOGV("%s", buffer);
	}

#define print 	printa
#define printw 	printa
}

#elif defined(__APPLE__)
namespace dbg
{
	static void printa(const char* format, ...)
    {
		va_list vars;
        va_start(vars, format);
        vprintf(format, vars);
        vprintf("\n",vars);
        va_end(vars);
    }

	#define print 	printa
	#define printw 	printa
}
#endif

namespace dbg
{
static const char* format_strings[] = {
	"%d",		// 32비트 옹근수
	"%I64d",	// 64비트 옹근수
	"%f",		// 32비트 소수
	"%lf",		// 64비트 소수
	"%02X",		// 8비트 hex
	"%04X",		// 16비트 hex
	"%08X",		// 32비트 hex
};

enum FORMAT_STRING_TYPE
{
	FST_binary = -1, 
	FST_integer = 0, 
	FST_long_integer, 
	FST_float, 
	FST_double, 
	FST_hex_byte, 
	FST_hex_word, 
	FST_hex_dword, 
};

#if defined(WIN32)
template <typename T>
static void PrintVector(const char *p_szFile, T* p_data, int p_nCount, FORMAT_STRING_TYPE p_Type, bool p_appendMode = false)
{
	FILE* fp = fopen(p_szFile, p_appendMode ? "ab" : "wb");
	if(!fp) return;
	if(p_Type == FST_binary)
	{
		fwrite(p_data, sizeof(T), p_nCount, fp);
	}
	else
	{
		char format[32];
		sprintf_s(format, "%s\r\n", format_strings[p_Type]);
		for (int i = 0;i < p_nCount;i++)
		{
			fprintf(fp, format, p_data[i]);
		}
	}
	fclose(fp);
}
#endif //WIN32

}
#endif //__cplusplus
