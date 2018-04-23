#ifdef _MSC_VER
#include <winsock2.h> 
#include <windows.h>
#include <io.h>                 // access函数
#endif
#include "Utility.h"

static bool s_bRetina = false;

/************************************************************************
* 函数功能: 设置高清标志
* 参数说明: 
*     [IN]: bRetina: true为高清, false非高清
* 返回说明: 
************************************************************************/
void SetRetina(bool bRetina)
{
    s_bRetina = bRetina;
}


/************************************************************************
* 函数功能: 设置高清标志
* 参数说明: 
* 返回说明: 
*     true为高清, false非高清
************************************************************************/
bool GetRetina(void)
{
    return s_bRetina;
}


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
bool IsFileExsit(const std::string &strFileName)
{
#ifdef WIN32
    return 0 == _access(strFileName.c_str(), 0);
#else
    return 0 == access(strFileName.c_str(), 0);
#endif
}


#ifdef WIN32
/***************************************************************
* 函数功能: 将标苹果平台下UTF-8字符格式转换为window平台下多字节格式
* 参数: [IN]需要转换的字符串
* 返回说明: 转换后的window平台字符格式
* 备注说明: window平台下可用此函数将下苹果的字符格式转换为window字符格式,予以正常显示
***************************************************************/
std::string UTF8_To_string(const std::string& str) 
{ 
    int nwLen = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0); 

    wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
    memset(pwBuf, 0, nwLen * 2 + 2); 

    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), pwBuf, nwLen); 

    int nLen = WideCharToMultiByte(CP_ACP, 0, pwBuf, -1, NULL, NULL, NULL, NULL); 

    char * pBuf = new char[nLen + 1]; 
    memset(pBuf, 0, nLen + 1); 

    WideCharToMultiByte(CP_ACP, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL); 

    std::string retStr = pBuf; 

    delete []pBuf; 
    delete []pwBuf; 

    pBuf = NULL; 
    pwBuf = NULL; 

    return retStr; 
}

/***************************************************************
* 函数功能: 将window平台下多字符格式转换为苹果平台下UTF-8字符格式
* 参数: [IN]需要转化的字符串
* 返回说明: 转换后的字符串
* 备注说明: 苹果平台下可用此函数将window下的字符格式转换为UTF-8字符格式,予以正常显示
***************************************************************/
std::string string_To_UTF8(const std::string & str) 
{ 
    int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0); 

    wchar_t * pwBuf = new wchar_t[nwLen + 1];//一定要加1，不然会出现尾巴 
    ZeroMemory(pwBuf, nwLen * 2 + 2); 

    ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen); 

    int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL); 

    char * pBuf = new char[nLen + 1]; 
    ZeroMemory(pBuf, nLen + 1); 

    ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL); 

    std::string retStr(pBuf); 

    delete []pwBuf; 
    delete []pBuf; 

    pwBuf = NULL; 
    pBuf  = NULL; 

    return retStr; 
}
#else

//IOS以下两个函数不做处理
std::string string_To_UTF8(const std::string & str) 
{
    return str;
}
std::string UTF8_To_string(const std::string& str) 
{
    return str;
}

#endif

/***********************************************************************************
* 函数功能: 将字串使用zip压缩后使用base64编码生成字串
* 参数：
*  [IN] data 二进制数据流
* 返回值：生成的字串 
************************************************************************************/
std::string ZipBase64Encode(const std::string& str)
{
    //uLong nOutLen = compressBound(str.length());
    //string strTmp;
    //strTmp.resize(nOutLen);
    //compress((Bytef*)(strTmp.c_str()), &nOutLen, reinterpret_cast<const Bytef*>(str.c_str()), str.length());
    //strTmp.resize(nOutLen);
    //return base64_encode(reinterpret_cast<const unsigned char*>(strTmp.c_str()), strTmp.length());
	return "";
}


/***********************************************************************************
* 函数功能: 还原ZipBase64Encode编码生成的字串
* 参数：
*  [IN] data 二进制数据流
* 返回值：成功返回生成的字串, 失败返回空串
************************************************************************************/
std::string ZipBase64Decode(const std::string& data)
{
    //string strDecoded = base64_decode(data);
    //uLongf nBuffLen = 1024 * 5; // 最大支持5K的数据
    //if (strDecoded.length() > nBuffLen)
    //{
    //    return "";
    //}
	//
    //// zip解压
    //string strTmp(nBuffLen, '\0');
    //uncompress((Bytef*)(strTmp.c_str()), &nBuffLen, reinterpret_cast<const Bytef*>(strDecoded.c_str()), strDecoded.length());
    //strTmp.resize(nBuffLen);
    //
    //return strTmp;
	return "";
}

/***********************************************************************************
* 函数功能: URL参数编码
* 参数：
*  [IN] str 字串
* 返回值：生成的字串 
************************************************************************************/
std::string URLEncode(const std::string& str)
{
	/*
    CURL *curl = curl_easy_init();
    char* pStrEncode = curl_easy_escape(curl, str.c_str(), str.length());
    std::string strRet = std::string(pStrEncode);
    
    curl_free(pStrEncode);
    curl_easy_cleanup(curl);
    return strRet;
	*/
	return "";
}