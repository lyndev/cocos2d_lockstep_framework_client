#include "DataParse.h"
#ifdef __linux__
#include <arpa/inet.h>
#endif

//{-- by wzq
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include<netinet/in.h> 
#endif
//--}

#ifdef GAME_SERVER
#ifdef WIN32
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")
#endif
#endif

using namespace cocos2d;

CDataParse::CDataParse(void):m_bNetByteOrder(true),m_nReadPos(0),m_pData(NULL),m_nDataLen(0)
{
}

/********************************************************************************
* 功  能: 带参构造函数
* 参  数: [IN] pData: 数据包指针
*         [IN] uDataLen: 数据包长度
*         [IN] isNetOrder: 数据包中字节是否是网络顺序 
* 返回值: 无
--------------------------------------------------------------------------------
* 修改记录 :
 日期       版本    作者    内容
 13-02-21   1.0     Joeson  创建
*******************************************************************************/
CDataParse::CDataParse(void *pData, unsigned int uDataLen, bool isNetOrder)
: m_pData((char*)pData), m_nDataLen(uDataLen), m_bNetByteOrder(isNetOrder), m_nReadPos(0)
{
}

CDataParse::~CDataParse(void)
{
}


/****************************************************************************
功能: 设置数据
参数:	pData:数据
		nDataLen:数据长度
		bOrder:是否是网络字节序
返回: 
****************************************************************************/
void   CDataParse::SetData(void * pData, unsigned int nDataLen, bool bOrder /* = true */)
{
	if (pData)
	{
		m_pData = (char *)pData;
	}
	m_nReadPos = 0;
	m_bNetByteOrder = bOrder;
	m_nDataLen = nDataLen;
}


/****************************************************************************
功能: 获得当前读取位置数据
参数: 
返回: 当前读取位置数据
****************************************************************************/
void *   CDataParse::GetData(void)
{
	if(!m_pData)
	{
		return NULL;
	}
	return (void *)(m_pData + m_nReadPos);
}



/****************************************************************************
功能: 设置字节序
参数: bOrder
true:网络字节序
false:本地字节序
返回: 
****************************************************************************/
void CDataParse::SetByteOrder(bool bOrder)
{
	m_bNetByteOrder = bOrder;
}


/****************************************************************************
功能: 获取字节序
参数: bOrder
true:网络字节序
false:本地字节序
返回: 
****************************************************************************/
bool CDataParse::GetByteOrder(void)
{
	return m_bNetByteOrder;
}

/****************************************************************************
功能: 读取字符串
参数: 
返回: 字符串
****************************************************************************/
std::string CDataParse::ReadString(void)
{
    std::string str("");
	if(NULL == m_pData || m_nReadPos + sizeof(int) >  m_nDataLen )
	{
		printf("null pointer or overfllow ReadString");
		return str;
	}
	int len = ( *(int*)&m_pData[m_nReadPos] );
	if(m_bNetByteOrder)
	{
		len = htonl(len);
	}
	m_nReadPos += sizeof( int );
	
	if( m_nReadPos + len > m_nDataLen )
	{
		printf("null pointer or overfllow ReadString");
		return str;
	}
	
	str.assign((char*)&m_pData[m_nReadPos],len);
	m_nReadPos += len;
	return str;
}

/****************************************************************************
功能：读取字符串，前2个字节存string的长度
参数：
返回：
作者：Jacken
****************************************************************************/
std::string CDataParse::ReadStringByUint16()
{
    std::string str("");
    if(NULL == m_pData || m_nReadPos + sizeof(unsigned short) >  m_nDataLen )
    {
		printf("null pointer or overfllow ReadString");
        return str;
    }
    unsigned short len = ( *(unsigned short*)&m_pData[m_nReadPos] );
    if(m_bNetByteOrder)
    {
        len = htonl(len);
    }
    m_nReadPos += sizeof( unsigned short );

    if( m_nReadPos + len > m_nDataLen )
    {
		printf("null pointer or overfllow ReadString");
        return str;
    }

    str.assign((char*)&m_pData[m_nReadPos],len);
    m_nReadPos += len;
    return str;
}

/****************************************************************************
功能: 读取字符串
参数: [out] pPos:字符串指针
	  [out]	len:pPos指向字符串的长度
返回: 字符串
****************************************************************************/
void CDataParse::ReadString(char * & pPos , int & len)			//读取字符串
{
	if(NULL == m_pData || m_nReadPos + sizeof(int) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadString");
		return ;
	}
	int length = ( *(int*)&m_pData[m_nReadPos] );
	if(m_bNetByteOrder)
	{
		length = ntohl(length);
	}
	m_nReadPos += sizeof( int );
	if( m_nReadPos + length > m_nDataLen )
	{
		printf("null pointer or overfllow ReadString");
		return ;
	}
	len = length;
	pPos = (char *)(&m_pData[m_nReadPos]);
    m_nReadPos += len;
}


