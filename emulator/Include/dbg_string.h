//////////////////////////////////////////////////////////////////////
// StringSafe.h: Safe String Code.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#pragma warning( disable : 4996) // asm 관련 경고를 무시한다.

#ifdef USE_FILE_LOG
	// 파일로그 남기기
	#define DEBUG_PRINT		STRING::String::FilePrintA	
#else
	#ifdef USE_OUTPUT_LOG
		// 출력창로그 남기기
		#define DEBUG_PRINT		STRING::String::DebugPrintA	
	#else
		// 파일로그 소스상에서 없애기
		#define DEBUG_PRINT	
		#define NOT_USE_LOG
	#endif
#endif

#ifdef NOT_USE_LOG
	#undef DEBUG_PRINT	
	#define DEBUG_PRINT	
#endif

namespace dbg
{
class String
{
public:
	///< 상수_
	enum Const 
	{
		maxBufferLen = 10240
	};

	/**
	   @breif 문자열 출력의 안전코드.
	   
	   @return 복사한 문자열의 길이.
	   @warning WCHAR/CHAR/TCHAR
	*/
        static int vsprintf_x(char *dst, int dstLen, const char *p_lpFormat, va_list w_pVars)
	{
		if (dstLen <= 0) return 0;
		if (p_lpFormat == NULL || dst == NULL) return 0;

		// 최대 dstLen-1 길이의 문자를 복사할수 있다.

		int nLen = _vscprintf(p_lpFormat, w_pVars);
		// 
		// 디버그 출력 내용 복사
		// 
		if(nLen < dstLen)
		{
			// 원천의 길이가 긴 경우 뒤부분을 잘라버린다.
			vsprintf(dst, p_lpFormat, w_pVars);
		}
		else
		{
			// 원천의 길이 작은 경우
			nLen = strcpy_x(dst, dstLen, "Too long print.");
		}

		return nLen;
	}

        static int vswprintf_x(WCHAR *dst, int dstLen, const WCHAR *p_lpFormat, va_list w_pVars )
	{
		if (dstLen <= 0) return 0;
		if (p_lpFormat == NULL || dst == NULL) return 0;

		// 최대 dstLen-1 길이의 문자를 복사할수 있다.

		int nLen = _vscwprintf(p_lpFormat, w_pVars);
		// 
		// 디버그 출력 내용 복사
		// 
		if(nLen < dstLen)
		{
			// 원천의 길이가 긴 경우 뒤부분을 잘라버린다.
			_vswprintf(dst, p_lpFormat, w_pVars);
		}
		else
		{
			// 원천의 길이 작은 경우
			nLen = wcscpy_x(dst, dstLen, L"Too long print.");
		}


		return nLen;
	}

        static int vstprintf_x(LPTSTR dst, int dstLen, LPCTSTR p_lpFormat, va_list w_pVars)
	{
		int nLen;

#ifdef UNICODE
		nLen = vswprintf_x(dst, dstLen, p_lpFormat, w_pVars);
#else
		nLen = vsprintf_x(dst, dstLen, p_lpFormat, w_pVars);
#endif
		return nLen;
	}

	/**
	   @breif 문자열 복사의 안전코드.
	   
	   @return 복사한 문자열의 길이.
	   @warning wchar, char, tchar
	*/
        static int strcpy_x(char *dst, int dstLen, IN const char *src)
	{
		// 최대 dstLen-1 길이의 문자를 복사할수 있다.
		if (dstLen <= 0) return 0;
		if (src == 0 || dst == 0) return 0;
		int nLen = (int)strlen(src);
		if(nLen < dstLen)
		{
			// 원천의 길이 작은 경우
			memcpy(dst, src, nLen + 1);
		}
		else
		{
			// 원천의 길이가 긴 경우 뒤부분을 잘라버린다.
			memcpy(dst, src, dstLen-1);
			dst[dstLen-1] = 0;
			nLen = dstLen-1;
		}
		return nLen;
	}
        static int wcscpy_x(LPWSTR dst, int dstLen, LPCWSTR src)
	{
		// 최대 dstLen-1 길이의 문자를 복사할수 있다.
		if (dstLen <= 0) return 0;
		if (src == 0 || dst == 0) return 0;
		int nLen = (int)wcslen(src);
		if(nLen < dstLen)
		{
			// 원천의 길이 작은 경우
			memcpy(dst, src, (nLen + 1)  *sizeof(WCHAR));
		}
		else
		{
			// 원천의 길이가 긴 경우 뒤부분을 잘라버린다.
			memcpy(dst, src, (dstLen - 1)  *sizeof(WCHAR));
			dst[dstLen - 1] = 0;
			nLen = dstLen - 1;
		}
		return nLen;
	}

