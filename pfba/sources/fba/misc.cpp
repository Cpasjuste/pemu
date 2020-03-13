//
// Created by cpasjuste on 13/03/2020.
//

#include <cmath>
#include "burner.h"

#define _tcsrchr       strrchr

TCHAR *ExtractFilename(TCHAR *fullname) {
    TCHAR *filename = fullname + _tcslen(fullname);

    do {
        filename--;
    } while (filename >= fullname && *filename != _T('\\') && *filename != _T('/') && *filename != _T(':'));

    return filename;
}

// StrLower() - leaves str untouched, returns modified string
TCHAR *StrLower(TCHAR *str) {
    static TCHAR szBuffer[256] = _T("");
    INT32 length = _tcslen(str);

    if (length > 255) length = 255;

    for (INT32 i = 0; i < length; i++) {
        if (str[i] >= _T('A') && str[i] <= _T('Z'))
            szBuffer[i] = (str[i] + _T(' '));
        else
            szBuffer[i] = str[i];
    }
    szBuffer[length] = 0;

    return &szBuffer[0];
}

TCHAR *LabelCheck(TCHAR *s, TCHAR *pszLabel) {
    INT32 nLen;
    if (s == NULL) {
        return NULL;
    }
    if (pszLabel == NULL) {
        return NULL;
    }
    nLen = _tcslen(pszLabel);

    SKIP_WS(s);                                                    // Skip whitespace

    if (_tcsncmp(s, pszLabel, nLen)) {                            // Doesn't match
        return NULL;
    }
    return s + nLen;
}

TCHAR *FileExt(TCHAR *str) {
    TCHAR *dot = _tcsrchr(str, _T('.'));

    return (dot) ? StrLower(dot) : str;
}

bool IsFileExt(TCHAR *str, TCHAR *ext) {
    return (_tcsicmp(ext, FileExt(str)) == 0);
}
