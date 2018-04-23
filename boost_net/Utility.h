/****************************************************************************
作者: 
功能: 常用头文件及宏定义
其他: 这里不允许添加游戏相关的头文件
****************************************************************************/

#pragma once 
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <map>
#include <list>
#include <vector>
#include <deque>
#include <string>
#include <time.h>
#include <algorithm>
#include <boostlibs/boost/thread.hpp>
#include "boostlibs/boost/date_time.hpp"
#include "platform/CCStdC.h"
////////////////////// 常用宏 //////////////////////
#ifdef _DEBUG
#define new DEBUG_NEW
#undef new
#endif

#ifdef COCOS2D_DEBUG
#define MEMORY(msg) CMemoryMonitor::GetInstance()->PrintMemory(msg);
#else
#define MEMORY(msg) ;
#endif

// 常用资源释放相关宏
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)          { if (p) { delete(p); (p) = NULL;} }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)    { if (p) { delete[] (p); (p) = NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)         { if (p) { (p)->Release(); (p) = NULL; } }
#endif

// 异常宏
#ifndef BREAK_IF
#define BREAK_IF(exp, log) if (exp){ LOGE(log); break; }
#endif

//宏开关
#define US_FRAME_FILE_SPR   // 定义此宏，则资源将从plist中加载
#define PIXEL_PER_GRID    (25)
#define MAX_PATH          260
#define DEFINESTRING(AB)    #AB
#define TEXTURE_FILE(AB)   DEFINESTRING(AB.pvr.ccz)
#define TEXTURE_FILE_CCZ(AB) DEFINESTRING(AB.pvr.ccz)
#define HALF_VALUE(VALUE) ((VALUE) >> 1)    // 对整形除2

// 定义最佳窗口大小
const float gResolution[2] = {1024, 768};
#define RESOLUTION_WIDTH (gResolution[0])
#define RESOLUTION_HEIGHT (gResolution[1])

#if defined(_MSC_VER) || defined(__BORLANDC__)
typedef unsigned __int64 INT_64;
#else
typedef unsigned long long int INT_64;
#endif


typedef INT_64				int64;
typedef int					int32;
typedef short				int16;
typedef char				int8;
typedef INT_64	            uint64;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		uint8;
typedef std::string			String;
typedef const std::string	CString;
typedef unsigned char		byte;

#ifndef GAME_SERVER
#if (defined(WIN32) && defined(_WINDOWS))
typedef int                 BOOL;
#else
typedef signed char		BOOL; 
#endif
#endif // !GAME_SERVER

typedef unsigned long       DWORD;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef float               FLOAT;
typedef FLOAT               *PFLOAT;
#ifndef GAME_SERVER
typedef BOOL                *PBOOL;
typedef BOOL                *LPBOOL;
#endif // !GAME_SERVER
typedef BYTE                *PBYTE;
typedef BYTE                *LPBYTE;
typedef int                 *PINT;
typedef int                 *LPINT;
typedef WORD                *PWORD;
typedef WORD                *LPWORD;
typedef long                *LPLONG;
typedef DWORD               *PDWORD;
typedef DWORD               *LPDWORD;
typedef void                *LPVOID;
typedef const void          *LPCVOID;

typedef signed char         INT8, *PINT8;
typedef signed short        INT16, *PINT16;
typedef unsigned char       UINT8, *PUINT8;
typedef unsigned short      UINT16, *PUINT16;

typedef int                 INT;
typedef unsigned int        UINT;
typedef unsigned int        *PUINT;

typedef long                LONG;

typedef std::vector<WORD>				WORD_VECTOR;
typedef std::vector<int>				INT_VECTOR;
typedef std::vector<DWORD>				DWORD_VECTOR;
typedef std::vector<std::string>		STRING_VECTOR;
typedef std::vector<BYTE>				BYTE_VECTOR;
typedef std::vector<float>				FLOAT_VECTOR;

#if !defined MAKEWORD
#define MAKEWORD(a, b)      ((WORD)(((BYTE)(((DWORD)(a)) & 0xff)) | ((WORD)((BYTE)(((DWORD)(b)) & 0xff))) << 8))
#endif

#if !defined MAKELONG
#define MAKELONG(a, b)      ((LONG)(((WORD)(((DWORD)(a)) & 0xffff)) | ((DWORD)((WORD)(((DWORD)(b)) & 0xffff))) << 16))
#endif

#if !defined LOWORD
#define LOWORD(l)           ((WORD)(((DWORD)(l)) & 0xffff))
#endif

#if !defined HIWORD
#define HIWORD(l)           ((WORD)((((DWORD)(l)) >> 16) & 0xffff))
#endif

#if !defined LOBYTE
#define LOBYTE(w)           ((BYTE)(((DWORD)(w)) & 0xff))
#endif

#if !defined HIBYTE
#define HIBYTE(w)           ((BYTE)((((DWORD)(w)) >> 8) & 0xff))
#endif


// 颜色控制宏定义
#ifndef COLOR_ARGB
#define COLOR_ARGB(a,r,g,b)		((DWORD(a) << 24) + (DWORD(r) << 16) + (DWORD(g) << 8) + DWORD(b))
#endif
#ifndef COLOR_GETA
#define COLOR_GETA(col)			((col) >> 24)
#endif
#ifndef COLOR_GETR
#define COLOR_GETR(col)			(((col) >> 16) & 0xFF)
#endif
#ifndef COLOR_GETG
#define COLOR_GETG(col)			(((col) >> 8) & 0xFF)
#endif
#ifndef COLOR_GETB
#define COLOR_GETB(col)			((col) & 0xFF)
#endif
#ifndef COLOR_SETA
#define COLOR_SETA(col,a)		(((col) & 0x00FFFFFF) + (DWORD(a) << 24))
#endif
#ifndef COLOR_SETR
#define COLOR_SETR(col,r)		(((col) & 0xFF00FFFF) + (DWORD(r) << 16))
#endif
#ifndef COLOR_SETG
#define COLOR_SETG(col,g)		(((col) & 0xFFFF00FF) + (DWORD(g) << 8))
#endif
#ifndef COLOR_SETB
#define COLOR_SETB(col,b)		(((col) & 0xFFFFFF00) + DWORD(b))
#endif

#ifndef GAME_SERVER
#ifndef INT_COLOR_TO_COCOS_COLOR
#define INT_COLOR_TO_COCOS_COLOR(col) cocos2d::ccc3(COLOR_GETR(col), COLOR_GETG(col), COLOR_GETB(col))
#endif
#endif

//
// 添加快速声明属性的宏 [13-02-19 Joeson]
// 参数varType: 属性类型，如std::string
// 参数varName: 变量名，如m_strName
// 参数fnPostfix: 函数后缀，如Name，函数形式：Get[fnPostfix]/Set[fnPostfix]
// 如: DECLARE_ATTRIBUTE_PASSREF(std::string, m_strName, Name);
// 
// 声明辅助宏，外部勿用
#define __DECLARE_ATTRIBUTE_NONUSED__(varType, varName, getter, setter, setterAcc)                      \
                                      public:    inline varType getter() const    { return varName; }   \
                                      setterAcc: inline void    setter(varType v) { varName = v; }      \
                                      private:   varType varName;
#define __DECLARE_ATTRIBUTE_NONUSED_R__(varType, varName, getter, setter, setterAcc)                                \
                                      public:    inline varType const& getter() const           { return varName; } \
                                      setterAcc: inline void           setter(varType const& v) { varName = v; }    \
                                      private:   varType varName;

// 公共可访问属性public: Get[fnPostfix], Set[fnPostfix], private: varType varName;
#define DECLARE_ATTRIBUTE(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED__(varType, varName, Get##fnPostfix, Set##fnPostfix, public)
// Set/Get函数以常引用传递
#define DECLARE_ATTRIBUTE_PASSREF(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED_R__(varType, varName, Get##fnPostfix, Set##fnPostfix, public)

// 只读属性 public: Get[fnPostfix](); protected: _Set[fnPostfix]; private: varType varName;
#define DECLARE_ATTRIBUTE_PROTECT(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED__(varType, varName, Get##fnPostfix, _Set##fnPostfix, protected)
#define DECLARE_ATTRIBUTE_PASSREF_PROTECT(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED_R__(varType, varName, Get##fnPostfix, _Set##fnPostfix, protected)

// 只读属性 public: Get[fnPostfix](); private: _Set[fnPostfix]; private: varType varName;
#define DECLARE_ATTRIBUTE_PRIVATE(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED__(varType, varName, Get##fnPostfix, _Set##fnPostfix, private)
#define DECLARE_ATTRIBUTE_PASSREF_PRIVATE(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED_R__(varType, varName, Get##fnPostfix, _Set##fnPostfix, private)

// bool型属性 getter形式是: Is[fnPostfix]
#define DECLARE_ATTRIBUTE_IS(varType, varName, fnPostfix)  \
            __DECLARE_ATTRIBUTE_NONUSED__(varType, varName, Is##fnPostfix, Set##fnPostfix, public)
#define DECLARE_ATTRIBUTE_PROTECT_IS(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED_R__(varType, varName, Is##fnPostfix, _Set##fnPostfix, protected)
#define DECLARE_ATTRIBUTE_PRIVATE_IS(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED_R__(varType, varName, Is##fnPostfix, _Set##fnPostfix, private)

// bool型属性 getter形式是: Can[fnPostfix]
#define DECLARE_ATTRIBUTE_CAN(varType, varName, fnPostfix)  \
            __DECLARE_ATTRIBUTE_NONUSED__(varType, varName, Can##fnPostfix, Set##fnPostfix, public)
#define DECLARE_ATTRIBUTE_PROTECT_CAN(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED_R__(varType, varName, Can##fnPostfix, _Set##fnPostfix, protected)
#define DECLARE_ATTRIBUTE_PRIVATE_CAN(varType, varName, fnPostfix)   \
            __DECLARE_ATTRIBUTE_NONUSED_R__(varType, varName, Is##fnPostfix, _Set##fnPostfix, private)

// 老方式. 慎用,不符合命名规范
// 参数varType: 属性的类型
// 参数attName: 属性名。m_[attName]形成成员变量，Get[attName]/Set[attName]形成访问接口
// 声明一个属性
#define DECLARE_ATTRIBUTE_EX(varType, attName)    \
            __DECLARE_ATTRIBUTE_NONUSED__(varType, m_##attName, Get##attName, Set##attName, public)
// Set/Get函数以常引用传递
#define DECLARE_ATTRIBUTE_PASSREF_EX(varType, attName)    \
            __DECLARE_ATTRIBUTE_NONUSED_R__(varType, m_##attName, Get##attName, Set##attName, public)
// 只读版本
#define DECLARE_ATTRIBUTE_READONLY_EX(varType, attName)    \
            __DECLARE_ATTRIBUTE_NONUSED__(varType, m_##attName, Get##attName, _Set##attName, protected)
#define DECLARE_ATTRIBUTE_PASSREF_READONLY_EX(varType, attName)    \
            __DECLARE_ATTRIBUTE_NONUSED_R__(varType, m_##attName, Get##attName, _Set##attName, protected)

#ifndef GAME_SERVER
// win平台下 vs支持的内存泄漏检测
#if defined WIN32 && defined _MSC_VER

#ifdef _DEBUG
#define DEBUG_CLIENTBLOCK   new( _CLIENT_BLOCK, __FILE__, __LINE__)
#else
#define DEBUG_CLIENTBLOCK
#endif

#define _CRTDBG_MAP_ALLOC

#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_CLIENTBLOCK
#endif   
#endif  

//  defined WIN32 && defined _MSC_VER
// abs函数。据报:NDK下木有abs函数
#ifndef abs
// 用模板函数来做，因为有些地方参数是传的表达式，用宏的话会计算多次
//template <class T> inline T abs(T value) { return (0 > (value) ? 0 - (value) : (value)); }
// 这个会涉及到包含顺序问题，和王卓强协商，最终还是采用_ABS_宏。13-3-25
#endif
#endif

inline std::string IntToString(int value) 
{
    static char buff[32] = {};

    memset(buff, 0, sizeof(buff));
    sprintf(buff, "%d", value);

    return buff;
}

inline int64 StringToInt64(char* pchar)
{
    int len = strlen(pchar);
    int64 result = 0;
    for (int i = 0; i < len; ++i)
    {
        char t = pchar[i];
        result += atoi(&t);
		if( i < len-1)
		{
			result *= 10;
		}
    }
    return result;
}

// 讲十六进制字符串转换为对应的十进制值
inline int64 hexTodec(const char* pchar)
{
    char n[64] = {0};
    strcpy(n, pchar);
    int p = 0, i = 0, j = 0;
    i = strlen(n)-1;
    int64 dec = 0;
    int64 tval = 0;
    while (i >= 0)
    {
        if (n[i] <= '9' && n[i] >= '0')
        {
            tval = n[i] - '0';
        }
        else if (n[i] <= 'f' && n[i] >= 'a')
        {
            tval = n[i] - 'a' + 10;
        }
        else if (n[i] <= 'F' && n[i] >= 'A')
        {
            tval = n[i] - 'A' + 10;
        }

        if (p > 0)
        {
            for (j = 0; j < p; j++)
            {
                tval *= 16;
            }
        }
        dec += tval;
        i--;
        p++;
    }
    return dec;
}

inline int StringToInt(const char *pStr)
{
	int iValue = atoi(pStr);
	return iValue;
}

/****************************************************************************
功能：浮点转成字串
参数：
返回：
作者：Jacken
****************************************************************************/
inline std::string FloatToStr(float _value)
{
    char buff[32] = {};
    sprintf(buff, "%g", _value);

    std::string str;
    str.insert(0, buff);
    return str;    
}

#define ONE_MILLISECOND	1000		// 一毫秒


/************************************************************************
* 函数功能: 设置高清标志
* 参数说明: 
*     [IN]: bRetina: true为高清, false非高清
* 返回说明: 
************************************************************************/
void SetRetina(bool bRetina);

/************************************************************************
* 函数功能: 设置高清标志
* 参数说明: 
* 返回说明: 
*     true为高清, false非高清
************************************************************************/
bool GetRetina(void);

inline float PhysicalToPixel(float fchangeData)
{
    if (true == GetRetina())
    {
		fchangeData = fchangeData * 2;
    }
	
	return fchangeData; 
}

inline float PixelToPhysical(float fchangeData)
{
	if (true == GetRetina())
	{
		fchangeData = fchangeData / 2;
	}

	return fchangeData;
}

// 时间结构
struct DataBase
{  
    WORD wDay;			// 天
    WORD wHour;			// 时
    WORD wMinute;		// 分
    WORD wSecond;		// 秒
};

// 时间结构
struct DataEX
{  
    int year;           // 年
    int month;          // 月
    int Day;            // 日
    int Hour;           // 时
    int Minute;         // 分
    int Second;         // 秒
    int week;           // 星期
};


//********************************************************************
//函数功能: 将剩余毫秒转换成时间
//第一参数: [IN] 剩余毫秒
//返回说明: 时间结构体
//备注说明: 
//********************************************************************
inline DataBase ConvertToData(DWORD dwCountDown)
{
	DataBase data = {};
	data.wDay = (WORD)(dwCountDown / 86400000);
	data.wHour = (WORD)((dwCountDown / 3600000) % 24);
	data.wMinute = (WORD)((dwCountDown % 3600000) / 60000);
	data.wSecond = (WORD)(((dwCountDown % 3600000) % 60000) / 1000);
	return data;
};
//********************************************************************
//函数功能: 将时间结构组装成字符串
//第一参数: [IN] 时间结构体
//返回说明: 字符串
//备注说明: 转换后的格式是 (00:00:00 小时:分:秒)
//********************************************************************
inline const char* ConvertToDataString(const DataBase &data)
{
	static char szData[512] = {};
	sprintf(szData, "%02d:%02d:%02d", ((data.wDay * 24) + data.wHour), data.wMinute, data.wSecond);
	return szData;
};

//********************************************************************
//函数功能: 将剩余毫秒组装成时间字符串
//第一参数: [IN] 剩余毫秒
//返回说明: 字符串
//备注说明: 转换后的格式是 (00:00:00 小时:分:秒)
//********************************************************************
inline const char* ConvertToDataString(DWORD dwCountDown)
{
	DataBase data = ConvertToData(dwCountDown);
	return ConvertToDataString(data);
};

//函数功能：得到当前给定的时间
//返回说明：字符串
//备注说明：如（2012－08－02 00：00：00）
inline const char * GetTimeString( time_t & rawtime)
{
	struct tm * timeinfo = localtime ( &rawtime );
	static char szData[512] = {0};
	sprintf(szData, "%d-%02d-%02d %02d:%02d:%02d", 
		timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, 
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
	return szData;
};
//函数功能：得到当前给定的时间
//返回说明：字符串
//备注说明：如（2012－08－02 00：00：00）
inline const char * GetTimeStringEX( time_t & rawtime)
{
    struct tm * timeinfo = localtime ( &rawtime );
    static char szData[512] = {0};
    sprintf(szData, "%d-%02d-%02d-%02d:%02d:%02d", 
        timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday, 
        timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
    return szData;
};

//函数功能：获得年月日时分秒,星期
//返回说明：字符串
//备注说明：星期一为1.....星期日为7
inline const void GetSysTimeEX(DataEX& dateEX)
{
    time_t rawtime;
    time ( &rawtime );
    struct tm * timeinfo = localtime (&rawtime);
    
    dateEX.year = timeinfo->tm_year + 1900;
    dateEX.month = timeinfo->tm_mon + 1;
    dateEX.Day = timeinfo->tm_mday;
    dateEX.Hour = timeinfo->tm_hour;
    dateEX.Minute = timeinfo->tm_min;
    dateEX.Second = timeinfo->tm_sec;
    if(timeinfo->tm_wday == 0)
        dateEX.week = 7;
    else
        dateEX.week = timeinfo->tm_wday;
};

/********************************************************************************
* 功  能: 根据UTC时间计算本地时区年月日时分秒，星期
* 参  数: [IN] utcTime: UTC时间
*         [OUT] dateEx: 时间。
* 返回值: 无
*******************************************************************************/
inline void GetTimeEx(time_t utcTime, DataEX &dateEx)
{
    struct tm *pTimeInfo = localtime(&utcTime);
	if (NULL == pTimeInfo)
	{ return; }
	
    dateEx.year  = pTimeInfo->tm_year + 1900;
    dateEx.month = pTimeInfo->tm_mon + 1;
    dateEx.Day   = pTimeInfo->tm_mday;
    dateEx.Hour  = pTimeInfo->tm_hour;
    dateEx.Minute= pTimeInfo->tm_min;
    dateEx.Second= pTimeInfo->tm_sec;
    if(pTimeInfo->tm_wday == 0)
    {
        dateEx.week = 7;
    }
    else
    {
        dateEx.week = pTimeInfo->tm_wday;
    }
}

/****************************************************************************
功能：获取系统今天已经过去了多少分
参数：
返回：
作者：Jacken
****************************************************************************/
inline int GetTodayMinutes()
{
    time_t rawtime;
    time ( &rawtime );
    struct tm * timeinfo = localtime ( &rawtime );

    return timeinfo->tm_hour * 60 + timeinfo->tm_min;
}

//函数功能: 获取当前的系统时间
//返回说明: 字符串
//备注说明: 如（2012-08-02 00:00:00）
inline const char* GetSystemTimeString(void)
{
	time_t rawtime;
	
	time ( &rawtime );
	return GetTimeString(rawtime);
};

//函数功能: 获取当前的系统时间
//返回说明: 字符串
//备注说明: 如（2012-08-02 00:00:00）
inline const char* GetSystemTimeStringEX(void)
{
    time_t rawtime;

    time ( &rawtime );
    return GetTimeStringEX(rawtime);
};

//函数功能：获得特定格式的时间值
//返回说明：字符串
//备注说明： 如(XX月XX日XX小时XX分XX秒
inline const char* GetTimeFormatString( time_t & rawtime)
{
	struct tm * timeinfo = localtime(&rawtime);
	static char szData[100] ={0,};
	sprintf(szData, "%02d-%02d %02d:%02d:%02d", 
		timeinfo->tm_mon + 1, timeinfo->tm_mday, 
		timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec );
	return szData;
};

//********************************************************************
//函数功能: 字符串解析
//第一参数: [IN] 待解析的字符串
//第二参数: [IN] 切割字符
//返回说明: 解析后的内容
//备注说明: 直接使用的服务器的函数
//********************************************************************
typedef std::vector<std::string> Tokens;
inline Tokens StrSplit(const std::string &src, const std::string &sep)
{
	Tokens r;
	std::string s;

	for (std::string::const_iterator i = src.begin(); i != src.end(); i++)
	{
		if (sep.find((*i)) != std::string::npos)
		{
			if (s.length())
			{
				r.push_back(s);
			}
			s = "";
		}
		else
		{
			s += (*i);
		}
	}

	if (s.length())
	{
		r.push_back(s);
	}
	return r;
};

#define _MAX_(a, b)             (((a) > (b)) ? (a) : (b))
#define _MIN_(a,b)              (((a) < (b)) ? (a) : (b))
#define _ABS_(a)                ((a) < 0 ? (-(a)) : (a))

extern std::string string_To_UTF8(const std::string & str);
extern std::string UTF8_To_string(const std::string& str);

#define ANSI2UTF8      string_To_UTF8


/********************************************************************************
* 功  能: 判断文件的存在函数
* 参  数: [IN] strFileName: 文件名
* 返回值: true  文件存在
*         false 文件不存在
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-03-04   1.0     Joeson  创建
*******************************************************************************/
extern bool IsFileExsit(const std::string &strFileName);

#ifndef GAME_SERVER
/*************************************************************************************
 * 函数原型：std::string UrlEncode(const std::string& szToEncode)  
 * 函数功能：Url转换为Encode字符串
 * 参    数：
 *    [IN] const std::string& szToEncode: 需要转换的字符串
 *   [OUT]：无
 * 返 回 值：转换后的Encode字符串
 * 备    注：
 -------------------------------------------------------------------------------------
 *修改记录 :
 日期        版本       作者                 内容
 2013.11.23   1.0       ttan                创建
**************************************************************************************/
inline std::string  UrlEncode(const std::string& szToEncode)  
{  
    std::string src = szToEncode;  
    char hex[] = "0123456789ABCDEF";  
    std::string dst;  
    for (size_t i = 0; i < src.size(); ++i)
    {  
        unsigned char cc = src[i];  
        // 判断是否是ascii码
        if (isascii(cc))  
        {  
            if (cc == ' ')  
            {  
                dst += "%20";  
            }  
            else 
            {
                dst += cc;  
            }
        }  
        else 
        {  
            unsigned char c = static_cast<unsigned char>(src[i]);
            dst += '%';  
            dst += hex[c / 16];  
            dst += hex[c % 16];
        }  
    }  
    return dst;  
}  

/*************************************************************************************
 * 函数原型：void StrReplace(std::string & strDst, const std::string & strIn)
 * 函数功能：把字串"{1}"替换成其它字串
 * 参    数：
 *    [OUT]：std::string & strDst：待替换的字串
 *    [IN]：const std::string & strIn: 待插入的字串
 * 返 回 值：无
**************************************************************************************/
inline void StringReplace(std::string & strDst, const std::string & strIn)
{
    std::string str = "{1}";
    int nStrLen = strlen(str.c_str());
    int nPos = strDst.find(str);
    if (nPos == std::string::npos)
    {
        //LOGE("替换失败,未寻到\"{1}\", strDst: " << strDst);
        return;
    }
    
    strDst.replace(nPos, nStrLen, strIn);
}
#endif

/*************************************************************************************
 * 函数原型：const std::string & EmptyString() const
 * 函数功能：返回一个空字串(用于返回字串引用)
 * 参    数：无
 * 返 回 值：无
**************************************************************************************/
inline const std::string & EmptyString()
{
    static std::string strEmpty = ""; 
    return strEmpty;
}

#ifndef GAME_SERVER
/***********************************************************************************
* 函数功能: 将字串使用zip压缩后使用base64编码生成字串
* 参数：
*  [IN] data 二进制数据流
* 返回值：生成的字串 
************************************************************************************/
extern std::string ZipBase64Encode(const std::string& str);


/***********************************************************************************
* 函数功能: 还原ZipBase64Encode编码生成的字串
* 参数：
*  [IN] data 二进制数据流
* 返回值：生成的字串 
************************************************************************************/
extern std::string ZipBase64Decode(const std::string& data);

/***********************************************************************************
* 函数功能: URL参数编码
* 参数：
*  [IN] str 字串
* 返回值：生成的字串 
************************************************************************************/
extern std::string URLEncode(const std::string& str);
#endif

/*************************************************************************************
 * 函数原型：inline const INT_64 currentTimeMillis()
 * 函数功能：获得当前时间戳（毫秒级）
 * 参    数：无
 * 返 回 值：无
**************************************************************************************/
inline const INT_64 currentTimeMillis()
{
    static boost::posix_time::ptime ptBeg(boost::gregorian::date(1970, 1, 1));
    boost::posix_time::ptime ptEnd = boost::posix_time::microsec_clock::universal_time();
    return (ptEnd - ptBeg).total_milliseconds();
}

/***********************************************************************************
* 函数功能: 获取系统时钟(已开机的浮点秒数)
* 参数：无
* 返回值：浮点秒数
************************************************************************************/
extern double GetOsClock();