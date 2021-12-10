#ifndef _TCHAR_H_
#define _TCHAR_H_

#include <stdint.h>

#ifdef UNICODE
#include <wchar.h>
#define __TEXT(q)	L##q

typedef wchar_t TCHAR;
typedef wchar_t _TCHAR;

#else

#define __TEXT(q)	q

typedef char TCHAR;

// WTF? This is a normal string function!
// damn linux. :)
//#define wcslen(void)
#include <cwchar>

#define _tcslen     strlen
#define _tcscpy     strcpy
#define _tcsncpy    strncpy

#define _tprintf    printf
#define _vstprintf  vsprintf
#define _vsntprintf vsnprintf
#define _stprintf   sprintf
#define _sntprintf  snprintf
#define _ftprintf   fprintf
#define _tsprintf   sprintf

#define _tcscmp     strcmp
#define _tcsncmp    strncmp
#define _tcsicmp    strcasecmp
#define _tcsnicmp   strncasecmp
#define _tcstol     strtol
#define _tcsstr     strstr

#define _fgetts     fgets
#define _fputts     fputs

#define	_istspace	isspace

#define _tfopen     fopen

#define _stricmp strcmp
#define _strnicmp strncmp

#ifdef WIN32
#define strnlen(A, B) (strlen(A))
#endif

// FBA function, change this!
#define dprintf printf

#endif // UNICODE

#define _TEXT(x)	__TEXT(x)
#define	_T(x)		__TEXT(x)

TCHAR* ANSIToTCHAR(const char* pszInString, TCHAR* pszOutString, int nOutSize);
char* TCHARToANSI(const TCHAR* pszInString, char* pszOutString, int nOutSize);

#endif //_TCHAR_H_