        static int tcscpy_x(LPTSTR dst, int dstLen, LPCTSTR src)
	{
#ifdef UNICODE
		return wcscpy_x(dst, dstLen, src);
#else
		return strcpy_x(dst, dstLen, src);
#endif
	}

	/**
	   @breif 안전한 문자열 붙이기
	   
	   @return	복사한 문자열의 길이
	   @warning WCHAR/CHAR/TCHAR
	*/
        static int strcat_x(char *dst, int dstLen, const char *src)
	{
		int len = (int)strlen(dst);
		return strcpy_x(dst + len, dstLen - len, src);
	}

        static int wcscat_x(WCHAR *dst, int dstLen, const WCHAR *src)
	{
		int len = (int)wcslen(dst);
		return wcscpy_x(dst + len, dstLen - len, src);
	}

        static int tcscat_x(LPTSTR dst, int dstLen, LPCTSTR src)
	{
//		int len = (int)_tcslen(dst);
//		return tcscpy_x(dst + len, dstLen - len, src);
	}

	/**
	   @breif 디버그 출력을 진행한다.
	   
	   @return 
	   @warning 
	*/
        static void DebugPrintW( const WCHAR* p_lpFormat, ... )
	{
		va_list w_pVars;

		va_start(w_pVars, p_lpFormat);

		WCHAR w_szBuffer[maxBufferLen];
		ZeroMemory(w_szBuffer, maxBufferLen * sizeof(WCHAR));

		String::vswprintf_x(w_szBuffer, maxBufferLen, p_lpFormat, w_pVars);
		String::wcscat_x(w_szBuffer, maxBufferLen, L"\r\n");

		OutputDebugStringW(w_szBuffer);

		va_end(w_pVars);
	}

        static void DebugPrintA( const CHAR* p_lpFormat, ... )
	{
		va_list w_pVars;

		va_start(w_pVars, p_lpFormat);

		CHAR w_szBuffer[maxBufferLen];
		ZeroMemory(w_szBuffer, maxBufferLen * sizeof(CHAR));

		String::vsprintf_x(w_szBuffer, maxBufferLen, p_lpFormat, w_pVars);
		String::strcat_x(w_szBuffer, maxBufferLen, "\r\n");

		OutputDebugStringA(w_szBuffer);

		va_end(w_pVars);
	}

        static void DebugPrintPosA(char* p_pszFile, int p_nLine)
	{
		char	w_szBuffer[maxBufferLen + 1];

		ZeroMemory(w_szBuffer, maxBufferLen * sizeof(char));

		sprintf_s(w_szBuffer, maxBufferLen + 1, "%s(%d) : ", p_pszFile, p_nLine);

		OutputDebugStringA((const char*)w_szBuffer);
	}

        static void WriteString(const CHAR* p_lpString,  const CHAR* p_lpFilePath = NULL)
	{
		if(p_lpFilePath == NULL)
			p_lpFilePath = "dbg.log";
		FILE* pf = fopen(p_lpFilePath, "ab");
		if(pf != NULL)
		{
			fseek(pf, 0, FILE_END);
			fwrite(p_lpString, 1, strlen(p_lpString), pf);

			fclose(pf);
		}

	}

        static void FilePrintA( const CHAR* p_lpFormat, ... )
	{
		va_list w_pVars;

		va_start(w_pVars, p_lpFormat);

		CHAR w_szBuffer[maxBufferLen];
		ZeroMemory(w_szBuffer, maxBufferLen * sizeof(CHAR));

		String::vsprintf_x(w_szBuffer, maxBufferLen, p_lpFormat, w_pVars);
		String::strcat_x(w_szBuffer, maxBufferLen, "\r\n");

		WriteString(w_szBuffer);

		va_end(w_pVars);
	}

        static void DebugPrint( LPCTSTR p_lpFormat, ... )
	{
		va_list w_pVars;

		va_start(w_pVars, p_lpFormat);

#ifdef UNICODE
		DebugPrintW( p_lpFormat, w_pVars );
#else
		DebugPrintA( p_lpFormat, w_pVars );
#endif

		va_end(w_pVars);
	}
};

}
