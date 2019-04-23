//
// ez.h
//
// useful functions, no dependency, header only.
// [2013/11/15 lhs]

#pragma once

#ifdef __cplusplus 

#ifdef WIN32
#include <windows.h>
#include <tchar.h>

#ifdef EZ_NO_ATL
#include <atlconv.h>
#pragma comment(lib, "atls.lib")

/*
http://sonbosun.tistory.com/6
CA2W(멀티바이트문자열); // A2W 와 같은 기능
CW2A(유니코드문자열); // W2A 와 같은 기능
예제)
char sz[] = "12345678901234567890123456789012345678901234567890";
TRACE(_T("%s"), CA2W(sz));

그러나, 내부 버퍼가 128 바이트밖에 되질 않기 때문에 그 이상의 문자열을 변환하고자 한다면
명시적으로 길이를 줘야 한다.

CA2WEX<길이>(멀티바이트문자열); 
CW2AEX<길이>(유니코드문자열);
예제)
char sz[] = "12345678901234567890123456789012345678901234567890"
"12345678901234567890123456789012345678901234567890"
"12345678901234567890123456789012345678901234567890"
"12345678901234567890123456789012345678901234567890"; // 200 바이트
TRACE(_T("%s"), CA2WEX<1024>(sz));
*/

#endif

#endif

#include <stdarg.h>
#include <stdio.h>

namespace ez {

#ifdef WIN32
const TCHAR* GetProcessDir()
{
	static TCHAR buf[MAX_PATH];

	buf[0] = 0;

	if (GetModuleFileName(NULL, buf, MAX_PATH) != 0) {
		TCHAR* p = _tcsrchr(buf, _T('\\'));
		if (p != NULL) {
			*p = 0;
		}
	}

	return buf;
}
#endif

}

#endif //__cplusplus