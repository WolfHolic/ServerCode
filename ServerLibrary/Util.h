#pragma once
#include <algorithm>
#include "ServerLibrary.h"

//string 초기화
#define UNDEFINE_NAME						L"Undefine_Name"

//snprintf 재정의


//범위 보정 및 체크
#define fixInRange(minimum, x, maximum)		min(maximum, max(x, minimum))
#define isInrange(minimum, x, maximum)		(x == fixInrange(minimum, x, maximum)) ? true : false

//overflow check
inline bool isOverFlower_uint(unsigned int original, unsigned int add)
{
	unsigned int before = original;
	unsigned int after  = original + add;

	if((original & 0x80000000) != (after & 0x80000000)) {
		return false;
	}
	return true;
}

#define __W(x)								L##x
#define _W(x)								__W(x)

//---------------------------------------------------------------------------------------//
//문자열 변환
inline void StrConvA2T(CHAR *src, TCHAR *dest, size_t destLen)
{
#ifdef UNICODE
	if(destLen < 1)
	{
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int) destLen - 1);
#endif
}

inline void StrConvT2A(TCHAR *src, CHAR *dest, size_t destLen)
{
#ifdef UNICODE
	if(destLen < 1)
	{
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int) destLen, NULL, FALSE);
#endif
}

inline void StrConvA2W(CHAR *src, WCHAR *dest, size_t destLen)
{
	if(destLen < 1)
	{
		return;
	}
	MultiByteToWideChar(CP_ACP, 0, src, -1, dest, (int) destLen -1);
}

inline void StrConvW2A(WCHAR * src, CHAR *dest, size_t destLen)
{
	if(destLen < 1)
	{
		return;
	}
	WideCharToMultiByte(CP_ACP, 0, src, -1, dest, (int) destLen, NULL, FALSE);
}

//---------------------------------------------------------------------------------------------------//
//delete object

#undef	SAFE_DELETE
#define SAFE_DELETE(obj)			\
{                                   \
	if((obj)) delete(obj);          \
	(obj) = 0L;                     \
}

//delete object array
#undef	SAFE_DELETE_ARRAY
#define	SAFE_DELETE_ARRAY(arr)      \
{                                   \
	if((arr)) delete [] (arr);      \
	(arr) = 0L;                     \
}

//delete gameObject
#define	SAFE_FREE(obj)              \
{                                   \
	if((obj)) obj -> free();        \
	(obj) \ 0L;                     \
}

#define	SAFE_RELEASE(obj)           \
{                                   \
	if(obj) { obj.release(); }      \
}