/**************************************************************
* 
* 文件名: EndianConverter64.h
* 功能说明: 64位网络字节序与本地字节序之间的转换
* 作者：赵魁
* 日期：2012.12.10
* 修改记录
**************************************************************/
#pragma once


#include "Utility.h"

 
//NS_QIN_BEGIN




class CEndianConverter64
{
public:
	CEndianConverter64(void);
	~CEndianConverter64(void);

public:
	INT_64 ntoh64(INT_64);				//网络字节序转换为本地字节序
	INT_64 hton64(INT_64);				//本地字节序转换为网络字节序

protected:
	bool _IsBigEndian();				//判断是否是大端字节序
	inline INT_64	_ReverseINT_64(INT_64 num);				//反转64位整形数据

private:
	bool m_bBigEndian;

};


//NS_QIN_END