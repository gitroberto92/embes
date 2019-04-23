#pragma once

class BDBSimpleInterface
{
public:
	///< Open Berkeley DB file, lpszDbPath is UTF8 string.
	virtual bool			Open(const char* lpszDbPath) = 0;

	///< Close already used db file
	virtual void			Close() = 0;

	///< Put Key/Data pair to opened DB file
	virtual bool			Write(const void* p_Key, int p_nKeyLen, const void* p_Data, int p_nDataLen) = 0;

	///< Get data with key, if there is no key, return null.
	virtual void*			Read(const void* p_Key, int p_nKeyLen, int &p_nDataLen) = 0;

	///< If p_key is exist in database, delete key/data pair and return true, else return false.
	virtual bool			DeleteKey(const void* p_Key, int p_nKeyLen) = 0;

	///< If p_key is exist in database, return true, else return false.
	virtual bool			ExistKey(const void* p_Key, int p_nKeyLen) = 0;

	///< Destroy buffer which is created here while calling Read() function.
	virtual void			DestroyBuffer(void *p_buf) = 0;

	///< If this instance is out of, please call this function.
	virtual void			Destroy() = 0;

	///< Delete all keys in BDB file.
	virtual void			DeleteAllKeys() = 0;

	virtual void*			GetFirstKey(int &p_nKeyLen) = 0;
	virtual void*			GetNextKey(int &p_nKeyLen) = 0;
};

#ifndef WIN32
#define __stdcall
#endif
BDBSimpleInterface* __stdcall GetSimpleBDBManager();