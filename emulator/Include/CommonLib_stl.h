#pragma once

#ifdef WIN32

// ��������
#include "../GeneralLibrary/CommonLib/file_std.h"

// ���ڷ� ó�� ����
#include "../GeneralLibrary/CommonLib/strmgr_std.h"

// XML ����
#include "../GeneralLibrary/CommonLib/xml/TinyXML/tinyxml.h"

#pragma comment(lib, "CommonLib_stl.lib")

// ����üũ
#include "../GeneralLibrary/CommonLib/TimeCheck.h"

// OS ��������
#include "../GeneralLibrary/CommonLib/osversion.h"

// XML ����
#include "../GeneralLibrary/CommonLib/xml/XML_Plus/xml.h"

#include "../GeneralLibrary/CommonLib/xml/rapidxml/rapidxml_utils.hpp"
#include "../GeneralLibrary/CommonLib/xml/rapidxml/rapidxml_print.hpp"

// ���ڷ�Ű �ؽ����̺��ڷ�
#include "../GeneralLibrary/CommonLib/StrMapData.h"

// ���μ����� �����޸�
#include "../GeneralLibrary/CommonLib/ProcShareMem.h"

// R/W ������
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

