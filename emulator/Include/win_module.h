/** 

	Copyright (c) 2013 PSJDC.

	@file win_module.h

	@date 2013-4-8

	@brief

		window module 을 위한 유틸 함수.
*/

#pragma once

#pragma warning(disable : 4996)
// DLL API (recommand dynamic using)

#ifdef _WINDOWS
namespace win
{

	// Useful functions for Init. Option
	static HMODULE GetModule(const char* p_ModuleName, const char* p_baseFolder = nullptr)
	{
		HMODULE hModule = NULL;

		char	w_wszModuleFilePath[1024] = "";
		char*	w_pwszFind;

		if(p_baseFolder == nullptr || p_baseFolder[0] == 0)
		{
			if (GetModuleFileNameA(NULL, w_wszModuleFilePath, 1024) == 0)
				return NULL;

			w_pwszFind = strrchr(w_wszModuleFilePath, '\\');
			if (w_pwszFind)
				w_pwszFind[1] = 0;
		}
		else
		{
			strcpy(w_wszModuleFilePath, p_baseFolder);
			strcat(w_wszModuleFilePath, "\\");
		}

		strcat(w_wszModuleFilePath, p_ModuleName);

		hModule = LoadLibraryA(w_wszModuleFilePath);

		return hModule;
	}

	static HMODULE GetModule(LPCWSTR p_lpwzModuleName, LPCWSTR p_lpcwBaseDir = nullptr)
	{
		HMODULE hModule = NULL;

		wchar_t	w_wszModuleFilePath[1024] = {0};
		wchar_t*	w_pwszFind;

		if(p_lpcwBaseDir == nullptr || p_lpcwBaseDir[0] == 0)
		{
			if (GetModuleFileNameW(NULL, w_wszModuleFilePath, 1024) == 0)
				return NULL;

			w_pwszFind = wcsrchr(w_wszModuleFilePath, L'\\');
			if (w_pwszFind)
				w_pwszFind[1] = 0;
		}
		else
		{
			wcscpy_s(w_wszModuleFilePath, p_lpcwBaseDir);
			wcscat_s(w_wszModuleFilePath, L"\\");
		}

		wcscat_s(w_wszModuleFilePath, p_lpwzModuleName);

		hModule = LoadLibraryW(w_wszModuleFilePath);

		return hModule;
	}
}
#endif
