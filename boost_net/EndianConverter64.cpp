/**************************************************************
* 
* 文件名: EndianConverter64.h
* 功能说明: 64位网络字节序与本地字节序之间的转换
* 作者：赵魁
* 日期：2012.12.10
* 修改记录
**************************************************************/

#include "EndianConverter64.h"

CEndianConverter64::CEndianConverter64(void)
{
	m_bBigEndian = _IsBigEndian();
}

CEndianConverter64::~CEndianConverter64(void)
{
}

//网络字节序转换为本地字节序
INT_64 CEndianConverter64::ntoh64(INT_64 n)
{
	if(!m_bBigEndian)
	{
		return _ReverseINT_64(n);
	}
	
	return n;
}

//本地字节序转换为网络字节序
INT_64 CEndianConverter64::hton64(INT_64 n)
{
	if(!m_bBigEndian)
	{
		return _ReverseINT_64(n);
	}

	return n;
}

/****************************************************************************
功能: 转换64位整形字节序
参数: 
返回: 64位整形数
****************************************************************************/
INT_64 CEndianConverter64::_ReverseINT_64(INT_64 num)
{
	union {
		INT_64 i;
		char c[8];
	} u, r;

	u.i = num;
	r.c[0] = u.c[7];
	r.c[1] = u.c[6];
	r.c[2] = u.c[5];
	r.c[3] = u.c[4];
	r.c[4] = u.c[3];
	r.c[5] = u.c[2];
	r.c[6] = u.c[1];
	r.c[7] = u.c[0];

	return r.i;
}

/****************************************************************************
功能: 判断是否是大端字节序
参数:	
返回: true：大端
	  false：小端
****************************************************************************/
bool CEndianConverter64::_IsBigEndian()  
{  
	const unsigned short n = 1;  
	if(*(char *)&n)  
	{  
		return false;  
	}  
	return true;  
} 


