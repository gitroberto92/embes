#pragma once

#ifdef WIN32

// 파일조작
#include "../GeneralLibrary/CommonLib/file_std.h"

// 문자렬 처리 관련
#include "../GeneralLibrary/CommonLib/strmgr_std.h"

// XML 관련
#include "../GeneralLibrary/CommonLib/xml/TinyXML/tinyxml.h"

#pragma comment(lib, "CommonLib_stl.lib")

// 날자체크
#include "../GeneralLibrary/CommonLib/TimeCheck.h"

// OS 버전관련
#include "../GeneralLibrary/CommonLib/osversion.h"

// XML 관련
#include "../GeneralLibrary/CommonLib/xml/XML_Plus/xml.h"

#include "../GeneralLibrary/CommonLib/xml/rapidxml/rapidxml_utils.hpp"
#include "../GeneralLibrary/CommonLib/xml/rapidxml/rapidxml_print.hpp"

// 문자렬키 해시테이블자료
#include "../GeneralLibrary/CommonLib/StrMapData.h"

// 프로세스간 고유메모리
#include "../GeneralLibrary/CommonLib/ProcShareMem.h"

// R/W 링버퍼
#include "RWBuffer.h"

#else
#include "../GeneralLibrary/CommonLib/file_std.h"
#include "../GeneralLibrary/CommonLib/strmgr_std.h"
// #include "../GeneralLibrary/CommonLib/xml/TinyXML/tinyxml.h"
// #include "../GeneralLibrary/CommonLib/xml/XML_Plus/xml.h"
// #include "../GeneralLibrary/CommonLib/xml/rapidxml/rapidxml_utils.hpp"
// #include "../GeneralLibrary/CommonLib/xml/rapidxml/rapidxml_print.hpp"
#include "RWBuffer.h"
#endif

