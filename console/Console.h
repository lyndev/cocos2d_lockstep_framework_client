/**************************************************************
 * 文件名: Console.h
 * 功能说明:用于控制控制台的属性，只能在windows下有效 
 * 作者：
 * 日期：2015/02/11
 * 修改记录
 * --------------------------------------------------------------------------------
 * 修改记录 :
  日期       版本    作者    内容
 **************************************************************/
#pragma once

//#include "CCPlatformConfig.h"

#if defined(_WIN32)
#include <Windows.h>
#else
#define NULL 0 
#endif //-WIN32



class CConsole
{
public:
#if defined(_WIN32)
    enum E_FORECOLOR
    {
        RED     = FOREGROUND_RED | FOREGROUND_INTENSITY,
        GREEN   = FOREGROUND_GREEN,
        WHITE   = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
        YELLOW  = FOREGROUND_GREEN | FOREGROUND_RED,
    };
#else
    enum E_FORECOLOR
    {
        RED     = 1,
        GREEN   = 2,
        WHITE   = 3,
        YELLOW  = 4,
    };
#endif //_WIN32

public:
    ~CConsole(void)
    {
#if defined(_WIN32)
        if (m_hConsole != NULL && m_hConsole != INVALID_HANDLE_VALUE)
            CloseHandle(m_hConsole);
#endif //_WIN32
    }

    static CConsole* GetInstance()
    {
        if (m_pInstance == NULL)
            m_pInstance = new CConsole();

        return m_pInstance;
    }

    bool SetColor(E_FORECOLOR clr)
    {   
#if defined(_WIN32)
        if (m_hConsole == INVALID_HANDLE_VALUE)
            return false;

        return ::SetConsoleTextAttribute(m_hConsole, clr);
#else
        return false;
#endif
    }

private:
    CConsole(void)
    {
#if defined(_WIN32)
        m_hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);
#endif
    }
private:
#if defined(_WIN32)
    HANDLE m_hConsole;
#endif //-WIN32
    static CConsole*   m_pInstance;
};

