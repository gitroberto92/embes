#ifndef __ULIB_BASE_STR_TYPE_H__
#define __ULIB_BASE_STR_TYPE_H__

#if (defined(__WIN32__) || defined(_WIN32)) && !defined(WIN32)
#define WIN32
#endif

#if defined(__ANDROID__) && !defined(ANDROID)
#define ANDROID
#endif

#if defined __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
#define     IPHONE
#define     IOS
#else
#define     MAC
#endif
#endif

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
#include <string>
#endif

#if defined(WIN32)
#ifdef __cplusplus
#define string_u	std::wstring
#endif
#define char_u		wchar_t
typedef wchar_t		utf16_t;

#define _UTEXT(x)		L##x

#define _ucscat         wcscat
#define _ucscat_s       wcscat_s
#define _ucschr         wcschr
#define _ucscpy         wcscpy
#define _ucscpy_s       wcscpy_s
#define _ucscspn        wcscspn
#define _ucslen         wcslen
#define _ucsnlen        wcsnlen
#define _ucsncat        wcsncat
#define _ucsncat_s      wcsncat_s
#define _ucsncat_l      _wcsncat_l
#define _ucsncat_s_l    _wcsncat_s_l
#define _ucsncpy        wcsncpy
#define _ucsncpy_s      wcsncpy_s
#define _ucsncpy_l      _wcsncpy_l
#define _ucsncpy_s_l    _wcsncpy_s_l
#define _ucspbrk        wcspbrk
#define _ucsrchr        wcsrchr
#define _ucsspn         wcsspn
#define _ucsstr         wcsstr
#define _ucstok         wcstok
#define _ucstok_s       wcstok_s
#define _ucstok_l       _wcstok_l
#define _ucstok_s_l     _wcstok_s_l
#define _ucserror       _wcserror
#define _ucserror_s     _wcserror_s
#define __ucserror      __wcserror
#define __ucserror_s    __wcserror_s
#define _ucscmp			wcscmp

#define _ucsdup         _wcsdup
#define _ucsnset        _wcsnset
#define _ucsnset_s      _wcsnset_s
#define _ucsnset_l      _wcsnset_l
#define _ucsnset_s_l    _wcsnset_s_l
#define _ucsrev         _wcsrev
#define _ucsset         _wcsset
#define _ucsset_s       _wcsset_s
#define _ucsset_l       _wcsset_l
#define _ucsset_s_l     _wcsset_s_l

#define _ucscmp         wcscmp
#define _ucsicmp        _wcsicmp
#define _ucsicmp_l      _wcsicmp_l
#define _ucsnccmp       wcsncmp
#define _ucsncmp        wcsncmp
#define _ucsncicmp      _wcsnicmp
#define _ucsncicmp_l    _wcsnicmp_l
#define _ucsnicmp       _wcsnicmp
#define _ucsnicmp_l     _wcsnicmp_l

#define _ucscoll        wcscoll
#define _ucscoll_l      _wcscoll_l
#define _ucsicoll       _wcsicoll
#define _ucsicoll_l     _wcsicoll_l
#define _ucsnccoll      _wcsncoll
#define _ucsnccoll_l    _wcsncoll_l
#define _ucsncoll       _wcsncoll
#define _ucsncoll_l     _wcsncoll_l
#define _ucsncicoll     _wcsnicoll
#define _ucsncicoll_l   _wcsnicoll_l
#define _ucsnicoll      _wcsnicoll
#define _ucsnicoll_l    _wcsnicoll_l

