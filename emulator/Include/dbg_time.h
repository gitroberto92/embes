//////////////////////////////////////////////////////////////////////
// dbg_time.h
//
//////////////////////////////////////////////////////////////////////
#pragma once
#ifdef __cplusplus 
#if defined(WIN32)
#include "dbg_string.h"
#elif defined(ANDROID)
#include <android/log.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#endif

namespace dbg
{
	const int  _maxBufferLen = 10240;

#if defined(WIN32)
	static void fprinta_time(LPCSTR p_lpFormat, ...)
	{
		va_list w_pVars;

		va_start(w_pVars, p_lpFormat);

		CHAR w_szBuffer[_maxBufferLen];

		SYSTEMTIME tm;
		GetLocalTime(&tm);
		sprintf(w_szBuffer, "[%d][%d]%02d-%02d %02d:%02d:%02d.%03d\t", GetCurrentProcessId(), GetCurrentThreadId(), tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);
		String::WriteString(w_szBuffer);

		ZeroMemory(w_szBuffer, _maxBufferLen * sizeof(CHAR));


		String::vsprintf_x(w_szBuffer, _maxBufferLen, p_lpFormat, w_pVars);
		String::strcat_x(w_szBuffer, _maxBufferLen, "\r\n");


		String::WriteString(w_szBuffer);

		va_end(w_pVars);	
	}

	static void printa_time(LPCSTR p_lpFormat, ...)
	{
		va_list w_pVars;

		va_start(w_pVars, p_lpFormat);

		CHAR w_szBuffer[_maxBufferLen];

		SYSTEMTIME tm;
		GetLocalTime(&tm);
		sprintf(w_szBuffer, "[%d][%d]%02d-%02d %02d:%02d:%02d.%03d\t", GetCurrentProcessId(), GetCurrentThreadId(), tm.wMonth, tm.wDay, tm.wHour, tm.wMinute, tm.wSecond, tm.wMilliseconds);

		String::WriteString(w_szBuffer);

		ZeroMemory(w_szBuffer, _maxBufferLen * sizeof(CHAR));

		String::vsprintf_x(w_szBuffer, _maxBufferLen, p_lpFormat, w_pVars);
		String::strcat_x(w_szBuffer, _maxBufferLen, "\r\n");

		OutputDebugStringA(w_szBuffer);

		va_end(w_pVars);
	}

#endif
}

#endif //__cplusplus