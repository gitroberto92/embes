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
CA2W(��Ƽ����Ʈ���ڿ�); // A2W �� ���� ���
CW2A(�����ڵ幮�ڿ�); // W2A �� ���� ���
����)
char sz[] = "12345678901234567890123456789012345678901234567890";
TRACE(_T("%s"), CA2W(sz));

�׷���, ���� ���۰� 128 ����Ʈ�ۿ� ���� �ʱ� ������ �� �̻��� ���ڿ��� ��ȯ�ϰ��� �Ѵٸ�
��������� ���̸� ��� �Ѵ�.

CA2WEX<����>(��Ƽ����Ʈ���ڿ�); 
CW2AEX<����>(�����ڵ幮�ڿ�);
����)
char sz[] = "12345678901234567890123456789012345678901234567890"
"12345678901234567890123456789012345678901234567890"
"12345678901234567890123456789012345678901234567890"
"12345678901234567890123456789012345678901234567890"; // 200 ����Ʈ
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