#define _uprintf        wprintf
#define _uprintf_l      _wprintf_l
#define _uprintf_s      wprintf_s
#define _uprintf_s_l    _wprintf_s_l
#define _uprintf_p      _wprintf_p
#define _uprintf_p_l    _wprintf_p_l
#define _ucprintf       _cwprintf
#define _ucprintf_l     _cwprintf_l
#define _ucprintf_s     _cwprintf_s
#define _ucprintf_s_l   _cwprintf_s_l
#define _ucprintf_p     _cwprintf_p
#define _ucprintf_p_l   _cwprintf_p_l
#define _vucprintf      _vcwprintf
#define _vucprintf_l    _vcwprintf_l
#define _vucprintf_s    _vcwprintf_s
#define _vucprintf_s_l  _vcwprintf_s_l
#define _vucprintf_p    _vcwprintf_p
#define _vucprintf_p_l  _vcwprintf_p_l
#define _fuprintf       fwprintf
#define _fuprintf_l     _fwprintf_l
#define _fuprintf_s     fwprintf_s
#define _fuprintf_s_l   _fwprintf_s_l
#define _fuprintf_p     _fwprintf_p
#define _fuprintf_p_l   _fwprintf_p_l
#define _suprintf       _swprintf
#define _suprintf_l     __swprintf_l
#define _suprintf_s     swprintf_s
#define _suprintf_s_l   _swprintf_s_l
#define _suprintf_p     _swprintf_p
#define _suprintf_p_l   _swprintf_p_l
#define _scuprintf      _scwprintf
#define _scuprintf_l    _scwprintf_l
#define _scuprintf_p    _scwprintf_p
#define _scuprintf_p_l  _scwprintf_p_l
#define _snuprintf      _snwprintf
#define _snuprintf_l    _snwprintf_l
#define _snuprintf_s    _snwprintf_s
#define _snuprintf_s_l  _snwprintf_s_l
#define _vuprintf       vwprintf
#define _vuprintf_l     _vwprintf_l
#define _vuprintf_s     vwprintf_s
#define _vuprintf_s_l   _vwprintf_s_l
#define _vuprintf_p     _vwprintf_p
#define _vuprintf_p_l   _vwprintf_p_l
#define _vfuprintf      vfwprintf
#define _vfuprintf_l    _vfwprintf_l
#define _vfuprintf_s    vfwprintf_s
#define _vfuprintf_s_l  _vfwprintf_s_l
#define _vfuprintf_p    _vfwprintf_p
#define _vfuprintf_p_l  _vfwprintf_p_l
#define _vsuprintf      vswprintf
#define _vsuprintf_l    _vswprintf_l
#define _vsuprintf_s    vswprintf_s
#define _vsuprintf_s_l  _vswprintf_s_l
#define _vsuprintf_p    _vswprintf_p
#define _vsuprintf_p_l  _vswprintf_p_l
#define _vscuprintf     _vscwprintf
#define _vscuprintf_l   _vscwprintf_l
#define _vscuprintf_p   _vscwprintf_p
#define _vscuprintf_p_l _vscwprintf_p_l
#define _vsnuprintf     _vsnwprintf
#define _vsnuprintf_l   _vsnwprintf_l
#define _vsnuprintf_s   _vsnwprintf_s
#define _vsnuprintf_s_l _vsnwprintf_s_l

#define _uscanf         wscanf
#define _uscanf_l       _wscanf_l
#define _uscanf_s       wscanf_s
#define _uscanf_s_l     _wscanf_s_l
#define _ucscanf        _cwscanf
#define _ucscanf_l      _cwscanf_l
#define _ucscanf_s      _cwscanf_s
#define _ucscanf_s_l    _cwscanf_s_l
#define _fuscanf        fwscanf
#define _fuscanf_l      _fwscanf_l
#define _fuscanf_s      fwscanf_s
#define _fuscanf_s_l    _fwscanf_s_l
#define _suscanf        swscanf
#define _suscanf_l      _swscanf_l
#define _suscanf_s      swscanf_s
#define _suscanf_s_l    _swscanf_s_l
#define _snuscanf       _snwscanf
#define _snuscanf_l     _snwscanf_l
#define _snuscanf_s     _snwscanf_s
#define _snuscanf_s_l   _snwscanf_s_l

#define _fuopen   _wfopen
#define _usopen   _wsopen

#else // defined(WIN32)
#ifdef __cplusplus
#define string_u		std::string
#endif
#define char_u			char
typedef unsigned short	utf16_t;

#define _UTEXT(x)		x

#define _ucscat     strcat
#define _ucscpy     strcpy
#define _ucsdup     _surdup
#define _ucslen     strlen
#define _ucsnlen    strnlen
#define _ucsrchr	strrchr
#define _ucsxfrm    strxfrm
#define _ucsxfrm_l  _surxfrm_l
#define _ucserror   strerror
#define __ucserror  _surerror
#define _ucscmp		strcmp

