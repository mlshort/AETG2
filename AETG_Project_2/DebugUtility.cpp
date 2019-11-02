/**
 *  @file       DebugUtility.cpp
 *  @brief      Implementation of DebugUtility.cpp
 *  @author     Mark L. Short
 *  @date       February 9, 2015
 */

#include "stdafx.h"

#include <stdlib.h>
#include <stdarg.h>
#include <Windows.h>

#include "DebugUtility.h"

#ifdef _DEBUG

    #if defined(UNICODE) || defined(_UNICODE)
        std::wofstream dbg;
    #else
        std::ofstream  dbg;
    #endif

#endif

int DebugTrace (const TCHAR* szFmt, ...) noexcept
{
    TCHAR szDebugMsg[512] = { 0 };

    va_list	vaArgs;
    va_start (vaArgs, szFmt);

    // use the format string and arguments to construct the debug output string
    int iReturnVal = _vsntprintf (szDebugMsg, 
                                  _countof (szDebugMsg) - 1, 
                                  szFmt, 
                                  vaArgs);
    va_end (vaArgs);

    ::OutputDebugString (szDebugMsg);
    return iReturnVal;

}

TCHAR* GetModulePath (TCHAR* szModulePath, size_t cchLen) noexcept
{
    TCHAR* szReturnVal = NULL;
    
    // Get the executable file path
    TCHAR szModuleFileName[_MAX_PATH] = { 0 };

    // Note, if HANDLE is NULL, GetModuleFileName is supposed to return the file
    // path to the current executable, but it appears that it is inconsistently 
    // returning filename as well....
    
    DWORD dwStrLen = ::GetModuleFileName (NULL, szModuleFileName, 
                                          _countof(szModuleFileName) );

    if (dwStrLen != 0)
    {
        TCHAR szDir[_MAX_PATH] = { 0 };

        _tsplitpath(szModuleFileName, szDir, &szDir[_MAX_DRIVE-1], NULL, NULL);

        szReturnVal = _tcsncpy(szModulePath, szDir, cchLen);
    }

    return szReturnVal;
}