/****************************************************************************
功能: 读取整形数据
参数: 
返回: 整形数据
****************************************************************************/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
int CDataParse::ReadINT(void)
{
	if(NULL == m_pData || m_nReadPos + sizeof(int) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadINT");
		return 0;
	}
	char tmpbuf[64] = {0};
	memcpy(tmpbuf, &m_pData[m_nReadPos], sizeof(int));
	int ret = *(int*)tmpbuf;
	if(m_bNetByteOrder)
	{
		ret = ntohl(ret);
	}
	m_nReadPos += sizeof( int );
	return ret;
}
#else
int CDataParse::ReadINT(void)
{
	if(NULL == m_pData || m_nReadPos + sizeof(int) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadINT");
		return 0;
	}
	int ret = ( *(int*)&m_pData[m_nReadPos] );
	if(m_bNetByteOrder)
	{
		ret = ntohl(ret);
	}
	m_nReadPos += sizeof( int );
	return ret;
}
#endif

/****************************************************************************
功能: 读取整形数据
参数: 
返回: 整形数据
****************************************************************************/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
short CDataParse::ReadINT16(void)
{
	if(NULL == m_pData || m_nReadPos + sizeof(short) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadINT16");
		return 0;
	}
	char tmpbuf[64] = {0};
	memcpy(tmpbuf, &m_pData[m_nReadPos], sizeof(short));
	short ret = *(short*)tmpbuf;
	if(m_bNetByteOrder)
	{
		ret = ntohs(ret);
	}
	m_nReadPos += sizeof( short );
	return ret;
}
#else
short CDataParse::ReadINT16(void)
{
	if(NULL == m_pData || m_nReadPos + sizeof(short) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadINT16");
		return 0;
	}
	short ret = ( *(short*)&m_pData[m_nReadPos] );
	if(m_bNetByteOrder)
	{
		ret = ntohs(ret);
	}
	m_nReadPos += sizeof( short );
	return ret;
}
#endif

/****************************************************************************
功能: 读取整形数据
参数: 
返回: 整形数据
****************************************************************************/
char CDataParse::ReadINT8(void)
{
	if(NULL == m_pData || m_nReadPos + sizeof(char) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadINT8");
		return 0;
	}
	char ret = ( *(char*)&m_pData[m_nReadPos] );
	m_nReadPos += sizeof( char );
	return ret;
}

/****************************************************************************
功能: 读取64位无符号整形数
参数: 
返回：64为整形数 
****************************************************************************/
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
INT_64 CDataParse::ReadINT_64(void)
{
	if(NULL == m_pData || m_nReadPos + sizeof(INT_64) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadINT_64");
		return 0;
	}
	char tmpbuf[64] = {0};
	memcpy(tmpbuf, &m_pData[m_nReadPos], sizeof(INT_64));
	INT_64 ret = *(INT_64*)tmpbuf;

	if(m_bNetByteOrder)
		ret = m_EndianCvt64.ntoh64(ret);
	m_nReadPos += sizeof( INT_64 );
	return ret;
}
#else
INT_64 CDataParse::ReadINT_64(void)
{
	if(NULL == m_pData || m_nReadPos + sizeof(INT_64) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadINT_64");
		return 0;
	}
	INT_64 ret =  *(INT_64*)&m_pData[m_nReadPos] ;
	if(m_bNetByteOrder)
		ret = m_EndianCvt64.ntoh64(ret);
	m_nReadPos += sizeof( INT_64 );
	return ret;
}
#endif

/****************************************************************************
功能: 读取浮点数
参数: 
返回: 浮点数
****************************************************************************/
float CDataParse::ReadFLOAT(void)
{
	if(NULL == m_pData || m_nReadPos + sizeof( float ) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadFLOAT");
		return 0;
	}
    
	float ret = 0;
    
    memcpy(&ret, m_pData + m_nReadPos, sizeof(float));
    
	m_nReadPos += sizeof( float );
    
	return ret;
}

/****************************************************************************
功能: 读取布尔值
参数: 
返回: 布尔值
****************************************************************************/
bool CDataParse::ReadBOOL(void)
{
	if(NULL == m_pData || m_nReadPos + sizeof(bool) > m_nDataLen )
	{
		printf("null pointer or overfllow ReadBOOL");
		return false;
	}
	bool ret =  *(bool*)&m_pData[m_nReadPos] ;
	m_nReadPos += sizeof( bool );
	return (bool)ret;
}


/****************************************************************************
功能: 读取无符号整形数据
参数: 
返回: 整形数据
****************************************************************************/
unsigned int CDataParse::ReadUINT(void)
{
	return (unsigned int)(ReadINT());
}


/****************************************************************************
功能: 读取无符号整形数据
参数: 
返回: 整形数据
****************************************************************************/
unsigned short CDataParse::ReadUINT16(void)
{
	return (unsigned short)(ReadINT16());
}

/****************************************************************************
功能: 读取无符号整形数据
参数: 
返回: 整形数据
****************************************************************************/
unsigned char CDataParse::ReadUINT8(void)
{
	return (unsigned char)(ReadINT8());
}


bool CDataParse::End()
{
	return m_nReadPos == m_nDataLen ? true : false ;
}