#define _uprintf        printf
#define _uprintf_l      _printf_l
#define _uprintf_s      printf_s
#define _uprintf_s_l    _printf_s_l
#define _uprintf_p      _printf_p
#define _uprintf_p_l    _printf_p_l
#define _ucprintf       _cprintf
#define _ucprintf_l     _cprintf_l
#define _ucprintf_s     _cprintf_s
#define _ucprintf_s_l   _cprintf_s_l
#define _ucprintf_p     _cprintf_p
#define _ucprintf_p_l   _cprintf_p_l
#define _vucprintf      _vcprintf
#define _vucprintf_l    _vcprintf_l
#define _vucprintf_s    _vcprintf_s
#define _vucprintf_s_l  _vcprintf_s_l
#define _vucprintf_p    _vcprintf_p
#define _vucprintf_p_l  _vcprintf_p_l
#define _fuprintf       fprintf
#define _fuprintf_l     _fprintf_l
#define _fuprintf_s     fprintf_s
#define _fuprintf_s_l   _fprintf_s_l
#define _fuprintf_p     _fprintf_p
#define _fuprintf_p_l   _fprintf_p_l
#define _suprintf       sprintf
#define _suprintf_l     _sprintf_l
#define _suprintf_s     sprintf_s
#define _suprintf_s_l   _sprintf_s_l
#define _suprintf_p     _sprintf_p
#define _suprintf_p_l   _sprintf_p_l
#define _scuprintf      _scprintf
#define _scuprintf_l    _scprintf_l
#define _scuprintf_p    _scprintf_p
#define _scuprintf_p_l  _scprintf_p_l
#define _snuprintf      _snprintf
#define _snuprintf_l    _snprintf_l
#define _snuprintf_s    _snprintf_s
#define _snuprintf_s_l  _snprintf_s_l
#define _vuprintf       vprintf
#define _vuprintf_l     _vprintf_l
#define _vuprintf_s     vprintf_s
#define _vuprintf_s_l   _vprintf_s_l
#define _vuprintf_p     _vprintf_p
#define _vuprintf_p_l   _vprintf_p_l
#define _vfuprintf      vfprintf
#define _vfuprintf_l    _vfprintf_l
#define _vfuprintf_s    vfprintf_s
#define _vfuprintf_s_l  _vfprintf_s_l
#define _vfuprintf_p    _vfprintf_p
#define _vfuprintf_p_l  _vfprintf_p_l
#define _vsuprintf      vsprintf
#define _vsuprintf_l    _vsprintf_l
#define _vsuprintf_s    vsprintf_s
#define _vsuprintf_s_l  _vsprintf_s_l
#define _vsuprintf_p    _vsprintf_p
#define _vsuprintf_p_l  _vsprintf_p_l
#define _vscuprintf     _vscprintf
#define _vscuprintf_l   _vscprintf_l
#define _vscuprintf_p   _vscprintf_p
#define _vscuprintf_p_l _vscprintf_p_l
#define _vsnuprintf     _vsnprintf
#define _vsnuprintf_l   _vsnprintf_l
#define _vsnuprintf_s   _vsnprintf_s
#define _vsnuprintf_s_l _vsnprintf_s_l

#define _uscanf         scanf
#define _uscanf_l       _scanf_l
#define _uscanf_s       scanf_s
#define _uscanf_s_l     _scanf_s_l
#define _ucscanf        _cscanf
#define _ucscanf_l      _cscanf_l
#define _ucscanf_s      _cscanf_s
#define _ucscanf_s_l    _cscanf_s_l
#define _fuscanf        fscanf
#define _fuscanf_l      _fscanf_l
#define _fuscanf_s      fscanf_s
#define _fuscanf_s_l    _fscanf_s_l
#define _suscanf        sscanf
#define _suscanf_l      _sscanf_l
#define _suscanf_s      sscanf_s
#define _suscanf_s_l    _sscanf_s_l
#define _snuscanf       _snscanf
#define _snuscanf_l     _snscanf_l
#define _snuscanf_s     _snscanf_s
#define _snuscanf_s_l   _snscanf_s_l

#define _fuopen   fopen
#define _usopen   _sopen
#endif

typedef const char_u* LPCUSTR;

#endif /*!__ULIB_BASE_STR_TYPE_H__*/
