
#pragma once

#include "dbg.h"
#include "windows.h"

static long long m_performanceFreq = 0;

class BlockTimeLogger
{
public:
	BlockTimeLogger(const char* p_title = "", bool p_end_print = false)
	{
		LARGE_INTEGER li;

		m_title = p_title;
		m_end_print = p_end_print;

		if (m_performanceFreq == 0)
		{
			QueryPerformanceFrequency(&li);
			m_performanceFreq = li.QuadPart;
			dbg::printa("QueryPerformanceFrequency = %I32d", m_performanceFreq);
		}
		m_startCount = QueryPerformanceCounter(&li);
		m_startCount = li.QuadPart;
	}
	~BlockTimeLogger()
	{
		if(m_end_print)
		{
			LARGE_INTEGER li;
			QueryPerformanceCounter(&li);
			m_endCount = li.QuadPart;

			dbg::printa("%s : %.3fms", m_title.c_str(), (double)(m_endCount - m_startCount) / m_performanceFreq * 1000);
		}
	}

	// ȣ��ð��� üũ�ϰ� ���ۺ����� ����ð��� ms������ ��ȯ.
	// ���ð��� ȣ��ð����� �ٲ۴�.
	double Check()
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		m_endCount = li.QuadPart;

		double t = (double)(m_endCount - m_startCount) / m_performanceFreq * 1000;
		m_startCount = m_endCount;
		return t;
	}

private:
	//static long long m_performanceFreq;
	std::string m_title;
	bool m_end_print;
	long long m_startCount;
	long long m_endCount;
};


