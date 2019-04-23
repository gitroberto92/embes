#ifndef __ULIB_BASE_TYPE_H__
#define __ULIB_BASE_TYPE_H__

#if (defined(__WIN32__) || defined(_WIN32)) && !defined(WIN32)
#define WIN32
#endif

#if defined(__ANDROID__) && !defined(ANDROID)
#define ANDROID
#endif

#if defined(__APPLE__)
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define     IPHONE
#define     IOS
#else
#define     MAC
#endif
#endif

#ifdef WIN32
#include <Windows.h>
#elif defined(ANDROID) || defined(IOS) || defined(MAC)
#include <stdio.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __APPLE__
//typedef signed char         BOOL;
#else
#define BOOL                bool
#endif
#define BOOL                bool
typedef char                CHAR;
typedef char*                PCHAR;
typedef signed char         INT8;
typedef unsigned char       UCHAR;
typedef unsigned char       UINT8;
typedef unsigned char       BYTE;
typedef short               SHORT;
typedef signed short        INT16;
typedef unsigned short      USHORT;
typedef unsigned short      UINT16;
typedef unsigned short      WORD;
typedef int                 INT;
typedef signed int          INT32;
typedef unsigned int        UINT;
typedef unsigned int        UINT32;
#ifdef __LP64__
typedef long                LONG;
typedef unsigned long       ULONG;
typedef unsigned int        DWORD;
#else
typedef long                LONG;
typedef unsigned long       ULONG;
typedef unsigned long       DWORD;
#endif
typedef int64_t             LONGLONG;
typedef int64_t             LONG64;
typedef intptr_t            INT_PTR;
typedef uintptr_t			UINT_PTR;
typedef int64_t             LONG_PTR;
typedef uint64_t			ULONG_PTR;
typedef unsigned long		DWORD_PTR;
typedef uint64_t			SSIZE_T;
typedef uint64_t			SIZE_T;
typedef int64_t				INT64;
typedef uint64_t			UINT64;
typedef char*				LPSTR;
typedef const char*			LPCSTR;
typedef const wchar_t*		LPCWSTR;
typedef void*				HMODULE;
typedef void*				HINSTANCE;
typedef void*				HANDLE;
typedef void*				LPVOID;
#ifdef __LP64__
typedef unsigned long		WPARAM;
#else
typedef unsigned int		WPARAM;
#endif
typedef unsigned long		LPARAM;
typedef BYTE*           	PBYTE;
typedef INT*           		PINT;
typedef DWORD*           	PDWORD;
typedef short*           	PSHORT;
typedef float*           	PFLOAT;
typedef void*           	VOID;

#define LOBYTE(w) ((BYTE)(((DWORD_PTR)(w)) & 0xff))
#define HIBYTE(w) ((BYTE)((((DWORD_PTR)(w)) >> 8) & 0xff))

#else
#error "The platform is not defined!"
#endif

#endif /*!__ULIB_BASE_TYPE_H__*/
