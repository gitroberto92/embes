#ifndef __ULIB_UNICODE_H__
#define __ULIB_UNICODE_H__

#include "ulib_basestrtype.h"

static size_t ULIB_Utf8ToUnicode(const char* szUtf8, utf16_t* szUnicode, size_t maxLenUnicode)
{
	if (szUtf8 == nullptr || szUnicode == nullptr || maxLenUnicode < 1)
	{
		return 0;
	}

	utf16_t* p16 = szUnicode;
	const char* p8 = szUtf8;
	utf16_t* end = szUnicode + maxLenUnicode - 1;

	while( *p8 != 0 )
	{
		if (p16 + 1 > end)
		{
			break;
		}

		if( ( 0xe0 == ( p8[0] & 0xe0 ) ) )
		{
			*p16 = ( ( p8[0] & 0x0f ) << 12 ) | 
				( ( p8[1] & 0x3f ) << 6 ) | 
				( p8[2] & 0x3f );

			p8 += 3;
		} 
		else if( 0xc0 == ( p8[0] & 0xc0 ) )
		{
			*p16 = ( ( p8[0] & 0x1f ) << 6 ) |
				( p8[1] & 0x3f );

			p8 += 2;
		} 
		else
		{
			*p16 = (p8[0] & 0x7f);

			p8++;
		}

		p16++;
	}
	*p16 = 0;

	return size_t(p16 - szUnicode);
}

static size_t ULIB_UnicodeToUtf8(const utf16_t* szUnicode, char* szUtf8, size_t maxLenUtf8)
{
	if (szUnicode == nullptr || szUtf8 == nullptr || maxLenUtf8 < 1)
	{
		return 0;
	}

	utf16_t c16;
	const utf16_t* p16 = szUnicode;
	char* p8 = szUtf8;
	char* end = szUtf8 + maxLenUtf8 - 1;

	while((c16 = *p16) != 0)
	{
		if( c16 < 0x80 )
		{
			if (p8 + 1 > end)
			{
				break;
			}
			p8[0] = (char)c16;
			p8++;
		} 
		else if( c16 < 0x800 )
		{
			if (p8 + 2 > end)
			{
				break;
			}
			p8[1] = ( c16 & 0x3f ) | 0x80;
			p8[0] = ( (( c16 << 2 ) & 0xcf00) | 0xc000) >> 8;
			p8 += 2;
		}
		else
		{
			if (p8 + 3 > end)
			{
				break;
			}
			p8[2] = ( c16 & 0x3f ) | 0x80;
			p8[1] = (char)(( (( c16 << 2 ) & 0x3f00) | 0x8000) >> 8);
			p8[0] = (char)(( (( c16 << 4 ) & 0x3f0000) | 0xe00000) >> 16);
			p8 += 3;
		}

		p16++;
	}

	p8[0] = 0;

	return size_t(p8 - szUtf8);
}

#endif
